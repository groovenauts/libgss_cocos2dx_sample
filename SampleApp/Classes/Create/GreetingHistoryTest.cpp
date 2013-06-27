// #define COCOS2D_DEBUG   1

//
// メニュー名
// Create/All History : 履歴の登録と取得
//
// 1. Create GreetingHistory by using CreateAction
//    履歴の登録
//
// 2. Fetch akk GreetingHistory by using AllAction
//    履歴の取得
//

#include "GreetingHistoryTest.h"

#include <libGSS/libGSS.h>
#include <time.h>

//////////////////////////////////////////////////////////////////////////
// local function
//////////////////////////////////////////////////////////////////////////

enum 
{
    kGreetingHistoryTestCreate = 0,
    kGreetingHistoryTestFetchAll,
    kGreetingHistoryTestsCount,
}; 

static int testIdx = -1; 

BaseNotificationLayer* createGreetingHistoryTest(int nIndex)
{
    switch(nIndex)
    {
    case kGreetingHistoryTestCreate: return new GreetingHistoryTestCreate();
    case kGreetingHistoryTestFetchAll: return new GreetingHistoryTestFetchAll();
    default: return 0;
    }
}

CCLayer* restartGreetingHistoryTest()
{
    GreetingHistoryTest* pContainerLayer = new GreetingHistoryTest;
    pContainerLayer->autorelease();

    BaseNotificationLayer* pTestLayer = createGreetingHistoryTest(testIdx);
    pTestLayer->autorelease();
    pContainerLayer->addBaseNotificationLayer(pTestLayer);

    return pContainerLayer;
}

CCLayer* nextGreetingHistoryTest()
{
    testIdx++;
    testIdx = testIdx % kGreetingHistoryTestsCount;

    return restartGreetingHistoryTest();
}

CCLayer* backGreetingHistoryTest()
{
    testIdx--;
    int total = kGreetingHistoryTestsCount;
    if( testIdx < 0 )
        testIdx += total;    

    return restartGreetingHistoryTest();
}

//////////////////////////////////////////////////////////////////////////
// implement GreetingHistoryTest
//////////////////////////////////////////////////////////////////////////

void GreetingHistoryTest::nextCallback(CCObject* pSender)
{
    CCScene* s = new GreetingHistoryTestScene();
    s->addChild( nextGreetingHistoryTest() );
    CCDirector::sharedDirector()->replaceScene(s);
    s->release();
}

void GreetingHistoryTest::backCallback(CCObject* pSender)
{
    CCScene* s = new GreetingHistoryTestScene();
    s->addChild( backGreetingHistoryTest() );
    CCDirector::sharedDirector()->replaceScene(s);
    s->release();
}

std::string GreetingHistoryTest::title()
{
    return "Player sample";
}

//////////////////////////////////////////////////////////////////////////
// implement GreetingHistoryTestScene
//////////////////////////////////////////////////////////////////////////

void GreetingHistoryTestScene::runThisTest()
{
    CCLayer* pLayer = nextGreetingHistoryTest();
    addChild(pLayer);
    
    CCDirector::sharedDirector()->replaceScene(this);
}

//////////////////////////////////////////////////////////////////////////
// implement GreetingHistoryTestCreate
//////////////////////////////////////////////////////////////////////////

std::string GreetingHistoryTestCreate::subtitle(){
    return "1. Create GreetingHistory by using CreateAction";
}

void GreetingHistoryTestCreate::execute(){
    CCLOG("GreetingHistoryTestDecrementLevel::execute");
    CCLOG("now sending request with get action.");
    libgss::Network* network = libgss::Network::instance();
    libgss::JSONObject attrs;
    attrs.set("sender_cd", network->playerId());
    attrs.set("receiver_cd", "fontana:1000002");
    attrs.set("send_at", time(NULL));
    attrs.set("greeting_cd", 1);
    
    libgss::CreateAction* action = new libgss::CreateAction("GreetingHistory", attrs);
    // action->setPlayerId(kDefaultFindPlayerId);
    libgss::ActionRequest* request = new libgss::ActionRequest();
    request->setCallback(this, gssActionResponse(GreetingHistoryTestCreate::OnComplete));
    request->addAction(action);
    libgss::Network::instance()->send(request);
    request->release();
    action->release();
    
    CCLOG("sent request successfully");
}



//////////////////////////////////////////////////////////////////////////
// implement GreetingHistoryTestFetchAll
//////////////////////////////////////////////////////////////////////////

std::string GreetingHistoryTestFetchAll::subtitle(){
    return "2. Fetch akk GreetingHistory by using AllAction";
}

void GreetingHistoryTestFetchAll::execute()
{
    CCLOG("GreetingHistoryTestNoConditions::execute");
    CCLOG("now sending request with all action.");
    
    libgss::AllAction* action = new libgss::AllAction("GreetingHistory");
    // action->setPlayerId(kDefaultFindPlayerId);
    libgss::ActionRequest* request = new libgss::ActionRequest();
    request->setCallback(this, gssActionResponse(GreetingHistoryTestFetchAll::OnComplete));
    request->addAction(action);
    libgss::Network::instance()->send(request);
    request->release();
    action->release();
    
    CCLOG("sent request successfully");
}





