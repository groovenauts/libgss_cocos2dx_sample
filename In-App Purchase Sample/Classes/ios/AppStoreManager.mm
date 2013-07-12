//
//  PuchaseManager.m
//  BridgeSample
//
//  Created by 下川 北斗 on 2013/07/04.
//
//

#import <StoreKit/StoreKit.h>

#import "AppStoreManager.h"

using namespace cocos2d;


#pragma mark - IOSPaymentTransactionManager (Objective-C) -
@interface IOSProductRequester : NSObject <SKProductsRequestDelegate>{
@private
    cocos2d::CCObject* callbackObj_;
    RequestProductResponse callbackMethod_;
}
@end


@implementation IOSProductRequester

+ (id)createInstance{
    return [[[IOSProductRequester alloc] init] autorelease];
}

- (void)requestProduct:(cocos2d::CCArray*)productIdentifiers
           callbackObj:(cocos2d::CCObject*)callbackObj
        callbackMethod:(RequestProductResponse)callbackMethod{
    [self retain];
    callbackObj->retain();
    callbackObj_ = callbackObj;
    callbackMethod_ = callbackMethod;
    
    NSMutableSet* identifiers = [NSMutableSet set];
    for (int i = 0; i < productIdentifiers->count(); i++) {
        cocos2d::CCString* identifier = (cocos2d::CCString*)productIdentifiers->objectAtIndex(i);
        [identifiers addObject:[NSString stringWithUTF8String:identifier->getCString()]];
    }
    SKProductsRequest *request= [[SKProductsRequest alloc] initWithProductIdentifiers:identifiers];
    request.delegate = self;
    [request start];
}

#pragma mark - SKProductsRequestDelegate implements

- (void)productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response{
    cocos2d::CCArray* validProducts = new cocos2d::CCArray();
    for(int i = 0; i < [response.products count]; i++){
        SKProduct* skProduct = [response.products objectAtIndex:i];
        
        NSNumberFormatter *priceFormatter = [[NSNumberFormatter alloc] init];
        [priceFormatter setFormatterBehavior:NSNumberFormatterBehavior10_4];
        [priceFormatter setNumberStyle:NSNumberFormatterCurrencyStyle];
        [priceFormatter setLocale:skProduct.priceLocale];
        NSString *formattedPrice = [priceFormatter stringFromNumber:skProduct.price];
        
        Product *product = new Product([skProduct.localizedDescription UTF8String],
                                       [skProduct.localizedTitle UTF8String],
                                       [formattedPrice UTF8String],
                                       [skProduct.productIdentifier UTF8String]);
        validProducts->addObject(product);
        product->release();
    }
    cocos2d::CCArray* invalidIdentifiers = new cocos2d::CCArray();
    for(int i = 0; i < [response.invalidProductIdentifiers count]; i++){
        NSString* identifier = [response.invalidProductIdentifiers objectAtIndex:i];
        invalidIdentifiers->addObject(ccs([identifier UTF8String]));
    }
    
    (callbackObj_->*callbackMethod_)(validProducts, invalidIdentifiers);
    validProducts->release();
    invalidIdentifiers->release();
    
    [self release];
}

@end


#pragma mark - IOSPaymentTransactionManager (Objective-C) -

@interface IOSPaymentTransactionManager : NSObject <SKPaymentTransactionObserver>{
@private
    IAppStoreTransactionObserver* transactionObserver_;
}
@property cocos2d::CCArray* products;
@end

static IOSPaymentTransactionManager* IOSPaymentTransactionManager_;

@implementation IOSPaymentTransactionManager

+ (IOSPaymentTransactionManager*)instance{
    if (!IOSPaymentTransactionManager_) {
        IOSPaymentTransactionManager_ = [[IOSPaymentTransactionManager alloc] init];
    }
    return IOSPaymentTransactionManager_;
}

- (void)setTransactionObserver:(IAppStoreTransactionObserver*)transactionObserver{
    transactionObserver_ = transactionObserver;
}

- (void)requestPayment:(const char*)productIdentifier{
    [self retain];
    
    SKMutablePayment *payment = [[SKMutablePayment alloc] init] ;
    payment.productIdentifier = [NSString stringWithUTF8String:productIdentifier];
    payment.quantity = 1;
    [[SKPaymentQueue defaultQueue] addPayment:payment];
    [payment release];
}


#pragma mark - SKPaymentTransactionObserver implements

- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions{
    for (int i = 0; i < [transactions count]; i++) {
        SKPaymentTransaction *transaction = [transactions objectAtIndex:i];
        switch (transaction.transactionState) {
            case SKPaymentTransactionStatePurchasing:
                if (transactionObserver_) {
                    transactionObserver_->transactionUpdatedToPurchasing();
                }
                break;
            case SKPaymentTransactionStatePurchased:{
                if (transactionObserver_) {
                    transactionObserver_->transactionUpdatedToPurchased([transaction.transactionIdentifier UTF8String],
                                                                        [transaction.transactionReceipt bytes],
                                                                        [transaction.transactionReceipt length]);
                }
                
                [queue finishTransaction:transaction];
                transactionObserver_->requestedFinishTransaction([transaction.transactionIdentifier UTF8String]);
                break;
            }
            case SKPaymentTransactionStateFailed:
                if (transactionObserver_) {
                    transactionObserver_->transactionUpdatedToFailed([transaction.transactionIdentifier UTF8String]);
                }
                [queue finishTransaction:transaction];
                transactionObserver_->requestedFinishTransaction([transaction.transactionIdentifier UTF8String]);
                break;
            case SKPaymentTransactionStateRestored:
                if (transactionObserver_) {
                    transactionObserver_->transactionUpdatedToRestored([transaction.transactionIdentifier UTF8String]);
                }
                break;
            default:
                break;
        }
    }
}

- (void)paymentQueue:(SKPaymentQueue *)queue removedTransactions:(NSArray *)transactions{
    for (int i = 0; i < [transactions count]; i++) {
        SKPaymentTransaction *transaction = [transactions objectAtIndex:i];
        if (transactionObserver_) {
            if(transaction.error){
                transactionObserver_->transactionRemovedWithError([[transaction transactionIdentifier] UTF8String]);
            }
            else{
                transactionObserver_->transactionRemovedWithSuccess([[transaction transactionIdentifier] UTF8String]);
            }
        }
    }
}

@end


#pragma mark - AppStoreManager (C++) -

void AppStoreManager::init(IAppStoreTransactionObserver* transactionObserver){
    [[IOSPaymentTransactionManager instance] setTransactionObserver:transactionObserver];
    [[SKPaymentQueue defaultQueue] addTransactionObserver:[IOSPaymentTransactionManager instance]];
}

void AppStoreManager::requestProduct(cocos2d::CCArray* productIdentifiers,
                                     cocos2d::CCObject* callbackObj,
                                     RequestProductResponse callbackMethod){
    [[IOSProductRequester createInstance] requestProduct:productIdentifiers
                                             callbackObj:callbackObj
                                          callbackMethod:callbackMethod];
}

void AppStoreManager::requestPayment(const std::string& productIdentifier){
    [[IOSPaymentTransactionManager instance] requestPayment:productIdentifier.c_str()];
}

cocos2d::CCArray* AppStoreManager::products(){
    return [IOSPaymentTransactionManager instance].products;
}
