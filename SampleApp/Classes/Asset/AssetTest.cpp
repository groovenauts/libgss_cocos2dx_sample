// #define COCOS2D_DEBUG   1

//
// メニュー名
// Asset Test : アセットのテスト
//
// アセットをダウンロードするテスト
// 1. 1. Basic asset request by using AssetRequest : コールバックを使うパターン
// 2. No callback pattern by using AssetRequest : コールバックを使わないパターン
//

#include "AssetTest.h"

#include "cocos2d.h"

#include <libGSS/libGSS.h>
#include <time.h>

//////////////////////////////////////////////////////////////////////////
// local function
//////////////////////////////////////////////////////////////////////////

enum 
{
    kAssetTestBasic = 0,
    kAssetTestWithNoCallback,
    
    kAssetTestsCount,
}; 

static int testIdx = 0;

BaseNotificationLayer* createAssetTest(int nIndex)
{
    switch(nIndex)
    {
    case kAssetTestBasic: return new AssetTestBasic();
    case kAssetTestWithNoCallback: return new AssetTestWithNoCallback();
    default: return 0;
    }
}

CCLayer* restartAssetTest()
{
    AssetTest* pContainerLayer = new AssetTest;
    pContainerLayer->autorelease();

    BaseNotificationLayer* pTestLayer = createAssetTest(testIdx);
    pTestLayer->autorelease();
    pContainerLayer->addBaseNotificationLayer(pTestLayer);

    return pContainerLayer;
}

CCLayer* nextAssetTest()
{
    testIdx++;
    testIdx = testIdx % kAssetTestsCount;

    return restartAssetTest();
}

CCLayer* backAssetTest()
{
    testIdx--;
    int total = kAssetTestsCount;
    if( testIdx < 0 )
        testIdx += total;    

    return restartAssetTest();
}

//////////////////////////////////////////////////////////////////////////
// implement AssetTest
//////////////////////////////////////////////////////////////////////////

void AssetTest::nextCallback(CCObject* pSender)
{
    CCScene* s = new AssetTestScene();
    s->addChild( nextAssetTest() );
    CCDirector::sharedDirector()->replaceScene(s);
    s->release();
}

void AssetTest::backCallback(CCObject* pSender)
{
    CCScene* s = new AssetTestScene();
    s->addChild( backAssetTest() );
    CCDirector::sharedDirector()->replaceScene(s);
    s->release();
}

std::string AssetTest::title()
{
    return "Asset sample";
}

//////////////////////////////////////////////////////////////////////////
// implement AssetTestScene
//////////////////////////////////////////////////////////////////////////

void AssetTestScene::runThisTest()
{
    testIdx--;
    CCLayer* pLayer = nextAssetTest();
    addChild(pLayer);
    
    CCDirector::sharedDirector()->replaceScene(this);
}

//////////////////////////////////////////////////////////////////////////
// implement AssetTestBasic
//////////////////////////////////////////////////////////////////////////

std::string AssetTestBasic::subtitle(){
    return "1. Basic asset request by using AssetRequest";
}

void AssetTestBasic::execute(){
    CCLOG("AssetTestBasic::execute");
    CCLOG("now sending asset request.");
    
    // ダウンロードのテストなので、ファイルがすでに存在したらいったん削除
    std::string path = CCFileUtils::sharedFileUtils()->getWritablePath().append("Icon.png");
    FILE *fp;
    if ((fp = fopen(path.c_str(), "r")) != NULL){
        CCLOG("File is already exists : %s", path.c_str());
        fclose(fp);
        remove(path.c_str());
        CCLOG("Removed File.");
    }

    // リクエスト
    libgss::AssetRequest* request = new libgss::AssetRequest(false, "Icon.png", this, gssAssetResponse(AssetTestBasic::OnComplete));
    libgss::Network::instance()->send(request);
    request->release();
    
    CCLOG("sent request");
}

void AssetTestBasic::OnComplete(libgss::AssetResponse* response){
    if (!response->success()) {
        CCLOG("error has occured when calling asset API. error code = %d", response->code());
        return;
    }
    
    std::vector<char>* data = response->data();
    if (data == NULL) {
        CCLOG("no data has returned.");
        return;
    }

    std::string path = CCFileUtils::sharedFileUtils()->getWritablePath().append("Icon.png");
    FILE *fp;
    if ((fp = fopen(path.c_str(), "w+")) == NULL) {
        CCLOG("error has occured when opening file.");
        return;
    }
    
    for(std::vector<char>::iterator it = data->begin(); it != data->end(); it++){
        char d =  *it;
        fwrite(&d, sizeof(char), 1, fp);
	}
    
    fclose(fp);
    
    CCLOG("File has created. %s", path.c_str());
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCSprite *icon = CCSprite::create(path.c_str(), CCRectMake(0, 0, 57, 57) );
    if(icon){
        icon->setPosition(cocos2d::CCPointMake(winSize.width / 2, winSize.height / 2));
        this->addChild(icon);
    }
}


//////////////////////////////////////////////////////////////////////////
// implement AssetTestWithNoCallback
//////////////////////////////////////////////////////////////////////////

std::string AssetTestWithNoCallback::subtitle(){
    return "2. No callback pattern by using AssetRequest";
}


void AssetTestWithNoCallback::execute(){
    CCLOG("AssetTestWithNoCallback::execute");
    CCLOG("now sending asset request.");
    
    // ダウンロードのテストなので、ファイルがすでに存在したらいったん削除
    std::string path = CCFileUtils::sharedFileUtils()->getWritablePath().append("Icon@2x.png");
    FILE *fp;
    if ((fp = fopen(path.c_str(), "r")) != NULL){
        CCLOG("File is already exists : %s", path.c_str());
        fclose(fp);
        remove(path.c_str());
        CCLOG("Removed File.");
    }

    // リクエスト
    request_ = new libgss::AssetRequest(false, "Icon@2x.png");
    libgss::Network::instance()->send(request_);
    
    pollingCount_ = 0;
    
    this->schedule(schedule_selector(AssetTestWithNoCallback::poll));
    
    CCLOG("sent request");    
}

void AssetTestWithNoCallback::poll(float time){
    CCLOG("polling response. (%d)", ++pollingCount_);
    
    libgss::AssetResponse* response = request_->getResponseIfFinished();
    if (response == NULL) {
        CCLOG("not received response yet.");
        return;
    }
    
    CCLOG("received response.");
    
    this->unschedule(schedule_selector(AssetTestWithNoCallback::poll));
    
    if (!response->success()) {
        CCLOG("error has occured when calling asset API. error code = %d", response->code());
        return;
    }
    
    std::vector<char>* data = response->data();
    if (data == NULL) {
        CCLOG("no data has returned.");
        return;
    }
    
    std::string path = CCFileUtils::sharedFileUtils()->getWritablePath().append("Icon@2x.png");
    FILE *fp;
    if ((fp = fopen(path.c_str(), "w+")) == NULL) {
        CCLOG("error has occured when opening file.");
        return;
    }
    
    for(std::vector<char>::iterator it = data->begin(); it != data->end(); it++){
        char d =  *it;
        fwrite(&d, sizeof(char), 1, fp);
	}
    
    fclose(fp);
    
    request_->release();
    
    CCLOG("File has created. %s", path.c_str());
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCSprite *icon = CCSprite::create(path.c_str(), CCRectMake(0, 0, 114, 114) );
    if (icon) {
        icon->setPosition(cocos2d::CCPointMake(winSize.width / 2, winSize.height / 2));
        this->addChild(icon);
    }
}
