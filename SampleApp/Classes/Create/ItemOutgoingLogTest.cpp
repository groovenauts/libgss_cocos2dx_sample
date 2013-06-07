// #define COCOS2D_DEBUG   1

//
// メニュー名
// Create Log : ログの登録
//
// 1. Create ItemOutgoingLog by using CreateAction
//    ログの登録
//

#include "ItemOutgoingLogTest.h"

#include <libGSS/libGSS.h>
#include <time.h>

//////////////////////////////////////////////////////////////////////////
// local function
//////////////////////////////////////////////////////////////////////////

enum 
{
    kItemOutgoingLogTestCreate = 0,
    kItemOutgoingLogTestsCount,
}; 

static int testIdx = -1; 

BaseNotificationLayer* createItemOutgoingLogTest(int nIndex)
{
    switch(nIndex)
    {
    case kItemOutgoingLogTestCreate: return new ItemOutgoingLogTestCreate();
    default: return 0;
    }
}

CCLayer* restartItemOutgoingLogTest()
{
    ItemOutgoingLogTest* pContainerLayer = new ItemOutgoingLogTest;
    pContainerLayer->autorelease();

    BaseNotificationLayer* pTestLayer = createItemOutgoingLogTest(testIdx);
    pTestLayer->autorelease();
    pContainerLayer->addBaseNotificationLayer(pTestLayer);

    return pContainerLayer;
}

CCLayer* nextItemOutgoingLogTest()
{
    testIdx++;
    testIdx = testIdx % kItemOutgoingLogTestsCount;

    return restartItemOutgoingLogTest();
}

CCLayer* backItemOutgoingLogTest()
{
    testIdx--;
    int total = kItemOutgoingLogTestsCount;
    if( testIdx < 0 )
        testIdx += total;    

    return restartItemOutgoingLogTest();
}

//////////////////////////////////////////////////////////////////////////
// implement ItemOutgoingLogTest
//////////////////////////////////////////////////////////////////////////

void ItemOutgoingLogTest::nextCallback(CCObject* pSender)
{
    CCScene* s = new ItemOutgoingLogTestScene();
    s->addChild( nextItemOutgoingLogTest() );
    CCDirector::sharedDirector()->replaceScene(s);
    s->release();
}

void ItemOutgoingLogTest::backCallback(CCObject* pSender)
{
    CCScene* s = new ItemOutgoingLogTestScene();
    s->addChild( backItemOutgoingLogTest() );
    CCDirector::sharedDirector()->replaceScene(s);
    s->release();
}

std::string ItemOutgoingLogTest::title()
{
    return "Player sample";
}

//////////////////////////////////////////////////////////////////////////
// implement ItemOutgoingLogTestScene
//////////////////////////////////////////////////////////////////////////

void ItemOutgoingLogTestScene::runThisTest()
{
    CCLayer* pLayer = nextItemOutgoingLogTest();
    addChild(pLayer);
    
    CCDirector::sharedDirector()->replaceScene(this);
}

//////////////////////////////////////////////////////////////////////////
// implement ItemOutgoingLogTestCreate
//////////////////////////////////////////////////////////////////////////

std::string ItemOutgoingLogTestCreate::subtitle(){
    return "1. Create ItemOutgoingLog by using CreateAction";
}

void ItemOutgoingLogTestCreate::execute(){
    CCLOG("ItemOutgoingLogTestDecrementLevel::execute");
    CCLOG("now sending request with get action.");
    libgss::JSONObject attrs;
    // libgss::Network* network = libgss::Network::instance();
    // attrs.set("player_id", network->playerId());
    attrs.set("created_at", time(NULL));
    attrs.set("level", 5);
    attrs.set("item_cd", textFieldAsInt());
    attrs.set("outgoing_route_cd", 1);
    attrs.set("amount", 1);
    
    libgss::CreateAction* action = new libgss::CreateAction("ItemOutgoingLog", attrs);
    // action->setPlayerId(kDefaultFindPlayerId);
    libgss::ActionRequest* request = new libgss::ActionRequest(this, gssActionResponse(ItemOutgoingLogTestCreate::OnComplete));
    request->addAction(action);
    libgss::Network::instance()->send(request);
    request->release();
    action->release();
    
    CCLOG("sent request successfully");
}

std::string ItemOutgoingLogTestCreate::defaultValue(){
    return "20001";
}

