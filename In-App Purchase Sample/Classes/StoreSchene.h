//
//  StoreSchene.h
//  In-App Purchase Sample
//
//  Created by 下川 北斗 on 2013/07/10.
//
//

#ifndef __In_App_Purchase_Sample__StoreSchene__
#define __In_App_Purchase_Sample__StoreSchene__

#include <iostream>

#include "cocos2d.h"
#include <libGSS/libGSS.h>

class Store : public cocos2d::CCLayer
{
    cocos2d::CCLabelTTF* _loadingLabel;
    cocos2d::CCArray* _products = new cocos2d::CCArray();
    
public:
    virtual bool init();
    
    static cocos2d::CCScene* scene();

    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(Store);
    
    ~Store(){
        _products->release();
    }
    
private:
    void loadedAppStoreProducts(libgss::ActionResponse* response);
    void storeItemSelectCallback(CCObject* pSender);
    void loadedProduct(cocos2d::CCArray* validIdentifiers, cocos2d::CCArray* invalidIdentifiers);
};


#endif /* defined(__In_App_Purchase_Sample__StoreSchene__) */
