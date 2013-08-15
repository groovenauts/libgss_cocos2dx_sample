//
//  MasterDiffsTest.h
//  SampleApp
//
//  Created by 下川 北斗 on 2013/08/09.
//
//

#ifndef __SampleApp__MasterDiffsTest__
#define __SampleApp__MasterDiffsTest__

#include "../BaseNotificationLayer.h"
#include "../KeyboardNotificationLayer.h"
#include "../GroundLayer.h"
#include <libGSS/libGSS.h>

#include <iostream>

class MasterDiffsTest : public GroundLayer
{
public:
    void nextCallback(CCObject* pSender);
    void backCallback(CCObject* pSender);
    
    std::string title();
};

class MasterDiffsTestScene : public TestScene
{
public:
    virtual void runThisTest();
};


class MasterDiffsTestBasic : public BaseNotificationLayer
{
public:
    std::string subtitle();
    
    void execute();
    
    void OnComplete(libgss::ActionResponse* response);
};

#endif /* defined(__SampleApp__MasterDiffsTest__) */
