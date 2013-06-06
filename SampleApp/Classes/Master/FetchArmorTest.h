#ifndef __FETCH_ARMOR_TEST_H__
#define __FETCH_ARMOR_TEST_H__

#include "../BaseNotificationLayer.h"
#include "../KeyboardNotificationLayer.h"
#include "../GroundLayer.h"
#include <libGSS/libGSS.h>

class BaseNotificationLayer;

/**
@brief    FetchArmorTest for retain prev, reset, next, main menu buttons.
*/
class FetchArmorTest : public GroundLayer
{
public:
    void nextCallback(CCObject* pSender);
    void backCallback(CCObject* pSender);

    std::string title();
};

class FetchArmorTestScene : public TestScene
{
public:
    std::string subtitle();
    virtual void runThisTest();
};



class FetchArmorAction : public BaseNotificationLayer
{
public:
    virtual void execute();
    virtual void buildCondition(libgss::Conditions* conditions){};
    
    void OnComplete(libgss::ActionResponse* response);
};



class FetchArmorTestNoConditions : public FetchArmorAction
{
public:
    std::string subtitle();
    void buildCondition(libgss::Conditions* conditions);
};

class FetchArmorTestSimpleConditions : public FetchArmorAction
{
public:
    std::string subtitle();
    void buildCondition(libgss::Conditions* conditions);
};

class FetchArmorTestAndJoined : public FetchArmorAction
{
public:
    std::string subtitle();
    void buildCondition(libgss::Conditions* conditions);
};

class FetchArmorTestOrJoined : public FetchArmorAction
{
public:
    std::string subtitle();
    void buildCondition(libgss::Conditions* conditions);
};

class FetchArmorTestNestedConditions : public FetchArmorAction
{
public:
    std::string subtitle();
    void buildCondition(libgss::Conditions* conditions);
};


class FetchArmorPaginateAction : public KeyboardNotificationLayer
{
public:
    std::string subtitle();
    virtual void execute();
    void OnComplete(libgss::ActionResponse* response);
protected:
    virtual std::string defaultValue();
};



#endif    // __FETCH_ARMOR_TEST_H__
