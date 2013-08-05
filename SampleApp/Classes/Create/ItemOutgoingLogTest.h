#ifndef __ITEM_OUTGOING_LOG_TEST_H__
#define __ITEM_OUTGOING_LOG_TEST_H__

#include "../KeyboardNotificationLayer.h"
#include "../GroundLayer.h"
#include <libGSS/libGSS.h>

class BaseNotificationLayer;

/**
@brief    ItemOutgoingLogTest for retain prev, reset, next, main menu buttons.
*/
class ItemOutgoingLogTest : public GroundLayer
{
public:
    void nextCallback(CCObject* pSender);
    void backCallback(CCObject* pSender);

    std::string title();
};

class ItemOutgoingLogTestScene : public TestScene
{
public:
    virtual void runThisTest();
};


class ItemOutgoingLogTestCreate : public KeyboardNotificationLayer
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
protected:
    virtual std::string defaultValue();
};


#endif    // __ITEM_OUTGOING_LOG_TEST_H__
