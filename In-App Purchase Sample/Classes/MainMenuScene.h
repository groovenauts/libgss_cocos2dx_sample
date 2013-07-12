//
//  MainMenuScene.h
//  In-App Purchase Sample
//
//  Created by 下川 北斗 on 2013/07/10.
//
//

#ifndef __In_App_Purchase_Sample__MainMenuScene__
#define __In_App_Purchase_Sample__MainMenuScene__

#include <iostream>
#include "cocos2d.h"
#include <libGSS/libGSS.h>

class MainMenu : public cocos2d::CCLayer
{
    cocos2d::CCLabelTTF* _progressLabel;
public:
    virtual bool init();
    
    static cocos2d::CCScene* scene();
    
    void storeMenuCallback(CCObject* pSender);
    
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(MainMenu);
    
private:
    void showMenu();
    void loggedIn(libgss::LoginResponse* response);
};


#endif /* defined(__In_App_Purchase_Sample__MainMenuScene__) */
