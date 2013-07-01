// #define COCOS2D_DEBUG   1

//
// メニュー名
// Get/Update GameData : ゲームデータの取得と更新
//
// item_cd 20001はポーションです。
//
// 1. Get GameData
// 2. Descrement HP by 7 by using UpdateGameDataAction : HPを7減らす
// 3. Increment HP by 5 by using UpdateGameDataAction : HPを5増やす
// 4. Waste Potion by using UpdateGameDataAction : ポーションを１つ捨てる
// 5. Get Potion by using UpdateGameDataAction : ポーションを１つ増やす
// 6. Use Item by using ExecuteAction : アイテムを使用する
//

#include "GameDataTest.h"

#include <libGSS/libGSS.h>


//////////////////////////////////////////////////////////////////////////
// local function
//////////////////////////////////////////////////////////////////////////

enum 
{
    kGameDataTestJustGet = 0,
    kGameDataTestDecrementHP,
    kGameDataTestIncrementHP,
    kGameDataTestDecrementPotion,
    kGameDataTestIncrementPotion,
    kExecuteActionTestUseItem,
    kGameDataTestsCount,
}; 

static int testIdx = 0;

BaseNotificationLayer* createGameDataTest(int nIndex)
{
    switch(nIndex)
    {
    case kGameDataTestJustGet: return new GameDataTestJustGet();
    case kGameDataTestDecrementHP: return new GameDataTestDecrementHP();
    case kGameDataTestIncrementHP: return new GameDataTestIncrementHP();
    case kGameDataTestDecrementPotion: return new GameDataTestDecrementPotion();
    case kGameDataTestIncrementPotion: return new GameDataTestIncrementPotion();
    case kExecuteActionTestUseItem: return new ExecuteActionTestUseItem();
    default: return 0;
    }
}

CCLayer* restartGameDataTest()
{
    GameDataTest* pContainerLayer = new GameDataTest;
    pContainerLayer->autorelease();

    BaseNotificationLayer* pTestLayer = createGameDataTest(testIdx);
    pTestLayer->autorelease();
    pContainerLayer->addBaseNotificationLayer(pTestLayer);

    return pContainerLayer;
}

CCLayer* nextGameDataTest()
{
    testIdx++;
    testIdx = testIdx % kGameDataTestsCount;

    return restartGameDataTest();
}

CCLayer* backGameDataTest()
{
    testIdx--;
    int total = kGameDataTestsCount;
    if( testIdx < 0 )
        testIdx += total;    

    return restartGameDataTest();
}

//////////////////////////////////////////////////////////////////////////
// implement GameDataTest
//////////////////////////////////////////////////////////////////////////

void GameDataTest::nextCallback(CCObject* pSender)
{
    CCScene* s = new GameDataTestScene();
    s->addChild( nextGameDataTest() );
    CCDirector::sharedDirector()->replaceScene(s);
    s->release();
}

void GameDataTest::backCallback(CCObject* pSender)
{
    CCScene* s = new GameDataTestScene();
    s->addChild( backGameDataTest() );
    CCDirector::sharedDirector()->replaceScene(s);
    s->release();
}

std::string GameDataTest::title()
{
    return "GameData sample";
}

//////////////////////////////////////////////////////////////////////////
// implement GameDataTestScene
//////////////////////////////////////////////////////////////////////////

void GameDataTestScene::runThisTest()
{
    testIdx--;
    CCLayer* pLayer = nextGameDataTest();
    addChild(pLayer);
    
    CCDirector::sharedDirector()->replaceScene(this);
}


//////////////////////////////////////////////////////////////////////////
// implement GameDataTestJustGet
//////////////////////////////////////////////////////////////////////////

std::string GameDataTestJustGet::subtitle(){
    return "1. Get GameData";
}

void GameDataTestJustGet::execute()
{
    CCLOG("GameDataTestNoConditions::execute");
    
    CCLOG("now sending request with all action.");
    
    libgss::ActionRequest *request = new libgss::ActionRequest();
    request->setCallback(this, gssActionResponse(GameDataTestJustGet::OnComplete));
    
    libgss::GetGameDataAction* getAction = new libgss::GetGameDataAction();
    request->addAction(getAction);
    libgss::Network::instance()->send(request);
    
    request->release();
    getAction->release();
    
    CCLOG("sent request");
}



//////////////////////////////////////////////////////////////////////////
// implement GameDataTestGetAndUpdate
//////////////////////////////////////////////////////////////////////////

