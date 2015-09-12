//
//  Countly.cpp
//  CountlyX
//
//  Created by Junaid on 07/07/2015.
//
//

//#include <execinfo.h>
#include <signal.h>
#include "Countly.h"
#include "CountlyUtils.h"
#include "CountlyUserDetails.h"
#include "CountlyBackTrace.h"
#include "external/json/writer.h"
#include "external/json/document.h"
#include "external/json/stringbuffer.h"
#include "CountlyDeviceInfoModel.h"
#include "CountlyConnectionQueue.h"
#include "CountlyExceptionHandler.h"
#include "CountlyDBManager.h"

#define COUNTLY_EVENT_SEND_THRESHOLD 10
#define COUNTLY_DEFAULT_UPDATE_INTERVAL 60.0


#define EVENT_OPEN "[CLY]_push_open"
#define EVENT_ACTION "[CLY]_push_action"

const char *Countly::COUNTLY_EVENT_ENTER_BACKGROUND = "didEnterBackgroundCallBack";
const char *Countly::COUNTLY_EVENT_ENTER_FORGROUND = "willEnterForegroundCallBack";

USING_NS_CC;
static Countly * instance = NULL;
Countly::Countly () {
  isAlreadyInitailized = false;
  isSuspended = false;
  startTime = time(NULL);
  auto dispatcher = Director::getInstance()->getEventDispatcher();
  
  dispatcher->addCustomEventListener(COUNTLY_EVENT_ENTER_BACKGROUND, std::bind(&Countly::didEnterBackgroundCallBack, this, std::placeholders::_1));
  
  dispatcher->addCustomEventListener(COUNTLY_EVENT_ENTER_FORGROUND, std::bind(&Countly::willEnterForegroundCallBack, this, std::placeholders::_1));
  
}


void Countly::didEnterBackgroundCallBack(EventCustom *event)
{
  this->suspend();
}

void Countly::willEnterForegroundCallBack(cocos2d::EventCustom *event) {
  this->resume();
}


Countly* Countly::sharedInstance () {
  if (!instance) {
    instance = new Countly();
    instance->eventQueue = new CountlyEventQueue();
    CountlyDeviceInfoModel::populateModel();
    CountlyDBManager::sharedInstance()->createDB();
  }
  return instance;
}

void Countly::androidNotifcations() {
  int openCount = UserDefault::getInstance()->getIntegerForKey(EVENT_OPEN, 0);
  __String *messageKey;
  for (int i = 1; i <= openCount; i++) {
    messageKey = __String::createWithFormat("%s_%d",EVENT_OPEN,i);
    string messageId =  UserDefault::getInstance()->getStringForKey(messageKey->getCString());
    recordPushEvent(EVENT_OPEN, messageId);
  }
  
  int actionCount = UserDefault::getInstance()->getIntegerForKey(EVENT_ACTION, 0);
  for (int i = 1; i <= actionCount; i++) {
    messageKey = __String::createWithFormat("%s_%d",EVENT_ACTION,i);
    string messageId =  UserDefault::getInstance()->getStringForKey(messageKey->getCString());
    recordPushEvent(EVENT_ACTION, messageId);
  }
  
  UserDefault::getInstance()->setIntegerForKey(EVENT_ACTION, 0);
  UserDefault::getInstance()->setIntegerForKey(EVENT_OPEN, 0);
}

void Countly::start(string appKey, string appHost) {
  isAlreadyInitailized = true;
  Director::getInstance()->getScheduler()->schedule(schedule_selector(Countly::onTimer), this, COUNTLY_DEFAULT_UPDATE_INTERVAL, CC_REPEAT_FOREVER, 0, false);
  
  CountlyConnectionQueue::sharedInstance()->setAppKey(appKey);
  CountlyConnectionQueue::sharedInstance()->setAppHost(appHost);
  CountlyConnectionQueue::sharedInstance()->beginSession();
 
  
  Map<std::string, __String*> customCrash;
  
  customCrash.insert("facebook_sdk", __String::create("3.5"));
  customCrash.insert("admob", __String::create("6.5"));
  startCrashReportingWithSegments(customCrash);
}

void Countly::reportCrash(string reason) {
  string error = CountlyBackTrace::getBacktrace();
  CountlyConnectionQueue::sharedInstance()->reportCrash(error, reason, false);
}

void Countly::terminateHandler()
{
  string reason = "";
  reason = "Terminated due to unknown reason :(";
  reportCrash(reason);
  std::abort();
}

void Countly::startCrashReporting() {
  CountlyExceptionHandler::startCrashReporting();
}


void Countly::startCrashReportingWithSegments(Map<string, __String*> custom) {
  CountlyConnectionQueue::sharedInstance()->setCrashCustom(custom);
  startCrashReporting();
  
}

void Countly::startOnCloudWithAppKey(string appKey) {
  start(appKey, "https://cloud.count.ly");
}

