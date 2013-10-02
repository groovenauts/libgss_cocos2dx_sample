#include "CurlDownloadTask.h"

#include "cocos2d.h"

//////////////////////////////////////////////////////////////////////////
// curl callback function
//////////////////////////////////////////////////////////////////////////

size_t write_data_callback(char *ptr, size_t size, size_t nmemb, void *userdata){
    CurlDownloadTask* task = (CurlDownloadTask*)userdata;
    task->writeData(ptr, size, nmemb);
    return size * nmemb;
}

int progress_callback(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow){
    CurlDownloadTask* task = (CurlDownloadTask*)clientp;
    pthread_mutex_lock(&task->mutex);
    task->progress(dltotal, dlnow);
    pthread_mutex_unlock(&task->mutex);
    
    return 0;
}

//////////////////////////////////////////////////////////////////////////
// CurlDownloadTask implement
//////////////////////////////////////////////////////////////////////////


void CurlDownloadTask::execute(){
    finished_ = false;
    CURL *curl = curl_easy_init();
    responseCode_ = -1;
    CURLcode code = this->doCurl(curl);
    if (code != CURLE_OK) {
        CCLOG("Error on curl. Error code: %d", code);
    }
    curl_easy_cleanup(curl);
    finished_ = true;
}

CURLcode CurlDownloadTask::doCurl(CURL *curl){
    CURLcode code;
    
    // HTTPヘッダの設定
    struct curl_slist *cHeaders = NULL;
    if(!httpHeaders_.empty()){
        for(std::vector<std::string>::iterator it = httpHeaders_.begin(); it != httpHeaders_.end(); it++){
            cHeaders = curl_slist_append(cHeaders, it->c_str());
        }
        code = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, cHeaders);
        if (code != CURLE_OK) {
            return code;
        }
    }
    
    // URL
    code = curl_easy_setopt(curl, CURLOPT_URL, url_.c_str());
    if (code != CURLE_OK) {
        return code;
    }
    code = curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
    if (code != CURLE_OK) {
        return code;
    }
    
    // ファイル書き込みcallbackの設定
    code = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data_callback);
    if (code != CURLE_OK) {
        return code;
    }
    code = curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
    if (code != CURLE_OK) {
        return code;
    }
    
    // プログレスcallbackの設定
    code = curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0);
    if (code != CURLE_OK) {
        return code;
    }
    code = curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, progress_callback);
    if (code != CURLE_OK) {
        return code;
    }
    code = curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, this);
    if (code != CURLE_OK) {
        return code;
    }
    
    // オプション設定
    memset(errorBuffer_, 0, sizeof(char) * CURL_ERROR_SIZE);
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

    // 実行
    code = curl_easy_perform(curl);
    if (code != CURLE_OK) {
        return code;
    }
    
    curl_slist_free_all(cHeaders);
    
    code = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &responseCode_);
    if (code != CURLE_OK || responseCode_ != 200)
    {
        code = CURLE_HTTP_RETURNED_ERROR;
    }
    
    return code;
}


void CurlDownloadTask::writeData(char *dataPtr, size_t size, size_t nmemb){
    FILE *fp;
    if ((fp = fopen(localPath_.c_str(), "w+")) == NULL) {
        CCLOG("error has occured when opening file.");
        return;
    }
    
    fwrite(dataPtr, size, nmemb, fp);
    
    fclose(fp);
    
    CCLOG("File has created. %s", localPath_.c_str());
}


void CurlDownloadTask::progress(double total, double downloaded){
    totalSize_ = total;
    downloadedSize_ = downloaded;
}


//////////////////////////////////////////////////////////////////////////
// DownloadProgressChecker implement
//////////////////////////////////////////////////////////////////////////

DownloadProgressChecker::DownloadProgressChecker(CurlDownloadTask* task){
    task_ = task;
    task_->retain();
}
DownloadProgressChecker::~DownloadProgressChecker(){
    if (task_) {
        task_->release();
    }
}

void DownloadProgressChecker::run(){
    cocos2d::CCScheduler* scheduler = cocos2d::CCDirector::sharedDirector()->getScheduler();
    scheduler->scheduleSelector(cocos2d::SEL_SCHEDULE(&DownloadProgressChecker::check), this, 0, false);
}

void DownloadProgressChecker::stop(){
    cocos2d::CCScheduler* scheduler = cocos2d::CCDirector::sharedDirector()->getScheduler();
    scheduler->unscheduleSelector(cocos2d::SEL_SCHEDULE(&DownloadProgressChecker::check), this);
}

void DownloadProgressChecker::check(){
    CCLOG("execute DownloadProgressChecker::check");
    if (task_->isFinished()) {
        this->stop();
        if (delegate) {
            delegate->notifyFinish();
        }
    }
    else if (task_->totalSize() > 0 && delegate){
        pthread_mutex_lock(&task_->mutex);
        delegate->notifyProgress(task_->totalSize(), task_->downloadedSize());
        pthread_mutex_unlock(&task_->mutex);
    }
}
