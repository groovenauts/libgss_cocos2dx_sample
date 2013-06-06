//
//  GroundLayer.h
//  SampleApp
//
//  Created by t-akima on 2013/05/10.
//
//

#ifndef __SampleApp__GroundLayer__
#define __SampleApp__GroundLayer__

#include "cocos2d.h"
#include "BaseNotificationLayer.h"

class GroundLayer : public CCLayer
{
protected:
    BaseNotificationLayer * m_pNotificationLayer;
public:
    GroundLayer();
    
    void executeCallback(CCObject* pSender);
    virtual void nextCallback(CCObject* pSender){};
    virtual void backCallback(CCObject* pSender){};
    
    virtual std::string title() = 0;
    
    void addBaseNotificationLayer(BaseNotificationLayer * pLayer);
    
    virtual void onEnter();
};

#endif /* defined(__SampleApp__GroundLayer__) */
