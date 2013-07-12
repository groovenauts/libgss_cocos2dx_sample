//
//  PurchaseProcess.cpp
//  In-App Purchase Sample
//
//  Created by 下川 北斗 on 2013/07/11.
//
//

#include "PurchaseScene.h"

#include "AppStoreManager.h"

#include <libGSS/libGSS.h>

#include "Settings.h"

using namespace cocos2d;


#define kPurchaseProcessingDataKey "processing_data"

#define kPurchaseStatusStarted "purchase_started"
#define kPurchaseStatusWaitingPurchasing "waiting_purchasing"
#define kPurchaseStatusPurchasing "purchasing"
#define kPurchaseStatusPurchased "purchased"
#define kPurchaseStatusRemovingTransaction "removing_transaction"
#define kPurchaseStatusTransactionRemoved "transaction_removed"

/** Base64 encode */
bool encodeBase64(const unsigned char* src, size_t length, std::string& dst){
    const std::string table("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/");
    std::string       cdst;
    
    for (std::size_t i = 0; i < length; ++i) {
        switch (i % 3) {
            case 0:
                cdst.push_back(table[(src[i] & 0xFC) >> 2]);
                if (i + 1 == length) {
                    cdst.push_back(table[(src[i] & 0x03) << 4]);
                    cdst.push_back('=');
                    cdst.push_back('=');
                }
                
                break;
            case 1:
                cdst.push_back(table[((src[i - 1] & 0x03) << 4) | ((src[i + 0] & 0xF0) >> 4)]);
                if (i + 1 == length) {
                    cdst.push_back(table[(src[i] & 0x0F) << 2]);
                    cdst.push_back('=');
                }
                
                break;
            case 2:
                cdst.push_back(table[((src[i - 1] & 0x0F) << 2) | ((src[i + 0] & 0xC0) >> 6)]);
                cdst.push_back(table[src[i] & 0x3F]);
                
                break;
        }
    }
    
    dst.swap(cdst);
    
    return true;
}

libgss::JSONObject* getProcessingData(){
    std::string processingData = CCUserDefault::sharedUserDefault()->getStringForKey(kPurchaseProcessingDataKey);
    if (processingData == "") {
        return NULL;
    }
    return libgss::JSONObject::parse(processingData, NULL);
}

void saveProcessingData(libgss::JSONObject* data){
    CCUserDefault::sharedUserDefault()->setStringForKey(kPurchaseProcessingDataKey, data->toString());
    CCUserDefault::sharedUserDefault()->flush();
}

void createProcessingData(const std::string& key, const std::string& data){
    libgss::JSONObject processingData;
    processingData.set(key, data);
    saveProcessingData(&processingData);
}

void updateProcessingData(const std::string& key, const std::string& data){
    libgss::JSONObject* processingData = getProcessingData();
    if (processingData == NULL) {
        createProcessingData(key, data);
    }
    else{
        processingData->set(key, data);
        saveProcessingData(processingData);
    }
}

void deleteProcessingData(){
    CCUserDefault::sharedUserDefault()->setStringForKey(kPurchaseProcessingDataKey, "");
    CCUserDefault::sharedUserDefault()->flush();
}


#pragma mark - PurchaseProcess implement

static PurchaseProcess* process_;

PurchaseProcess* PurchaseProcess::instance(){
    if (!process_) {
        process_ = new PurchaseProcess();
    }
    return process_;
}


void PurchaseProcess::run(const std::string& productId){
    CCDirector::sharedDirector()->pushScene(PurchaseModal::scene("Processing purchase..."));
    
    libgss::JSONObject data;
    data.set("product_id", productId);
    data.set("status", kPurchaseStatusStarted);
    saveProcessingData(&data);
    
    PurchaseProcess::instance()->requestPayment();
}

void PurchaseProcess::requestPayment(){
    libgss::JSONObject* data = getProcessingData();
    AppStoreManager::requestPayment(data->get("product_id")->toString());
    updateProcessingData("status", kPurchaseStatusWaitingPurchasing);
}

bool PurchaseProcess::existsProcessingData(){
    libgss::JSONObject* data = getProcessingData();
    if (data == NULL) {
        return false;
    }
    else{
        CCLOG("Exists processing purchase data. %s", data->toString().c_str());
        return true;
    }
}

void PurchaseProcess::resume(){
    CCDirector::sharedDirector()->pushScene(PurchaseModal::scene("Processing resumed purchase..."));
    
    libgss::JSONObject* processingData = getProcessingData();
    std::string status = processingData->get("status")->toString();
    if (status == kPurchaseStatusStarted) {
        PurchaseProcess::instance()->requestPayment();
    }
    else if (status == kPurchaseStatusWaitingPurchasing){
        // do nothing. wait for notification.
    }
    else if (status == kPurchaseStatusPurchasing){
        // do nothing. wait for notification.
    }
    else if (status == kPurchaseStatusPurchased){
        // do nothing. wait for notification.
    }
    else if (status == kPurchaseStatusRemovingTransaction){
        PurchaseProcess::instance()->updateGameData();
    }
    else if (status == kPurchaseStatusTransactionRemoved){
        PurchaseProcess::instance()->updateGameData();
    }
}

