//
//  Settings.cpp
//  SampleApp
//
//  Created by 下川 北斗 on 2013/05/27.
//
//

// メニュー名
// Settings : 接続先設定画面
//
// 以下の項目を設定可能です。
// * host名
// * HTTPのポート
// * HTTPS（SSL）のポート
// * 公開アセットを取得するURLのルート
//

#include <iostream>

#include <libGSS/libGSS.h>

#include "Settings.h"
#include "testResource.h"
#include "AppDelegate.h"

USING_NS_CC_EXT;

const char* Settings::kDefaultConfigServerUrlBase = "http://localhost:3002";
const char* Settings::kDefaultClientVersion = "develop";
const char* Settings::kDefaultDeviceType = "1";

const char* Settings::kSettingsKeyConfigServerUrlBase = "jp.groovenauts.libgss.config_server_url_base";
const char* Settings::kSettingsKeyClientVersion = "jp.groovenauts.libgss.client_version";
const char* Settings::kSettingsKeyDeviceType = "jp.groovenauts.libgss.device_type";
const char* Settings::kSettingsKeyConsumerSecret = "jp.groovenauts.libgss.consumer_secret";


void Settings::applySettings(){
    std::string urlBase = CCUserDefault::sharedUserDefault()->getStringForKey(kSettingsKeyConfigServerUrlBase,
                                                                              kDefaultConfigServerUrlBase);
    std::string clientVersion = CCUserDefault::sharedUserDefault()->getStringForKey(kSettingsKeyClientVersion, "");
    if (clientVersion == "") {
        clientVersion = Settings::kDefaultClientVersion;
    }
    std::string deviceType = CCUserDefault::sharedUserDefault()->getStringForKey(kSettingsKeyDeviceType, "");
    if (deviceType == "") {
        deviceType = Settings::kDefaultDeviceType;
    }
    
    libgss::Network::instance()->setPlatform("fontana");
    
    AppDelegate* appDelegate = ((AppDelegate*)CCApplication::sharedApplication());
    appDelegate->willStartLoadApiServerConfig();
    libgss::Network::instance()->initWithConfigServer(urlBase, clientVersion, deviceType, appDelegate);
    
    std::string consumerSecret = CCUserDefault::sharedUserDefault()->getStringForKey(kSettingsKeyConsumerSecret, "");
    libgss::Network::instance()->setConsumerSecret(consumerSecret);
    CCLog("Consumer secret: %s", consumerSecret.c_str());
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	#include "TargetConditionals.h"
	#if TARGET_IPHONE_SIMULATOR
		libgss::Network::instance()->setUseSslLogin(false); // エミュレータでのテスト時はSSLを使用しない
	#endif
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	#if DISABLE_SSL
		libgss::Network::instance()->setUseSslLogin(false);
	#endif
#else
	#error
#endif
}

