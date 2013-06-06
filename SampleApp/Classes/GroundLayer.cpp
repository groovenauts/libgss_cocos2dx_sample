//
//  GroundLayer.cpp
//  SampleApp
//
//  Created by t-akima on 2013/05/10.
//
//

#include "GroundLayer.h"


//////////////////////////////////////////////////////////////////////////
// implement GroundLayer
//////////////////////////////////////////////////////////////////////////

GroundLayer::GroundLayer()
: m_pNotificationLayer(0)
{
}

void GroundLayer::executeCallback(CCObject* pSender)
{
    m_pNotificationLayer->execute();
}


void GroundLayer::addBaseNotificationLayer(BaseNotificationLayer * pLayer)
{
    m_pNotificationLayer = pLayer;
    addChild(pLayer);
}

void GroundLayer::onEnter()
{
    CCLayer::onEnter();
    
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    
    CCLabelTTF* label = CCLabelTTF::create(title().c_str(), "Arial", 24);
    addChild(label);
    label->setPosition(ccp(s.width/2, s.height-50));
    
    std::string subTitle = m_pNotificationLayer->subtitle();
    if(! subTitle.empty())
    {
        CCLabelTTF* l = CCLabelTTF::create(subTitle.c_str(), "Thonburi", 16);
        addChild(l, 1);
        l->setPosition(ccp(s.width/2, s.height-80));
    }
    
    CCMenuItemImage *item1 = CCMenuItemImage::create("Images/b1.png", "Images/b2.png", this, menu_selector(GroundLayer::backCallback));
    CCMenuItemImage *item2 = CCMenuItemImage::create("Images/r1.png","Images/r2.png", this, menu_selector(GroundLayer::executeCallback) );
    CCMenuItemImage *item3 = CCMenuItemImage::create("Images/f1.png", "Images/f2.png", this, menu_selector(GroundLayer::nextCallback) );
    
    CCMenu *menu = CCMenu::create(item1, item2, item3, NULL);
    menu->setPosition(CCPointZero);
    item1->setPosition(ccp(VisibleRect::center().x - item2->getContentSize().width*2, VisibleRect::bottom().y+item2->getContentSize().height/2));
    item2->setPosition(ccp(VisibleRect::center().x, VisibleRect::bottom().y+item2->getContentSize().height/2));
    item3->setPosition(ccp(VisibleRect::center().x + item2->getContentSize().width*2, VisibleRect::bottom().y+item2->getContentSize().height/2));
    
    addChild(menu, 1);
}
