#ifndef __SampleApp__CurlAssetTest__
#define __SampleApp__CurlAssetTest__

#include "../BaseNotificationLayer.h"
#include "../GroundLayer.h"
#include "CurlDownloadTask.h"
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


class CurlAssetTestSingleThread : public BaseNotificationLayer
{
public:
    std::string subtitle();
    virtual void execute();
};

class CurlAssetTestMultiThread : public BaseNotificationLayer, DownloadProgressCheckerDelegate
{
    CurlDownloadTask* task_ = NULL;
    DownloadProgressChecker* responseChecker_ = NULL;

    pthread_t thread_;
    
public:
    ~CurlAssetTestMultiThread(){
        if (task_) {
            task_->autorelease();
        }
        if (responseChecker_) {
            responseChecker_->delegate = NULL;
            responseChecker_->autorelease();
        }
    }
    
    std::string subtitle();
    virtual void execute();

    // DownloadProgressCheckerDelegate 実装
    void notifyFinish();
    void notifyProgress(double total, double downloaded);
};


#endif /* defined(__SampleApp__CurlAssetTest__) */
