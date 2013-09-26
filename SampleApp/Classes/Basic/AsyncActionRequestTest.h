//
//  AsyncActionRequestTest.h
//  BridgeSample
//
//  Created by 下川 北斗 on 2013/09/26.
//
//

#ifndef __BridgeSample__AsyncActionRequestTest__
#define __BridgeSample__AsyncActionRequestTest__

#include <iostream>

#include "../KeyboardNotificationLayer.h"
#include "../GroundLayer.h"
#include <libGSS/libGSS.h>
#include "cocos2d.h"


/**
 @brief    SimpleActionTest for retain prev, reset, next, main menu buttons.
 */
class AsyncActionRequestTest : public GroundLayer
{
public:
    void nextCallback(CCObject* pSender);
    void backCallback(CCObject* pSender);
    
    std::string title();
};

class AsyncActionRequestTestScene : public TestScene
{
public:
    virtual void runThisTest();
};


class AsyncActionRequestTestSendRequest : public KeyboardNotificationLayer
{
public:
    std::string subtitle();
    virtual void execute();
    std::string defaultValue();
    void OnComplete(libgss::ActionResponse* response) {
        if (response->success()) {
            CCLOG("%s", response->outputs()->toString().c_str());
        }
        else if(response->existsNewerVersionClient()){
            CCLOG("New version client has released.");
        }
        else{
            CCLOG("Error has occured. %d", response->code());
        }
    }
};

class AsyncActionRequestTestGetResult : public BaseNotificationLayer
{
public:
    std::string subtitle();
    virtual void execute();
    void OnComplete(libgss::ActionResponse* response) {
        if (response->success()) {
            CCLOG("%s", response->outputs()->toString().c_str());
        }
        else if(response->existsNewerVersionClient()){
            CCLOG("New version client has released.");
        }
        else{
            CCLOG("Error has occured. %d", response->code());
        }
    }
};


#endif /* defined(__BridgeSample__AsyncActionRequestTest__) */
