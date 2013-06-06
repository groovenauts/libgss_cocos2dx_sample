// #define COCOS2D_DEBUG   1

#include "FetchArmorTest.h"

#include <libGSS/libGSS.h>


//////////////////////////////////////////////////////////////////////////
// local function
//////////////////////////////////////////////////////////////////////////

enum 
{
    kFetchArmorTestNoConditions = 0,
    kFetchArmorTestSimpleConditions,
    kFetchArmorTestAndJoined,
    kFetchArmorTestOrJoined,
    kFetchArmorTestNestedConditions,
    kFetchArmorPaginateAction,
    kFetchArmorTestsCount,
}; 

static int testIdx = -1; 

BaseNotificationLayer* createFetchArmorTest(int nIndex)
{
    switch(nIndex)
    {
    case kFetchArmorTestNoConditions: return new FetchArmorTestNoConditions();
    case kFetchArmorTestSimpleConditions: return new FetchArmorTestSimpleConditions();
    case kFetchArmorTestAndJoined: return new FetchArmorTestAndJoined();
    case kFetchArmorTestOrJoined: return new FetchArmorTestOrJoined();
    case kFetchArmorTestNestedConditions: return new FetchArmorTestNestedConditions();
    case kFetchArmorPaginateAction: return new FetchArmorPaginateAction();
    default: return 0;
    }
}

CCLayer* restartFetchArmorTest()
{
    FetchArmorTest* pContainerLayer = new FetchArmorTest;
    pContainerLayer->autorelease();

    BaseNotificationLayer* pTestLayer = createFetchArmorTest(testIdx);
    pTestLayer->autorelease();
    pContainerLayer->addBaseNotificationLayer(pTestLayer);

    return pContainerLayer;
}

CCLayer* nextFetchArmorTest()
{
    testIdx++;
    testIdx = testIdx % kFetchArmorTestsCount;

    return restartFetchArmorTest();
}

CCLayer* backFetchArmorTest()
{
    testIdx--;
    int total = kFetchArmorTestsCount;
    if( testIdx < 0 )
        testIdx += total;    

    return restartFetchArmorTest();
}

//////////////////////////////////////////////////////////////////////////
// implement FetchArmorTest
//////////////////////////////////////////////////////////////////////////

void FetchArmorTest::nextCallback(CCObject* pSender)
{
    CCScene* s = new FetchArmorTestScene();
    s->addChild( nextFetchArmorTest() );
    CCDirector::sharedDirector()->replaceScene(s);
    s->release();
}

void FetchArmorTest::backCallback(CCObject* pSender)
{
    CCScene* s = new FetchArmorTestScene();
    s->addChild( backFetchArmorTest() );
    CCDirector::sharedDirector()->replaceScene(s);
    s->release();
}

std::string FetchArmorTest::title()
{
    return "Fetch Armor sample";
}

//////////////////////////////////////////////////////////////////////////
// implement FetchArmorTestScene
//////////////////////////////////////////////////////////////////////////

void FetchArmorTestScene::runThisTest()
{
    CCLayer* pLayer = nextFetchArmorTest();
    addChild(pLayer);
    
    CCDirector::sharedDirector()->replaceScene(this);
}


//////////////////////////////////////////////////////////////////////////
// implement FetchArmorAction
//////////////////////////////////////////////////////////////////////////

void FetchArmorAction::execute()
{
    CCLOG("FetchArmorTestNoConditions::execute");
    
    CCLOG("now sending request with all action.");
    
    libgss::ActionRequest *request = new libgss::ActionRequest(this, gssActionResponse(FetchArmorTestNoConditions::OnComplete));
    libgss::AllAction* action = new libgss::AllAction("Armor");
    action->orders()->add("armor_id", libgss::Orders::Orders::kAsc);
    buildCondition(action->conditions());
    request->addAction(action);
    libgss::Network::instance()->send(request);
    request->release();
    action->release();
    
    CCLOG("sent request successfully");
    
}

