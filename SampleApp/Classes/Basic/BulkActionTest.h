#ifndef __BULK_ACTION_TEST_H__
#define __BULK_ACTION_TEST_H__

#include "../BaseNotificationLayer.h"
#include "../GroundLayer.h"
#include <libGSS/libGSS.h>

class BaseNotificationLayer;

/**
@brief    BulkActionTest for retain prev, reset, next, main menu buttons.
*/
class BulkActionTest : public GroundLayer
{
public:
    void nextCallback(CCObject* pSender);
    void backCallback(CCObject* pSender);

    std::string title();
};

class BulkActionTestScene : public TestScene
{
public:
    virtual void runThisTest();
};


class BulkActionTest3Actions : public BaseNotificationLayer
{
public:
    std::string subtitle();
    virtual void execute();
    void OnComplete(libgss::ActionResponse* response) {
        log(response->outputs()->toString());
    }
};

class BulkActionTest2Success1Error : public BaseNotificationLayer
{
public:
    std::string subtitle();
    virtual void execute();
    void OnComplete(libgss::ActionResponse* response) {
        log(response->outputs()->toString());
    }
};




#endif    // __BULK_ACTION_TEST_H__
