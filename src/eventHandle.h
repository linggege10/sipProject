//
// Created by root on 18-3-27.
//

#ifndef SIPPROJECT_EVENTHANDLE_H
#define SIPPROJECT_EVENTHANDLE_H

#include <eXosip2/eXosip.h>
#include <osip2/osip_mt.h>
#include <osip2/osip.h>
#include "config.h"


class eventHandle {

public:



 void eventMessage(eXosip_event *event,ReadProperties &prop );
    void Register(ReadProperties &prop);

    void eventSubscription(eXosip_event *event,ReadProperties &prop );




};


#endif //SIPPROJECT_EVENTHANDLE_H
