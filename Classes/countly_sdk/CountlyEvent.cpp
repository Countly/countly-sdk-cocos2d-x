//
//  CountlyEvent.cpp
//  CountlyX
//
//  Created by Junaid on 14/07/2015.
//
//

#include "CountlyEvent.h"
#include "CountlyUtils.h"

using namespace rapidjson;

CountlyEvent::CountlyEvent() {
  key = "";
  count = 0;
  sum = 0.0f;
  segmentation.clear();
}


void CountlyEvent::populateEvent(string pKey, int pCount) {
  key = pKey;
  count = pCount;
}
void CountlyEvent::populateEvent(string pKey, int pCount, float pSum) {
  key = pKey;
  sum = pSum;
  count = pCount;
}
void CountlyEvent::populateEvent(string pKey, int pCount, Map<string, __String*> pSegmentation) {
  key = pKey;
  count = pCount;
  segmentation = pSegmentation;
}
void CountlyEvent::populateEvent(string pKey, int pCount, float pSum, Map<string, __String*> pSegmentation) {
  key = pKey;
  sum = pSum;
  count = pCount;
  segmentation = pSegmentation;
}

void CountlyEvent::serializedData(rapidjson::Writer<rapidjson::StringBuffer> &writer) {
//  rapidjson::StringBuffer s;
//  rapidjson::Writer<rapidjson::StringBuffer> writer(s);
  
  writer.StartObject();
  writer.String("key");
  writer.String(key.c_str());
  writer.String("count");
  writer.Uint(count);
  if(sum > 0) {
    writer.String("sum");
    writer.Double(sum);
  }
  if(!segmentation.empty()){
    writer.String("segmentation");
    writer.StartObject();
    std::vector<std::string> mapKeyVec;
    mapKeyVec = segmentation.keys();
    for(auto key : mapKeyVec)
    {
      auto value = (__String*)segmentation.at(key);
      writer.String(key.c_str());
      writer.String(value->getCString());
    }
    writer.EndObject();
  }
  writer.EndObject();
  
}