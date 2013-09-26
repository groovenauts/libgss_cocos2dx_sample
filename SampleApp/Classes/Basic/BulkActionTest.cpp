// #define COCOS2D_DEBUG   1

//
// メニュー名
// Bulk Action : バルクアクションのテスト
//
// 1. 3 actions in 1 request
//    3つのアクションを1リクエストで送信するパターン
//
// 2. 2 success actions and 1 error action in 1 request:
//    ３つのアクションを１つのリクエストで送信し、そのうち２つは成功するが１つは失敗するパターン
// 


#include "BulkActionTest.h"

#include <libGSS/libGSS.h>
#include <time.h>

//////////////////////////////////////////////////////////////////////////
// local function
//////////////////////////////////////////////////////////////////////////

enum 
{
    kBulkActionTest3Actions = 0,
    kBulkActionTest2Success1Error,
    kBulkActionTest3ActionsIncludesSsl,
    
    kBulkActionTestsCount,
}; 

static int testIdx = 0;

BaseNotificationLayer* createBulkActionTest(int nIndex)
{
    switch(nIndex)
    {
    case kBulkActionTest3Actions: return new BulkActionTest3Actions();
    case kBulkActionTest2Success1Error: return new BulkActionTest2Success1Error();
    case kBulkActionTest3ActionsIncludesSsl: return new BulkActionTest3ActionsIncludesSsl();
    default: return 0;
    }
}

CCLayer* restartBulkActionTest()
{
    BulkActionTest* pContainerLayer = new BulkActionTest;
    pContainerLayer->autorelease();

    BaseNotificationLayer* pTestLayer = createBulkActionTest(testIdx);
    pTestLayer->autorelease();
    pContainerLayer->addBaseNotificationLayer(pTestLayer);

    return pContainerLayer;
}

CCLayer* nextBulkActionTest()
{
    testIdx++;
    testIdx = testIdx % kBulkActionTestsCount;

    return restartBulkActionTest();
}

CCLayer* backBulkActionTest()
{
    testIdx--;
    int total = kBulkActionTestsCount;
    if( testIdx < 0 )
        testIdx += total;    

    return restartBulkActionTest();
}

//////////////////////////////////////////////////////////////////////////
// implement BulkActionTest
//////////////////////////////////////////////////////////////////////////

void BulkActionTest::nextCallback(CCObject* pSender)
{
    CCScene* s = new BulkActionTestScene();
    s->addChild( nextBulkActionTest() );
    CCDirector::sharedDirector()->replaceScene(s);
    s->release();
}

void BulkActionTest::backCallback(CCObject* pSender)
{
    CCScene* s = new BulkActionTestScene();
    s->addChild( backBulkActionTest() );
    CCDirector::sharedDirector()->replaceScene(s);
    s->release();
}

std::string BulkActionTest::title()
{
    return "Bulk action sample";
}

//////////////////////////////////////////////////////////////////////////
// implement BulkActionTestScene
//////////////////////////////////////////////////////////////////////////

void BulkActionTestScene::runThisTest()
{
    testIdx--;
    CCLayer* pLayer = nextBulkActionTest();
    addChild(pLayer);
    
    CCDirector::sharedDirector()->replaceScene(this);
}

//////////////////////////////////////////////////////////////////////////
// implement BulkActionTest3Actions
//////////////////////////////////////////////////////////////////////////

std::string BulkActionTest3Actions::subtitle(){
    return "1. 3 actions in 1 request";
}

