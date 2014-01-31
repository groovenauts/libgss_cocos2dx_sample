#ifndef __ARMOR_UPGRADE1_TEST_H__
#define __ARMOR_UPGRADE1_TEST_H__

#include "../KeyboardNotificationLayer.h"
#include "../GroundLayer.h"
#include <libGSS/libGSS.h>

class BaseNotificationLayer;

/**
@brief    ArmorUpgrade1Test for retain prev, reset, next, main menu buttons.
*/
class ArmorUpgrade1Test : public GroundLayer
{
public:
    void nextCallback(CCObject* pSender);
    void backCallback(CCObject* pSender);

    std::string title();
};

class ArmorUpgrade1TestScene : public TestScene
{
public:
    virtual void runThisTest();
};



class ArmorUpgrade1TestJustGet : public KeyboardNotificationLayer
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

protected:
    virtual std::string defaultValue();

};


#endif    // __ARMOR_UPGRADE1_TEST_H__
