//
//  CurlAssetTest.h
//  SampleApp
//
//  Created by 下川 北斗 on 2013/10/02.
//
//

#ifndef __SampleApp__CurlAssetTest__
#define __SampleApp__CurlAssetTest__

#include "curl/curl.h"
#include "../BaseNotificationLayer.h"
#include "../GroundLayer.h"
#include <libGSS/libGSS.h>
#include <iostream>

class CurlAssetTest : public GroundLayer
{
public:
    void nextCallback(CCObject* pSender);
    void backCallback(CCObject* pSender);
    
    std::string title();
};

class CurlAssetTestScene : public TestScene
{
public:
    virtual void runThisTest();
};


class CurlAssetTestBasic : public BaseNotificationLayer
{
    std::string filePath_;
    char errorBuffer_[CURL_ERROR_SIZE];
public:
    std::string subtitle();
    virtual void execute();
    
    void writeData(char *dataPtr, size_t size, size_t nmemb);
    CURLcode doCurl(CURL *curl, std::string& url, std::vector<std::string> &headers, int* responseCode);
};



#endif /* defined(__SampleApp__CurlAssetTest__) */
