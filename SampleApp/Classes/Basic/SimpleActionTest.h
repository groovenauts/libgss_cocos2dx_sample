#ifndef __SIMPLE_ACTION_TEST_H__
#define __SIMPLE_ACTION_TEST_H__

#include "../BaseNotificationLayer.h"
#include "../GroundLayer.h"
#include <libGSS/libGSS.h>
#include "cocos2d.h"

class BaseNotificationLayer;

/**
@brief    SimpleActionTest for retain prev, reset, next, main menu buttons.
*/
class SimpleActionTest : public GroundLayer
{
public:
    void nextCallback(CCObject* pSender);
    void backCallback(CCObject* pSender);

    std::string title();
};

class SimpleActionTestScene : public TestScene
{
public:
    virtual void runThisTest();
};


class SimpleActionTest1Action : public BaseNotificationLayer
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
            CCLOG("Error has occured. %d\n%s", response->code(), response->message().c_str());
        }
    }
};


class SimpleActionTestWithNoCallback : public BaseNotificationLayer
{
    int pollingCount_;
    libgss::ActionRequest* request_;
public:
    std::string subtitle();
    virtual void execute();
    void poll(float time);
};


#endif    // __SIMPLE_ACTION_TEST_H__
