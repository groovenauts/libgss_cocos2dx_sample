// #define COCOS2D_DEBUG   1

//
// メニュー名
// Get/Update Player: プレイヤーの取得と更新
//
// 1. Get Player by using GetPlayerAction : プレイヤーのデータを取得
// 2. Descrement Level by 1 by using UpdatePlayerAction : レベルを１つ下げる
// 3. Increment Level by 1 by using UpdatePlayerAction : レベルを１つ上げる
//


#include "PlayerTest.h"

#include <libGSS/libGSS.h>


//////////////////////////////////////////////////////////////////////////
// local function
//////////////////////////////////////////////////////////////////////////

enum 
{
    kPlayerTestJustGet = 0,
    kPlayerTestDecrementLevel,
    kPlayerTestIncrementLevel,
    kPlayerTestsCount,
}; 

static int testIdx = -1; 

BaseNotificationLayer* createPlayerTest(int nIndex)
{
    switch(nIndex)
    {
    case kPlayerTestJustGet: return new PlayerTestJustGet();
    case kPlayerTestDecrementLevel: return new PlayerTestDecrementLevel();
    case kPlayerTestIncrementLevel: return new PlayerTestIncrementLevel();
    default: return 0;
    }
}

CCLayer* restartPlayerTest()
{
    PlayerTest* pContainerLayer = new PlayerTest;
    pContainerLayer->autorelease();

    BaseNotificationLayer* pTestLayer = createPlayerTest(testIdx);
    pTestLayer->autorelease();
    pContainerLayer->addBaseNotificationLayer(pTestLayer);

    return pContainerLayer;
}

CCLayer* nextPlayerTest()
{
    testIdx++;
    testIdx = testIdx % kPlayerTestsCount;

    return restartPlayerTest();
}

CCLayer* backPlayerTest()
{
    testIdx--;
    int total = kPlayerTestsCount;
    if( testIdx < 0 )
        testIdx += total;    

    return restartPlayerTest();
}

//////////////////////////////////////////////////////////////////////////
// implement PlayerTest
//////////////////////////////////////////////////////////////////////////

void PlayerTest::nextCallback(CCObject* pSender)
{
    CCScene* s = new PlayerTestScene();
    s->addChild( nextPlayerTest() );
    CCDirector::sharedDirector()->replaceScene(s);
    s->release();
}

void PlayerTest::backCallback(CCObject* pSender)
{
    CCScene* s = new PlayerTestScene();
    s->addChild( backPlayerTest() );
    CCDirector::sharedDirector()->replaceScene(s);
    s->release();
}

std::string PlayerTest::title()
{
    return "Player sample";
}

//////////////////////////////////////////////////////////////////////////
// implement PlayerTestScene
//////////////////////////////////////////////////////////////////////////

void PlayerTestScene::runThisTest()
{
    CCLayer* pLayer = nextPlayerTest();
    addChild(pLayer);
    
    CCDirector::sharedDirector()->replaceScene(this);
}


//////////////////////////////////////////////////////////////////////////
// implement PlayerTestJustGet
//////////////////////////////////////////////////////////////////////////

std::string PlayerTestJustGet::subtitle(){
    return "1. Get Player by using GetPlayerAction";
}

void PlayerTestJustGet::execute()
{
    CCLOG("PlayerTestNoConditions::execute");
    CCLOG("now sending request with all action.");
    
    libgss::GetPlayerAction* action = new libgss::GetPlayerAction();
    // action->setPlayerId(kDefaultFindPlayerId);
    libgss::ActionRequest* request = new libgss::ActionRequest();
    request->setCallback(this, gssActionResponse(PlayerTestJustGet::OnComplete));
    request->addAction(action);
    libgss::Network::instance()->send(request);
    request->release();
    action->release();
    
    CCLOG("sent request successfully");
}



//////////////////////////////////////////////////////////////////////////
// implement PlayerTestGetAndUpdate
//////////////////////////////////////////////////////////////////////////

void PlayerTestGetAndUpdate::execute(){
    CCLOG("PlayerTestDecrementLevel::execute");
    CCLOG("now sending request with get action.");
    
    libgss::GetPlayerAction* action = new libgss::GetPlayerAction();
    // action->setPlayerId(kDefaultFindPlayerId);
    libgss::ActionRequest* request = new libgss::ActionRequest();
    request->setCallback(this, gssActionResponse(PlayerTestGetAndUpdate::OnCompleteToGet));
    request->addAction(action);
    libgss::Network::instance()->send(request);
    request->release();
    action->release();
    
    CCLOG("sent request successfully");
}


void PlayerTestGetAndUpdate::OnCompleteToGet(libgss::ActionResponse* response) {
    log(response->outputs()->toString());

    CCLOG("PlayerTestDecrementLevel::update");
    CCLOG("now sending request with update action.");
    libgss::JSONObject* player = response->outputs()->get(0)->get("result");
    
    update(player);
    
    libgss::ActionRequest *request = new libgss::ActionRequest();
    request->setCallback(this, gssActionResponse(PlayerTestGetAndUpdate::OnCompleteToUpdate));
    libgss::UpdatePlayerAction* updateAction = new libgss::UpdatePlayerAction(*player);
    request->addAction(updateAction);
    libgss::Network::instance()->send(request);
    request->release();
    updateAction->release();
    
    CCLOG("sent request successfully");
}


//////////////////////////////////////////////////////////////////////////
// implement PlayerTestDecrementLevel
//////////////////////////////////////////////////////////////////////////
std::string PlayerTestDecrementLevel::subtitle(){
    return "2. Descrement Level by 1 by using UpdatePlayerAction";
}
void PlayerTestDecrementLevel::update(libgss::JSONObject* player){
    int level = player->get("level")->toInt();
    level -= 1;
    if (level < 0) level = 0;
    player->set("level", level);
}

//////////////////////////////////////////////////////////////////////////
// implement PlayerTestIncrementLevel
//////////////////////////////////////////////////////////////////////////
std::string PlayerTestIncrementLevel::subtitle(){
    return "3. Increment Level by 1 by using UpdatePlayerAction";
}
void PlayerTestIncrementLevel::update(libgss::JSONObject* player){
    int level = player->get("level")->toInt();
    level += 1;
    player->set("level", level);
}


