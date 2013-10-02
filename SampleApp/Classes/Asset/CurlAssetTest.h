//
//  CurlAssetTest.h
//  SampleApp
//
//  Created by 下川 北斗 on 2013/10/02.
//
//

#ifndef __SampleApp__CurlAssetTest__
#define __SampleApp__CurlAssetTest__

#include "../BaseNotificationLayer.h"
#include "../GroundLayer.h"
#include <libGSS/libGSS.h>
#include <iostream>

class CurlAssetTest : public GroundLayer
{
public:
    void nextCallback(CCObject* pSender);
    void backCallback(CCObject* pSender);
    
    std::string title();
};

class CurlAssetTestScene : public TestScene
{
public:
    virtual void runThisTest();
};


class CurlAssetTestSingleThread : public BaseNotificationLayer
{
public:
    std::string subtitle();
    virtual void execute();
};



#endif /* defined(__SampleApp__CurlAssetTest__) */
