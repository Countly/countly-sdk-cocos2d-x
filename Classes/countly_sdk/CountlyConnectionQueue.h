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
public:
  __String *locationString;
  static CountlyConnectionQueue* sharedInstance();
  
  void tick();
  void beginSession();
  void setAppKey(string key);
  void setAppHost(string host);
  void addToQueue(__String* data);
  void recordEvents(string events);
  void httpRequestUrl(__String *url);
  void endSessionWithDuration(int duration);
  void updateSessionWithDuration(int duration);
  
  //Http Response Callback
  void onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
  
};

#endif /* defined(__CountlyX__CountlyConnectionQueue__) */
