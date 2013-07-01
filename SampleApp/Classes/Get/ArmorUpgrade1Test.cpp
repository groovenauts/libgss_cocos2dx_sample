// #define COCOS2D_DEBUG   1

//
// メニュー名
// Get Dictionary : 辞書テーブルからの取得
//
// 1. Get ArmorUpgrade1 by using GetDictionaryAction
//    入力を指定して辞書テーブルから出力を取得する
//

#include "ArmorUpgrade1Test.h"

#include <libGSS/libGSS.h>


//////////////////////////////////////////////////////////////////////////
// local function
//////////////////////////////////////////////////////////////////////////

enum 
{
    kArmorUpgrade1TestJustGet = 0,
    kArmorUpgrade1TestsCount,
}; 

static int testIdx = 0;

BaseNotificationLayer* createArmorUpgrade1Test(int nIndex)
{
    switch(nIndex)
    {
    case kArmorUpgrade1TestJustGet: return new ArmorUpgrade1TestJustGet();
    default: return 0;
    }
}

CCLayer* restartArmorUpgrade1Test()
{
    ArmorUpgrade1Test* pContainerLayer = new ArmorUpgrade1Test;
    pContainerLayer->autorelease();

    BaseNotificationLayer* pTestLayer = createArmorUpgrade1Test(testIdx);
    pTestLayer->autorelease();
    pContainerLayer->addBaseNotificationLayer(pTestLayer);

    return pContainerLayer;
}

CCLayer* nextArmorUpgrade1Test()
{
    testIdx++;
    testIdx = testIdx % kArmorUpgrade1TestsCount;

    return restartArmorUpgrade1Test();
}

CCLayer* backArmorUpgrade1Test()
{
    testIdx--;
    int total = kArmorUpgrade1TestsCount;
    if( testIdx < 0 )
        testIdx += total;    

    return restartArmorUpgrade1Test();
}

//////////////////////////////////////////////////////////////////////////
// implement ArmorUpgrade1Test
//////////////////////////////////////////////////////////////////////////

void ArmorUpgrade1Test::nextCallback(CCObject* pSender)
{
    CCScene* s = new ArmorUpgrade1TestScene();
    s->addChild( nextArmorUpgrade1Test() );
    CCDirector::sharedDirector()->replaceScene(s);
    s->release();
}

void ArmorUpgrade1Test::backCallback(CCObject* pSender)
{
    CCScene* s = new ArmorUpgrade1TestScene();
    s->addChild( backArmorUpgrade1Test() );
    CCDirector::sharedDirector()->replaceScene(s);
    s->release();
}

std::string ArmorUpgrade1Test::title()
{
    return "ArmorUpgrade1 sample";
}

//////////////////////////////////////////////////////////////////////////
// implement ArmorUpgrade1TestScene
//////////////////////////////////////////////////////////////////////////

void ArmorUpgrade1TestScene::runThisTest()
{
    testIdx--;
    CCLayer* pLayer = nextArmorUpgrade1Test();
    addChild(pLayer);
    
    CCDirector::sharedDirector()->replaceScene(this);
}


//////////////////////////////////////////////////////////////////////////
// implement ArmorUpgrade1TestJustGet
//////////////////////////////////////////////////////////////////////////

std::string ArmorUpgrade1TestJustGet::subtitle(){
    return "1. Get ArmorUpgrade1 by using GetDictionaryAction";
}

void ArmorUpgrade1TestJustGet::execute()
{
    CCLOG("ArmorUpgrade1TestJustGet::execute");
    CCLOG("now sending request with get action.");
    
    libgss::GetDictionaryAction* action = new libgss::GetDictionaryAction("ArmorUpgrade1", text_field->getString());
    libgss::ActionRequest* request = new libgss::ActionRequest();
    request->setCallback(this, gssActionResponse(ArmorUpgrade1TestJustGet::OnComplete));
    request->addAction(action);
    libgss::Network::instance()->send(request);
    request->release();
    action->release();
    
    CCLOG("sent request");
}


std::string ArmorUpgrade1TestJustGet::defaultValue(){
    return "10001";
}


