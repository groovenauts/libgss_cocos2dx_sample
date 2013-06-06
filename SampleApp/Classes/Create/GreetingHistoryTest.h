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
    std::string subtitle();
    virtual void runThisTest();
};


class GreetingHistoryTestCreate : public BaseNotificationLayer
{
public:
    std::string subtitle();
    virtual void execute();
    void OnComplete(libgss::ActionResponse* response) {
        log(response->outputs()->toString());
    }
};



class GreetingHistoryTestFetchAll : public BaseNotificationLayer
{
public:
    std::string subtitle();
    virtual void execute();
    void OnComplete(libgss::ActionResponse* response) {
        log(response->outputs()->toString());
    }
};



#endif    // __GREETING_HISTORY_TEST_H__
