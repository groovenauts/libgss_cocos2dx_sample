//
//  CurlDownloadTask.h
//  SampleApp
//
//  Created by 下川 北斗 on 2013/10/02.
//
//

#ifndef __SampleApp__CurlDownloadTask__
#define __SampleApp__CurlDownloadTask__

#include <iostream>
#include <vector>

#include "cocos2d.h"
#include "curl/curl.h"

/**
 ファイルの1ダウンロードを管理するクラス
 */
class CurlDownloadTask: public cocos2d::CCObject{
    std::string url_;
    std::string localPath_;
    
    double totalSize_ = 0;
    double downloadedSize_ = 0;
    
    std::vector<std::string> httpHeaders_;
    char errorBuffer_[CURL_ERROR_SIZE];
    
    bool finished_;
    int responseCode_ = -1;
    
public:
    pthread_mutex_t mutex; // マルチスレッドで利用する場合用
    
    /**
     コンストラクタ
     @param url ダウンロードURL
     @param localPath ファイルが保存されるローカルのパス
     */
    CurlDownloadTask(std::string url, std::string localPath):url_(url), localPath_(localPath){
        pthread_mutex_init(&mutex, NULL);
    }
    ~CurlDownloadTask(){
        pthread_mutex_destroy(&mutex);
    }
    
    /** HTTPのヘッダを設定 */
    void setHttpHeaders(std::vector<std::string> &httpHeaders){httpHeaders_ = httpHeaders;}
    
    /** URLを取得 */
    std::string url(){return url_;}
    /** ローカルのパスを取得 */
    std::string localPath(){return localPath_;}
    /** ダウンロードする合計サイズを取得（サイズ未取得の場合、0） */
    double totalSize(){return totalSize_;}
    /** ダウンロードしたサイズを取得 */
    double downloadedSize(){return downloadedSize_;}
    /** HTTPレスポンスコードを取得 */
    int responseCode(){return responseCode_;}
    
    /** ダウンロードの実行 */
    void execute();
    
    /** [コールバック用] データ書き込み */
    void writeData(char *dataPtr, size_t size, size_t nmemb);
    /** [コールバック用] プログレスの設定 */
    void progress(double total, double downloaded);
    
    bool isFinished(){
        return finished_;
    }
    
private:
    CURLcode doCurl(CURL *curl);
};



class DownloadProgressCheckerDelegate {
public:
    virtual void notifyFinish() = 0;
    virtual void notifyProgress(double total, double downloaded) = 0;
};

class DownloadProgressChecker : public cocos2d::CCObject {
    CurlDownloadTask* task_;
public:
    DownloadProgressCheckerDelegate* delegate;

    DownloadProgressChecker(CurlDownloadTask* task);
    ~DownloadProgressChecker();
    
    void run();
    void stop();
    
    void check();
};


#endif /* defined(__SampleApp__CurlDownloadTask__) */