void FetchArmorAction::OnComplete(libgss::ActionResponse* response){
    std::string s = response->outputs()->toString();
    int size = s.length() + 1;
    char* buf = new char[size];
    memcpy(buf, s.c_str(), size);
    CCLOG(buf);
}


//////////////////////////////////////////////////////////////////////////
// implement FetchArmorTestNoConditions
//////////////////////////////////////////////////////////////////////////

std::string FetchArmorTestNoConditions::subtitle(){
    return "Press Circle button to fetch all data.\nResponse is shown in log.";
}

void FetchArmorTestNoConditions::buildCondition(libgss::Conditions *conditions) {
    // do nothing
}

//////////////////////////////////////////////////////////////////////////
// implement FetchArmorTestSimpleConditions
//////////////////////////////////////////////////////////////////////////

std::string FetchArmorTestSimpleConditions::subtitle(){
    return "deffence_pt = 24";
}

void FetchArmorTestSimpleConditions::buildCondition(libgss::Conditions *conditions)
{
    conditions->add("deffence_pt", 24);
}

//////////////////////////////////////////////////////////////////////////
// implement FetchArmorTestAndJoined
//////////////////////////////////////////////////////////////////////////

std::string FetchArmorTestAndJoined::subtitle(){
    return "(offence_pt >= 10) and (offence_pt < 20)";
}

void FetchArmorTestAndJoined::buildCondition(libgss::Conditions *conditions)
{
    conditions->add("offence_pt$gte", 10);
    conditions->add("offence_pt$lt", 20);
}
//////////////////////////////////////////////////////////////////////////
// implement FetchArmorTestOrJoined
//////////////////////////////////////////////////////////////////////////

std::string FetchArmorTestOrJoined::subtitle(){
    return "(offence_pt >= 25) or (deffence_pt >= 20)";
}

void FetchArmorTestOrJoined::buildCondition(libgss::Conditions *conditions)
{
    conditions->setLogicalLinkOperator(libgss::Conditions::kOperatorOr);
    conditions->add("offence_pt$gte", 25);
    conditions->add("deffence_pt$gte", 20);
}

//////////////////////////////////////////////////////////////////////////
// implement FetchArmorTestNestedConditions
//////////////////////////////////////////////////////////////////////////

std::string FetchArmorTestNestedConditions::subtitle(){
    return "(armor_id >= 10010) and\n ((offence_pt >= 25) or (deffence_pt >= 20))";
}

void FetchArmorTestNestedConditions::buildCondition(libgss::Conditions *conditions)
{
    libgss::Conditions sub_cond(libgss::Conditions::kOperatorOr);
    sub_cond.add("offence_pt$gte", 25);
    sub_cond.add("deffence_pt$gte", 20);
    conditions->add("armor_id$gte", 10010);
    conditions->add(sub_cond);
}



//////////////////////////////////////////////////////////////////////////
// implement FetchArmorPaginateAction
//////////////////////////////////////////////////////////////////////////

std::string FetchArmorPaginateAction::subtitle() {
    return "pagination with perPage = 5";
}

void FetchArmorPaginateAction::execute()
{
    CCLOG("FetchArmorPaginateAction::execute");
    
    CCLOG("now sending request with all action.");
    
    libgss::ActionRequest *request = new libgss::ActionRequest(this, gssActionResponse(FetchArmorTestNoConditions::OnComplete));
    libgss::AllAction* action = new libgss::AllAction("Armor");
    action->orders()->add("armor_id", libgss::Orders::Orders::kAsc);
    action->setPagenation(textFieldAsInt(), 5);
    request->addAction(action);
    libgss::Network::instance()->send(request);
    request->release();
    action->release();
    
    CCLOG("sent request successfully");
    
}

void FetchArmorPaginateAction::OnComplete(libgss::ActionResponse* response){
    std::string s = response->outputs()->toString();
    int size = s.length() + 1;
    char* buf = new char[size];
    memcpy(buf, s.c_str(), size);
    CCLOG(buf);
}

std::string FetchArmorPaginateAction::defaultValue() {
    return "1";
}
