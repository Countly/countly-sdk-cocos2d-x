//
//  CountlyUserDetails.cpp
//  CountlyX
//
//  Created by Junaid on 13/08/2015.
//
//

#include "CountlyUserDetails.h"
#include "Countly.h"

static CountlyUserDetails * instance = NULL;

CountlyUserDetails* CountlyUserDetails::sharedInstance () {
  if (!instance) {
    instance = new CountlyUserDetails();
  }
  return instance;
}

void CountlyUserDetails::setUserData(Map<string, ::__String *> pUserMap) {
  userMap = pUserMap;
}

void CountlyUserDetails::setUserData(Map<string, __String*> pUserMap, Map<string, __String*> pUserCustom) {
  userMap = pUserMap;
  userCustom = pUserCustom;
}

string CountlyUserDetails::serialize() {
  rapidjson::StringBuffer s;
  rapidjson::Writer<rapidjson::StringBuffer> writer(s);
  
  writer.StartObject();
  auto value = userMap.at(kCLYUserName);
  if (value != nullptr) {
    writer.String(kCLYUserName);
    writer.String(value->getCString());
  }
  
  value = userMap.at(kCLYUserUsername);
  if (value != nullptr) {
    writer.String(kCLYUserUsername);
    writer.String(value->getCString());
  }
  
  value = userMap.at(kCLYUserEmail);
  if (value != nullptr) {
    writer.String(kCLYUserEmail);
    writer.String(value->getCString());
  }
  
  value = userMap.at(kCLYUserOrganization);
  if (value != nullptr) {
    writer.String(kCLYUserOrganization);
    writer.String(value->getCString());
  }
  
  value = userMap.at(kCLYUserPhone);
  if (value != nullptr) {
    writer.String(kCLYUserPhone);
    writer.String(value->getCString());
  }
  
  value = userMap.at(kCLYUserGender);
  if (value != nullptr) {
    writer.String(kCLYUserGender);
    writer.String(value->getCString());
  }
  
  value = userMap.at(kCLYUserPicture);
  if (value != nullptr) {
    writer.String(kCLYUserPicture);
    writer.String(value->getCString());
  }
  
  value = userMap.at(kCLYUserPicturePath);
  if (value != nullptr) {
    writer.String(kCLYUserPicturePath);
    writer.String(value->getCString());
  }
  
  value = userMap.at(kCLYUserBirthYear);
  if (value != nullptr) {
    writer.String(kCLYUserBirthYear);
    writer.Uint(value->intValue());
  }
  
  if(!userCustom.empty()){
    writer.String(kCLYUserCustom);
    writer.StartObject();
    std::vector<std::string> mapKeyVec;
    mapKeyVec = userCustom.keys();
    for(auto key : mapKeyVec)
    {
      auto value = (__String*)userCustom.at(key);
      writer.String(key.c_str());
      writer.String(value->getCString());
    }
    writer.EndObject();
  }
  writer.EndObject();
  
  return s.GetString();
}