//
//  CurlAssetTest.cpp
//  SampleApp
//
//  Created by 下川 北斗 on 2013/10/02.
//
//

#include "CurlAssetTest.h"
#include "CurlDownloadTask.h"


//////////////////////////////////////////////////////////////////////////
// local function
//////////////////////////////////////////////////////////////////////////

enum
{
    kCurlAssetTestSingleThread,
    
    kCurlAssetTestsCount,
};

static int testIdx = 0;

BaseNotificationLayer* createCurlAssetTest(int nIndex)
{
    switch(nIndex)
    {
        case kCurlAssetTestSingleThread: return new CurlAssetTestSingleThread();
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
    int responseCode = task->execute();
    
    CCLOG("HTTP status code is: %d", responseCode);

    if (responseCode == 200) {
        // ダウンロードした画像を表示
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        CCSprite *icon = CCSprite::create(filePath.c_str(), CCRectMake(0, 0, 114, 114) );
        if (icon) {
            icon->setPosition(cocos2d::CCPointMake(winSize.width / 2, winSize.height / 2));
            this->addChild(icon);
        }
    }
}

