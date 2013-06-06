#ifndef __LOGIN_TEST_H__
#define __LOGIN_TEST_H__

#include "../KeyboardNotificationLayer.h"
#include "../GroundLayer.h"
#include <libGSS/libGSS.h>

class KeyboardNotificationLayer;

/**
@brief    LoginTest for retain prev, reset, next, main menu buttons.
*/
class LoginTest : public GroundLayer
{
public:
    void nextCallback(CCObject* pSender);
    void backCallback(CCObject* pSender);

    std::string title();
    void addKeyboardNotificationLayer(BaseNotificationLayer * pLayer);
};

//////////////////////////////////////////////////////////////////////////
// LoginTestImpl for test TextFieldTTF default behavior.
//////////////////////////////////////////////////////////////////////////

class LoginTestImpl : public KeyboardNotificationLayer
{
public:
    // BaseNotificationLayer
    virtual std::string subtitle();
    
    // BaseNotificationLayer
    virtual void execute();
    
    void OnComplete(libgss::LoginResponse *response);

};

//////////////////////////////////////////////////////////////////////////
// LoginTestWithNoCallbackImpl for test login with no callback.
//////////////////////////////////////////////////////////////////////////

class LoginTestWithNoCallbackImpl : public KeyboardNotificationLayer
{
    int pollingCount_;
    libgss::LoginRequest *request_;
public:
    // BaseNotificationLayer
    virtual std::string subtitle();
    
    // BaseNotificationLayer
    virtual void execute();
    
    void poll(float timer);
};

//////////////////////////////////////////////////////////////////////////
// LoginTestWithRegisterImpl for test login with register.
//////////////////////////////////////////////////////////////////////////

class LoginTestWithRegisterImpl : public BaseNotificationLayer
{    
public:
    // BaseNotificationLayer
    virtual std::string subtitle();
    
    // BaseNotificationLayer
    virtual void execute();
    
    void OnComplete(libgss::LoginResponse *response);
};



class LoginTestScene : public TestScene
{
public:
    virtual void runThisTest();
};

#endif    // __LOGIN_TEST_H__
