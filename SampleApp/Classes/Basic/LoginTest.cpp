// #define COCOS2D_DEBUG   1

//
// メニュー名
// Login : ログイン
//
// 1. Login with player_id : player_idを指定した通常のログイン
// 2. No callback pattern  : コールバックなしのログイン
// 3. Register and login   : 初回ログイン（サーバでplayer_idを生成してログイン）
//
// Register and login

#include "LoginTest.h"

#include <libGSS/libGSS.h>

//////////////////////////////////////////////////////////////////////////
// local function
//////////////////////////////////////////////////////////////////////////

enum 
{
    kLoginTestImpl = 0,
    kLoginTestWithNoCallbackImpl,
    kLoginTestWithRegister ,
    kLoginTestsCount,
}; 

static int testIdx = 0;

BaseNotificationLayer* createLoginTest(int nIndex)
{
    switch(nIndex)
    {
    case kLoginTestImpl: return new LoginTestImpl();
    case kLoginTestWithRegister: return new LoginTestWithRegisterImpl();
    case kLoginTestWithNoCallbackImpl: return new LoginTestWithNoCallbackImpl();
//    case kTextFieldTTFActionTest: return new TextFieldTTFActionTest();
    default: return 0;
    }
}

CCLayer* restartLoginTest()
{
    LoginTest* pContainerLayer = new LoginTest;
    pContainerLayer->autorelease();

    BaseNotificationLayer* pTestLayer = createLoginTest(testIdx);
    pTestLayer->autorelease();
    pContainerLayer->addKeyboardNotificationLayer(pTestLayer);

    return pContainerLayer;
}

CCLayer* nextLoginTest()
{
    testIdx++;
    testIdx = testIdx % kLoginTestsCount;

    return restartLoginTest();
}

CCLayer* backLoginTest()
{
    testIdx--;
    int total = kLoginTestsCount;
    if( testIdx < 0 )
        testIdx += total;    

    return restartLoginTest();
}

//////////////////////////////////////////////////////////////////////////
// implement LoginTest
//////////////////////////////////////////////////////////////////////////

void LoginTest::nextCallback(CCObject* pSender)
{
    CCScene* s = new LoginTestScene();
    s->addChild( nextLoginTest() );
    CCDirector::sharedDirector()->replaceScene(s);
    s->release();
}

void LoginTest::backCallback(CCObject* pSender)
{
    CCScene* s = new LoginTestScene();
    s->addChild( backLoginTest() );
    CCDirector::sharedDirector()->replaceScene(s);
    s->release();
}

std::string LoginTest::title()
{
    return "Login";
}

void LoginTest::addKeyboardNotificationLayer(BaseNotificationLayer * pLayer)
{
        m_pNotificationLayer = pLayer;
        addChild(pLayer);
}

//////////////////////////////////////////////////////////////////////////
// implement LoginTestImpl
//////////////////////////////////////////////////////////////////////////

std::string LoginTestImpl::subtitle()
{
    return "1. Login with player_id";
}

void LoginTestImpl::execute()
{
    CCLOG("LoginTestImpl::onTextFieldDetachWithIME");
    
    CCLOG("now sending login request...");
    
    libgss::LoginRequest *request = new libgss::LoginRequest(text_field->getString(), "ichiro");
    request->setCallback(this, gssLoginResponse(LoginTestImpl::OnComplete));
    libgss::Network::instance()->send(request);
    request->release();
    
    CCLOG("sent login request");
}

void LoginTestImpl::OnComplete(libgss::LoginResponse *response){
    if (response->success()) {
        CCLOG("Login success! player id is: %s", libgss::Network::instance()->playerId().c_str());
    }
    else if(response->existsNewerVersionClient()){
        CCLOG("New version client has released.");
    }
    else{
        CCLOG("Login failure! error code: %d", response->code());
        CCLOG("%s", response->message().c_str());
    }
}

//////////////////////////////////////////////////////////////////////////
// implement LoginTestWithNoCallbackImpl
//////////////////////////////////////////////////////////////////////////

std::string LoginTestWithNoCallbackImpl::subtitle()
{
    return "2. No callback pattern";
}

void LoginTestWithNoCallbackImpl::execute()
{
    CCLOG("LoginTestWithNoCallbackImpl::execute");
    
    CCLOG("now sending login request...");
    
    request_ = new libgss::LoginRequest(text_field->getString(), "ichiro");
    libgss::Network::instance()->send(request_);
    request_->release();
    
    pollingCount_ = 0;
    
    this->schedule(schedule_selector(LoginTestWithNoCallbackImpl::poll));
    
    CCLOG("sent login request");
}

void LoginTestWithNoCallbackImpl::poll(float timer){
    CCLOG("polling response. (%d)", ++pollingCount_);
    
    libgss::LoginResponse* response = request_->getResponseIfFinished();
    if (response == NULL) {
        CCLOG("not received response yet.");
        return;
    }
    
    CCLOG("received response.");
    if (response->success()) {
        CCLOG("Login success! player id is: %s", libgss::Network::instance()->playerId().c_str());
    }
    else if(response->existsNewerVersionClient()){
        CCLOG("New version client has released.");
    }
    else{
        CCLOG("Login failure! error code: %d", response->code());
        CCLOG("%s", response->message().c_str());
    }
    
    this->unschedule(schedule_selector(LoginTestWithNoCallbackImpl::poll));
    
    request_->release();
}

//////////////////////////////////////////////////////////////////////////
// implement LoginTestWithRegisterImpl
//////////////////////////////////////////////////////////////////////////

std::string LoginTestWithRegisterImpl::subtitle(){
    return "3. Register and login";
}

void LoginTestWithRegisterImpl::execute(){
    CCLOG("LoginTestWithNoCallbackImpl::execute");
    
    CCLOG("now sending login request...");
    
    libgss::LoginRequest *request = new libgss::LoginRequest("", "");
    request->setCallback(this, gssLoginResponse(LoginTestWithRegisterImpl::OnComplete));
    libgss::Network::instance()->send(request);
    request->release();
    
    CCLOG("sent login request");
}

void LoginTestWithRegisterImpl::OnComplete(libgss::LoginResponse *response){
    if (response->success()) {
        CCLOG("Login success! player id is: %s", libgss::Network::instance()->playerId().c_str());
    }
    else if(response->existsNewerVersionClient()){
        CCLOG("New version client has released.");
    }
    else{
        CCLOG("Login failure! error code: %d", response->code());
        CCLOG("%s", response->message().c_str());
    }
}


//////////////////////////////////////////////////////////////////////////
// implement LoginTestScene
//////////////////////////////////////////////////////////////////////////

void LoginTestScene::runThisTest()
{
    testIdx--;
    CCLayer* pLayer = nextLoginTest();
    addChild(pLayer);

    CCDirector::sharedDirector()->replaceScene(this);
}
