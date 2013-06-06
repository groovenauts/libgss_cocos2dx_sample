//
//  KeyboardNotificationLayer.h
//  SampleApp
//
//  Created by t-akima on 2013/05/10.
//
//

#ifndef __SampleApp__KeyboardNotificationLayer__
#define __SampleApp__KeyboardNotificationLayer__

#include "BaseNotificationLayer.h"

//////////////////////////////////////////////////////////////////////////
// KeyboardNotificationLayer for test IME keyboard notification.
//////////////////////////////////////////////////////////////////////////

class KeyboardNotificationLayer : public BaseNotificationLayer, public CCTextFieldDelegate
{
public:
    KeyboardNotificationLayer();
    
    virtual void onClickTrackNode(bool bClicked);
    
    virtual void registerWithTouchDispatcher();
    virtual void keyboardWillShow(CCIMEKeyboardNotificationInfo& info);
    
    // CCLayer
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    // CCLayer
    virtual void onEnter();
    
    // CCTextFieldDelegate
    virtual bool onTextFieldDetachWithIME(cocos2d::CCTextFieldTTF *sender);
    
protected:
    CCNode * m_pTrackNode;
    CCPoint  m_beginPos;
    CCTextFieldTTF* text_field;
    
    virtual std::string defaultValue();
    int textFieldAsInt();
};


#endif /* defined(__SampleApp__KeyboardNotificationLayer__) */
