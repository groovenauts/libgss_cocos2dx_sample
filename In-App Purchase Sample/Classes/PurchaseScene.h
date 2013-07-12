//
//  PurchaseScene.h
//  In-App Purchase Sample
//
//  Created by 下川 北斗 on 2013/07/11.
//
//

#ifndef __In_App_Purchase_Sample__PurchaseScene__
#define __In_App_Purchase_Sample__PurchaseScene__

#include <iostream>

#include "cocos2d.h"
#include "AppStoreManager.h"
#include <libGSS/libGSS.h>

class PurchaseProcess: public cocos2d::CCObject, public IAppStoreTransactionObserver{
public:
    static PurchaseProcess* instance();
    
    static void run(const std::string& productId);
    static bool existsProcessingData();
    static void resume();
    
    void transactionUpdatedToPurchasing();
    void transactionUpdatedToPurchased(const std::string& transactionId, const void* receiptData, int receiptDataLength);
    void transactionUpdatedToFailed(const std::string& transactionId);
    void transactionUpdatedToRestored(const std::string& transactionId);
    void requestedFinishTransaction(const std::string& transactionId);
    void transactionRemovedWithSuccess(const std::string& transactionId);
    void transactionRemovedWithError(const std::string& transactionId);
private:
    PurchaseProcess():CCObject(), IAppStoreTransactionObserver(){};
    void requestPayment();
    void updateGameData();
    void didUpdateGameData(libgss::ActionResponse* response);
    void login();
    void loggedIn(libgss::LoginResponse* response);
};


class PurchaseModal : public cocos2d::CCLayer
{
    cocos2d::CCLabelTTF* _mainLabel;
public:
    virtual bool init();
    
    static cocos2d::CCScene* scene(const std::string &message);
    
    void setMessage(const std::string& message);

    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(PurchaseModal);
};



#endif /* defined(__In_App_Purchase_Sample__PurchaseScene__) */
