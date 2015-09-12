//
//  CountlyConnection.cpp
//  CountlyX
//
//  Created by Junaid on 27/08/2015.
//
//

#include "CountlyConnection.h"

CountlyConnection::CountlyConnection(time_t pTime,__String*  pUrl) {
  timeStamp = pTime;
  url = pUrl;
  url->retain();
}

CountlyConnection::~CountlyConnection() {
  url->release();
}

void CountlyConnection::populateConnection(time_t pTime, cocos2d::__String *pUrl) {
  timeStamp = pTime;
  url = pUrl;
}

time_t CountlyConnection::getTime() {
  return timeStamp;
}

__String* CountlyConnection::getUrl() {
  return url;
}