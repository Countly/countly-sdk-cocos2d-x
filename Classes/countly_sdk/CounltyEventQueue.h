//
//  CounltyEventQueue.h
//  CountlyX
//
//  Created by Junaid on 14/07/2015.
//
//

#ifndef __CountlyX__CounltyEventQueue__
#define __CountlyX__CounltyEventQueue__

#include <stdio.h>
#include "CountlyEvent.h"

USING_NS_CC;
using namespace std;
class CountlyEventQueue {
private:
  Vector<CountlyEvent*> eventQueue;
public:
  
  int eventCount();
  string eventsUrl();
  Vector<CountlyEvent*> getEvents();
  void addEvent(CountlyEvent* event);
  void removeEvent(CountlyEvent* event);
  
  void recordEvent(string pKey, int pCount);
  void recordEvent(string pKey, int pCount, float pSum);
  void recordEvent(string pKey, int pCount, Map<string, __String*> pSegmentation);
  void recordEvent(string pKey, int pCount, float pSum, Map<string, __String*> pSegmentation);
};

#endif /* defined(__CountlyX__CounltyEventQueue__) */
