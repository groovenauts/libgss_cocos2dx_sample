//
//  In_App_Purchase_SampleAppDelegate.h
//  In-App Purchase Sample
//
//  Created by 下川 北斗 on 2013/07/10.
//  Copyright __MyCompanyName__ 2013年. All rights reserved.
//

#ifndef  _APP_DELEGATE_H_
#define  _APP_DELEGATE_H_

#include "CCApplication.h"

/**
@brief    The cocos2d Application.

The reason to implement with private inheritance is to hide some interface details of CCDirector.
*/
class  AppDelegate : private cocos2d::CCApplication
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
};

#endif // _APP_DELEGATE_H_

