//
//  CurlAssetTest.cpp
//  SampleApp
//
//  Created by 下川 北斗 on 2013/10/02.
//
//

#include "CurlAssetTest.h"


//////////////////////////////////////////////////////////////////////////
// local function
//////////////////////////////////////////////////////////////////////////

enum
{
    kCurlAssetTestBasic,
    
    kCurlAssetTestsCount,
};

static int testIdx = 0;

BaseNotificationLayer* createCurlAssetTest(int nIndex)
{
    switch(nIndex)
    {
        case kCurlAssetTestBasic: return new CurlAssetTestBasic();
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
// implement CurlAssetTestBasic
//////////////////////////////////////////////////////////////////////////

size_t write_data_callback( char *ptr, size_t size, size_t nmemb, void *obj){
    CurlAssetTestBasic* test = (CurlAssetTestBasic*)obj;
    test->writeData(ptr, size, nmemb);
    return size * nmemb;
}

std::string CurlAssetTestBasic::subtitle(){
    return "1. Asset with curl, without using AssetRequest (single thread)";
}

void CurlAssetTestBasic::execute(){
    
    filePath_ = CCFileUtils::sharedFileUtils()->getWritablePath().append("Icon@2x.png");
    
    // ダウンロードのテストなので、ファイルがすでに存在したらいったん削除
    FILE *fp;
    if ((fp = fopen(filePath_.c_str(), "r")) != NULL){
        CCLOG("File is already exists : %s", filePath_.c_str());
        fclose(fp);
        remove(filePath_.c_str());
        CCLOG("Removed File.");
    }
    
    libgss::Network* network = libgss::Network::instance();
    std::string url = network->apiServerHttpUrlBase()
    + "/api/1.0.0/assets?auth_token="
    + network->authToken() + "&path=Icon@2x.png";
    
    std::vector<std::string> headers;
    headers.push_back(std::string("X-Client-Version: ") + network->clientVersion());
    headers.push_back(std::string("X-Device-Type: ") + network->deviceType());
    
    int responseCode = -1;
    
    CURL *curl = curl_easy_init();
    this->doCurl(curl, url, headers, &responseCode);
    curl_easy_cleanup(curl);
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCSprite *icon = CCSprite::create(filePath_.c_str(), CCRectMake(0, 0, 114, 114) );
    if (icon) {
        icon->setPosition(cocos2d::CCPointMake(winSize.width / 2, winSize.height / 2));
        this->addChild(icon);
    }
}

CURLcode CurlAssetTestBasic::doCurl(CURL *curl, std::string& url, std::vector<std::string> &headers, int* responseCode){
    CURLcode code;
    
    code = curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errorBuffer_);
    if (code != CURLE_OK) {
        return code;
    }
    //    code = curl_easy_setopt(curl, CURLOPT_TIMEOUT, 60);
    //    if (code != CURLE_OK) {
    //        return false;
    //    }
    //    code = curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 300);
    //    if (code != CURLE_OK) {
    //        return false;
    //    }
    
    struct curl_slist *cHeaders = NULL;
    if(!headers.empty()){
        for(std::vector<std::string>::iterator it=headers.begin();it!=headers.end();it++){
            cHeaders = curl_slist_append(cHeaders,it->c_str());
        }
        code = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, cHeaders);
        if (code != CURLE_OK) {
            return code;
        }
    }
    
    code = curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    if (code != CURLE_OK) {
        return code;
    }
    
    code = curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, url.c_str());
    if (code != CURLE_OK) {
        return code;
    }
    
    code = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data_callback);
    if (code != CURLE_OK) {
        return code;
    }
    
    code = curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
    if (code != CURLE_OK) {
        return code;
    }
    
    code = curl_easy_perform(curl);
    if (code != CURLE_OK) {
        return code;
    }
    
    curl_slist_free_all(cHeaders);
    
    code = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, responseCode);
    if (code != CURLE_OK || *responseCode != 200)
    {
        code = CURLE_HTTP_RETURNED_ERROR;
    }
    
    return code;
}

void CurlAssetTestBasic::writeData(char *dataPtr, size_t size, size_t nmemb){
    FILE *fp;
    if ((fp = fopen(filePath_.c_str(), "w+")) == NULL) {
        CCLOG("error has occured when opening file.");
        return;
    }
    
    fwrite(dataPtr, size, nmemb, fp);
    
    fclose(fp);
    
    CCLOG("file was created. %s", filePath_.c_str());
}
