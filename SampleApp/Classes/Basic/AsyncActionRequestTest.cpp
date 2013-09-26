//
// メニュー名
// Async Action REquest : バルクアクションのテスト
//
// 1. Send async action request:
//    アクションの非同期リクエストを送信
//
// 2. Get async action result:
//    非同期リクエストの結果を取得
//

#include "AsyncActionRequestTest.h"

#include <strstream>
#include <libGSS/libGSS.h>
#include <time.h>

//////////////////////////////////////////////////////////////////////////
// local function
//////////////////////////////////////////////////////////////////////////

enum
{
    kAsyncActionRequestTestSendRequest = 0,
    kAsyncActionRequestTestGetResult,
    
    kAsyncActionRequestTestsCount // テスト数
};

static int testIdx = 0;

static cocos2d::CCDictionary asyncRequests;

BaseNotificationLayer* createAsyncActionRequestTest(int nIndex)
{
    switch(nIndex)
    {
        case kAsyncActionRequestTestSendRequest: return new AsyncActionRequestTestSendRequest();
        case kAsyncActionRequestTestGetResult: return new AsyncActionRequestTestGetResult();
        default: return 0;
    }
}

CCLayer* restartAsyncActionRequestTest()
{
    AsyncActionRequestTest* pContainerLayer = new AsyncActionRequestTest;
    pContainerLayer->autorelease();
    
    BaseNotificationLayer* pTestLayer = createAsyncActionRequestTest(testIdx);
    pTestLayer->autorelease();
    pContainerLayer->addBaseNotificationLayer(pTestLayer);
    
    return pContainerLayer;
}

CCLayer* nextAsyncActionRequestTest()
{
    testIdx++;
    testIdx = testIdx % kAsyncActionRequestTestsCount;
    
    return restartAsyncActionRequestTest();
}

CCLayer* backAsyncActionRequestTest()
{
    testIdx--;
    int total = kAsyncActionRequestTestsCount;
    if( testIdx < 0 )
        testIdx += total;
    
    return restartAsyncActionRequestTest();
}

//////////////////////////////////////////////////////////////////////////
// implement AsyncActionRequestTest
//////////////////////////////////////////////////////////////////////////

void AsyncActionRequestTest::nextCallback(CCObject* pSender)
{
    CCScene* s = new AsyncActionRequestTestScene();
    s->addChild( nextAsyncActionRequestTest() );
    CCDirector::sharedDirector()->replaceScene(s);
    s->release();
}

void AsyncActionRequestTest::backCallback(CCObject* pSender)
{
    CCScene* s = new AsyncActionRequestTestScene();
    s->addChild( backAsyncActionRequestTest() );
    CCDirector::sharedDirector()->replaceScene(s);
    s->release();
}

std::string AsyncActionRequestTest::title()
{
    return "Async Action Request sample";
}


std::string AsyncActionRequestTestSendRequest::defaultValue() {
    return "100";
}


//////////////////////////////////////////////////////////////////////////
// implement AsyncActionRequestTestScene
//////////////////////////////////////////////////////////////////////////

void AsyncActionRequestTestScene::runThisTest()
{
    testIdx--;
    CCLayer* pLayer = nextAsyncActionRequestTest();
    addChild(pLayer);
    
    CCDirector::sharedDirector()->replaceScene(this);
}

//////////////////////////////////////////////////////////////////////////
// implement AsyncActionRequestTestSendRequest
//////////////////////////////////////////////////////////////////////////

std::string AsyncActionRequestTestSendRequest::subtitle(){
    return "1. Send async action request";
}

void AsyncActionRequestTestSendRequest::execute(){
    CCLOG("AsyncActionRequestTestSendRequest::execute");
    
    libgss::AsyncActionRequest* request = new libgss::AsyncActionRequest();
    request->setCallback(this, gssActionResponse(AsyncActionRequestTestSendRequest::OnComplete));
    
    libgss::AllAction* action = new libgss::AllAction("Armor");
    
    istrstream istr(text_field->getString());
    int id;
    istr >> id;
    action->setId(id);
    action->orders()->add("armor_id", libgss::Orders::Orders::kAsc);
    action->conditions()->add("offence_pt$gte", 25);
    request->addAction(action);
    
    libgss::Network::instance()->send(request);
    
    asyncRequests.setObject(request, "AsyncActionRequestTestSendRequest");
    
    request->release();
    action->release();
    
    CCLOG("sent request");
}


//////////////////////////////////////////////////////////////////////////
// implement AsyncActionRequestTestGetResult
//////////////////////////////////////////////////////////////////////////

std::string AsyncActionRequestTestGetResult::subtitle(){
    return "2. Get async action result";
}

void AsyncActionRequestTestGetResult::execute(){
    CCLOG("AsyncActionRequestTestGetResult::execute");
    
    libgss::AsyncActionRequest* sentRequest = (libgss::AsyncActionRequest*)asyncRequests.objectForKey("AsyncActionRequestTestSendRequest");
    if (!sentRequest) {
        CCLOG("No async request sent.");
        return;
    }
    
    libgss::AsyncResultRequest* request = new libgss::AsyncResultRequest(sentRequest);
    request->setCallback(this, gssActionResponse(AsyncActionRequestTestGetResult::OnComplete));
    
    libgss::Network::instance()->send(request);
    request->release();
    
    CCLOG("sent request");
}

