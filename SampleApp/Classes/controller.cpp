#include "controller.h"
#include "testResource.h"
#include "tests.h"


#define LINE_SPACE          40

static CCPoint s_tCurPos = CCPointZero;

static TestScene* CreateTestScene(int nIdx)
{
    CCDirector::sharedDirector()->purgeCachedData();

    TestScene* pScene = NULL;

    switch (nIdx)
    {
    case SETTINGS:
        pScene = new SettingsScene(); break;
    case TEST_MAINTENANCE_STATUS:
        pScene = new MaintenanceTestScene(); break;
    case TEST_LOGIN:
        pScene = new LoginTestScene(); break;
    case TEST_SIMPLE_ACTION:
        pScene = new SimpleActionTestScene(); break;
    case TEST_BULK_ACTION:
        pScene = new BulkActionTestScene(); break;
    case TEST_ASYNC_ACTION:
        pScene = new AsyncActionRequestTestScene(); break;
    case TEST_API_ERROR:
        pScene = new ApiErrorTestScene(); break;
    case TEST_FETCH_ARMOR:
        pScene = new FetchArmorTestScene(); break;
    case TEST_MASTER_DIFFS:
        pScene = new MasterDiffsTestScene(); break;
    case TEST_GAME_DATA:
        pScene = new GameDataTestScene(); break;
    case TEST_PLAYER:
        pScene = new PlayerTestScene(); break;
    case TEST_ARMOR_UPGRADE1:
        pScene = new ArmorUpgrade1TestScene(); break;
    case TEST_SHOP_SCHEDULE:
        pScene = new ShopScheduleTestScene(); break;
    case TEST_REQUIRED_EXPERIENCE:
        pScene = new RequiredExperienceTestScene(); break;
    case TEST_COMPOSITION1:
        pScene = new Composition1TestScene(); break;
    case TEST_GREETING_HISTORY:
        pScene = new GreetingHistoryTestScene(); break;
    case TEST_ITEM_OUTGOING_LOG:
        pScene = new ItemOutgoingLogTestScene(); break;
    case TEST_ASSET:
        pScene = new AssetTestScene(); break;
    case TEST_CURL_ASSET:
        pScene = new CurlAssetTestScene(); break;
    default:
        break;
    }
    
    return pScene;
}

TestController::TestController()
: m_tBeginPos(CCPointZero)
{
    // add close menu
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(s_pPathClose, s_pPathClose, this, menu_selector(TestController::closeCallback) );
    CCMenu* pMenu =CCMenu::create(pCloseItem, NULL);

    pMenu->setPosition( CCPointZero );
    pCloseItem->setPosition(ccp( VisibleRect::right().x - 30, VisibleRect::top().y - 30));

    // add menu items for tests
    m_pItemMenu = CCMenu::create();
    for (int i = 0; i < TESTS_COUNT; ++i)
    {
// #if (CC_TARGET_PLATFORM == CC_PLATFORM_MARMALADE)
//         CCLabelBMFont* label = CCLabelBMFont::create(g_aTestNames[i].c_str(),  "fonts/arial16.fnt");
// #else
        CCLabelTTF* label = CCLabelTTF::create(g_aTestNames[i].c_str(), "Arial", 24);
// #endif        
        CCMenuItemLabel* pMenuItem = CCMenuItemLabel::create(label, this, menu_selector(TestController::menuCallback));

        m_pItemMenu->addChild(pMenuItem, i + 10000);
        pMenuItem->setPosition( ccp( VisibleRect::center().x, (VisibleRect::top().y - (i + 1) * LINE_SPACE) ));
    }

    m_pItemMenu->setContentSize(CCSizeMake(VisibleRect::getVisibleRect().size.width, (TESTS_COUNT + 1) * (LINE_SPACE)));
    m_pItemMenu->setPosition(s_tCurPos);
    addChild(m_pItemMenu);

    setTouchEnabled(true);

    addChild(pMenu, 1);

}

TestController::~TestController()
{
}

void TestController::menuCallback(CCObject * pSender)
{
    // get the userdata, it's the index of the menu item clicked
    CCMenuItem* pMenuItem = (CCMenuItem *)(pSender);
    int nIdx = pMenuItem->getZOrder() - 10000;

    // create the test scene and run it
    TestScene* pScene = CreateTestScene(nIdx);
    if (pScene)
    {
        pScene->runThisTest();
        pScene->release();
    }
}

void TestController::closeCallback(CCObject * pSender)
{
    CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void TestController::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
    CCSetIterator it = pTouches->begin();
    CCTouch* touch = (CCTouch*)(*it);

    m_tBeginPos = touch->getLocation();    
}

void TestController::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
    CCSetIterator it = pTouches->begin();
    CCTouch* touch = (CCTouch*)(*it);

    CCPoint touchLocation = touch->getLocation();    
    float nMoveY = touchLocation.y - m_tBeginPos.y;

    CCPoint curPos  = m_pItemMenu->getPosition();
    CCPoint nextPos = ccp(curPos.x, curPos.y + nMoveY);

    if (nextPos.y < 0.0f)
    {
        m_pItemMenu->setPosition(CCPointZero);
        return;
    }

    if (nextPos.y > ((TESTS_COUNT + 1)* LINE_SPACE - VisibleRect::getVisibleRect().size.height))
    {
        m_pItemMenu->setPosition(ccp(0, ((TESTS_COUNT + 1)* LINE_SPACE - VisibleRect::getVisibleRect().size.height)));
        return;
    }

    m_pItemMenu->setPosition(nextPos);
    m_tBeginPos = touchLocation;
    s_tCurPos   = nextPos;
}
