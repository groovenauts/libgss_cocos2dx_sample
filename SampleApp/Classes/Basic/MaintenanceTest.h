//
//  MaintenanceTest.h
//  SampleApp
//
//  Created by 下川 北斗 on 2013/08/05.
//
//

#ifndef __SampleApp__MaintenanceTest__
#define __SampleApp__MaintenanceTest__

#include <iostream>

#include <libGSS/libGSS.h>

#include "cocos2d.h"
#include "cocos-ext.h"

#include "../KeyboardNotificationLayer.h"
#include "../GroundLayer.h"

class MaintenanceTest : public GroundLayer
{
public:
    void nextCallback(CCObject* pSender);
    void backCallback(CCObject* pSender);
    
    std::string title();
};

class MaintenanceTestScene : public TestScene
{
public:
    virtual void runThisTest();
};


class MaintenanceTestBasic : public BaseNotificationLayer
{
public:
    std::string subtitle();
    virtual void execute();
    void OnComplete(libgss::MaintenanceStatusResponse* response);
};

#endif /* defined(__SampleApp__MaintenanceTest__) */
