//
//  MainMenuScene.cpp
//  In-App Purchase Sample
//
//  Created by 下川 北斗 on 2013/07/10.
//
//

#include "MainMenuScene.h"

#include "StoreSchene.h"
#include "Settings.h"

using namespace cocos2d;


CCScene* MainMenu::scene()
{
    CCScene *scene = CCScene::create();
    MainMenu *layer = MainMenu::create();
    scene->addChild(layer);
    return scene;
}

bool MainMenu::init()
{
    if ( !CCLayer::init() ){
        return false;
    }
    
    this->showMenu();
    
    return true;
}

void MainMenu::showMenu(){
    CCLabelTTF* label = CCLabelTTF::create("Go to Store", "Arial", 24);
    CCMenuItemLabel *storeMenuItem = CCMenuItemLabel::create(label, this, menu_selector(MainMenu::storeMenuCallback));
    storeMenuItem->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width / 2,
                                   CCDirector::sharedDirector()->getWinSize().height / 2));
    
    CCMenu* pMenu = CCMenu::create(storeMenuItem, NULL);
    pMenu->setPosition( CCPointZero );
    this->addChild(pMenu, 1);
}

void MainMenu::storeMenuCallback(CCObject* pSender){

    if (libgss::Network::instance()->playerId() == "") {
        this->removeAllChildren();
        
        // Login
        _progressLabel = CCLabelTTF::create("Logging in...", "Arial", 28);
        _progressLabel->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width / 2,
                                        CCDirector::sharedDirector()->getWinSize().height / 2));
        this->addChild(_progressLabel);
        libgss::LoginRequest* loginRequest = new libgss::LoginRequest(Settings::playerId(), Settings::nickname(),
                                                                      Settings::deviceType(), Settings::deviceId());
        loginRequest->setCallback(this, gssLoginResponse(MainMenu::loggedIn));
        libgss::Network::instance()->send(loginRequest);
        loginRequest->release();
        
        CCLOG("Sent login request.");
    }
    else{
        CCDirector::sharedDirector()->replaceScene(Store::scene());
    }
}

void MainMenu::loggedIn(libgss::LoginResponse* response){
    if(response->success()){
        CCLOG("Successfully logged in.");
        CCDirector::sharedDirector()->replaceScene(Store::scene());
    }
    else{
        std::string message("Failed to login.\n");
        message.append(response->message());
        _progressLabel->setString(message.c_str());
    }
}
