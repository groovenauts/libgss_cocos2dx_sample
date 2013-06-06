// #define COCOS2D_DEBUG   1

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

static int testIdx = -1; 

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
    CCLayer* pLayer = nextRequiredExperienceTest();
    addChild(pLayer);
    
    CCDirector::sharedDirector()->replaceScene(this);
}


//////////////////////////////////////////////////////////////////////////
// implement RequiredExperienceTestJustGet
//////////////////////////////////////////////////////////////////////////

std::string RequiredExperienceTestJustGet::subtitle(){
    return "Press Circle button to get Player.";
}

void RequiredExperienceTestJustGet::execute()
{
    CCLOG("RequiredExperienceTestJustGet::execute");
    CCLOG("now sending request with get action.");
    
    libgss::GetIntRangeAction* action = new libgss::GetIntRangeAction("RequiredExperience", textFieldAsInt());
    libgss::ActionRequest* request = new libgss::ActionRequest(this, gssActionResponse(RequiredExperienceTestJustGet::OnComplete));
    request->addAction(action);
    libgss::Network::instance()->send(request);
    request->release();
    action->release();
    
    CCLOG("sent request successfully");
}


std::string RequiredExperienceTestJustGet::defaultValue(){
    return "100";
}


