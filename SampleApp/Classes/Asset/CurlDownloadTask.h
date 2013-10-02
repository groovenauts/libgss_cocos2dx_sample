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
class CurlDownloadTask: cocos2d::CCObject{
    std::string url_;
    std::string localPath_;
    
    double totalSize_;
    double downloadedSize_;
    
    std::vector<std::string> httpHeaders_;
    char errorBuffer_[CURL_ERROR_SIZE];
    
public:
    
    /**
     コンストラクタ
     @param url ダウンロードURL
     @param localPath ファイルが保存されるローカルのパス
     */
    CurlDownloadTask(std::string url, std::string localPath)
        :url_(url), localPath_(localPath){}
    
    /** HTTPのヘッダを設定 */
    void setHttpHeaders(std::vector<std::string> &httpHeaders){httpHeaders_ = httpHeaders;}
    
    /** URLを取得 */
    std::string url(){return url_;}
    /** ローカルのパスを取得 */
    std::string localPath(){return localPath_;}
    
    int execute();
    
    void writeData(char *dataPtr, size_t size, size_t nmemb);
    void progress(double total, double downloaded);
    
private:
    CURLcode doCurl(CURL *curl, int* responseCode);
};

#endif /* defined(__SampleApp__CurlDownloadTask__) */
