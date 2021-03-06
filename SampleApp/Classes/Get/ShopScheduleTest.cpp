// #define COCOS2D_DEBUG   1

//
// メニュー名
// Get Schedule : 期間テーブルからの取得
//
// 1. Get ShopSchedule by using GetScheduleAction
//    日時を指定してその日時に該当する値を取得
//


#include "ShopScheduleTest.h"

#include <libGSS/libGSS.h>


//////////////////////////////////////////////////////////////////////////
// local function
//////////////////////////////////////////////////////////////////////////

enum 
{
    kShopScheduleTestJustGet = 0,
    kShopScheduleTestsCount,
}; 

static int testIdx = 0;

BaseNotificationLayer* createShopScheduleTest(int nIndex)
{
    switch(nIndex)
    {
    case kShopScheduleTestJustGet: return new ShopScheduleTestJustGet();
    default: return 0;
    }
}

CCLayer* restartShopScheduleTest()
{
    ShopScheduleTest* pContainerLayer = new ShopScheduleTest;
    pContainerLayer->autorelease();

    BaseNotificationLayer* pTestLayer = createShopScheduleTest(testIdx);
    pTestLayer->autorelease();
    pContainerLayer->addBaseNotificationLayer(pTestLayer);

    return pContainerLayer;
}

CCLayer* nextShopScheduleTest()
{
    testIdx++;
    testIdx = testIdx % kShopScheduleTestsCount;

    return restartShopScheduleTest();
}

CCLayer* backShopScheduleTest()
{
    testIdx--;
    int total = kShopScheduleTestsCount;
    if( testIdx < 0 )
        testIdx += total;    

    return restartShopScheduleTest();
}

//////////////////////////////////////////////////////////////////////////
// implement ShopScheduleTest
//////////////////////////////////////////////////////////////////////////

void ShopScheduleTest::nextCallback(CCObject* pSender)
{
    CCScene* s = new ShopScheduleTestScene();
    s->addChild( nextShopScheduleTest() );
    CCDirector::sharedDirector()->replaceScene(s);
    s->release();
}

void ShopScheduleTest::backCallback(CCObject* pSender)
{
    CCScene* s = new ShopScheduleTestScene();
    s->addChild( backShopScheduleTest() );
    CCDirector::sharedDirector()->replaceScene(s);
    s->release();
}

std::string ShopScheduleTest::title()
{
    return "ShopSchedule sample";
}

//////////////////////////////////////////////////////////////////////////
// implement ShopScheduleTestScene
//////////////////////////////////////////////////////////////////////////

void ShopScheduleTestScene::runThisTest()
{
    testIdx--;
    CCLayer* pLayer = nextShopScheduleTest();
    addChild(pLayer);
    
    CCDirector::sharedDirector()->replaceScene(this);
}


//////////////////////////////////////////////////////////////////////////
// implement ShopScheduleTestJustGet
//////////////////////////////////////////////////////////////////////////

std::string ShopScheduleTestJustGet::subtitle(){
    return "1. Get ShopSchedule by using GetScheduleAction";
}

void ShopScheduleTestJustGet::execute()
{
    CCLOG("ShopScheduleTestNoConditions::execute");
    CCLOG("now sending request with get action.");
    
    libgss::GetScheduleAction* action = new libgss::GetScheduleAction("ShopSchedule");
    std::string s = text_field->getString();
    if (!s.empty()){
        action->setTime(s);
    }
    // action->setPlayerId(kDefaultFindPlayerId);
    libgss::ActionRequest* request = new libgss::ActionRequest();
    request->setCallback(this, gssActionResponse(ShopScheduleTestJustGet::OnComplete));
    request->addAction(action);
    libgss::Network::instance()->send(request);
    request->release();
    action->release();
    
    CCLOG("sent request");
}


std::string ShopScheduleTestJustGet::defaultValue(){
    return "2012/7/20 13:00:00+09:00";
}


