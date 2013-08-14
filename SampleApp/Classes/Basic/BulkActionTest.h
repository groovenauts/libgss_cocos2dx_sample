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
        if (response->success()) {
            CCLOG("%s", response->outputs()->toString().c_str());
        }
        else if(response->existsNewerVersionClient()){
            CCLOG("New version client has released.");
        }
        else{
            CCLOG("Error has occured.");
        }
    }
};

class BulkActionTest2Success1Error : public BaseNotificationLayer
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
            CCLOG("Error has occured.");
        }
    }
};




#endif    // __BULK_ACTION_TEST_H__
