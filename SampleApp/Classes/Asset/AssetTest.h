#ifndef __ASSET_TEST_H__
#define __ASSET_TEST_H__

#include "../BaseNotificationLayer.h"
#include "../GroundLayer.h"
#include <libGSS/libGSS.h>

class BaseNotificationLayer;

/**
@brief    AssetTest for retain prev, reset, next, main menu buttons.
*/
class AssetTest : public GroundLayer
{
public:
    void nextCallback(CCObject* pSender);
    void backCallback(CCObject* pSender);

    std::string title();
};

class AssetTestScene : public TestScene
{
public:
    virtual void runThisTest();
};


class AssetTestBasic : public BaseNotificationLayer
{
public:
    std::string subtitle();
    virtual void execute();
    void OnComplete(libgss::AssetResponse* response);
};


class AssetTestWithNoCallback : public BaseNotificationLayer
{
    int pollingCount_;
    libgss::AssetRequest* request_;
public:
    std::string subtitle();
    virtual void execute();
    void poll(float time);
};


#endif    // __ASSET_TEST_H__
