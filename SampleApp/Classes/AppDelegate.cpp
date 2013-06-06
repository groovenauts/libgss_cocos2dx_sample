//
//  SampleAppAppDelegate.cpp
//  SampleApp
//
//  Created by 下川 北斗 on 2013/04/05.
//  Copyright __MyCompanyName__ 2013年. All rights reserved.
//

#include "AppDelegate.h"

#include "cocos2d.h"
#include "controller.h"
#include "Settings.h"

#include <libGSS/libGSS.h>

USING_NS_CC;

AppDelegate::AppDelegate()
{

}

AppDelegate::~AppDelegate()
{
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
    CCDirector *pDirector = CCDirector::sharedDirector();
    pDirector->setOpenGLView(CCEGLView::sharedOpenGLView());
    
    Settings::applySettings();
    
    CCSize screenSize = CCEGLView::sharedOpenGLView()->getFrameSize();
    
    CCSize designSize = CCSizeMake(480, 320);
    
    if (screenSize.height > 320)
    {
        CCSize resourceSize = CCSizeMake(960, 640);
        std::vector<std::string> searchPaths;
        searchPaths.push_back("hd");
        // pFileUtils->setSearchPaths(searchPaths);
        pDirector->setContentScaleFactor(resourceSize.height/designSize.height);
    }
    
    CCEGLView::sharedOpenGLView()->setDesignResolutionSize(designSize.width, designSize.height, kResolutionNoBorder);
    
    // turn on display FPS
    pDirector->setDisplayStats(true);
    
    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);
    
    CCScene * pScene = CCScene::create();
    CCLayer * pLayer = new TestController();
    pLayer->autorelease();
    
    pScene->addChild(pLayer);
    pDirector->runWithScene(pScene);
    

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    CCDirector::sharedDirector()->pause();

    // if you use SimpleAudioEngine, it must be paused
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->resume();
    
    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
