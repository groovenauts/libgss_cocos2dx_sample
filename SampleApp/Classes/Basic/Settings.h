//
//  Settings.h
//  SampleApp
//
//  Created by 下川 北斗 on 2013/05/27.
//
//

#ifndef __SampleApp__Settings__
#define __SampleApp__Settings__

#include <iostream>

#include "cocos2d.h"
#include "cocos-ext.h"

#include "../BaseNotificationLayer.h"
#include "../GroundLayer.h"

// UserDefault keys

class Settings{
    
public:
    static void applySettings();
    
    static const char* kDefaultHost;
    static const int kInvalidPort;
    static const char* kSettingsKeyHost;
    static const char* kSettingsKeyHttpPort;
    static const char* kSettingsKeySslPort;
    static const char* kSettingsKeyPublicAssetRoot;
    static const char* kSettingsKeyConsumerSecret;
};

class SettingsLayer : public CCLayer, public cocos2d::extension::CCEditBoxDelegate
{
public:
    SettingsLayer(){};
    ~SettingsLayer(){};
    
    void onEnter();
    std::string title(){return "Settings";}
    
    void editBoxEditingDidBegin(cocos2d::extension::CCEditBox* editBox);
    void editBoxEditingDidEnd(cocos2d::extension::CCEditBox* editBox);
    void editBoxTextChanged(cocos2d::extension::CCEditBox* editBox, const std::string& text);
    void editBoxReturn(cocos2d::extension::CCEditBox* editBox);
private:
    cocos2d::extension::CCEditBox* hostEditBox_;
    cocos2d::extension::CCEditBox* httpPortEditBox_;
    cocos2d::extension::CCEditBox* sslPortEditBox_;
    cocos2d::extension::CCEditBox* publicAssetRootEditBox_;
    cocos2d::extension::CCEditBox* consumerSecretEditBox_;
};


class SettingsScene : public TestScene {
    
public:
    void runThisTest();
    void MainMenuCallback(CCObject* pSender);
};

#endif /* defined(__SampleApp__Settings__) */
