//
//  CountlyCrashDetail.h
//  CountlyX
//
//  Created by Junaid on 01/08/2015.
//
//

#ifndef __CountlyX__CountlyCrashDetail__
#define __CountlyX__CountlyCrashDetail__

#include <stdio.h>

class CountlyCrashDetail {
  
public:
  
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) || (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
  
  static bool isMuted();
  static bool isRooted();
  static int batteryLevel();
  static bool isInBackground();
  static float OpenGLESversion();
  static const char* getCPUType();
  static const char*  orientation();
  static const char* manufacturer();
  static long getRamCurrent();
  static long getRamTotal();
  static long getDiskCurrent();
  static long getDiskTotal();
  static long connectionType();
  #else
  
  static bool isMuted() {return false;};
  static bool isRooted() {return false;};
  static int batteryLevel() {return 0;};
  static bool isInBackground() {return false;};
  static float OpenGLESversion() {return 0.0f;};
  static const char* getCPUType() {return "";};
  static const char*  orientation() {return "";};
  static const char* manufacturer() {return "";};
  static long freeRAM() {return 0;};
  static long totalRAM() {return 0;};
  static long freeDisk() {return 0;};
  static long totalDisk() {return 0;};
  static long connectionType() {return 0;};
#endif
};

#endif /* defined(__CountlyX__CountlyCrashDetail__) */
