//
// Created by root on 18-3-27.
//

#include "eventHandle.h"

#include <eXosip2/eXosip.h>
#include <osip2/osip_mt.h>
#include <osip2/osip.h>
#include "Markup.h"
#include <string.h>
#include <string>
#include <unistd.h>
#include "config.h"
#include "XEvent.h"
#include "easylogging++.h"


using namespace std;




void eventHandle::invite(ReadProperties &prop){






    osip_message_t *invite = NULL;
    char localip[128];
    char tmp[4096];
    char *source_call;
    char *dest_call;

    source_call = (char*)malloc(100);
    dest_call = (char*)malloc(100);
    sprintf(source_call, "sip:%s@%s", (prop.user).c_str(),(prop.myIp).c_str());
    sprintf(dest_call, "sip:%s@%s",(prop.serverId).c_str(),(prop.serverIp).c_str());

    int i = eXosip_call_build_initial_invite (&invite, dest_call, source_call, NULL, "This si a call for a conversation");
    if (i != 0)
    {
        printf ("Intial INVITE failed!\n");

    }
    osip_message_set_supported (invite, "100rel");
    //eXosip_guess_localip (AF_INET, localip, 128); // 获取本地IP
    //SDP格式
    snprintf (tmp, 4096,
              "v=0\r\n"
                      "o=33030000001180000002 0 0 In IP4 192.168.0.130"
                      "s=play\r\n"
                      "c=In IP4 192.168.0.130\r\n"
                      "t=0 0\r\n"
                      "m=video  6000 RTP/AVP 96 98 97\r\n"    // 音频、传输端口、传输类型、格式列表
                      "a=recvonly \r\n"    // 音频、传输端口、传输类型、格式列表
                      "a=rtpmap:96 PS/90000a=rtpmap:96 PS/90000\r\n"          // 以下为具体描述格式列表中的
                      "a=rtpmap:98 H264/90000\r\n"
                      "a=rtpmap:97 MPEG4/90000\r\n");

    osip_message_set_body (invite, tmp, strlen(tmp));
    osip_message_set_content_type (invite, "application/sdp");

    eXosip_lock ();
    i = eXosip_call_send_initial_invite (invite);
    eXosip_unlock ();



}



void eventHandle::Register(ReadProperties &prop){

    prop.initConfig();

    char fromuser[256];
    char proxy[256];
    char route[256];
    char contact[256];

    sprintf(fromuser,"sip:%s@%s",(prop.user).c_str(),(prop.myIp).c_str());
    sprintf(proxy,"sip:%s@%s",(prop.serverId).c_str(),(prop.serverIp).c_str());
    // sprintf(route,"<sip:%s:%d;lr>","122.224.82.77",5160);
    sprintf(contact,"<sip:%s:%s>",(prop.user).c_str(),(prop.myIp).c_str());

    eXosip_clear_authentication_info();
    osip_message_t *reg = NULL;
    int m_nregid = eXosip_register_build_initial_register(fromuser, proxy, contact,3600,&reg);
    //提前输入了验证信息，在消息为401处，用eXosip_automatic_action()自动处理
    eXosip_add_authentication_info((prop.user).c_str(),(prop.user).c_str(),(prop.password).c_str(), "MD5",NULL);
    if(reg==NULL)
        return;
    // osip_message_set_route(reg,route);
    if(eXosip_register_send_register(m_nregid, reg)!=0)
        return ;

}


void eventHandle::eventSubscription(eXosip_event *event,ReadProperties &prop ){


    osip_body_t *body;
    osip_message_get_body (event->request, 0, &body);
    printf ("I get the msg is: %s/n", body->body);

    CMarkup xmlreq;
    xmlreq.SetDoc(body->body);
    string SN;
    string DeviceID;

    while (xmlreq.FindElem("Query"))
    {
        xmlreq.IntoElem();
        if(xmlreq.FindElem("SN")){

            SN= xmlreq.GetData();
        }
        if(xmlreq.FindElem("DeviceID")){
            DeviceID= xmlreq.GetData();

        }

    }



    /* search for the user to see if he has been
    previously accepted or not! */
    osip_message_t * answer;


    CMarkup xmlanswer;
    xmlanswer.SetDoc("<?xml version=\"1.0\" ?>\r\n");
    xmlanswer.AddElem("Response");
    xmlanswer.IntoElem();
    xmlanswer.AddElem("CmdType", "Catalog");
    xmlanswer.AddElem("SN", SN);
    xmlanswer.AddElem("DeviceID", DeviceID);
    xmlanswer.AddElem("Result", "OK");
    eXosip_insubscription_build_answer (event->tid, 200,&answer);
    osip_message_set_content_type(answer, "Application/MANSCDP+xml");
    osip_message_set_body(answer, xmlanswer.GetDoc().c_str(), strlen( xmlanswer.GetDoc().c_str()));
    eXosip_insubscription_send_answer (event->tid, 200,answer);


    char to[256];
    char from[256];
    char route[256];
    char contact[256];
    sprintf(to,"sip:%s@%s",(prop.serverId).c_str(),(prop.serverIp).c_str());
    sprintf(from,"sip:%s@%s",(prop.user).c_str(),(prop.myIp).c_str());
    //sprintf(route,"<sip:%s:%d;lr>","122.224.82.77",5160);

    sprintf(contact,"<sip:%s:%s>",(prop.serverId).c_str(),(prop.serverIp).c_str());

    osip_message_t *message ;
  //  int iRet =eXosip_insubscription_build_notify(event->did,200,200,&message);
    int iRet =eXosip_message_build_request(&message,"NOTIFY",to,from,NULL);

    CMarkup xml;
    xml.SetDoc("<?xml version=\"1.0\" ?>\r\n");
    xml.AddElem("Response");
    xml.IntoElem();
    xml.AddElem("CmdType", "Catalog");
    xml.AddElem("SN", SN);
    xml.AddElem("DeviceID", DeviceID);
    xml.AddElem("SumNum", 4);
    xml.AddElem("DeviceListNum=2");
    xml.IntoElem();
    xml.AddElem("Item");
    xml.IntoElem();
    xml.AddElem("DeviceID",DeviceID);


    osip_message_set_content_type(message, "Application/MANSCDP+xml");
    osip_message_set_header(message, "Event", "presence");
    osip_message_set_body(message, xml.GetDoc().c_str(), strlen( xml.GetDoc().c_str()));
 //   int mssId=eXosip_message_send_request(message);






}
XEventBase *base = new Epoll(1024);

