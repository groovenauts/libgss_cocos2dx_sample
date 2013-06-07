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

USING_NS_CC_EXT;

const char* Settings::kDefaultHost = "localhost";
const int Settings::kInvalidPort = -1;

const char* Settings::kSettingsKeyHost = "jp.groovenauts.libgss.host";
const char* Settings::kSettingsKeyHttpPort = "jp.groovenauts.libgss.http_port";
const char* Settings::kSettingsKeySslPort = "jp.groovenauts.libgss.ssl_port";
const char* Settings::kSettingsKeyPublicAssetRoot = "jp.groovenauts.libgss.public_asset_root";


void Settings::applySettings(){
    std::string host = CCUserDefault::sharedUserDefault()->getStringForKey(kSettingsKeyHost, kDefaultHost);
    if (host == "") {
        host = kDefaultHost;
    }
    libgss::Network::instance()->setHost(host);
    CCLog("Host: %s", host.c_str());
    
    int httpPort = CCUserDefault::sharedUserDefault()->getIntegerForKey(kSettingsKeyHttpPort, kInvalidPort);
    libgss::Network::instance()->setHttpPort(httpPort);
    CCLog("HTTP Port: %d", httpPort);
    
    int sslPort = CCUserDefault::sharedUserDefault()->getIntegerForKey(kSettingsKeySslPort, kInvalidPort);
    libgss::Network::instance()->setSslPort(sslPort);
    CCLog("SSL Port: %d", sslPort);

    std::string publicAssetRoot = CCUserDefault::sharedUserDefault()->getStringForKey(kSettingsKeyPublicAssetRoot, "http://localhost:3000/a/");
    libgss::Network::instance()->setPublicAssetRoot(publicAssetRoot);
    CCLog("Public asset root: %s", publicAssetRoot.c_str());

#ifdef __APPLE__
#include "TargetConditionals.h"
#if TARGET_IPHONE_SIMULATOR
    libgss::Network::instance()->setUseSslLogin(false); // エミュレータでのテスト時はSSLを使用しない
#endif
#endif
}

