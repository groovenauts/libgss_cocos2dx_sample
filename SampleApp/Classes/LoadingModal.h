//
//  LoadingModal.h
//  SampleApp
//
//  Created by 下川 北斗 on 2013/08/02.
//
//

#ifndef __SampleApp__LoadingModal__
#define __SampleApp__LoadingModal__

#include <iostream>
#include "cocos2d.h"

class LoadingModal: public cocos2d::CCScene{
public:
    bool init(std::string message){
        cocos2d::CCScene::init();
        cocos2d::CCLayer* layer = cocos2d::CCLayer::create();
        
        this->addChild(layer);
        
        CCLOG("%s", message.c_str());
        
        cocos2d::CCSize s = cocos2d::CCDirector::sharedDirector()->getWinSize();
        cocos2d::CCLabelTTF* label = cocos2d::CCLabelTTF::create(message == ""? "Loading...": message.c_str(), "Arial", 40);
        layer->setPosition(ccp(s.width / 2, s.height / 2));
        layer->addChild(label);
        
        return true;
    }
    
    // not using CREATE_FUNC because of passing the message.
    static LoadingModal* create(std::string message){
        LoadingModal *pRet = new LoadingModal();
        if (pRet && pRet->init(message))
        {
            pRet->autorelease();
            return pRet;
        }
        else
        {
            delete pRet;
            pRet = NULL;
            return NULL;
        }
    }
};


#endif /* defined(__SampleApp__LoadingModal__) */
