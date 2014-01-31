// #define COCOS2D_DEBUG   1

//
// メニュー名
// Simple Action : 簡単なアクションのテスト
//
// 1. 1 action in 1 request: 1アクションを実行するリクエスト
// 2. no callback pattern  : コールバックなしのパターン
//

#include "SimpleActionTest.h"

#include <libGSS/libGSS.h>
#include <time.h>

//////////////////////////////////////////////////////////////////////////
// local function
//////////////////////////////////////////////////////////////////////////

enum 
{
    kSimpleActionTest1Action = 0,
    kSimpleActionTestWithNoCallback,
    kSimpleActionTestsCount,
}; 

static int testIdx = 0;

BaseNotificationLayer* createSimpleActionTest(int nIndex)
{
    switch(nIndex)
    {
    case kSimpleActionTest1Action: return new SimpleActionTest1Action();
    case kSimpleActionTestWithNoCallback: return new SimpleActionTestWithNoCallback();
    default: return 0;
    }
}

CCLayer* restartSimpleActionTest()
{
    SimpleActionTest* pContainerLayer = new SimpleActionTest;
    pContainerLayer->autorelease();

    BaseNotificationLayer* pTestLayer = createSimpleActionTest(testIdx);
    pTestLayer->autorelease();
    pContainerLayer->addBaseNotificationLayer(pTestLayer);

    return pContainerLayer;
}

CCLayer* nextSimpleActionTest()
{
    testIdx++;
    testIdx = testIdx % kSimpleActionTestsCount;

    return restartSimpleActionTest();
}

CCLayer* backSimpleActionTest()
{
    testIdx--;
    int total = kSimpleActionTestsCount;
    if( testIdx < 0 )
        testIdx += total;    

    return restartSimpleActionTest();
}

//////////////////////////////////////////////////////////////////////////
// implement SimpleActionTest
//////////////////////////////////////////////////////////////////////////

void SimpleActionTest::nextCallback(CCObject* pSender)
{
    CCScene* s = new SimpleActionTestScene();
    s->addChild( nextSimpleActionTest() );
    CCDirector::sharedDirector()->replaceScene(s);
    s->release();
}

void SimpleActionTest::backCallback(CCObject* pSender)
{
    CCScene* s = new SimpleActionTestScene();
    s->addChild( backSimpleActionTest() );
    CCDirector::sharedDirector()->replaceScene(s);
    s->release();
}

std::string SimpleActionTest::title()
{
    return "Player sample";
}

//////////////////////////////////////////////////////////////////////////
// implement SimpleActionTestScene
//////////////////////////////////////////////////////////////////////////

void SimpleActionTestScene::runThisTest()
{
    testIdx--;
    CCLayer* pLayer = nextSimpleActionTest();
    addChild(pLayer);
    
    CCDirector::sharedDirector()->replaceScene(this);
}

//////////////////////////////////////////////////////////////////////////
// implement SimpleActionTest1Action
//////////////////////////////////////////////////////////////////////////

std::string SimpleActionTest1Action::subtitle(){
    return "1. 1 action in 1 request";
}

void SimpleActionTest1Action::execute(){
    CCLOG("SimpleActionTest1Action::execute");
    
    libgss::ActionRequest* request = new libgss::ActionRequest();
    request->setCallback(this, gssActionResponse(SimpleActionTest1Action::OnComplete));
    
    libgss::AllAction* action = new libgss::AllAction("Armor");
    action->setId(1);
    action->orders()->add("armor_id", libgss::Orders::Orders::kAsc);
    action->conditions()->add("offence_pt$gte", 25);
    request->addAction(action);
    
    libgss::Network::instance()->send(request);
    request->release();
    action->release();
    
    CCLOG("sent request");
}


//////////////////////////////////////////////////////////////////////////
// implement SimpleActionTestWithNoCallback
//////////////////////////////////////////////////////////////////////////

std::string SimpleActionTestWithNoCallback::subtitle(){
    return "2. no callback pattern";
}


void SimpleActionTestWithNoCallback::execute(){
    CCLOG("SimpleActionTestWithNoCallback::execute");
    
    CCLOG("now sending request with all action.");
    
    request_ = new libgss::ActionRequest();
    
    libgss::AllAction* action = new libgss::AllAction("Armor");
    action->setId(1);
    action->orders()->add("armor_id", libgss::Orders::Orders::kAsc);
    action->conditions()->add("offence_pt$gte", 25);
    request_->addAction(action);
    
    libgss::Network::instance()->send(request_);
    request_->release();
    action->release();
    
    pollingCount_ = 0;
    
    this->schedule(schedule_selector(SimpleActionTestWithNoCallback::poll));
    
    CCLOG("sent request");    
}

void SimpleActionTestWithNoCallback::poll(float time){
    CCLOG("polling response. (%d)", ++pollingCount_);
    
    libgss::ActionResponse* response = request_->getResponseIfFinished();
    if (response == NULL) {
        CCLOG("not received response yet.");
        return;
    }
    
    CCLOG("received response.");
    
    this->unschedule(schedule_selector(SimpleActionTestWithNoCallback::poll));
    
    if (response->success()) {
        CCLOG("%s", response->outputs()->toString().c_str());
    }
    else if(response->existsNewerVersionClient()){
        CCLOG("New version client has released.");
    }
    else{
        CCLOG("Error has occured. %d\n%s", response->code(), response->message().c_str());
    }
    
    request_->release();
}
