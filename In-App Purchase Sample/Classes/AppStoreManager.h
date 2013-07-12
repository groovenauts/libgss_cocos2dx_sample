//
//  PuchaseManager.h
//  BridgeSample
//
//  Created by 下川 北斗 on 2013/07/04.
//
//


#ifndef __BridgeSample__AppStoreManager__
#define __BridgeSample__AppStoreManager__

#include <iostream>
#include "cocos2d.h"


typedef void (cocos2d::CCObject::*RequestProductResponse)(cocos2d::CCArray* validProducts, cocos2d::CCArray* invalidIdentifiers);
#define requestProductResponse(_SELECTOR) (RequestProductResponse)(&_SELECTOR)

class IAppStoreTransactionObserver{
public:
    virtual void transactionUpdatedToPurchasing() = 0;
    virtual void transactionUpdatedToPurchased(const std::string& transactionId, const void* receiptData, int receiptDataLength) = 0;
    virtual void transactionUpdatedToFailed(const std::string& transactionId) = 0;
    virtual void transactionUpdatedToRestored(const std::string& transactionId) = 0;
    virtual void requestedFinishTransaction(const std::string& transactionId) = 0;
    virtual void transactionRemovedWithError(const std::string& transactionId) = 0;
    virtual void transactionRemovedWithSuccess(const std::string& transactionId) = 0;
};


class AppStoreManager {
public:
    static void init(IAppStoreTransactionObserver* transactionObserver);
    static void requestProduct(cocos2d::CCArray* productIdentifiers,
                               cocos2d::CCObject* callbackObj,
                               RequestProductResponse callbackMethod);
    static void requestPayment(const std::string& productIdentifier);
    static cocos2d::CCArray* products();
};

class Product : public cocos2d::CCObject{
    std::string localizedDescription_;
    std::string localizedTitle_;
    std::string price_;
    std::string productIdentifier_;
    
public:
    Product(const std::string& localizedDescription,
                 const std::string& localizedTitle,
                 const std::string& price,
                 const std::string& productIdentifier)
    :localizedDescription_(localizedDescription)
    ,localizedTitle_(localizedTitle)
    ,price_(price)
    ,productIdentifier_(productIdentifier){};
    
    std::string localizedDescription(){return localizedDescription_;}
    std::string localizedTitle(){return localizedTitle_;}
    std::string price(){return price_;}
    std::string productIdentifier(){return productIdentifier_;}
};


#endif /* defined(__BridgeSample__AppStoreManager__) */
