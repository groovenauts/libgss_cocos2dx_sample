//
//  ApiErrorTest.h
//  SampleApp
//
//  Created by 下川 北斗 on 2013/05/31.
//
//

#ifndef __SampleApp__ApiErrorTest__
#define __SampleApp__ApiErrorTest__

#include <iostream>

#include <libGSS/libGSS.h>

#include "cocos2d.h"
#include "cocos-ext.h"

#include "../KeyboardNotificationLayer.h"
#include "../GroundLayer.h"

class ApiErrorTest : public GroundLayer
{
public:
    void nextCallback(CCObject* pSender);
    void backCallback(CCObject* pSender);
    
    std::string title();
};

class ApiErrorTestScene : public TestScene
{
public:
    std::string subtitle();
    virtual void runThisTest();
};


class ApiErrorTestInvalidURL : public BaseNotificationLayer
{
public:
    std::string subtitle();
    virtual void execute();
    void OnComplete(libgss::ActionResponse* response);
};


class ApiErrorTestInvalidPlayerId : public BaseNotificationLayer
{
public:
    std::string subtitle();
    virtual void execute();
    void OnComplete(libgss::LoginResponse* response);
};
#endif /* defined(__SampleApp__ApiErrorTest__) */