void SettingsLayer::onEnter()
{
    CCLayer::onEnter();
    
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    
    CCLog("%f, %f", s.width, s.height);
    
    CCLabelTTF* label = CCLabelTTF::create(title().c_str(), "Arial", 24);
    addChild(label);
    label->setPosition(ccp(s.width/2, s.height-50));
    
    CCPoint visibleOrigin = CCEGLView::sharedOpenGLView()->getVisibleOrigin();
    CCSize visibleSize = CCEGLView::sharedOpenGLView()->getVisibleSize();

    CCLog("%f, %f", visibleSize.width, visibleSize.height);

    CCSize labelSize = CCSizeMake(50, 30);
    CCSize editBoxSize = CCSizeMake(visibleSize.width - 100 - labelSize.width, 30);
    
    float editBoxX = visibleOrigin.x + visibleSize.width / 2 + labelSize.width;
    
    // ホスト設定
    float hostY = visibleOrigin.y + visibleSize.height * 7 / 10;
    CCLabelTTF* hostLabel = CCLabelTTF::create("host", "Arial", 18);
    hostLabel->setPosition(ccp(50 + labelSize.width, hostY));
    hostLabel->setColor(ccWHITE);
    hostLabel->setAnchorPoint(ccp(1, 0.5));
    addChild(hostLabel);
    hostEditBox_ = CCEditBox::create(editBoxSize, CCScale9Sprite::create("Images/orange_edit.png"));
    hostEditBox_->setPosition(ccp(editBoxX, hostY));
    std::string hostPlaceHolder = std::string("Default: ") + Settings::kDefaultHost;
    hostEditBox_->setPlaceHolder(hostPlaceHolder.c_str());
    hostEditBox_->setPlaceholderFontColor(ccGRAY);
    hostEditBox_->setReturnType(kKeyboardReturnTypeDone);
    hostEditBox_->setDelegate(this);
    addChild(hostEditBox_);
    
    std::string host = CCUserDefault::sharedUserDefault()->getStringForKey(Settings::kSettingsKeyHost, "");
    if (host != "") {
        hostEditBox_->setText(host.c_str());
    }

    // HTTPポート設定
    float httpPortY = visibleOrigin.y + visibleSize.height * 6 / 10;
    CCLabelTTF* httpPortLabel = CCLabelTTF::create("HTTP port", "Arial", 18);
    httpPortLabel->setPosition(ccp(50 + labelSize.width, httpPortY));
    httpPortLabel->setColor(ccWHITE);
    httpPortLabel->setAnchorPoint(ccp(1, 0.5));
    addChild(httpPortLabel);
    httpPortEditBox_ = CCEditBox::create(editBoxSize, CCScale9Sprite::create("Images/orange_edit.png"));
    httpPortEditBox_->setPosition(ccp(editBoxX, httpPortY));
    httpPortEditBox_->setPlaceHolder("Default: no specify");
    httpPortEditBox_->setPlaceholderFontColor(ccGRAY);
    httpPortEditBox_->setMaxLength(8);
    httpPortEditBox_->setReturnType(kKeyboardReturnTypeDone);
    httpPortEditBox_->setDelegate(this);
    addChild(httpPortEditBox_);
    
    int httpPort = CCUserDefault::sharedUserDefault()->getIntegerForKey(Settings::kSettingsKeyHttpPort, Settings::kInvalidPort);
    if (httpPort != Settings::kInvalidPort) {
        std::ostringstream os;
        os << httpPort;
        httpPortEditBox_->setText(os.str().c_str());
    }

    // SSLポート設定
    float sslPortY = visibleOrigin.y + visibleSize.height * 5 / 10;
    CCLabelTTF* sslPortLabel = CCLabelTTF::create("SSL port", "Arial", 18);
    sslPortLabel->setPosition(ccp(50 + labelSize.width, sslPortY));
    sslPortLabel->setColor(ccWHITE);
    sslPortLabel->setAnchorPoint(ccp(1, 0.5));
    addChild(sslPortLabel);
    sslPortEditBox_ = CCEditBox::create(editBoxSize, CCScale9Sprite::create("Images/orange_edit.png"));
    sslPortEditBox_->setPosition(ccp(editBoxX, sslPortY));
    sslPortEditBox_->setPlaceHolder("Default: no specify");
    sslPortEditBox_->setPlaceholderFontColor(ccGRAY);
    sslPortEditBox_->setMaxLength(8);
    sslPortEditBox_->setReturnType(kKeyboardReturnTypeDone);
    sslPortEditBox_->setDelegate(this);
    addChild(sslPortEditBox_);
    
    int sslPort = CCUserDefault::sharedUserDefault()->getIntegerForKey(Settings::kSettingsKeySslPort, Settings::kInvalidPort);
    if (sslPort != Settings::kInvalidPort) {
        std::ostringstream os;
        os << sslPort;
        sslPortEditBox_->setText(os.str().c_str());
    }

    // Public Asset Root設定
    float publicAssetRootY = visibleOrigin.y + visibleSize.height * 4 / 10;
    CCLabelTTF* publicAssetRootLabel = CCLabelTTF::create("Public Asset Root", "Arial", 18);
    publicAssetRootLabel->setPosition(ccp(50 + labelSize.width, publicAssetRootY));
    publicAssetRootLabel->setColor(ccWHITE);
    publicAssetRootLabel->setAnchorPoint(ccp(1, 0.5));
    addChild(publicAssetRootLabel);
    publicAssetRootEditBox_ = CCEditBox::create(editBoxSize, CCScale9Sprite::create("Images/orange_edit.png"));
    publicAssetRootEditBox_->setPosition(ccp(editBoxX, publicAssetRootY));
    publicAssetRootEditBox_->setPlaceHolder("Default: http://localhost:3000/a/");
    publicAssetRootEditBox_->setPlaceholderFontColor(ccGRAY);
    publicAssetRootEditBox_->setMaxLength(8);
    publicAssetRootEditBox_->setReturnType(kKeyboardReturnTypeDone);
    publicAssetRootEditBox_->setDelegate(this);
    addChild(publicAssetRootEditBox_);
    
    std::string publicAssetRoot = CCUserDefault::sharedUserDefault()->getStringForKey(Settings::kSettingsKeyPublicAssetRoot, "");
    if (publicAssetRoot != "") {
        publicAssetRootEditBox_->setText(publicAssetRoot.c_str());
    }
}


void SettingsLayer::editBoxEditingDidBegin(cocos2d::extension::CCEditBox* editBox){}

void SettingsLayer::editBoxEditingDidEnd(cocos2d::extension::CCEditBox* editBox){
    if (editBox == hostEditBox_) {
        CCUserDefault::sharedUserDefault()->setStringForKey(Settings::kSettingsKeyHost, editBox->getText());
    }
    else if(editBox == httpPortEditBox_ || editBox == sslPortEditBox_){
        int port;
        const char* key = (editBox == httpPortEditBox_)
                            ?Settings::kSettingsKeyHttpPort
                            :Settings::kSettingsKeySslPort;
        std::istringstream is(editBox->getText());
        if(is >> port){
            CCUserDefault::sharedUserDefault()->setIntegerForKey(key, port);
            std::ostringstream os;
            os << port;
            if(os.str() != editBox->getText()){
                editBox->setText(os.str().c_str());
            }
        }
        else{
            CCUserDefault::sharedUserDefault()->setIntegerForKey(key, Settings::kInvalidPort);
            editBox->setText("");
        }
    }
    if(editBox == publicAssetRootEditBox_){
        CCUserDefault::sharedUserDefault()->setStringForKey(Settings::kSettingsKeyPublicAssetRoot, editBox->getText());
    }
    CCUserDefault::sharedUserDefault()->flush();
}

void SettingsLayer::editBoxTextChanged(cocos2d::extension::CCEditBox* editBox, const std::string& text){}

void SettingsLayer::editBoxReturn(cocos2d::extension::CCEditBox* editBox){}



void SettingsScene::runThisTest(){
    SettingsLayer *layer = new SettingsLayer();
    addChild(layer);
    
    CCDirector::sharedDirector()->replaceScene(this);
    layer->release();
}

void SettingsScene::MainMenuCallback(CCObject* pSender){
    TestScene::MainMenuCallback(pSender);
    
    Settings::applySettings();
}