LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/HelloWorldScene.cpp \
                   ../../Classes/CountlySDK/CountlyiOS/CountlyCplus/Countly.cpp \
                   ../../Classes/CountlySDK/CountlyAndroid/CountlyDeviceInfo.cpp \
                   ../../Classes/CountlySDK/CountlyAndroid/CountlyCrashDetail.cpp \
                   ../../Classes/CountlySDK/CountlyiOS/CountlyCplus/CountlyEvent.cpp \
                   ../../Classes/CountlySDK/CountlyiOS/CountlyCplus/CountlyUtils.cpp \
                   ../../Classes/CountlySDK/CountlyAndroid/CountlyExceptionHandler.cpp \
                   ../../Classes/CountlySDK/CountlyiOS/CountlyCplus/CountlyBackTrace.cpp \
                   ../../Classes/CountlySDK/CountlyiOS/CountlyCplus/CountlyDBManager.cpp \
                   ../../Classes/CountlySDK/CountlyiOS/CountlyCplus/CountlyEventQueue.cpp \
                   ../../Classes/CountlySDK/CountlyiOS/CountlyCplus/CountlyConnection.cpp \
                   ../../Classes/CountlySDK/CountlyiOS/CountlyCplus/CountlyUserDetails.cpp \
                   ../../Classes/CountlySDK/CountlyiOS/CountlyCplus/CountlyDeviceInfoModel.cpp \
                   ../../Classes/CountlySDK/CountlyiOS/CountlyCplus/CountlyConnectionQueue.cpp \
                   ../../Classes/CountlySDK/CountlyiOS/CountlyCplus/ThirdPartyLibs/Sqlite/sqlite3.c

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../Classes/CountlySDK/CountlyAndroid
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../Classes/CountlySDK/CountlyiOS/CountlyCplus

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
