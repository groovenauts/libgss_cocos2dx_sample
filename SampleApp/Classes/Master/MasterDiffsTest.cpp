//
//  MasterDiffsTest.cpp
//  SampleApp
//
//  Created by 下川 北斗 on 2013/08/09.
//
//

#include "MasterDiffsTest.h"


//////////////////////////////////////////////////////////////////////////
// local function
//////////////////////////////////////////////////////////////////////////

enum
{
    kMasterDiffsTestBasic = 0,

    kMasterDiffsTestsCount,
};

static int testIdx = 0;

BaseNotificationLayer* createMasterDiffsTest(int nIndex)
{
    switch(nIndex)
    {
        case kMasterDiffsTestBasic: return new MasterDiffsTestBasic();
        default: return 0;
    }
}

CCLayer* restartMasterDiffsTest()
{
    MasterDiffsTest* pContainerLayer = new MasterDiffsTest;
    pContainerLayer->autorelease();
    
    BaseNotificationLayer* pTestLayer = createMasterDiffsTest(testIdx);
    pTestLayer->autorelease();
    pContainerLayer->addBaseNotificationLayer(pTestLayer);
    
    return pContainerLayer;
}

CCLayer* nextMasterDiffsTest()
{
    testIdx++;
    testIdx = testIdx % kMasterDiffsTestsCount;
    
    return restartMasterDiffsTest();
}

CCLayer* backMasterDiffsTest()
{
    testIdx--;
    int total = kMasterDiffsTestsCount;
    if( testIdx < 0 )
        testIdx += total;
    
    return restartMasterDiffsTest();
}

//////////////////////////////////////////////////////////////////////////
// implement MasterDiffsTest
//////////////////////////////////////////////////////////////////////////

void MasterDiffsTest::nextCallback(CCObject* pSender)
{
    CCScene* s = new MasterDiffsTestScene();
    s->addChild( nextMasterDiffsTest() );
    CCDirector::sharedDirector()->replaceScene(s);
    s->release();
}

void MasterDiffsTest::backCallback(CCObject* pSender)
{
    CCScene* s = new MasterDiffsTestScene();
    s->addChild( backMasterDiffsTest() );
    CCDirector::sharedDirector()->replaceScene(s);
    s->release();
}

std::string MasterDiffsTest::title()
{
    return "Master diffs sample";
}

//////////////////////////////////////////////////////////////////////////
// implement MasterDiffsTestScene
//////////////////////////////////////////////////////////////////////////

void MasterDiffsTestScene::runThisTest()
{
    testIdx--;
    CCLayer* pLayer = nextMasterDiffsTest();
    addChild(pLayer);
    
    CCDirector::sharedDirector()->replaceScene(this);
}


//////////////////////////////////////////////////////////////////////////
// implement MasterDiffsAction
//////////////////////////////////////////////////////////////////////////

std::string MasterDiffsTestBasic::subtitle()
{
    return "1. Master diffs basic test.";
}

void MasterDiffsTestBasic::execute()
{
    CCLOG("MasterDiffsTestBasic::execute");
    
    CCLOG("now sending request with master_diffs action.");
    
    libgss::ActionRequest *request = new libgss::ActionRequest();
    request->setCallback(this, gssActionResponse(MasterDiffsTestBasic::OnComplete));
    libgss::JSONObject downloadedVersions;
    downloadedVersions.set("Item", "");
    downloadedVersions.set("Armor", "1.0");
    libgss::MasterDiffsAction* action = new libgss::MasterDiffsAction(downloadedVersions);
    request->addAction(action);
    libgss::Network::instance()->send(request);
    request->release();
    action->release();
    
    CCLOG("sent request");
}

void MasterDiffsTestBasic::OnComplete(libgss::ActionResponse* response){
    if (response->success()) {
        CCLOG("%s", response->outputs()->toString().c_str());
    }
    else if(response->existsNewerVersionClient()){
        CCLOG("New version client has released.");
    }
    else{
        CCLOG("Error has occured. %d\n%s", response->code(), response->message().c_str());
    }
}
