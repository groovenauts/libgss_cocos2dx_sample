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
    
    static const char* kDefaultConfigServerUrlBase;
    static const char* kDefaultClientVersion;
    static const char* kDefaultDeviceType;
    static const char* kDefaultMaintenanceStatusUrl;
    static const char* kDefaultPublicAssetRoot;
    
    static const char* kSettingsKeyConfigServerUrlBase;
    static const char* kSettingsKeyClientVersion;
    static const char* kSettingsKeyDeviceType;
    static const char* kSettingsKeyMaintenanceStatusUrl;
    static const char* kSettingsKeyPublicAssetRoot;
    static const char* kSettingsKeyConsumerSecret;
};

class SettingsLayer : public CCLayer, public cocos2d::extension::CCEditBoxDelegate
{
    cocos2d::CCLabelTTF *messageLabel_ = NULL;
    std::string message_;
    
public:
    SettingsLayer(){};
    SettingsLayer(std::string message):message_(message){}
    ~SettingsLayer(){};
    
    void onEnter();
    std::string title(){return "Settings";}
    
    void editBoxEditingDidBegin(cocos2d::extension::CCEditBox* editBox);
    void editBoxEditingDidEnd(cocos2d::extension::CCEditBox* editBox);
    void editBoxTextChanged(cocos2d::extension::CCEditBox* editBox, const std::string& text);
    void editBoxReturn(cocos2d::extension::CCEditBox* editBox);
private:
    cocos2d::extension::CCEditBox* configServerEditBox_;
    cocos2d::extension::CCEditBox* clientVersionEditBox_;
    cocos2d::extension::CCEditBox* deviceTypeEditBox_;
    cocos2d::extension::CCEditBox* maintenanceStatusUrlEditBox_;
    cocos2d::extension::CCEditBox* publicAssetRootEditBox_;
    cocos2d::extension::CCEditBox* consumerSecretEditBox_;
};


class SettingsScene : public TestScene {
    std::string message_;
    
public:
    SettingsScene():TestScene(){}
    SettingsScene(std::string message):TestScene(), message_(message){}
    
    void runThisTest();
    void MainMenuCallback(CCObject* pSender);
};

#endif /* defined(__SampleApp__Settings__) */