void BulkActionTest3Actions::execute(){
    CCLOG("BulkActionTest3Actions::execute");
    CCLOG("now sending request with bulk actions.");
    
    libgss::ActionRequest* request = new libgss::ActionRequest();
    request->setCallback(this, gssActionResponse(BulkActionTest3Actions::OnComplete));
    
    libgss::AllAction* action1 = new libgss::AllAction("Armor");
    action1->setId(1);
    action1->orders()->add("armor_id", libgss::Orders::Orders::kAsc);
    action1->conditions()->add("offence_pt$gte", 25);
    request->addAction(action1);
    
    libgss::GetScheduleAction* action2 = new libgss::GetScheduleAction("ShopSchedule");
    action2->setId(2);
    request->addAction(action2);
    
    libgss::JSONObject attrs;
    libgss::Network* network = libgss::Network::instance();
    attrs.set("sender_cd", network->playerId());
    attrs.set("receiver_cd", "fontana:1000002");
    attrs.set("send_at", time(NULL));
    attrs.set("greeting_cd", 1);
    libgss::CreateAction* action3 = new libgss::CreateAction("GreetingHistory", attrs);
    action3->setId(3);
    request->addAction(action3);
    
    libgss::Network::instance()->send(request);
    request->release();
    action1->release();
    action2->release();
    action3->release();
    
    CCLOG("sent request");
}

//////////////////////////////////////////////////////////////////////////
// implement BulkActionTest2Success1Error
//////////////////////////////////////////////////////////////////////////

std::string BulkActionTest2Success1Error::subtitle(){
    return "2. 2 success actions and 1 error action in 1 request";
}

void BulkActionTest2Success1Error::execute(){
    CCLOG("BulkActionTest2Success1Error::execute");
    CCLOG("now sending request with bulk actions.");
    
    libgss::ActionRequest* request = new libgss::ActionRequest();
    request->setCallback(this, gssActionResponse(BulkActionTest2Success1Error::OnComplete));
    
    libgss::AllAction* action1 = new libgss::AllAction("Armor");
    action1->setId(1);
    action1->orders()->add("armor_id", libgss::Orders::Orders::kAsc);
    action1->conditions()->add("offence_pt$gte", 25);
    request->addAction(action1);
    
    libgss::GetScheduleAction* action2 = new libgss::GetScheduleAction("UnplannedSchedule");
    action2->setId(2);
    request->addAction(action2);
    
    libgss::JSONObject attrs;
    libgss::Network* network = libgss::Network::instance();
    attrs.set("sender_cd", network->playerId());
    attrs.set("receiver_cd", "fontana:1000002");
    attrs.set("send_at", time(NULL));
    attrs.set("greeting_cd", 1);
    libgss::CreateAction* action3 = new libgss::CreateAction("GreetingHistory", attrs);
    action3->setId(3);
    request->addAction(action3);
    
    libgss::Network::instance()->send(request);
    request->release();
    action1->release();
    action2->release();
    action3->release();
    
    CCLOG("sent request");
}



//////////////////////////////////////////////////////////////////////////
// implement BulkActionTest3ActionsIncludesSsl
//////////////////////////////////////////////////////////////////////////

std::string BulkActionTest3ActionsIncludesSsl::subtitle(){
    return "3. 3 actions. And 1 of them requires SSL.";
}

void BulkActionTest3ActionsIncludesSsl::execute(){
    CCLOG("BulkActionTest3ActionsIncludesSsl::execute");
    CCLOG("now sending request with bulk actions.");
    
    libgss::ActionRequest* request = new libgss::ActionRequest();
    request->setCallback(this, gssActionResponse(BulkActionTest3ActionsIncludesSsl::OnComplete));
    
    libgss::AllAction* action1 = new libgss::AllAction("Armor");
    action1->setId(1);
    action1->orders()->add("armor_id", libgss::Orders::Orders::kAsc);
    action1->conditions()->add("offence_pt$gte", 25);
    request->addAction(action1);
    
    libgss::GetScheduleAction* action2 = new libgss::GetScheduleAction("ShopSchedule");
    action2->setId(2);
    action2->setSslRequired(true);
    request->addAction(action2);
    
    libgss::JSONObject attrs;
    libgss::Network* network = libgss::Network::instance();
    attrs.set("sender_cd", network->playerId());
    attrs.set("receiver_cd", "fontana:1000002");
    attrs.set("send_at", time(NULL));
    attrs.set("greeting_cd", 1);
    libgss::CreateAction* action3 = new libgss::CreateAction("GreetingHistory", attrs);
    action3->setId(3);
    request->addAction(action3);
    
    libgss::Network::instance()->send(request);
    request->release();
    action1->release();
    action2->release();
    action3->release();
    
    CCLOG("sent request");
}

