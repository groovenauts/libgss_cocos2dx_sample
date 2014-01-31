LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES:=\
hellocpp/main.cpp \
../../Classes/AppDelegate.cpp \
../../Classes/Asset/AssetTest.cpp \
../../Classes/Asset/CurlAssetTest.cpp \
../../Classes/Asset/CurlDownloadTask.cpp \
../../Classes/Basic/ApiErrorTest.cpp \
../../Classes/Basic/AsyncActionRequestTest.cpp \
../../Classes/Basic/BulkActionTest.cpp \
../../Classes/Basic/LoginTest.cpp \
../../Classes/Basic/MaintenanceTest.cpp \
../../Classes/Basic/Settings.cpp \
../../Classes/Basic/SimpleActionTest.cpp \
../../Classes/controller.cpp \
../../Classes/Create/GreetingHistoryTest.cpp \
../../Classes/Create/ItemOutgoingLogTest.cpp \
../../Classes/Get/ArmorUpgrade1Test.cpp \
../../Classes/Get/Composition1Test.cpp \
../../Classes/Get/RequiredExperienceTest.cpp \
../../Classes/Get/ShopScheduleTest.cpp \
../../Classes/GroundLayer.cpp \
../../Classes/KeyboardNotificationLayer.cpp \
../../Classes/LoadingModal.cpp \
../../Classes/Master/FetchArmorTest.cpp \
../../Classes/Master/MasterDiffsTest.cpp \
../../Classes/testBasic.cpp \
../../Classes/Update/GameDataTest.cpp \
../../Classes/Update/PlayerTest.cpp \
../../Classes/VisibleRect.cpp \

LOCAL_C_INCLUDES:=\
$(LOCAL_PATH)/../../Classes \
$(LOCAL_PATH)/../../Classes/Asset \
$(LOCAL_PATH)/../../Classes/Basic \
$(LOCAL_PATH)/../../Classes/Create \
$(LOCAL_PATH)/../../Classes/Get \
$(LOCAL_PATH)/../../Classes/Master \
$(LOCAL_PATH)/../../Classes/Master/Web \
$(LOCAL_PATH)/../../Classes/Update \
$(LOCAL_PATH)/../../../libgss/include \

LOCAL_WHOLE_STATIC_LIBRARIES += libgss

include $(BUILD_SHARED_LIBRARY)

$(call import-add-path,$(LOCAL_PATH)/../../..)
$(call import-module,libgss)
