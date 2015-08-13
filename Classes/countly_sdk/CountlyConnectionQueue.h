//
//  CountlyConnectionQueue.h
//  CountlyX
//
//  Created by Junaid on 13/07/2015.
//
//

#ifndef __CountlyX__CountlyConnectionQueue__
#define __CountlyX__CountlyConnectionQueue__

#include <stdio.h>
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "network/HttpClient.h"

USING_NS_CC;

using namespace std;

class CountlyConnectionQueue {
private:
  bool bgTask;
  string appKey;
  string appHost;
  bool startedWithTest;
  CountlyConnectionQueue();
  Vector<__String*> dataQueue;
  Map<string, __String*> crashCustom;
public:
  __String *locationString;
  static CountlyConnectionQueue* sharedInstance();
  
  void tick();
  string getAppKey();
  string getAppHost();
  void beginSession();
  void sendUserDetails();
  void setAppKey(string key);
  void setAppHost(string host);
  void addToQueue(__String* data);
  void recordEvents(string events);
  void httpRequestUrl(__String *url, bool isImmediate = false);
  void reportCrash(string crashReport);
  void endSessionWithDuration(int duration);
  void updateSessionWithDuration(int duration);
  void setCrashCustom(Map<string, __String*> custom);
  void reportCrash(string error, string reason, bool nonfatal);
  
  //Http Response Callback
  void onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
  
};

#endif /* defined(__CountlyX__CountlyConnectionQueue__) */
