//
//  SampleAppAppDelegate.h
//  SampleApp
//
//  Created by 下川 北斗 on 2013/04/05.
//  Copyright __MyCompanyName__ 2013年. All rights reserved.
//

#ifndef  _APP_DELEGATE_H_
#define  _APP_DELEGATE_H_

#include "CCApplication.h"
#include <libGSS/libGSS.h>

/**
@brief    The cocos2d Application.

The reason to implement with private inheritance is to hide some interface details of CCDirector.
*/
class  AppDelegate : private cocos2d::CCApplication, public libgss::LoadApiServerConfigDelegate
{
public:
    AppDelegate();
    virtual ~AppDelegate();

    /**
    @brief    Implement CCDirector and CCScene init code here.
    @return true    Initialize success, app continue.
    @return false   Initialize failed, app terminate.
    */
    virtual bool applicationDidFinishLaunching();

    /**
    @brief  The function is called when the application enters the background
    @param  the pointer of the application instance
    */
    virtual void applicationDidEnterBackground();

    /**
    @brief  The function is called when the application enters the foreground
    @param  the pointer of the application instance
    */
    virtual void applicationWillEnterForeground();
    
    /**
     @brief The function is called when the application settings are to be loaded from config server.
     */
    void willStartLoadApiServerConfig();
    
    // override
    void didLoadApiServerConfigWithError(int code, const std::string& message);
    
    // override
    void didLoadApiServerConfigWithSuccess();
};

#endif // _APP_DELEGATE_H_

