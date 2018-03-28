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
#include "XEvent.h"
#include "config.h"
#include "eventHandle.h"


using namespace std;


void process_event();

int m_nregid=0;//注册id

XEventBase *base = new Epoll(1024);


void hello_cb2(TimerEvent *te, int a)
{
    cout << "Hello world cb2:" << a << endl;
    base->add_timer(te);
}
ReadProperties prop;
int main()
{

//    //使用方法1 添加普通函数
//    base->add_timer(new TimerEvent(milliseconds(5000)), hello_cb2, 444);
//    base->loop();
//
    el::Configurations conf("/home/hzl/project/github/echoServer/sipProject/src/log.conf");
    el::Loggers::reconfigureAllLoggers(conf);
//
//    LOG(TRACE)   << "***** trace log  *****";
//    LOG(DEBUG)   << "***** debug log  *****";
//    LOG(ERROR)   << "***** error log  *****";
//    LOG(WARNING) << "***** warning log  *****";


    int i=0;
    i=eXosip_init();
    if(i!=0)
        return -1;
    printf("=========  %s\n",eXosip_get_version());
    unsigned short port=5067;//本地sip端口

    i=eXosip_listen_addr(IPPROTO_UDP,NULL,port,AF_INET,0);//17是udp，不想过多的引入头文件，故用数字表示一下
    if(i!=0)
    {
        printf("listen fail!\n");
        eXosip_quit();
        return -1;
    }
    eventHandle eHandle;


    eHandle.Register(prop);

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

        eXosip_lock();

        eXosip_automatic_action();

        eXosip_unlock();
        printf("event->type   ======  %d\n",event->type);

        if(event->type==EXOSIP_CALL_ACK)

        {

        }else if(event->type==EXOSIP_IN_SUBSCRIPTION_NEW){

            eventHandle eh;
            eh.eventSubscription(event,prop);





        }else if(event->type==EXOSIP_MESSAGE_NEW){
            eventHandle eh;
           eh.eventMessage(event,prop);


        }


        else if(event->type==EXOSIP_CALL_ANSWERED)

        {

        }

        else if(event->type==EXOSIP_REGISTRATION_FAILURE)

        {

            if(event->response && event->response->status_code==401)

            {

                printf("======  401   =%d\n",event->rid);
//#if 0
//                osip_message_t *reg = NULL;
//                        eXosip_lock();
//                        osip_www_authenticate_t *dest = NULL;
//                        osip_message_get_www_authenticate(event->response,0,&dest);
//                        if(dest == NULL)
//                          continue;
//                        char realm[256];
//                        eXosip_clear_authentication_info();
//                        strcpy(realm,osip_www_authenticate_get_realm(dest));
//                eXosip_add_authentication_info("33030000001180000002","33030000001180000002","123456", "MD5",NULL);
//			eXosip_register_build_register(event->rid, 3600, &reg);
//			if(reg==NULL)
//			{
//				printf("eXosip_register_build_register  failed!\n");
//                          	continue;
//			}
//			printf("authenticate=%s  ver=%s\n",realm,reg->sip_version);
//                        eXosip_register_send_register(event->rid,reg);
//                        eXosip_unlock();
//
//#endif

            }

        }
        else if(event->type==EXOSIP_REGISTRATION_SUCCESS)
        {
            char *registerUser=event->response->from->url->username;
            string registerUserString=registerUser;

            LOG(INFO)    << registerUserString+"register sccess";

            printf("register sccess!\n");
        }



        eXosip_event_free(event);

    }

}



