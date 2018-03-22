#include <eXosip2/eXosip.h>
#include <sys/socket.h>
#include <osip2/osip_mt.h>
#include <osip2/osip.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <string>
#include <iostream>
#include "Markup.h"

using namespace std;

void process_event();
void Register();
void devicequery();
int m_nregid=0;//注册id
int main()
{
    int i=0;
    i=eXosip_init();
    if(i!=0)
        return -1;
    printf("=========  %s\n",eXosip_get_version());
    unsigned short port=5064;//本地sip端口

    i=eXosip_listen_addr(IPPROTO_UDP,NULL,port,AF_INET,0);//17是udp，不想过多的引入头文件，故用数字表示一下
    if(i!=0)
    {
        printf("listen fail!\n");
        eXosip_quit();
        return -1;
    }


    devicequery();
    process_event();
    sleep(5000);

    getchar();
    return 0;
}



void process_event()

{
    eXosip_event *event;

    for(;;)

    {

        if (!(event = eXosip_event_wait(0, 50)))
            continue ;


        printf("event->type   ======  %d\n",event->type);

        if(event->type==EXOSIP_CALL_ACK)

        {

        }

        else if(event->type==EXOSIP_CALL_ANSWERED)

        {

        }

        else if(event->type==EXOSIP_REGISTRATION_FAILURE)

        {

            if(event->response && event->response->status_code==401)

            {
                printf("======  401   =%d\n",event->rid);


            }

        }
        else if(event->type==EXOSIP_REGISTRATION_SUCCESS)
        {
            printf("register sccess!\n");
        }
        else if(event->type==31)
        {
            printf("4444!\n");
        }




        eXosip_event_free(event);

    }

}


void  devicequery(){

    //构建目录订阅代码
    string strSubXml = "";





    char to[256];
    char from[256];
    char route[256];
    char contact[256];
    sprintf(to,"sip:%s@%s","33030000001180000002","192.168.11.130:5000");
    sprintf(from,"sip:%s@%s","33030000002000000001","122.224.82.78:5160");
    sprintf(route,"<sip:%s:%d;lr>","122.224.82.77",5160);

    sprintf(contact,"<sip:%s:%s>","33030000001180000002","192.168.11.130:5000");



    osip_message_t *message ;
    int iRet =eXosip_message_build_request(&message,"MESSAGE",to,from,route);

    CMarkup xml;
    xml.SetDoc("<?xml version=\"1.0\" ?>\r\n");
    xml.AddElem("Query");
    xml.IntoElem();
    xml.AddElem("CmdType", "Catalog");
    xml.AddElem("SN", rand());
    xml.AddElem("DeviceID", "33030000001180000002");
//    xml.AddElem("StartAlarmPriority", 1);
 //   xml.AddElem("EndAlarmPriority", 4);
  //  xml.AddElem("AlarmMethod", 0);
    xml.AddElem("StartTime", "2015-01-01T00:00:00");
    xml.AddElem("EndTime", "2018-05-01T23:00:00");
    osip_message_set_content_type(message, "Application/MANSCDP+xml");
    cout<<xml.GetDoc().c_str();
    osip_message_set_body(message, xml.GetDoc().c_str(), strlen( xml.GetDoc().c_str()));
    //osip_message_set_contact(pSubScribeMsg, TmpBuf);
    int mssId=eXosip_message_send_request(message);



  // cout<<"1";




}
