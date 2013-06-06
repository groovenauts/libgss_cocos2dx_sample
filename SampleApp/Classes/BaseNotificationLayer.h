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
    
protected:
    void log(std::string s) {
        int size = s.length() + 1;
        char* buf = new char[size];
        memcpy(buf, s.c_str(), size);
        CCLOG(buf);
    }
};


#endif /* defined(__SampleApp__BaseNotificationLayer__) */
