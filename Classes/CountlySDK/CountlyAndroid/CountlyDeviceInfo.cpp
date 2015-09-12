//
//  CountlyDeviceInfo.cpp
//  CountlyX
//
//  Created by Junaid on 08/07/2015.
//
//

#include "Countly.h"
#include "CountlyDeviceInfo.h"
#include "CountlyConnectionQueue.h"
#include "CountlyDeviceInfo.h"
#include "platform/android/jni/JniHelper.h"
#include <jni.h>

USING_NS_CC;

#define  CLASS_NAME "org.countly.sdk/CountlyHelper"


extern "C"
{
  JNIEXPORT jstring JNICALL Java_org_countly_sdk_CountlyHelper_reportCrash(JNIEnv* env, jobject thiz, jstring error, jstring reason, jboolean nonfatal);

  JNIEXPORT jstring JNICALL Java_org_countly_sdk_CountlyHelper_onRegistrationId(JNIEnv* env, jobject thiz, jstring registrationId, jboolean startedWithTest);

  JNIEXPORT jstring JNICALL Java_org_countly_sdk_CountlyHelper_recordPushEvent(JNIEnv* env, jobject thiz, jstring key, jstring messageId);
};

JNIEXPORT jstring JNICALL Java_org_countly_sdk_CountlyHelper_reportCrash(JNIEnv* env, jobject thiz, jstring myError, jstring myReason, jboolean myNonfatal)
{

  const char* error = env->GetStringUTFChars(myError, NULL);
  const char* reason = env->GetStringUTFChars(myReason, NULL);
  bool nonfatal = myNonfatal;

  CountlyConnectionQueue::sharedInstance()->reportCrash(error, reason, nonfatal);

  env->ReleaseStringUTFChars(myError, error);
  env->ReleaseStringUTFChars(myReason, reason);
}

JNIEXPORT jstring JNICALL Java_org_countly_sdk_CountlyHelper_onRegistrationId(JNIEnv* env, jobject thiz, jstring mRegistrationId, jboolean startedWithTest)
{

  bool pStartedWithTest = startedWithTest;
  const char* registrationId = env->GetStringUTFChars(mRegistrationId, NULL);

  CountlyConnectionQueue::sharedInstance()->tokenSession(registrationId, "android_token", pStartedWithTest);

  env->ReleaseStringUTFChars(mRegistrationId, registrationId);
}

JNIEXPORT jstring JNICALL Java_org_countly_sdk_CountlyHelper_recordPushEvent(JNIEnv* env, jobject thiz, jstring mKey, jstring mMessageId)
{

  const char* key = env->GetStringUTFChars(mKey, NULL);
  const char* messageId = env->GetStringUTFChars(mMessageId, NULL);

  Countly::sharedInstance()->recordPushEvent(key, messageId, 1);

  env->ReleaseStringUTFChars(mKey, key);
  env->ReleaseStringUTFChars(mMessageId, messageId);
}

const char* CountlyDeviceInfo::locale() {

	JniMethodInfo methodInfo;
	  jstring jstr;
	  if (JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "getLocale", "()Ljava/lang/String;"))
	  {
	    jstr = (jstring)methodInfo.env->CallStaticObjectMethod(methodInfo.classID, methodInfo.methodID);
	  }
	  methodInfo.env->DeleteLocalRef(methodInfo.classID);

	  const char* locale = methodInfo.env->GetStringUTFChars(jstr, NULL);

	  return locale;
}
const char* CountlyDeviceInfo::bundleId() {

	JniMethodInfo methodInfo;
	  jstring jstr;
	  if (JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "getStore", "()Ljava/lang/String;"))
	  {
	    jstr = (jstring)methodInfo.env->CallStaticObjectMethod(methodInfo.classID, methodInfo.methodID);
	  }
	  methodInfo.env->DeleteLocalRef(methodInfo.classID);

	  const char* bundleId = methodInfo.env->GetStringUTFChars(jstr, NULL);

	  return bundleId;
}
const char* CountlyDeviceInfo::appVersion() {

	JniMethodInfo methodInfo;
	  jstring jstr;
	  if (JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "getAppVersion", "()Ljava/lang/String;"))
	  {
	    jstr = (jstring)methodInfo.env->CallStaticObjectMethod(methodInfo.classID, methodInfo.methodID);
	  }
	  methodInfo.env->DeleteLocalRef(methodInfo.classID);

	  const char* appVersion = methodInfo.env->GetStringUTFChars(jstr, NULL);

	  return appVersion;

}

