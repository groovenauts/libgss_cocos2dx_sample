//
//  StoreSchene.cpp
//  In-App Purchase Sample
//
//  Created by 下川 北斗 on 2013/07/10.
//
//

#include "StoreSchene.h"
#include "AppStoreManager.h"
#include "PurchaseScene.h"

using namespace cocos2d;

#define kLoadItemsActionId 1

CCScene* Store::scene()
{
    CCScene *scene = CCScene::create();
    Store *layer = Store::create();
    scene->addChild(layer);
    return scene;
}


bool Store::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    _loadingLabel = CCLabelTTF::create("Loading items...", "Arial", 30);
    _loadingLabel->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width / 2,
                                   CCDirector::sharedDirector()->getWinSize().height / 2));
    this->addChild(_loadingLabel);
    
    libgss::ActionRequest* request = new libgss::ActionRequest();
    request->setCallback(this, gssActionResponse(Store::loadedAppStoreProducts));
    libgss::AllAction* action = new libgss::AllAction("AppStoreProduct");
    action->setId(kLoadItemsActionId);
    request->addAction(action);
    action->release();
    libgss::Network::instance()->send(request);
    request->release();
    
    return true;
}

void Store::loadedAppStoreProducts(libgss::ActionResponse* response){
    if(!response->success()){
        _loadingLabel->setString("Failed to load items!");
        return;
    }
    
    libgss::JSONObject* result = response->result(kLoadItemsActionId);
    CCLOG("%s", result->toString().c_str());
    
    // ストアアイテム一覧取得
    CCArray* identifiers = new cocos2d::CCArray();
    for (int i = 0; i < result->arraySize(); i++) {
        CCString* identifier = ccs(result->get(i)->get("product_id")->toString());
        identifiers->addObject(identifier);
    }
    AppStoreManager::requestProduct(identifiers, this, (RequestProductResponse)&Store::loadedProduct);
    identifiers->release();
}

void Store::loadedProduct(cocos2d::CCArray* validProducts, cocos2d::CCArray* invalidIdentifiers){
    static const int kLineHeight = 40;
    
    this->removeAllChildren();
    
    CCMenu* menu =CCMenu::create();
    menu->setPosition( CCPointZero );
    CCSize visibleSize = CCEGLView::sharedOpenGLView()->getVisibleSize();
    
    _products->release();
    validProducts->retain();
    _products = validProducts;
    
    // build product menu
    for (int i = 0; i < validProducts->count(); i++) {
        Product* product = (Product*)validProducts->objectAtIndex(i);

        CCLabelTTF* nameLabel = CCLabelTTF::create(product->localizedTitle().c_str(), "Arial", 28);
        nameLabel->setPosition(CCPointMake(0, 0));
        nameLabel->setAnchorPoint(CCPointMake(0, 0));
        nameLabel->setHorizontalAlignment(kCCTextAlignmentLeft);
        CCMenuItemLabel* menuItem = CCMenuItemLabel::create(nameLabel, this, menu_selector(Store::storeItemSelectCallback));

        CCLabelTTF* priceLabel = CCLabelTTF::create(product->price().c_str(), "Arial", 12);
        priceLabel->setPosition(CCPointMake(100, 0));
        priceLabel->setAnchorPoint(CCPointMake(0, 0));
        priceLabel->setHorizontalAlignment(kCCTextAlignmentLeft);
        menuItem->addChild(priceLabel);
        
        CCLabelTTF* descriptionLabel = CCLabelTTF::create(product->localizedDescription().c_str(), "Arial", 12);
        descriptionLabel->setPosition(CCPointMake(100, 14));
        descriptionLabel->setAnchorPoint(CCPointMake(0, 0));
        descriptionLabel->setHorizontalAlignment(kCCTextAlignmentLeft);
        menuItem->addChild(descriptionLabel);
        
        menuItem->setPosition(CCPointMake(60, visibleSize.height - (i + 1) * kLineHeight));
        menuItem->setUserData(product);
        menu->addChild(menuItem, i + 10000);
    }
    this->addChild(menu);
}

void Store::storeItemSelectCallback(CCObject* sender){
    Product* product = (Product*)((CCNode*)sender)->getUserData();
    PurchaseProcess::run(product->productIdentifier());
}
