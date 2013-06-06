//
//  KeyboardNotificationLayer.cpp
//  SampleApp
//
//  Created by t-akima on 2013/05/10.
//
//

#include "KeyboardNotificationLayer.h"

static CCRect getRect(CCNode * pNode)
{
    CCRect rc;
    rc.origin = pNode->getPosition();
    rc.size = pNode->getContentSize();
    rc.origin.x -= rc.size.width / 2;
    rc.origin.y -= rc.size.height / 2;
    return rc;
}



//////////////////////////////////////////////////////////////////////////
// implement KeyboardNotificationLayer
//////////////////////////////////////////////////////////////////////////

KeyboardNotificationLayer::KeyboardNotificationLayer()
: m_pTrackNode(0)
{
    setTouchEnabled(true);
}

void KeyboardNotificationLayer::registerWithTouchDispatcher()
{
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
}

void KeyboardNotificationLayer::keyboardWillShow(CCIMEKeyboardNotificationInfo& info)
{
    CCLOG("TextInputTest:keyboardWillShowAt(origin:%f,%f, size:%f,%f)",
          info.end.origin.x, info.end.origin.y, info.end.size.width, info.end.size.height);
    
    if (! m_pTrackNode)
    {
        return;
    }
    
    CCRect rectTracked = getRect(m_pTrackNode);
    CCLOG("TextInputTest:trackingNodeAt(origin:%f,%f, size:%f,%f)",
          rectTracked.origin.x, rectTracked.origin.y, rectTracked.size.width, rectTracked.size.height);
    
    // if the keyboard area doesn't intersect with the tracking node area, nothing need to do.
    if (! rectTracked.intersectsRect(info.end))
    {
        return;
    }
    
    // assume keyboard at the bottom of screen, calculate the vertical adjustment.
    float adjustVert = info.end.getMaxY() - rectTracked.getMinY();
    CCLOG("TextInputTest:needAdjustVerticalPosition(%f)", adjustVert);
    
    // move all the children node of KeyboardNotificationLayer
    CCArray * children = getChildren();
    CCNode * node = 0;
    int count = children->count();
    CCPoint pos;
    for (int i = 0; i < count; ++i)
    {
        node = (CCNode*)children->objectAtIndex(i);
        pos = node->getPosition();
        pos.y += adjustVert;
        node->setPosition(pos);
    }
}

// CCLayer function

bool KeyboardNotificationLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    CCLOG("++++++++++++++++++++++++++++++++++++++++++++");
    m_beginPos = pTouch->getLocation();
    return true;
}

void KeyboardNotificationLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if (! m_pTrackNode)
    {
        return;
    }
    
    CCPoint endPos = pTouch->getLocation();
    
    float delta = 5.0f;
    if (::abs(endPos.x - m_beginPos.x) > delta
        || ::abs(endPos.y - m_beginPos.y) > delta)
    {
        // not click
        m_beginPos.x = m_beginPos.y = -1;
        return;
    }
    
    // decide the trackNode is clicked.
    CCRect rect;
    CCPoint point = convertTouchToNodeSpaceAR(pTouch);
    CCLOG("KeyboardNotificationLayer:clickedAt(%f,%f)", point.x, point.y);
    
    rect = getRect(m_pTrackNode);
    CCLOG("KeyboardNotificationLayer:TrackNode at(origin:%f,%f, size:%f,%f)",
          rect.origin.x, rect.origin.y, rect.size.width, rect.size.height);
    
    this->onClickTrackNode(rect.containsPoint(point));
    CCLOG("----------------------------------");
}


void KeyboardNotificationLayer::onClickTrackNode(bool bClicked)
{
    CCTextFieldTTF * pTextField = (CCTextFieldTTF*)m_pTrackNode;
    if (bClicked)
    {
        // TextFieldTTFTest be clicked
        CCLOG("LoginTestImpl:CCTextFieldTTF attachWithIME");
        pTextField->attachWithIME();
    }
    else
    {
        // TextFieldTTFTest not be clicked
        CCLOG("LoginTestImpl:CCTextFieldTTF detachWithIME");
        pTextField->detachWithIME();
    }
}


#define FONT_NAME                       "Thonburi"
#define FONT_SIZE                       36


void KeyboardNotificationLayer::onEnter()
{
    BaseNotificationLayer::onEnter();
    
    // add CCTextFieldTTF
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    
    text_field = CCTextFieldTTF::textFieldWithPlaceHolder("",
                                                          FONT_NAME,
                                                          FONT_SIZE);
    text_field->setString(defaultValue().c_str());
    addChild(text_field);
    
    text_field->setDelegate(this);
    
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    // on android, CCTextFieldTTF cannot auto adjust its position when soft-keyboard pop up
    // so we had to set a higher position to make it visable
    text_field->setPosition(ccp(s.width / 2, s.height/2 + 50));
#else
    text_field->setPosition(ccp(s.width / 2, s.height / 2));
#endif
    
    m_pTrackNode = text_field;
}

bool KeyboardNotificationLayer::onTextFieldDetachWithIME(cocos2d::CCTextFieldTTF *sender)
{
    this->execute();
    return false;
}

std::string KeyboardNotificationLayer::defaultValue() {
    return "1000001";
}


int KeyboardNotificationLayer::textFieldAsInt() {
    std::string s = text_field->getString();
    stringstream ss(s);
    int res;
    return ss >> res ? res : 0;
}
