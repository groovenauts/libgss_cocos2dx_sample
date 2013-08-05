//
//  BaseNotificationLayer.h
//  SampleApp
//
//  Created by t-akima on 2013/05/10.
//
//

#ifndef __SampleApp__BaseNotificationLayer__
#define __SampleApp__BaseNotificationLayer__

#include "testBasic.h"

//////////////////////////////////////////////////////////////////////////
// BaseNotificationLayer for test IME keyboard notification.
//////////////////////////////////////////////////////////////////////////

class BaseNotificationLayer : public CCLayer, public CCIMEDelegate
{
public:
    virtual std::string subtitle() = 0;
    
    virtual void execute(){};
};


#endif /* defined(__SampleApp__BaseNotificationLayer__) */
