//
//  CountlyEvent.h
//  CountlyX
//
//  Created by Junaid on 14/07/2015.
//
//

#ifndef __CountlyX__CountlyEvent__
#define __CountlyX__CountlyEvent__

#include <stdio.h>
#include "cocos2d.h"
#include "external/json/writer.h"
#include "external/json/document.h"
#include "external/json/stringbuffer.h"

USING_NS_CC;
using namespace std;
class CountlyEvent : public Ref {
private:
  int count;
  float sum;
  string key;
  time_t timestamp;
  Map<string, __String*> segmentation;
public:
  CountlyEvent();
  void populateEvent(string pKey, int pCount);
  void populateEvent(string pKey, int pCount, float pSum);
  void serializedData(rapidjson::Writer<rapidjson::StringBuffer> &writer);
  void populateEvent(string pKey, int pCount, Map<string, __String*> pSegmentation);
  void populateEvent(string pKey, int pCount, float pSum, Map<string, __String*> pSegmentation);
};

#endif /* defined(__CountlyX__CountlyEvent__) */
