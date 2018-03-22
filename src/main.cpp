#include <eXosip2/eXosip.h>
#include <sys/socket.h>
#include <osip2/osip_mt.h>
#include <osip2/osip.h>
#include <unistd.h>
#include <netinet/in.h>




void process_event();
void Register();
int m_nregid=0;//注册id
int main()
{
    int i=0;
    i=eXosip_init();
    if(i!=0)
        return -1;
    printf("=========  %s\n",eXosip_get_version());
    unsigned short port=5066;//本地sip端口

    i=eXosip_listen_addr(IPPROTO_UDP,NULL,port,AF_INET,0);//17是udp，不想过多的引入头文件，故用数字表示一下
    if(i!=0)
    {
        printf("listen fail!\n");
        eXosip_quit();
        return -1;
    }
    Register();
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





        }else if(event->type==EXOSIP_MESSAGE_NEW){


            osip_body_t *body;
            osip_message_get_body (event->request, 0, &body);
            printf ("I get the msg is: %s/n", body->body);


            osip_message_t * answer;
            eXosip_message_build_answer (event->tid, 200,&answer);
            eXosip_message_send_answer (event->tid, 200,answer);






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
//                        eXosip_add_authentication_info("022000000110000","022000000110000","123456", "MD5",realm);
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
            printf("register sccess!\n");
        }



        eXosip_event_free(event);

    }

}



void Register()

{
    printf("1 ======  Register()\n");

    char fromuser[256];
    char proxy[256];
    char route[256];
    char contact[256];

    sprintf(fromuser,"sip:%s@%s","33030000001180000002","192.168.11.130:5160");
    sprintf(proxy,"sip:%s@%s","33030000001180000002","122.224.82.77:5160");
   // sprintf(route,"<sip:%s:%d;lr>","122.224.82.77",5160);
    sprintf(contact,"<sip:%s:%s>","33030000001180000002","192.168.11.130:5000");

    eXosip_clear_authentication_info();
    osip_message_t *reg = NULL;
    m_nregid = eXosip_register_build_initial_register(fromuser, proxy, contact,3600,&reg);
    //提前输入了验证信息，在消息为401处，用eXosip_automatic_action()自动处理
    eXosip_add_authentication_info("33030000001180000002","33030000001180000002","123456", "MD5",NULL);
    if(reg==NULL)
        return;
   // osip_message_set_route(reg,route);
    if(eXosip_register_send_register(m_nregid, reg)!=0)
        return ;

}