void SettingsLayer::onEnter()
{
    CCLayer::onEnter();
    
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    
    CCLog("%f, %f", s.width, s.height);
    
    CCLabelTTF* label = CCLabelTTF::create(title().c_str(), "Arial", 24);
    addChild(label);
    label->setPosition(ccp(s.width / 2, s.height - 50));
    
    CCPoint visibleOrigin = CCEGLView::sharedOpenGLView()->getVisibleOrigin();
    CCSize visibleSize = CCEGLView::sharedOpenGLView()->getVisibleSize();

    CCLog("%f, %f", visibleSize.width, visibleSize.height);

    CCSize labelSize = CCSizeMake(50, 30);
    CCSize editBoxSize = CCSizeMake(visibleSize.width - 100 - labelSize.width, 30);
    
    float editBoxX = visibleOrigin.x + visibleSize.width / 2 + labelSize.width;
    
    // configサーバーURL設定
    float hostY = visibleOrigin.y + visibleSize.height * 7 / 10;
    CCLabelTTF* configServerLabel = CCLabelTTF::create("Config Server", "Arial", 12);
    configServerLabel->setPosition(ccp(60 + labelSize.width, hostY));
    configServerLabel->setColor(ccWHITE);
    configServerLabel->setAnchorPoint(ccp(1, 0.5));
    addChild(configServerLabel);
    configServerEditBox_ = CCEditBox::create(editBoxSize, CCScale9Sprite::create("Images/orange_edit.png"));
    configServerEditBox_->setPosition(ccp(editBoxX, hostY));
    configServerEditBox_->setPlaceHolder(Settings::kDefaultConfigServerUrlBase);
    configServerEditBox_->setPlaceholderFontColor(ccGRAY);
    configServerEditBox_->setReturnType(kKeyboardReturnTypeDone);
    configServerEditBox_->setDelegate(this);
    addChild(configServerEditBox_);
    
    std::string configServer = CCUserDefault::sharedUserDefault()->getStringForKey(Settings::kSettingsKeyConfigServerUrlBase, "");
    if (configServer != "") {
        configServerEditBox_->setText(configServer.c_str());
    }

    // クライアントバージョン
    float clientVersionY = visibleOrigin.y + visibleSize.height * 6 / 10;
    CCLabelTTF* clientVersionLabel = CCLabelTTF::create("Client Version", "Arial", 12);
    clientVersionLabel->setPosition(ccp(60 + labelSize.width, clientVersionY));
    clientVersionLabel->setColor(ccWHITE);
    clientVersionLabel->setAnchorPoint(ccp(1, 0.5));
    addChild(clientVersionLabel);
    clientVersionEditBox_ = CCEditBox::create(editBoxSize, CCScale9Sprite::create("Images/orange_edit.png"));
    clientVersionEditBox_->setPosition(ccp(editBoxX, clientVersionY));
    clientVersionEditBox_->setPlaceHolder(Settings::kDefaultClientVersion);
    clientVersionEditBox_->setPlaceholderFontColor(ccGRAY);
    clientVersionEditBox_->setMaxLength(30);
    clientVersionEditBox_->setReturnType(kKeyboardReturnTypeDone);
    clientVersionEditBox_->setDelegate(this);
    addChild(clientVersionEditBox_);
    
    std::string clientVersion = CCUserDefault::sharedUserDefault()->getStringForKey(Settings::kSettingsKeyClientVersion,
                                                                                    Settings::kDefaultClientVersion);
    if (clientVersion != Settings::kDefaultClientVersion) {
        clientVersionEditBox_->setText(clientVersion.c_str());
    }

    // デバイス種別設定
    float deviceTypeY = visibleOrigin.y + visibleSize.height * 5 / 10;
    CCLabelTTF* sslPortLabel = CCLabelTTF::create("Device Type", "Arial", 12);
    sslPortLabel->setPosition(ccp(60 + labelSize.width, deviceTypeY));
    sslPortLabel->setColor(ccWHITE);
    sslPortLabel->setAnchorPoint(ccp(1, 0.5));
    addChild(sslPortLabel);
    deviceTypeEditBox_ = CCEditBox::create(editBoxSize, CCScale9Sprite::create("Images/orange_edit.png"));
    deviceTypeEditBox_->setPosition(ccp(editBoxX, deviceTypeY));
    deviceTypeEditBox_->setPlaceHolder(Settings::kDefaultDeviceType);
    deviceTypeEditBox_->setPlaceholderFontColor(ccGRAY);
    deviceTypeEditBox_->setMaxLength(20);
    deviceTypeEditBox_->setReturnType(kKeyboardReturnTypeDone);
    deviceTypeEditBox_->setDelegate(this);
    addChild(deviceTypeEditBox_);
    
    std::string deviceType = CCUserDefault::sharedUserDefault()->getStringForKey(Settings::kSettingsKeyDeviceType, "1");
    if (deviceType != "") {
        deviceTypeEditBox_->setText(deviceType.c_str());
    }

    // Consumer Secret
    float consumerSecretY = visibleOrigin.y + visibleSize.height * 4 / 10;
    CCLabelTTF* consumerSecretLabel = CCLabelTTF::create("Consumer Secret", "Arial", 12);
    consumerSecretLabel->setPosition(ccp(60 + labelSize.width, consumerSecretY));
    consumerSecretLabel->setColor(ccWHITE);
    consumerSecretLabel->setAnchorPoint(ccp(1, 0.5));
    addChild(consumerSecretLabel);
    consumerSecretEditBox_ = CCEditBox::create(editBoxSize, CCScale9Sprite::create("Images/orange_edit.png"));
    consumerSecretEditBox_->setPosition(ccp(editBoxX, consumerSecretY));
    consumerSecretEditBox_->setPlaceHolder("Default: empty (no request sign)");
    consumerSecretEditBox_->setPlaceholderFontColor(ccGRAY);
    consumerSecretEditBox_->setReturnType(kKeyboardReturnTypeDone);
    consumerSecretEditBox_->setDelegate(this);
    addChild(consumerSecretEditBox_);
    
    std::string consumerSecret = CCUserDefault::sharedUserDefault()->getStringForKey(Settings::kSettingsKeyConsumerSecret, "");
    if (consumerSecret != "") {
        consumerSecretEditBox_->setText(consumerSecret.c_str());
    }
    
    if (message_ != "") {
        messageLabel_ = CCLabelTTF::create(message_.c_str(), "Arial", 18);
        addChild(messageLabel_, 1);
        messageLabel_->setPosition(ccp(s.width / 2, 25));
        messageLabel_->setColor(ccRED);
    }
}


void SettingsLayer::editBoxEditingDidBegin(cocos2d::extension::CCEditBox* editBox){}

void SettingsLayer::editBoxEditingDidEnd(cocos2d::extension::CCEditBox* editBox){
    std::string key;
    if (editBox == configServerEditBox_) {
        key = Settings::kSettingsKeyConfigServerUrlBase;
    }
    if (editBox == clientVersionEditBox_) {
        key = Settings::kSettingsKeyClientVersion;
    }
    else if(editBox == deviceTypeEditBox_){
        key = Settings::kSettingsKeyDeviceType;
    }
    else if(editBox == clientVersionEditBox_){
        key = Settings::kSettingsKeyClientVersion;
    }
    else if(editBox == consumerSecretEditBox_){
        key = Settings::kSettingsKeyConsumerSecret;
    }
    
    CCUserDefault::sharedUserDefault()->setStringForKey(key.c_str(), editBox->getText());
    CCUserDefault::sharedUserDefault()->flush();
    
    if (messageLabel_) {
        messageLabel_->setString("");
    }
}

void SettingsLayer::editBoxTextChanged(cocos2d::extension::CCEditBox* editBox, const std::string& text){}

void SettingsLayer::editBoxReturn(cocos2d::extension::CCEditBox* editBox){}


void SettingsScene::runThisTest(){
    SettingsLayer *layer = new SettingsLayer(message_);
    addChild(layer);
    
    CCDirector::sharedDirector()->replaceScene(this);
    layer->release();
}

void SettingsScene::MainMenuCallback(CCObject* pSender){
    TestScene::MainMenuCallback(pSender);
    
    Settings::applySettings();
}
