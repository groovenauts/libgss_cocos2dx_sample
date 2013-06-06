#ifndef __PLAYER_TEST_H__
#define __PLAYER_TEST_H__

#include "../BaseNotificationLayer.h"
#include "../GroundLayer.h"
#include <libGSS/libGSS.h>

class BaseNotificationLayer;

/**
@brief    PlayerTest for retain prev, reset, next, main menu buttons.
*/
class PlayerTest : public GroundLayer
{
public:
    void nextCallback(CCObject* pSender);
    void backCallback(CCObject* pSender);

    std::string title();
};

class PlayerTestScene : public TestScene
{
public:
    std::string subtitle();
    virtual void runThisTest();
};



class PlayerTestJustGet : public BaseNotificationLayer
{
public:
    std::string subtitle();
    virtual void execute();
    void OnComplete(libgss::ActionResponse* response) {
        log(response->outputs()->toString());
    }
};


class PlayerTestGetAndUpdate : public BaseNotificationLayer
{
public:

    virtual void execute();
    void OnCompleteToGet(libgss::ActionResponse* response);
    
    virtual void update(libgss::JSONObject* player){};
    void OnCompleteToUpdate(libgss::ActionResponse* response){
        log(response->outputs()->toString());
    }
};


class PlayerTestDecrementLevel : public PlayerTestGetAndUpdate
{
public:
    std::string subtitle();
    virtual void update(libgss::JSONObject* player);
};

class PlayerTestIncrementLevel : public PlayerTestGetAndUpdate
{
public:
    std::string subtitle();
    virtual void update(libgss::JSONObject* player);
};




#endif    // __PLAYER_TEST_H__
