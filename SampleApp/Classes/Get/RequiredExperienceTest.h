#ifndef __REQUIRED_EXPERIENCE_TEST_H__
#define __REQUIRED_EXPERIENCE_TEST_H__

#include "../KeyboardNotificationLayer.h"
#include "../GroundLayer.h"
#include <libGSS/libGSS.h>

class BaseNotificationLayer;

/**
@brief    RequiredExperienceTest for retain prev, reset, next, main menu buttons.
*/
class RequiredExperienceTest : public GroundLayer
{
public:
    void nextCallback(CCObject* pSender);
    void backCallback(CCObject* pSender);

    std::string title();
};

class RequiredExperienceTestScene : public TestScene
{
public:
    std::string subtitle();
    virtual void runThisTest();
};



class RequiredExperienceTestJustGet : public KeyboardNotificationLayer
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


#endif    // __REQUIRED_EXPERIENCE_TEST_H__
