#ifndef __GREETING_HISTORY_TEST_H__
#define __GREETING_HISTORY_TEST_H__

#include "../BaseNotificationLayer.h"
#include "../GroundLayer.h"
#include <libGSS/libGSS.h>

class BaseNotificationLayer;

/**
@brief    GreetingHistoryTest for retain prev, reset, next, main menu buttons.
*/
class GreetingHistoryTest : public GroundLayer
{
public:
    void nextCallback(CCObject* pSender);
    void backCallback(CCObject* pSender);

    std::string title();
};

class GreetingHistoryTestScene : public TestScene
{
public:
    virtual void runThisTest();
};


class GreetingHistoryTestCreate : public BaseNotificationLayer
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



class GreetingHistoryTestFetchAll : public BaseNotificationLayer
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



#endif    // __GREETING_HISTORY_TEST_H__