void Countly::onTimer(float dt) {
  if (isSuspended == true) {
    return;
  }
  
  time_t _time = time(0); // now
  struct tm currTime = *localtime(&_time);
  
  unsentSessionLength = difftime(mktime(&currTime), mktime(&lastTime));
  
  int duration = unsentSessionLength;
  
  CountlyConnectionQueue::sharedInstance()->updateSessionWithDuration(duration);
  unsentSessionLength -= duration;
  recordEvents();
  
}

void Countly::suspend() {
  
  isSuspended = true;
  recordEvents();
  
  time_t _time = time(0); // now
  struct tm currTime = *localtime(&_time);
  
  unsentSessionLength = difftime(mktime(&currTime), mktime(&lastTime));
  
  int duration = unsentSessionLength;
  CountlyConnectionQueue::sharedInstance()->endSessionWithDuration(duration);
  unsentSessionLength -= duration;
}

void Countly::resume() {
  
  CountlyDeviceInfoModel::populateModel();  
  time_t _time = time(0); // now
  lastTime = *localtime(&_time);
  
  CountlyConnectionQueue::sharedInstance()->beginSession();
  isSuspended = false;
}

void Countly::setLocation(double latitude, double longitude) {
  CountlyConnectionQueue::sharedInstance()->locationString = __String::createWithFormat("%f,%f",latitude,longitude);
}

void Countly::recordEvent(string pKey, int pCount) {
  eventQueue->recordEvent(pKey, pCount);
  checkEventThreshold();
}
void Countly::recordEvent(string pKey, float pSum, int pCount) {
  eventQueue->recordEvent(pKey, pCount, pSum);
  checkEventThreshold();
}

void Countly::recordPushEvent(string pKey, string messageId, int pCount) {
  Map<std::string, __String*> data;
  data.insert("i", __String::create(messageId));
  recordEvent(pKey, data, pCount);
}
void Countly::recordEvent(string pKey, Map<string, __String*> pSegmentation, int pCount) {
  eventQueue->recordEvent(pKey, pCount, pSegmentation);
  checkEventThreshold();
}
void Countly::recordEvent(string pKey, Map<string, __String*> pSegmentation, float pSum, int pCount) {
  eventQueue->recordEvent(pKey, pCount, pSum, pSegmentation);
  checkEventThreshold();
}

void Countly::addDBEvent(string pKey, Map<string, __String*> pSegmentation, float pSum, time_t time, int pCount) {
  eventQueue->addDBEvent(pKey, pCount, pSum, pSegmentation, time);
}


void Countly::recordUserDetails(Map<string, __String*> pUserMap) {
  CountlyUserDetails::sharedInstance()->setUserData(pUserMap);
  CountlyConnectionQueue::sharedInstance()->sendUserDetails();
}

void Countly::recordUserDetails(Map<string, __String*> pUserMap, Map<string, __String*> pUserCustom) {
  CountlyUserDetails::sharedInstance()->setUserData(pUserMap, pUserCustom);
  CountlyConnectionQueue::sharedInstance()->sendUserDetails();
}


void Countly::checkEventThreshold() {
  if(eventQueue->eventCount() >= COUNTLY_EVENT_SEND_THRESHOLD) {
    string eventUrl = eventQueue->eventsUrl();
    CountlyConnectionQueue::sharedInstance()->recordEvents(eventUrl);
  }
}

void Countly::recordEvents() {
  if(eventQueue->eventCount() > 0) {
    string eventUrl = eventQueue->eventsUrl();
    CountlyConnectionQueue::sharedInstance()->recordEvents(eventUrl);
  }
}

time_t Countly::getStartTime() {
  return  startTime;
}

long Countly::timeSinceLaunch() {
  return time(NULL) - getStartTime();
}
// Temp Functions for testing


void Countly::printDeviceInfo() {
	CCLOG("printDeviceInfo");
	
  CCLOG("%s",CountlyDeviceInfoModel::getDeviceId());
//  CCLOG("%s",CountlyDeviceInfoModel::getUserAgent());
  CCLOG("%s",CountlyDeviceInfoModel::getDeviceModel());
//  CCLOG(CountlyDeviceInfo::getCarrierName());
  CCLOG("%s",CountlyDeviceInfoModel::getDeviceResolution());
  CCLOG("%s",CountlyDeviceInfoModel::getDeviceSystemName());
  CCLOG("%s",CountlyDeviceInfoModel::getDeviceSystemVersion());
}



int Countly::jsonTest() {
  rapidjson::StringBuffer s;
  rapidjson::Writer<rapidjson::StringBuffer> writer(s);
  
  writer.StartObject();
  writer.String("hello");
  writer.String("world");
  writer.String("t");
  writer.Bool(true);
  writer.String("f");
  writer.Bool(false);
  writer.String("n");
  writer.Null();
  writer.String("i");
  writer.Uint(123);
  writer.String("pi");
  writer.Double(3.1416);
  writer.String("a");
  writer.StartArray();
  for (unsigned i = 0; i < 4; i++)
    writer.Uint(i);
  writer.EndArray();
  writer.EndObject();
  
  
  CCLOG("%s",s.GetString() );
  
  string strEncode = CountlyUtils::urlencode(s.GetString());
  
  CCLOG("%s",strEncode.c_str());
  
//  cout << s.GetString() << endl;
  
  return 0;
}
