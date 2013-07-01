// #define COCOS2D_DEBUG   1

//
// メニュー名
// Get IntRange : 整数範囲テーブルからの取得
//
// 1. Get RequiredExperience by using GetIntRangeAction
//    整数を指定して、その整数に対応する値を取得
//

#include "RequiredExperienceTest.h"

#include <libGSS/libGSS.h>


//////////////////////////////////////////////////////////////////////////
// local function
//////////////////////////////////////////////////////////////////////////

enum 
{
    kRequiredExperienceTestJustGet = 0,
    kRequiredExperienceTestsCount,
}; 

static int testIdx = 0;

BaseNotificationLayer* createRequiredExperienceTest(int nIndex)
{
    switch(nIndex)
    {
    case kRequiredExperienceTestJustGet: return new RequiredExperienceTestJustGet();
    default: return 0;
    }
}

CCLayer* restartRequiredExperienceTest()
{
    RequiredExperienceTest* pContainerLayer = new RequiredExperienceTest;
    pContainerLayer->autorelease();

    BaseNotificationLayer* pTestLayer = createRequiredExperienceTest(testIdx);
    pTestLayer->autorelease();
    pContainerLayer->addBaseNotificationLayer(pTestLayer);

    return pContainerLayer;
}

CCLayer* nextRequiredExperienceTest()
{
    testIdx++;
    testIdx = testIdx % kRequiredExperienceTestsCount;

    return restartRequiredExperienceTest();
}

CCLayer* backRequiredExperienceTest()
{
    testIdx--;
    int total = kRequiredExperienceTestsCount;
    if( testIdx < 0 )
        testIdx += total;    

    return restartRequiredExperienceTest();
}

//////////////////////////////////////////////////////////////////////////
// implement RequiredExperienceTest
//////////////////////////////////////////////////////////////////////////

void RequiredExperienceTest::nextCallback(CCObject* pSender)
{
    CCScene* s = new RequiredExperienceTestScene();
    s->addChild( nextRequiredExperienceTest() );
    CCDirector::sharedDirector()->replaceScene(s);
    s->release();
}

void RequiredExperienceTest::backCallback(CCObject* pSender)
{
    CCScene* s = new RequiredExperienceTestScene();
    s->addChild( backRequiredExperienceTest() );
    CCDirector::sharedDirector()->replaceScene(s);
    s->release();
}

std::string RequiredExperienceTest::title()
{
    return "RequiredExperience sample";
}

//////////////////////////////////////////////////////////////////////////
// implement RequiredExperienceTestScene
//////////////////////////////////////////////////////////////////////////

void RequiredExperienceTestScene::runThisTest()
{
    testIdx--;
    CCLayer* pLayer = nextRequiredExperienceTest();
    addChild(pLayer);
    
    CCDirector::sharedDirector()->replaceScene(this);
}


//////////////////////////////////////////////////////////////////////////
// implement RequiredExperienceTestJustGet
//////////////////////////////////////////////////////////////////////////

std::string RequiredExperienceTestJustGet::subtitle(){
    return "1. Get RequiredExperience by using GetIntRangeAction";
}

void RequiredExperienceTestJustGet::execute()
{
    CCLOG("RequiredExperienceTestJustGet::execute");
    CCLOG("now sending request with get action.");
    
    libgss::GetIntRangeAction* action = new libgss::GetIntRangeAction("RequiredExperience", textFieldAsInt());
    libgss::ActionRequest* request = new libgss::ActionRequest();
    request->setCallback(this, gssActionResponse(RequiredExperienceTestJustGet::OnComplete));
    request->addAction(action);
    libgss::Network::instance()->send(request);
    request->release();
    action->release();
    
    CCLOG("sent request");
}


std::string RequiredExperienceTestJustGet::defaultValue(){
    return "100";
}


