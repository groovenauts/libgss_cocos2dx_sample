#ifndef __SIMPLE_ACTION_TEST_H__
#define __SIMPLE_ACTION_TEST_H__

#include "../BaseNotificationLayer.h"
#include "../GroundLayer.h"
#include <libGSS/libGSS.h>

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
    std::string subtitle();
    virtual void runThisTest();
};


class SimpleActionTest1Action : public BaseNotificationLayer
{
public:
    std::string subtitle();
    virtual void execute();
    void OnComplete(libgss::ActionResponse* response) {
        log(response->outputs()->toString());
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
