// #define COCOS2D_DEBUG   1

//
// メニュー名
// Curl Asset Test : curlによるアセットのテスト
//
// libgss::AssetRequest を利用せずにcurlでアセットをダウンロードするテスト
// 1. Asset with curl, without using AssetRequest (single thread) : シングルスレッドでのサンプル
// 2. Asset with curl, without using AssetRequest (multi thread) : マルチスレッドでのサンプル
//


#include "CurlAssetTest.h"

#include <pthread.h>

//////////////////////////////////////////////////////////////////////////
// local function
//////////////////////////////////////////////////////////////////////////

enum
{
    kCurlAssetTestSingleThread,
    kCurlAssetTestMultiThread,
    
    kCurlAssetTestsCount,
};

static int testIdx = 0;

BaseNotificationLayer* createCurlAssetTest(int nIndex)
{
    switch(nIndex)
    {
        case kCurlAssetTestSingleThread: return new CurlAssetTestSingleThread();
        case kCurlAssetTestMultiThread: return new CurlAssetTestMultiThread();
        default: return 0;
    }
}

CCLayer* restartCurlAssetTest()
{
    CurlAssetTest* pContainerLayer = new CurlAssetTest;
    pContainerLayer->autorelease();
    
    BaseNotificationLayer* pTestLayer = createCurlAssetTest(testIdx);
    pTestLayer->autorelease();
    pContainerLayer->addBaseNotificationLayer(pTestLayer);
    
    return pContainerLayer;
}

CCLayer* nextCurlAssetTest()
{
    testIdx++;
    testIdx = testIdx % kCurlAssetTestsCount;
    
    return restartCurlAssetTest();
}

CCLayer* backCurlAssetTest()
{
    testIdx--;
    int total = kCurlAssetTestsCount;
    if( testIdx < 0 )
        testIdx += total;
    
    return restartCurlAssetTest();
}

//////////////////////////////////////////////////////////////////////////
// implement CurlAssetTest
//////////////////////////////////////////////////////////////////////////

void CurlAssetTest::nextCallback(CCObject* pSender)
{
    CCScene* s = new CurlAssetTestScene();
    s->addChild( nextCurlAssetTest() );
    CCDirector::sharedDirector()->replaceScene(s);
    s->release();
}

void CurlAssetTest::backCallback(CCObject* pSender)
{
    CCScene* s = new CurlAssetTestScene();
    s->addChild( backCurlAssetTest() );
    CCDirector::sharedDirector()->replaceScene(s);
    s->release();
}

std::string CurlAssetTest::title()
{
    return "Asset sample with curl";
}

//////////////////////////////////////////////////////////////////////////
// implement CurlAssetTestScene
//////////////////////////////////////////////////////////////////////////

void CurlAssetTestScene::runThisTest()
{
    testIdx--;
    CCLayer* pLayer = nextCurlAssetTest();
    addChild(pLayer);
    
    CCDirector::sharedDirector()->replaceScene(this);
}


//////////////////////////////////////////////////////////////////////////
// implement CurlAssetTestSingleThread
//////////////////////////////////////////////////////////////////////////

std::string CurlAssetTestSingleThread::subtitle(){
    return "1. Asset with curl, without using AssetRequest (single thread)";
}

void CurlAssetTestSingleThread::execute(){
    
    std::string filePath = CCFileUtils::sharedFileUtils()->getWritablePath().append("Icon@2x.png");
    
    // ダウンロードのテストなので、ファイルがすでに存在したらいったん削除
    FILE *fp;
    if ((fp = fopen(filePath.c_str(), "r")) != NULL){
        CCLOG("File is already exists : %s", filePath.c_str());
        fclose(fp);
        remove(filePath.c_str());
        CCLOG("Removed File.");
    }
    
    // 接続情報
    libgss::Network* network = libgss::Network::instance();
    std::string url = network->apiServerHttpUrlBase()
                        + "/api/1.0.0/assets?auth_token="
                        + network->authToken() + "&path=Icon@2x.png";
    std::vector<std::string> headers;
    headers.push_back(std::string("X-Client-Version: ") + network->clientVersion());
    headers.push_back(std::string("X-Device-Type: ") + network->deviceType());
    
    // ダウンロード
    CurlDownloadTask* task = new CurlDownloadTask(url, filePath.c_str());
    task->setHttpHeaders(headers);
    task->execute();
    
    CCLOG("HTTP status code is: %d", task->responseCode());

    if (task->responseCode() == 200) {
        // ダウンロードした画像を表示
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        CCSprite *icon = CCSprite::create(filePath.c_str(), CCRectMake(0, 0, 114, 114) );
        if (icon) {
            icon->setPosition(cocos2d::CCPointMake(winSize.width / 2, winSize.height / 2));
            this->addChild(icon);
        }
    }
    
    task->release();
}



//////////////////////////////////////////////////////////////////////////
// implement CurlAssetTestMultiThread
//////////////////////////////////////////////////////////////////////////

void* executeTaskThread(void* param){
    CCLOG("start executeTaskThread.");
    
    CurlDownloadTask* task = (CurlDownloadTask*)param;
    task->retain();
    task->execute();
    while (1) {
        if (task->isFinished()) {
            break;
        }
    }
    task->release();

    CCLOG("end executeTaskThread.");
    return 0;
}


std::string CurlAssetTestMultiThread::subtitle(){
    return "2. Asset with curl, without using AssetRequest (multi thread)";
}

void CurlAssetTestMultiThread::execute(){
    
    std::string filePath = CCFileUtils::sharedFileUtils()->getWritablePath().append("Icon@2x.png");
    
    // ダウンロードのテストなので、ファイルがすでに存在したらいったん削除
    FILE *fp;
    if ((fp = fopen(filePath.c_str(), "r")) != NULL){
        CCLOG("File is already exists : %s", filePath.c_str());
        fclose(fp);
        remove(filePath.c_str());
        CCLOG("Removed File.");
    }
    
    // 接続情報
    libgss::Network* network = libgss::Network::instance();
    std::string url = network->apiServerHttpUrlBase()
    + "/api/1.0.0/assets?auth_token="
    + network->authToken() + "&path=Icon@2x.png";
    std::vector<std::string> headers;
    headers.push_back(std::string("X-Client-Version: ") + network->clientVersion());
    headers.push_back(std::string("X-Device-Type: ") + network->deviceType());
    
    
    if (task_) {
        task_->release();
    }
    task_ = new CurlDownloadTask(url, filePath.c_str());
    task_->setHttpHeaders(headers);
    
    if (responseChecker_) {
        responseChecker_->delegate = NULL;
        responseChecker_->release();
    }
    responseChecker_ = new DownloadProgressChecker(task_);
    responseChecker_->delegate = this;
    responseChecker_->run();
    
    pthread_create(&thread_, NULL, executeTaskThread, task_);
    pthread_detach(thread_);
}

void CurlAssetTestMultiThread::notifyFinish(){
    CCLOG("HTTP status code is: %d", task_->responseCode());
    
    if (task_->responseCode() == 200) {
        // ダウンロードした画像を表示
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        CCSprite *icon = CCSprite::create(task_->localPath().c_str(), CCRectMake(0, 0, 114, 114) );
        if (icon) {
            icon->setPosition(cocos2d::CCPointMake(winSize.width / 2, winSize.height / 2));
            this->addChild(icon);
        }
    }
}

void CurlAssetTestMultiThread::notifyProgress(double total, double downloaded){
    CCLOG("downloaded %f of %f ...", downloaded, total);
}