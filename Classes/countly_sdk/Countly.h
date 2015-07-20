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
using namespace std;
USING_NS_CC;
class Countly : public Ref {
private:
  
  bool isSuspended;
  struct tm lastTime;
  double unsentSessionLength;
  CountlyEventQueue *eventQueue;
  
  Countly ();
  
public:
  static const char* COUNTLY_EVENT_ENTER_BACKGROUND;
  static const char* COUNTLY_EVENT_ENTER_FORGROUND;
  
  static Countly* sharedInstance ();
  
  void resume();
  void suspend();
  void recordEvents();
  void onTimer(float dt);
  void checkEventThreshold();
  void start(string appKey, string appHost);
  void startOnCloudWithAppKey(string appKey);
  void setLocation(double latitude, double longitude);
  void didEnterBackgroundCallBack(EventCustom *event);
  void willEnterForegroundCallBack(EventCustom *event);
  
  void recordEvent(string pKey, int pCount);
  void recordEvent(string pKey, int pCount, float pSum);
  void recordEvent(string pKey, int pCount, Map<string, __String*> pSegmentation);
  void recordEvent(string pKey, int pCount, float pSum, Map<string, __String*> pSegmentation);
  
  //TODO : Temperary functions for testing only
  void printDeviceInfo();
  int jsonTest();
 
};


#endif /* defined(__CountlyX__Countly__) */
