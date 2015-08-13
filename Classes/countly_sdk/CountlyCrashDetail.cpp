//
//  CountlyCrashDetail.cpp
//  CountlyX
//
//  Created by Junaid on 01/08/2015.
//
//

#include "CountlyCrashDetail.h"
#include "platform/android/jni/JniHelper.h"
#include <jni.h>
#include <android/log.h>

USING_NS_CC;

#define  CLASS_NAME "org.count.ly.sdk/CrashDetail"

bool CountlyCrashDetail::isMuted() {
  JniMethodInfo methodInfo;
  jboolean ret = false;
  
  if (! JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "isMuted", "()Z"))
  {
    return ret;
  }
  
  ret = methodInfo.env->CallStaticBooleanMethod(methodInfo.classID, methodInfo.methodID);
  methodInfo.env->DeleteLocalRef(methodInfo.classID);
  
  return ret;
}
bool CountlyCrashDetail::isRooted() {
  JniMethodInfo methodInfo;
  jboolean ret = false;
  
  if (! JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "isRooted", "()Z"))
  {
    return ret;
  }
  
  ret = methodInfo.env->CallStaticBooleanMethod(methodInfo.classID, methodInfo.methodID);
  methodInfo.env->DeleteLocalRef(methodInfo.classID);
  
  return ret;
}
int CountlyCrashDetail::batteryLevel() {
  JniMethodInfo methodInfo;
  jint ret = 0;
  
  if (! JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "getBatteryLevel", "()I"))
  {
    return ret;
  }
  
  ret = methodInfo.env->CallStaticIntMethod(methodInfo.classID, methodInfo.methodID);
  methodInfo.env->DeleteLocalRef(methodInfo.classID);
  
  return ret;
}
bool CountlyCrashDetail::isInBackground() {
  JniMethodInfo methodInfo;
  jboolean ret = false;
  
  if (! JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "isInBackground", "()Z"))
  {
    return ret;
  }
  
  ret = methodInfo.env->CallStaticBooleanMethod(methodInfo.classID, methodInfo.methodID);
  methodInfo.env->DeleteLocalRef(methodInfo.classID);
  
  return ret;
}

float CountlyCrashDetail::OpenGLESversion() {
  JniMethodInfo methodInfo;
  jfloat ret = 0.0f;
  
  if (! JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "getOpenGL", "()F"))
  {
    return ret;
  }
  
  ret = methodInfo.env->CallStaticFloatMethod(methodInfo.classID, methodInfo.methodID);
  methodInfo.env->DeleteLocalRef(methodInfo.classID);
  
  return ret;
}
const char* CountlyCrashDetail::getCPUType() {
  
  JniMethodInfo methodInfo;
  jstring jstr;
  if (JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "getCpu", "()Ljava/lang/String;"))
  {
    jstr = (jstring)methodInfo.env->CallStaticObjectMethod(methodInfo.classID, methodInfo.methodID);
  }
  methodInfo.env->DeleteLocalRef(methodInfo.classID);
  
  const char* cpuType = methodInfo.env->GetStringUTFChars(jstr, NULL);
  
  return cpuType;
}
const char*  CountlyCrashDetail::orientation() {
  
  JniMethodInfo methodInfo;
  jstring jstr;
  if (JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "getOrientation", "()Ljava/lang/String;"))
  {
    jstr = (jstring)methodInfo.env->CallStaticObjectMethod(methodInfo.classID, methodInfo.methodID);
  }
  methodInfo.env->DeleteLocalRef(methodInfo.classID);
  
  const char* orientation = methodInfo.env->GetStringUTFChars(jstr, NULL);
  
  return orientation;
}
const char* CountlyCrashDetail::manufacturer() {
  
  JniMethodInfo methodInfo;
  jstring jstr;
  if (JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "getManufacturer", "()Ljava/lang/String;"))
  {
    jstr = (jstring)methodInfo.env->CallStaticObjectMethod(methodInfo.classID, methodInfo.methodID);
  }
  methodInfo.env->DeleteLocalRef(methodInfo.classID);
  
  const char* manufacturer = methodInfo.env->GetStringUTFChars(jstr, NULL);
  
  return manufacturer;
}
long CountlyCrashDetail::getRamCurrent() {
  JniMethodInfo methodInfo;
  jlong ret = 0.0f;
  
  if (! JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "getRamCurrent", "()J"))
  {
    return ret;
  }
  
  ret = methodInfo.env->CallStaticLongMethod(methodInfo.classID, methodInfo.methodID);
  methodInfo.env->DeleteLocalRef(methodInfo.classID);
  
  return ret;
}
long CountlyCrashDetail::getRamTotal() {
  JniMethodInfo methodInfo;
  jlong ret = 0.0f;
  
  if (! JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "getRamTotal", "()J"))
  {
    return ret;
  }
  
  ret = methodInfo.env->CallStaticLongMethod(methodInfo.classID, methodInfo.methodID);
  methodInfo.env->DeleteLocalRef(methodInfo.classID);
  
  long ram = ret;
  
  __android_log_print(ANDROID_LOG_INFO, "Countly", "The ram is %ld", ram);
  return ram;
}
long CountlyCrashDetail::getDiskCurrent() {
  JniMethodInfo methodInfo;
  jlong ret = 0.0f;
  
  if (! JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "getDiskCurrent", "()J"))
  {
    return ret;
  }
  
  ret = methodInfo.env->CallStaticLongMethod(methodInfo.classID, methodInfo.methodID);
  methodInfo.env->DeleteLocalRef(methodInfo.classID);
  
  return ret;
}
long CountlyCrashDetail::getDiskTotal() {
  JniMethodInfo methodInfo;
  jlong ret = 0.0f;
  
  if (! JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "getDiskTotal", "()J"))
  {
    return ret;
  }
  
  ret = methodInfo.env->CallStaticLongMethod(methodInfo.classID, methodInfo.methodID);
  methodInfo.env->DeleteLocalRef(methodInfo.classID);
  
  return ret;
}
long CountlyCrashDetail::connectionType() {
  JniMethodInfo methodInfo;
  jlong ret = 0.0f;
  
  if (! JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "isOnline", "()J"))
  {
    return ret;
  }
  
  ret = methodInfo.env->CallStaticLongMethod(methodInfo.classID, methodInfo.methodID);
  methodInfo.env->DeleteLocalRef(methodInfo.classID);
  
  return ret;
}