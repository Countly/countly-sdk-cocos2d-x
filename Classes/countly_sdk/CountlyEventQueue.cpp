//
//  CounltyEventQueue.cpp
//  CountlyX
//
//  Created by Junaid on 14/07/2015.
//
//

#include "CountlyEventQueue.h"
#include "CountlyUtils.h"
#include "CountlyDBManager.h"
#include "CountlyConnectionQueue.h"
#include "external/json/writer.h"
#include "external/json/document.h"
#include "external/json/stringbuffer.h"

void CountlyEventQueue::addEvent(CountlyEvent* event) {
  eventQueue.pushBack(event);
  CountlyDBManager::sharedInstance()->insertEvent(event);
  event->release();
}

void CountlyEventQueue::addDBEvent(CountlyEvent* event) {
  eventQueue.pushBack(event);
  event->release();
}


void CountlyEventQueue::upateEvent(CountlyEvent *event) {
  CountlyDBManager::sharedInstance()->updateEvent(event);
}

void CountlyEventQueue::removeEvent(CountlyEvent *event) {
  eventQueue.eraseObject(event);
}

void CountlyEventQueue::removeAllEvents() {
  CountlyDBManager::sharedInstance()->removeAllEvents();
  eventQueue.clear();
}

int CountlyEventQueue::eventCount() {
  return (int)eventQueue.size();
}

Vector<CountlyEvent*> CountlyEventQueue::getEvents() {
  return eventQueue;
}

void CountlyEventQueue::recordEvent(string pKey, int pCount) {
  for(CountlyEvent* event : eventQueue)
  {
    if(event->getKey() == pKey) {
      event->addCount(pCount);
      upateEvent(event);
      return;
    }
  }
  CountlyEvent *event = new CountlyEvent();
  event->populateEvent(pKey, pCount);
  addEvent(event);
  
}
void CountlyEventQueue::recordEvent(string pKey, int pCount, float pSum) {
  for(CountlyEvent* event : eventQueue)
  {
    if(event->getKey() == pKey) {
      event->addSum(pSum);
      event->addCount(pCount);
      event->addjustTime(time(NULL));
      upateEvent(event);
      return;
    }
  }
  CountlyEvent *event = new CountlyEvent();
  event->populateEvent(pKey, pCount, pSum);
  addEvent(event);
  event->addjustTime(time(NULL));
  
}
void CountlyEventQueue::recordEvent(string pKey, int pCount, Map<string, __String*> pSegmentation) {
  for(CountlyEvent* event : eventQueue)
  {
    bool isSegEqual = CountlyUtils::isMapEquals(event->getSegmentation(), pSegmentation);
    if(event->getKey() == pKey && isSegEqual) {
      event->addCount(pCount);
      event->addjustTime(time(NULL));
      upateEvent(event);
      return;
    }
  }
  CountlyEvent *event = new CountlyEvent();
  event->populateEvent(pKey, pCount, pSegmentation);
  addEvent(event);
  
}
void CountlyEventQueue::recordEvent(string pKey, int pCount, float pSum, Map<string, __String*> pSegmentation) {
  for(CountlyEvent* event : eventQueue)
  {
    bool isSegEqual = CountlyUtils::isMapEquals(event->getSegmentation(), pSegmentation);
    if(event->getKey() == pKey && isSegEqual) {
      event->addSum(pSum);
      event->addCount(pCount);
      event->addjustTime(time(NULL));
      upateEvent(event);
      return;
    }
  }
  CountlyEvent *event = new CountlyEvent();
  event->populateEvent(pKey, pCount, pSum, pSegmentation);
  addEvent(event);
}

void CountlyEventQueue::addDBEvent(string pKey, int pCount, float pSum, Map<string, __String*> pSegmentation, time_t time) {
  
  CountlyEvent *event = new CountlyEvent();
  event->populateDBEvent(pKey, pCount, pSum, pSegmentation, time);
  addDBEvent(event);
}

string CountlyEventQueue::eventsUrl() {
  rapidjson::StringBuffer s;
  rapidjson::Writer<rapidjson::StringBuffer> writer(s);
  
  writer.StartArray();
  for(CountlyEvent* event : eventQueue)
  {
    event->serializedData(writer);
  }
  writer.EndArray();
  removeAllEvents();
  log("%s",s.GetString());
  
  string strEncode = CountlyUtils::urlencode(s.GetString());
  
  return strEncode;

}