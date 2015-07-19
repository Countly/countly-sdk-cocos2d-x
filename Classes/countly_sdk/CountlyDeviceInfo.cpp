//
//  CountlyDeviceInfo.cpp
//  CountlyX
//
//  Created by Junaid on 08/07/2015.
//
//

#include "CountlyDeviceInfo.h"

#include "CountlyDeviceInfo.h"
#include "platform/android/jni/JniHelper.h"
#include <jni.h>

USING_NS_CC;

#define  CLASS_NAME "org.count.ly.sdk/CountlyHelper"

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
  
  const char* deviceModel = methodInfo.env->GetStringUTFChars(jstr, NULL);
  
  return deviceModel;
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
  
  const char* deviceSysVers = methodInfo.env->GetStringUTFChars(jstr, NULL);
  
  return deviceSysVers;
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