void keepLive(TimerEvent *te, string  SN,string DeviceID,ReadProperties prop)
{



    char to[256];
    char from[256];
    char route[256];
    char contact[256];
    sprintf(to,"sip:%s@%s",(prop.serverId).c_str(),(prop.serverIp).c_str());
    sprintf(from,"sip:%s@%s",(prop.user).c_str(),(prop.myIp).c_str());
    //sprintf(route,"<sip:%s:%d;lr>","122.224.82.77",5160);

    sprintf(contact,"<sip:%s:%s>",(prop.serverId).c_str(),(prop.serverIp).c_str());

    osip_message_t *message ;
    int iRet =eXosip_message_build_request(&message,"MESSAGE",to,from,NULL);

    CMarkup xml;
    xml.SetDoc("<?xml version=\"1.0\" ?>\r\n");
    xml.AddElem("Notify");
    xml.IntoElem();
    xml.AddElem("CmdType", "Keepalive");
    xml.AddElem("SN", SN);
    xml.AddElem("DeviceID", DeviceID);
    xml.AddElem("Status", "OK");
    osip_message_set_content_type(message, "Application/MANSCDP+xml");
    osip_message_set_body(message, xml.GetDoc().c_str(), strlen( xml.GetDoc().c_str()));
    int mssId=eXosip_message_send_request(message);



    base->add_timer(te);
}

void eventHandle::eventMessage(eXosip_event *event,ReadProperties &prop){

    osip_body_t *body;
    osip_message_get_body (event->request, 0, &body);
    printf ("I get the msg is: %s/n", body->body);

    CMarkup xmlreq;
    xmlreq.SetDoc(body->body);
    string SN;
    string DeviceID;

    while (xmlreq.FindElem("Query"))
    {
        xmlreq.IntoElem();
        if(xmlreq.FindElem("SN")){

            SN= xmlreq.GetData();
        }
        if(xmlreq.FindElem("DeviceID")){
            DeviceID= xmlreq.GetData();

        }

    }



    osip_message_t * answer;
    eXosip_message_build_answer (event->tid, 200,&answer);
    eXosip_message_send_answer (event->tid, 200,answer);

    char to[256];
    char from[256];
    char route[256];
    char contact[256];
    sprintf(to,"sip:%s@%s",(prop.serverId).c_str(),(prop.serverIp).c_str());
    sprintf(from,"sip:%s@%s",(prop.user).c_str(),(prop.myIp).c_str());
    sprintf(contact,"<sip:%s:%s>",(prop.serverId).c_str(),(prop.serverIp).c_str());

    osip_message_t *message ;
    int iRet =eXosip_message_build_request(&message,"MESSAGE",to,from,NULL);

    CMarkup xml;
    xml.SetDoc("<?xml version=\"1.0\" ?>\r\n");
    xml.AddElem("Response");
    xml.IntoElem();
    xml.AddElem("CmdType", "Catalog");
    xml.AddElem("SN", SN);
    xml.AddElem("DeviceID", DeviceID);
    osip_message_set_content_type(message, "Application/MANSCDP+xml");
    osip_message_set_body(message, xml.GetDoc().c_str(), strlen( xml.GetDoc().c_str()));
    int mssId=eXosip_message_send_request(message);


    base->add_timer(new TimerEvent(milliseconds(atoi(prop.keepLiveSecond.c_str()))), keepLive, SN,DeviceID,prop);
    base->loop();







}
