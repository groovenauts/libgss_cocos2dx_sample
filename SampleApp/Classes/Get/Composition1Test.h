#ifndef __COMPOSITION1_TEST_H__
#define __COMPOSITION1_TEST_H__

#include "../KeyboardNotificationLayer.h"
#include "../GroundLayer.h"
#include <libGSS/libGSS.h>

class BaseNotificationLayer;

/**
@brief    Composition1Test for retain prev, reset, next, main menu buttons.
*/
class Composition1Test : public GroundLayer
{
public:
    void nextCallback(CCObject* pSender);
    void backCallback(CCObject* pSender);

    std::string title();
};

class Composition1TestScene : public TestScene
{
public:
    virtual void runThisTest();
};



class Composition1TestJustGet : public KeyboardNotificationLayer
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
    
protected:
    virtual std::string defaultValue();
};


class Composition1TestDice1 : public BaseNotificationLayer
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


#endif    // __COMPOSITION1_TEST_H__
