//
//  In_App_Purchase_SampleAppDelegate.cpp
//  In-App Purchase Sample
//
//  Created by 下川 北斗 on 2013/07/10.
//  Copyright __MyCompanyName__ 2013年. All rights reserved.
//

#include "AppDelegate.h"

#include "cocos2d.h"
#include <libGSS/libGSS.h>

#include "SimpleAudioEngine.h"
#include "MainMenuScene.h"
#include "AppStoreManager.h"
#include "PurchaseScene.h"
#include "Settings.h"

USING_NS_CC;
using namespace CocosDenshion;

AppDelegate::AppDelegate()
{

}

AppDelegate::~AppDelegate()
{
}

bool AppDelegate::applicationDidFinishLaunching()
{
    AppStoreManager::init(PurchaseProcess::instance());
    
    // initialize libGSS::Network
    Settings::initNetwork();
    
    // initialize director
    CCDirector *pDirector = CCDirector::sharedDirector();
    pDirector->setOpenGLView(CCEGLView::sharedOpenGLView());
    pDirector->setDisplayStats(true);
    pDirector->setAnimationInterval(1.0 / 60);

    // run
    pDirector->runWithScene(MainMenu::scene());
    
    if (PurchaseProcess::existsProcessingData()) {
        PurchaseProcess::resume();
    }
    
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    CCDirector::sharedDirector()->stopAnimation();
    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    SimpleAudioEngine::sharedEngine()->pauseAllEffects();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->startAnimation();
    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
    SimpleAudioEngine::sharedEngine()->resumeAllEffects();
}