const char* CountlyDeviceInfo::getDeviceId()
{
  JniMethodInfo methodInfo;
  jstring jstr;
  if (JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "getDeviceId", "()Ljava/lang/String;"))
  {
    jstr = (jstring)methodInfo.env->CallStaticObjectMethod(methodInfo.classID, methodInfo.methodID);
  }
  methodInfo.env->DeleteLocalRef(methodInfo.classID);

  const char* deviceSysVers = methodInfo.env->GetStringUTFChars(jstr, NULL);

  return deviceSysVers;
}

const char* CountlyDeviceInfo::getUserAgent()
{
  JniMethodInfo methodInfo;
  jstring jstr;
  if (JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "getUserAgent", "()Ljava/lang/String;"))
  {
    jstr = (jstring)methodInfo.env->CallStaticObjectMethod(methodInfo.classID, methodInfo.methodID);
  }
  methodInfo.env->DeleteLocalRef(methodInfo.classID);

  const char* userAgent = methodInfo.env->GetStringUTFChars(jstr, NULL);

  return userAgent;
}

const char* CountlyDeviceInfo::getDeviceModel()
{
  JniMethodInfo methodInfo;
  jstring jstr;
  if (JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "getDeviceModel", "()Ljava/lang/String;"))
  {
    jstr = (jstring)methodInfo.env->CallStaticObjectMethod(methodInfo.classID, methodInfo.methodID);
  }
  methodInfo.env->DeleteLocalRef(methodInfo.classID);

  const char* deviceModel = methodInfo.env->GetStringUTFChars(jstr, NULL);

  return deviceModel;
}

const char* CountlyDeviceInfo::getCarrierName()
{
  JniMethodInfo methodInfo;
  jstring jstr;
  if (JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "getCarrierName", "()Ljava/lang/String;"))
  {
    jstr = (jstring)methodInfo.env->CallStaticObjectMethod(methodInfo.classID, methodInfo.methodID);
  }
  methodInfo.env->DeleteLocalRef(methodInfo.classID);

  const char* carrierName = methodInfo.env->GetStringUTFChars(jstr, NULL);

  return carrierName;
}


const char* CountlyDeviceInfo::getDeviceSystemName()
{
  JniMethodInfo methodInfo;
  jstring jstr;
  if (JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "getDeviceSystemName", "()Ljava/lang/String;"))
  {
    jstr = (jstring)methodInfo.env->CallStaticObjectMethod(methodInfo.classID, methodInfo.methodID);
  }
  methodInfo.env->DeleteLocalRef(methodInfo.classID);

  const char* deviceSysName = methodInfo.env->GetStringUTFChars(jstr, NULL);

  return deviceSysName;
}

const char* CountlyDeviceInfo::getDeviceSystemVersion()
{
  JniMethodInfo methodInfo;
  jstring jstr;
  if (JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "getDeviceSystemVersion", "()Ljava/lang/String;"))
  {
    jstr = (jstring)methodInfo.env->CallStaticObjectMethod(methodInfo.classID, methodInfo.methodID);
  }
  methodInfo.env->DeleteLocalRef(methodInfo.classID);

  const char* deviceSysVers = methodInfo.env->GetStringUTFChars(jstr, NULL);

  return deviceSysVers;
}


