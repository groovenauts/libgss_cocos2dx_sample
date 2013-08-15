//
//  MaintenanceTest.cpp
//  SampleApp
//
//  Created by 下川 北斗 on 2013/08/05.
//
//

#include "MaintenanceTest.h"


#include <libGSS/libGSS.h>

#include "Settings.h"

//////////////////////////////////////////////////////////////////////////
// local function
//////////////////////////////////////////////////////////////////////////

enum
{
    kMaintenanceStatus = 0,
    
    kMaintenanceTestsCount,
};

static int testIdx = 0;

BaseNotificationLayer* createMaintenanceTest(int nIndex)
{
    switch(nIndex)
    {
        case kMaintenanceStatus: return new MaintenanceTestBasic();
        default: return 0;
    }
}

CCLayer* restartMaintenanceTest()
{
    MaintenanceTest* pContainerLayer = new MaintenanceTest();
    pContainerLayer->autorelease();
    
    BaseNotificationLayer* pTestLayer = createMaintenanceTest(testIdx);
    pTestLayer->autorelease();
    pContainerLayer->addBaseNotificationLayer(pTestLayer);
    
    return pContainerLayer;
}

CCLayer* nextMaintenanceTest()
{
    testIdx++;
    testIdx = testIdx % kMaintenanceTestsCount;
    
    return restartMaintenanceTest();
}

CCLayer* backMaintenanceTest()
{
    testIdx--;
    int total = kMaintenanceTestsCount;
    if( testIdx < 0 )
        testIdx += total;
    
    return restartMaintenanceTest();
}

//////////////////////////////////////////////////////////////////////////
// implement MaintenanceTest
//////////////////////////////////////////////////////////////////////////

void MaintenanceTest::nextCallback(CCObject* pSender)
{
    CCScene* s = new MaintenanceTestScene();
    s->addChild( nextMaintenanceTest() );
    CCDirector::sharedDirector()->replaceScene(s);
    s->release();
}

void MaintenanceTest::backCallback(CCObject* pSender)
{
    CCScene* s = new MaintenanceTestScene();
    s->addChild( backMaintenanceTest() );
    CCDirector::sharedDirector()->replaceScene(s);
    s->release();
}

std::string MaintenanceTest::title()
{
    return "Maintenance Status Sample";
}

//////////////////////////////////////////////////////////////////////////
// implement MaintenanceTestScene
//////////////////////////////////////////////////////////////////////////

void MaintenanceTestScene::runThisTest()
{
    testIdx--;
    CCLayer* pLayer = nextMaintenanceTest();
    addChild(pLayer);
    
    CCDirector::sharedDirector()->replaceScene(this);
}


//////////////////////////////////////////////////////////////////////////
// implement MaintenanceTestInvalidPlayerId
//////////////////////////////////////////////////////////////////////////

std::string MaintenanceTestBasic::subtitle(){
    return "1. Confirm server maintenance status.";
}

void MaintenanceTestBasic::execute(){
    CCLOG("MaintenanceTestBasic::execute");
    CCLOG("now sending request.");
    
    libgss::MaintenanceStatusRequest* request = new libgss::MaintenanceStatusRequest();
    request->setCallback(this, gssMaintenanceStatusResponse(MaintenanceTestBasic::OnComplete));
    libgss::Network::instance()->send(request);
    request->release();
    
    CCLOG("sent request.");
}

void MaintenanceTestBasic::OnComplete(libgss::MaintenanceStatusResponse* response){
    if (response->isInMaintenance()) {
        CCLOG("completed.\n%s", response->message().c_str());
    }
    else if( response->success() || response->code() == 404){
        CCLOG("completed. Not in maintenance.");
    }
    else{
        CCLOG("Error has occured.");
    }
}
