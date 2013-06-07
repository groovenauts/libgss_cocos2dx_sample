#ifndef _TESTS_H_
#define _TESTS_H_

#include "Basic/Settings.h"
#include "Basic/LoginTest.h"
#include "Basic/SimpleActionTest.h"
#include "Basic/BulkActionTest.h"
#include "Basic/ApiErrorTest.h"

#include "Master/FetchArmorTest.h"

#include "Update/GameDataTest.h"
#include "Update/PlayerTest.h"

#include "Get/ArmorUpgrade1Test.h"
#include "Get/ShopScheduleTest.h"
#include "Get/RequiredExperienceTest.h"
#include "Get/Composition1Test.h"

#include "Create/GreetingHistoryTest.h"
#include "Create/ItemOutgoingLogTest.h"

#include "Asset/AssetTest.h"

enum
{
    SETTINGS,
    TEST_TEXT_INPUT,
    TEST_SIMPLE_ACTION,
    TEST_BULK_ACTION,
    TEST_API_ERROR,
    TEST_FETCH_ARMOR,
    TEST_GAME_DATA,
    TEST_PLAYER,
    TEST_ARMOR_UPGRADE1,
    TEST_SHOP_SCHEDULE,
    TEST_REQUIRED_EXPERIENCE,
    TEST_COMPOSITION1,
    TEST_GREETING_HISTORY,
    TEST_ITEM_OUTGOING_LOG,
    TEST_ASSET,
    
    TESTS_COUNT,
};

const std::string g_aTestNames[TESTS_COUNT] = {
    "Settings",
    "Login",
    "Simple Action",
    "Bulk Action",
    "API Error Test",
    "All Master",
    "Get/Update GameData",
    "Get/Update Player",
    "Get Dictionary",
    "Get Schedule",
    "Get IntRange",
    "Get/Dice Probability",
    "Create/All History",
    "Create Log",
    "Asset Test",
};

#endif
