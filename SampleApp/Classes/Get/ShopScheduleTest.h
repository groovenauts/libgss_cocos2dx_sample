#ifndef __SHOP_SCHEDULE_TEST_H__
#define __SHOP_SCHEDULE_TEST_H__

#include "../KeyboardNotificationLayer.h"
#include "../GroundLayer.h"
#include <libGSS/libGSS.h>

class BaseNotificationLayer;

/**
@brief    ShopScheduleTest for retain prev, reset, next, main menu buttons.
*/
class ShopScheduleTest : public GroundLayer
{
public:
    void nextCallback(CCObject* pSender);
    void backCallback(CCObject* pSender);

    std::string title();
};

class ShopScheduleTestScene : public TestScene
{
public:
    std::string subtitle();
    virtual void runThisTest();
};



class ShopScheduleTestJustGet : public KeyboardNotificationLayer
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


#endif    // __SHOP_SCHEDULE_TEST_H__
