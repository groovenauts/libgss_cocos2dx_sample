#ifndef __GAME_DATA_TEST_H__
#define __GAME_DATA_TEST_H__

#include "../BaseNotificationLayer.h"
#include "../KeyboardNotificationLayer.h"
#include "../GroundLayer.h"
#include <libGSS/libGSS.h>

class BaseNotificationLayer;

/**
@brief    GameDataTest for retain prev, reset, next, main menu buttons.
*/
class GameDataTest : public GroundLayer
{
public:
    void nextCallback(CCObject* pSender);
    void backCallback(CCObject* pSender);

    std::string title();
};

class GameDataTestScene : public TestScene
{
public:
    std::string subtitle();
    virtual void runThisTest();
};



class GameDataTestJustGet : public BaseNotificationLayer
{
public:
    std::string subtitle();
    virtual void execute();
    void OnComplete(libgss::ActionResponse* response) {
        log(response->outputs()->toString());
    }
};


class GameDataTestGetAndUpdate : public BaseNotificationLayer
{
public:

    virtual void execute();
    void OnCompleteToGet(libgss::ActionResponse* response);
    
    virtual void update(libgss::JSONObject* gameData){};
    void OnCompleteToUpdate(libgss::ActionResponse* response){
        log(response->outputs()->toString());
    }
};


class GameDataTestDecrementHP : public GameDataTestGetAndUpdate
{
public:
    std::string subtitle();
    virtual void update(libgss::JSONObject* gameData);
};

class GameDataTestIncrementHP : public GameDataTestGetAndUpdate
{
public:
    std::string subtitle();
    virtual void update(libgss::JSONObject* gameData);
};


class GameDataTestDecrementPotion : public GameDataTestGetAndUpdate
{
public:
    std::string subtitle();
    virtual void update(libgss::JSONObject* gameData);
};

class GameDataTestIncrementPotion : public GameDataTestGetAndUpdate
{
public:
    std::string subtitle();
    virtual void update(libgss::JSONObject* gameData);
};


class ExecuteActionTestUseItem : public KeyboardNotificationLayer
{
public:
    std::string subtitle();
    virtual void execute();
    void OnComplete(libgss::ActionResponse* response) {
        log(response->outputs()->toString());
    }
protected:
    std::string defaultValue();
};


#endif    // __GAME_DATA_TEST_H__
