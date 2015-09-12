//
//  CountlyExceptionHandler.cpp
//  CountlyX
//
//  Created by Junaid on 24/07/2015.
//
//


#include "Countly.h"
#include "CountlyExceptionHandler.h"
#include "platform/android/jni/JniHelper.h"
#include <jni.h>

USING_NS_CC;

#define  CLASS_NAME "org.countly.sdk/CountlyHelper"


void CountlyExceptionHandler::exitSignalHandler(int _ignored) {
}

void CountlyExceptionHandler::printStacktrace(std::string reason) {
  
}

void CountlyExceptionHandler::signalHandler(int sig) {
  
}

void signal_handler(int signal) {
  
}

void CountlyExceptionHandler::terminateHandler() {
  
}

void CountlyExceptionHandler::startCrashReporting(){
  
  std::set_terminate(Countly::terminateHandler);
  
  JniMethodInfo methodInfo;
  
  if ( JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "enableCrashReporting", "()V"))
  {
    methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
    methodInfo.env->DeleteLocalRef(methodInfo.classID);
    
  }
  
  
}

void CountlyExceptionHandler::testCrash(){
  
  JniMethodInfo methodInfo;
  
  if ( JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "testCrash", "()V"))
  {
    methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
    methodInfo.env->DeleteLocalRef(methodInfo.classID);
    
  }
  
  
}