void PurchaseProcess::transactionUpdatedToPurchasing(){
    CCLOG("Transaction status updated to 'purchasing'.");
    updateProcessingData("status", kPurchaseStatusPurchasing);
}

void PurchaseProcess::transactionUpdatedToPurchased(const std::string& transactionId,
                                                       const void* receiptData,
                                                       int receiptDataLength){
    
    CCLOG("Transaction status updated to 'purchased'. %s", transactionId.c_str());
//    std::string receipt((const char*)receiptData, receiptDataLength);
//    CCLOG("Receipt (raw): %s", receipt.c_str());
    std::string base64Receipt;
    encodeBase64((const unsigned char*)receiptData,
                 receiptDataLength,
                 base64Receipt);
//    CCLOG("Receipt (base64): %s", base64Receipt.c_str());
    
    libgss::JSONObject* processingData = getProcessingData();
    if (!processingData) {
        processingData = new libgss::JSONObject();
        processingData->autorelease();
    }
    processingData->set("status", kPurchaseStatusPurchased);
    processingData->set("receipt_data", base64Receipt);
    processingData->set("transaction_id", transactionId.c_str());
    saveProcessingData(processingData);
}

void PurchaseProcess::transactionUpdatedToFailed(const std::string& transactionId){
    CCLOG("Transaction failed. %s", transactionId.c_str());
}

void PurchaseProcess::transactionUpdatedToRestored(const std::string& transactionId){
    CCLOG("Transaction restored. %s", transactionId.c_str());
}

void PurchaseProcess::requestedFinishTransaction(const std::string& transactionId){
    CCLOG("Requested finish transaction. %s", transactionId.c_str());
    updateProcessingData("status", kPurchaseStatusRemovingTransaction);
}

void PurchaseProcess::transactionRemovedWithSuccess(const std::string& transactionId){
    CCLOG("Transaction removed. %s", transactionId.c_str());
    
    updateProcessingData("status", kPurchaseStatusTransactionRemoved);
    
    this->updateGameData();
}

void PurchaseProcess::transactionRemovedWithError(const std::string& transactionId){
    CCLOG("Transaction removed. %s", transactionId.c_str());
    deleteProcessingData();
    CCDirector::sharedDirector()->popScene();
}

void PurchaseProcess::login(){
    // Login
    libgss::LoginRequest* loginRequest = new libgss::LoginRequest(Settings::playerId(), Settings::nickname());
    loginRequest->setCallback(this, gssLoginResponse(PurchaseProcess::loggedIn));
    libgss::Network::instance()->send(loginRequest);
    loginRequest->release();
    
    CCLOG("Sent login request.");
}

void PurchaseProcess::loggedIn(libgss::LoginResponse* response){
    if (response->success()) {
        this->updateGameData();
    }
    else{
        CCLOG("Failed to login!");
    }
}

void PurchaseProcess::updateGameData(){
    std::string base64Receipt = getProcessingData()->get("receipt_data")->toString();
    
    if (libgss::Network::instance()->playerId() == "") {
        this->login();
        return;
    }
    
    libgss::ActionRequest *request = new libgss::ActionRequest();
    request->setCallback(this, gssActionResponse(PurchaseProcess::didUpdateGameData));
    libgss::ExecuteAction* action = new libgss::ExecuteAction("RubyStoredScript", "process_receipt");
    libgss::JSONObject args;
    args.set("receipt_data", base64Receipt.c_str());
    action->setArgs(args);
    request->addAction(action);
    libgss::Network::instance()->send(request);
}

void PurchaseProcess::didUpdateGameData(libgss::ActionResponse* response){
    if (!response->success()) {
        CCLOG("Fail to update game data.　Retry.");
        this->updateGameData();
        return;
    }
    
    deleteProcessingData();
    CCLOG("%s", response->outputs()->toString().c_str());
    if (response->success()) {
        CCLOG("Game data udpated.");
    }
    else{
        CCLOG("Failed to update game data.");
    }
    CCDirector::sharedDirector()->popScene();
}


#pragma mark - PurchaseModal implement

CCScene* PurchaseModal::scene(const std::string& message){
    CCScene *scene = CCScene::create();
    PurchaseModal *layer = PurchaseModal::create();
    layer->setMessage(message);
    scene->addChild(layer);
    return scene;
}

bool PurchaseModal::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    _mainLabel = CCLabelTTF::create("", "Arial", 30);
    _mainLabel->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width / 2,
                                   CCDirector::sharedDirector()->getWinSize().height / 2));
    this->addChild(_mainLabel);

    return true;
}

void PurchaseModal::setMessage(const std::string& message){
    _mainLabel->setString(message.c_str());
}