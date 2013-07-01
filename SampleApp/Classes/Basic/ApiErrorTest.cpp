//
//  ApiErrorTest.cpp
//  SampleApp
//
//  Created by 下川 北斗 on 2013/05/31.
//
//

//
// メニュー名
// API Error Test: APIエラーのテスト
//
// 1. Action request to invalid URL: 繋がらないURLへのアクションリクエスト
// 2. Login request with invalid player ID: 不正なプレイヤーIDでのログインリクエスト
//

#include "ApiErrorTest.h"

#include <sstream>
#include <libGSS/libGSS.h>

#include "Settings.h"

//////////////////////////////////////////////////////////////////////////
// local function
//////////////////////////////////////////////////////////////////////////

enum
{
    kApiErrorTestInvalidURL = 0,
    kApiErrorTestInvalidPlayerId,

    kApiErrorTestsCount,
};

static int testIdx = 0;

BaseNotificationLayer* createApiErrorTest(int nIndex)
{
    switch(nIndex)
    {
        case kApiErrorTestInvalidURL: return new ApiErrorTestInvalidURL();
        case kApiErrorTestInvalidPlayerId: return new ApiErrorTestInvalidPlayerId();
        default: return 0;
    }
}

CCLayer* restartApiErrorTest()
{
    ApiErrorTest* pContainerLayer = new ApiErrorTest;
    pContainerLayer->autorelease();
    
    BaseNotificationLayer* pTestLayer = createApiErrorTest(testIdx);
    pTestLayer->autorelease();
    pContainerLayer->addBaseNotificationLayer(pTestLayer);
    
    return pContainerLayer;
}

CCLayer* nextApiErrorTest()
{
    testIdx++;
    testIdx = testIdx % kApiErrorTestsCount;
    
    return restartApiErrorTest();
}

CCLayer* backApiErrorTest()
{
    testIdx--;
    int total = kApiErrorTestsCount;
    if( testIdx < 0 )
        testIdx += total;
    
    return restartApiErrorTest();
}

void showError(libgss::Response* response)
{
    if (response->success()) {
        return;
    }
    
    if (response->code() >= 400) {
        stringstream ss;
        ss << "code = " << response->code() << "\n" << response->message();
        CCMessageBox(ss.str().c_str(), "Request Error");
    }
    else if(response->code() < 0){
        stringstream ss;
        ss << "code = " << response->code() << "\n" << response->message();
        CCMessageBox(ss.str().c_str(), "Access Error");
    }
    else{
        stringstream ss;
        ss << "code = " << response->code() << "\n" << response->message();
        CCMessageBox(ss.str().c_str(), "Unknown Error");
    }

}

//////////////////////////////////////////////////////////////////////////
// implement ApiErrorTest
//////////////////////////////////////////////////////////////////////////

void ApiErrorTest::nextCallback(CCObject* pSender)
{
    CCLOG("root URL: %s", libgss::Network::instance()->sslRootUrl().c_str());
    
    CCScene* s = new ApiErrorTestScene();
    s->addChild( nextApiErrorTest() );
    CCDirector::sharedDirector()->replaceScene(s);
    s->release();
}

void ApiErrorTest::backCallback(CCObject* pSender)
{
    CCScene* s = new ApiErrorTestScene();
    s->addChild( backApiErrorTest() );
    CCDirector::sharedDirector()->replaceScene(s);
    s->release();
}

std::string ApiErrorTest::title()
{
    return "API Error Sample";
}

//////////////////////////////////////////////////////////////////////////
// implement ApiErrorTestScene
//////////////////////////////////////////////////////////////////////////

void ApiErrorTestScene::runThisTest()
{
    testIdx--;
    CCLayer* pLayer = nextApiErrorTest();
    addChild(pLayer);
    
    CCDirector::sharedDirector()->replaceScene(this);
}

//////////////////////////////////////////////////////////////////////////
// implement ApiErrorTestInvalidURL
//////////////////////////////////////////////////////////////////////////

std::string ApiErrorTestInvalidURL::subtitle(){
    return "1. Action request to invalid URL";
}

void ApiErrorTestInvalidURL::execute(){
    CCLOG("ApiErrorTestInvalidURL::execute");
    
    libgss::Network::instance()->setHost("noexists-host.invalid");
    
    CCLOG("now sending request.");
    
    libgss::ActionRequest* request = new libgss::ActionRequest();
    request->setCallback(this, gssActionResponse(ApiErrorTestInvalidURL::OnComplete));
    
    libgss::CountAction* action = new libgss::CountAction("Armor");
    action->setId(1);
    request->addAction(action);
    
    libgss::Network::instance()->send(request);
    request->release();
    action->release();
    
    CCLOG("sent request.");
}

void ApiErrorTestInvalidURL::OnComplete(libgss::ActionResponse* response){
    showError(response);
    Settings::applySettings();
}


//////////////////////////////////////////////////////////////////////////
// implement ApiErrorTestInvalidPlayerId
//////////////////////////////////////////////////////////////////////////

std::string ApiErrorTestInvalidPlayerId::subtitle(){
    return "2. Login request with invalid player ID";
}

void ApiErrorTestInvalidPlayerId::execute(){
    CCLOG("ApiErrorTestInvalidPlayerId::execute");
    CCLOG("now sending request.");
    
    libgss::LoginRequest* request = new libgss::LoginRequest("non-exists-player-id", "ichiro");
    request->setCallback(this, gssLoginResponse(ApiErrorTestInvalidPlayerId::OnComplete));
    libgss::Network::instance()->send(request);
    request->release();
    
    CCLOG("sent request.");
}

void ApiErrorTestInvalidPlayerId::OnComplete(libgss::LoginResponse* response){
    showError(response);
}
