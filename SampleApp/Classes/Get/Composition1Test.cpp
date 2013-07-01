// #define COCOS2D_DEBUG   1

//
// メニュー名
// Get/Dice Probability : 確率テーブルからの取得と抽選
//
// 1. Get Composition1 by using GetProbabilityAction
//    ある結果について確率テーブルから情報を取得する
//
// 2. Dice for {\"10001\": 3} by using DiceAction
//    確率抽選
//

#include "Composition1Test.h"

#include <libGSS/libGSS.h>


//////////////////////////////////////////////////////////////////////////
// local function
//////////////////////////////////////////////////////////////////////////

enum 
{
    kComposition1TestDice1 = 0,
    kComposition1TestJustGet,
    kComposition1TestsCount,
}; 

static int testIdx = 0;

BaseNotificationLayer* createComposition1Test(int nIndex)
{
    switch(nIndex)
    {
    case kComposition1TestDice1: return new Composition1TestDice1();
    case kComposition1TestJustGet: return new Composition1TestJustGet();
    default: return 0;
    }
}

CCLayer* restartComposition1Test()
{
    Composition1Test* pContainerLayer = new Composition1Test;
    pContainerLayer->autorelease();

    BaseNotificationLayer* pTestLayer = createComposition1Test(testIdx);
    pTestLayer->autorelease();
    pContainerLayer->addBaseNotificationLayer(pTestLayer);

    return pContainerLayer;
}

CCLayer* nextComposition1Test()
{
    testIdx++;
    testIdx = testIdx % kComposition1TestsCount;

    return restartComposition1Test();
}

CCLayer* backComposition1Test()
{
    testIdx--;
    int total = kComposition1TestsCount;
    if( testIdx < 0 )
        testIdx += total;    

    return restartComposition1Test();
}

//////////////////////////////////////////////////////////////////////////
// implement Composition1Test
//////////////////////////////////////////////////////////////////////////

void Composition1Test::nextCallback(CCObject* pSender)
{
    CCScene* s = new Composition1TestScene();
    s->addChild( nextComposition1Test() );
    CCDirector::sharedDirector()->replaceScene(s);
    s->release();
}

void Composition1Test::backCallback(CCObject* pSender)
{
    CCScene* s = new Composition1TestScene();
    s->addChild( backComposition1Test() );
    CCDirector::sharedDirector()->replaceScene(s);
    s->release();
}

std::string Composition1Test::title()
{
    return "Composition1 sample";
}

//////////////////////////////////////////////////////////////////////////
// implement Composition1TestScene
//////////////////////////////////////////////////////////////////////////

void Composition1TestScene::runThisTest()
{
    testIdx--;
    CCLayer* pLayer = nextComposition1Test();
    addChild(pLayer);
    
    CCDirector::sharedDirector()->replaceScene(this);
}


//////////////////////////////////////////////////////////////////////////
// implement Composition1TestJustGet
//////////////////////////////////////////////////////////////////////////

std::string Composition1TestJustGet::subtitle(){
    return "1. Get Composition1 by using GetProbabilityAction";
}

void Composition1TestJustGet::execute()
{
    CCLOG("Composition1TestNoConditions::execute");
    CCLOG("now sending request with get action.");
    
    libgss::GetProbabilityAction* action = new libgss::GetProbabilityAction("Composition1", text_field->getString());
    // action->setPlayerId(kDefaultFindPlayerId);
    libgss::ActionRequest* request = new libgss::ActionRequest();
    request->setCallback(this, gssActionResponse(Composition1TestJustGet::OnComplete));
    request->addAction(action);
    libgss::Network::instance()->send(request);
    request->release();
    action->release();
    
    CCLOG("sent request");
}


std::string Composition1TestJustGet::defaultValue(){
    return "10002";
}




//////////////////////////////////////////////////////////////////////////
// implement Composition1TestDice1
//////////////////////////////////////////////////////////////////////////

std::string Composition1TestDice1::subtitle(){
    return "2. Dice for {\"10001\": 3} by using DiceAction";
}

void Composition1TestDice1::execute()
{
    CCLOG("Composition1TestDice1::execute");
    CCLOG("now sending request with get action.");
    
    libgss::JSONObject obj = new libgss::JSONObject();
    obj.set("10001", 3);
    
    libgss::DiceAction* action = new libgss::DiceAction("Composition1");
    action->conditions()->add("element", obj);
    // action->setPlayerId(kDefaultFindPlayerId);
    libgss::ActionRequest* request = new libgss::ActionRequest();
    request->setCallback(this, gssActionResponse(Composition1TestDice1::OnComplete));
    request->addAction(action);
    libgss::Network::instance()->send(request);
    request->release();
    action->release();
    
    CCLOG("sent request");
}

