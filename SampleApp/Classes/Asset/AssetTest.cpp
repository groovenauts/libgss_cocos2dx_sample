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
    kPrivateAssetTest = 0,
    kPrivateAssetTestWithNoCallback,
    kPublicAssetTest,
    
    kAssetTestsCount,
}; 

static int testIdx = 0;

BaseNotificationLayer* createAssetTest(int nIndex)
{
    switch(nIndex)
    {
    case kPrivateAssetTest: return new PrivateAssetTest();
    case kPrivateAssetTestWithNoCallback: return new PrivateAssetTestWithNoCallback();
    case kPublicAssetTest: return new PublicAssetTest();
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

std::string PrivateAssetTest::subtitle(){
    return "1. Private asset request by using AssetRequest";
}

void PrivateAssetTest::execute(){
    CCLOG("AssetTestBasic::execute");
    CCLOG("now sending asset request.");
    
    // リクエスト
    libgss::AssetRequest* request = new libgss::AssetRequest(false, "Icon.png", this, gssAssetResponse(PrivateAssetTest::OnComplete));
    libgss::Network::instance()->send(request);
    request->release();
    
    CCLOG("sent request");
}

void PrivateAssetTest::OnComplete(libgss::AssetResponse* response){
    if (!response->success()) {
        CCLOG("Error has occured. %d\n%s", response->code(), response->message().c_str());
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

std::string PrivateAssetTestWithNoCallback::subtitle(){
    return "2. No callback pattern by using AssetRequest";
}


void PrivateAssetTestWithNoCallback::execute(){
    CCLOG("AssetTestWithNoCallback::execute");
    CCLOG("now sending asset request.");
    
    // リクエスト
    request_ = new libgss::AssetRequest(false, "Icon@2x.png");
    libgss::Network::instance()->send(request_);
    
    pollingCount_ = 0;
    
    this->schedule(schedule_selector(PrivateAssetTestWithNoCallback::poll));
    
    CCLOG("sent request");    
}

void PrivateAssetTestWithNoCallback::poll(float time){
    CCLOG("polling response. (%d)", ++pollingCount_);
    
    libgss::AssetResponse* response = request_->getResponseIfFinished();
    if (response == NULL) {
        CCLOG("not received response yet.");
        return;
    }
    
    CCLOG("received response.");
    
    this->unschedule(schedule_selector(PrivateAssetTestWithNoCallback::poll));
    
    if (!response->success()) {
        CCLOG("Error has occured. %d\n%s", response->code(), response->message().c_str());
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


//////////////////////////////////////////////////////////////////////////
// implement PublicAssetTest
//////////////////////////////////////////////////////////////////////////

std::string PublicAssetTest::subtitle(){
    return "3. Public asset request by using AssetRequest";
}

void PublicAssetTest::execute(){
    CCLOG("AssetTestBasic::execute");
    CCLOG("now sending asset request.");
    
    // リクエスト
    libgss::AssetRequest* request = new libgss::AssetRequest(true, "Default.png", this, gssAssetResponse(PublicAssetTest::OnComplete));
    libgss::Network::instance()->send(request);
    request->release();
    
    CCLOG("sent request");
}

void PublicAssetTest::OnComplete(libgss::AssetResponse* response){
    if (!response->success()) {
        CCLOG("Error has occured. %d\n%s", response->code(), response->message().c_str());
        return;
    }
    
    std::vector<char>* data = response->data();
    if (data == NULL) {
        CCLOG("no data has returned.");
        return;
    }
    
    std::string path = CCFileUtils::sharedFileUtils()->getWritablePath().append("Default.png");
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
    
    CCSprite *pict = CCSprite::create(path.c_str(), CCRectMake(-80, -120, 320, 480) );
    if(pict){
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        CCLOG(" ===> (%f, %f)", winSize.width, winSize.height);
        pict->setPosition(cocos2d::CCPointMake(winSize.width / 2, winSize.height / 2));
        this->addChild(pict);
    }
}

