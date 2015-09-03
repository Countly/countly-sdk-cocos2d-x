//
//  Countly.h
//  CountlyX
//
//  Created by Junaid on 07/07/2015.
//
//

#ifndef __CountlyX__Countly__
#define __CountlyX__Countly__

#include "cocos2d.h"
#include "CountlyEventQueue.h"

// User Details Constants
#define kCLYUserName "name"
#define kCLYUserUsername "username"
#define kCLYUserEmail "email"
#define kCLYUserOrganization "organization"
#define kCLYUserPhone "phone"
#define kCLYUserGender "gender"
#define kCLYUserPicture "picture"
#define kCLYUserPicturePath "picturePath"
#define kCLYUserBirthYear "byear"
#define kCLYUserCustom "custom"

using namespace std;
USING_NS_CC;
class Countly : public Ref {
private:
  time_t startTime;
  bool isSuspended;
  struct tm lastTime;
  double unsentSessionLength;
  CountlyEventQueue *eventQueue;
  
  Countly ();
  
  static void reportCrash(string reason);
  
public:
  static const char* COUNTLY_EVENT_ENTER_BACKGROUND;
  static const char* COUNTLY_EVENT_ENTER_FORGROUND;
  
  static Countly* sharedInstance ();
  
  void resume();
  void suspend();
  void recordEvents();
  time_t getStartTime();
  void onTimer(float dt);
  void checkEventThreshold();
  static void terminateHandler();
  void startCrashReporting();
  long timeSinceLaunch();
  void androidNotifcations();
  void startCrashReportingWithSegments(Map<string, __String*> custom);
  void start(string appKey, string appHost);
  void startOnCloudWithAppKey(string appKey);
  void setLocation(double latitude, double longitude);
  void didEnterBackgroundCallBack(EventCustom *event);
  void willEnterForegroundCallBack(EventCustom *event);
  

  void recordEvent(string pKey, int pCount = 1);
  void recordEvent(string pKey, float pSum, int pCount = 1);
  void recordPushEvent(string pKey, string messageId, int pCount = 1);
  void recordEvent(string pKey, Map<string, __String*> pSegmentation, int pCount = 1);
  void recordEvent(string pKey, Map<string, __String*> pSegmentation, float pSum, int pCount = 1);
  
  void addDBEvent(string pKey, Map<string, __String*> pSegmentation, float pSum, time_t time, int pCount = 1);
  
  void recordUserDetails(Map<string, __String*> pUserMap);
  void recordUserDetails(Map<string, __String*> pUserMap, Map<string, __String*> pUserCustom);
  
  //TODO : Temperary functions for testing only
  void printDeviceInfo();
  int jsonTest();
 
};


#endif /* defined(__CountlyX__Countly__) */
