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
    std::string subtitle();
    virtual void runThisTest();
};



class Composition1TestJustGet : public KeyboardNotificationLayer
{
public:
    std::string subtitle();
    virtual void execute();
    void OnComplete(libgss::ActionResponse* response) {
        log(response->outputs()->toString());
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
        log(response->outputs()->toString());
    }
};


#endif    // __COMPOSITION1_TEST_H__