void GameDataTestGetAndUpdate::execute(){
    CCLOG("GameDataTestDecrementHP::execute");
    CCLOG("now sending request with get action.");
    
    libgss::ActionRequest *request = new libgss::ActionRequest();
    request->setCallback(this, gssActionResponse(GameDataTestDecrementHP::OnCompleteToGet));
    
    libgss::GetGameDataAction* getAction = new libgss::GetGameDataAction();
    request->addAction(getAction);
    libgss::Network::instance()->send(request);
    
    request->release();
    getAction->release();
    
    CCLOG("sent request");
}


void GameDataTestGetAndUpdate::OnCompleteToGet(libgss::ActionResponse* response) {
    log(response->outputs()->toString());

    CCLOG("GameDataTestDecrementHP::update");
    CCLOG("now sending request with update action.");
    libgss::JSONObject* gameData = response->outputs()->get(0)->get("result");
    
    update(gameData);
    
    libgss::ActionRequest *request = new libgss::ActionRequest();
    request->setCallback(this, gssActionResponse(GameDataTestDecrementHP::OnCompleteToUpdate));
    libgss::UpdateGameDataAction* updateAction = new libgss::UpdateGameDataAction(*gameData);
    request->addAction(updateAction);
    libgss::Network::instance()->send(request);
    
    request->release();
    updateAction->release();
    
    CCLOG("sent request");
}


//////////////////////////////////////////////////////////////////////////
// implement GameDataTestDecrementHP
//////////////////////////////////////////////////////////////////////////
std::string GameDataTestDecrementHP::subtitle(){
    return "2. Descrement HP by 7 by using UpdateGameDataAction";
}
void GameDataTestDecrementHP::update(libgss::JSONObject* gameData){
    int hp = gameData->get("content")->get("hp")->toInt();
    hp -= 7;
    if (hp < 0) hp = 0;
    gameData->get("content")->set("hp", hp);
}

//////////////////////////////////////////////////////////////////////////
// implement GameDataTestIncrementHP
//////////////////////////////////////////////////////////////////////////
std::string GameDataTestIncrementHP::subtitle(){
    return "3. Increment HP by 5 by using UpdateGameDataAction";
}
void GameDataTestIncrementHP::update(libgss::JSONObject* gameData){
    int hp = gameData->get("content")->get("hp")->toInt();
    int max_hp = gameData->get("content")->get("max_hp")->toInt();
    hp += 5;
    if (hp > max_hp) hp = max_hp;
    gameData->get("content")->set("hp", hp);
}


//////////////////////////////////////////////////////////////////////////
// implement GameDataTestDecrementPotion
//////////////////////////////////////////////////////////////////////////
std::string GameDataTestDecrementPotion::subtitle(){
    return "4. Waste Potion by using UpdateGameDataAction";
}
void GameDataTestDecrementPotion::update(libgss::JSONObject* gameData){
    libgss::JSONObject* items = gameData->get("content")->get("items");
    int cnt = (items->contains("20001")) ? items->get("20001")->toInt() : 0;
    cnt -= 1;
    if (cnt < 1) {
        items->remove("20001");
    } else {
        items->set("20001", cnt);
    }
}

//////////////////////////////////////////////////////////////////////////
// implement GameDataTestIncrementPotion
//////////////////////////////////////////////////////////////////////////
std::string GameDataTestIncrementPotion::subtitle(){
    return "5. Get Potion by using UpdateGameDataAction";
}
void GameDataTestIncrementPotion::update(libgss::JSONObject* gameData){
    libgss::JSONObject* items = gameData->get("content")->get("items");
    int cnt = (items->contains("20001")) ? items->get("20001")->toInt() : 0;
    cnt += 1;
    gameData->get("content")->get("items")->set("20001", cnt);
}


//////////////////////////////////////////////////////////////////////////
// implement ExecuteActionTestUseItem
//////////////////////////////////////////////////////////////////////////

std::string ExecuteActionTestUseItem::subtitle(){
    return "6. Use Item by using ExecuteAction";
}

void ExecuteActionTestUseItem::execute(){
        CCLOG("ExecuteActionTestUseItem::execute");
        CCLOG("now sending request with bulk actions.");
    
        libgss::ActionRequest* request = new libgss::ActionRequest();
        request->setCallback(this, gssActionResponse(ExecuteActionTestUseItem::OnComplete));
    
        libgss::ExecuteAction* action = new libgss::ExecuteAction("ItemRubyStoredScript", "use_item");
        action->setId(1);
        libgss::JSONObject args;
        args.set("item_cd", text_field->getString());
        action->setArgs(args);
        request->addAction(action);
    
        libgss::Network::instance()->send(request);
        request->release();
        action->release();
    
        CCLOG("sent request");
    }

std::string ExecuteActionTestUseItem::defaultValue() {
        return "20001";
}


