//
//  CountlyConnection.h
//  CountlyX
//
//  Created by Junaid on 27/08/2015.
//
//

#ifndef __CountlyX__CountlyConnection__
#define __CountlyX__CountlyConnection__

#include <stdio.h>
#include "cocos2d.h"

USING_NS_CC;

class CountlyConnection : public Ref {
  time_t timeStamp;
  __String* url;
  
public:
  ~CountlyConnection();
  CountlyConnection(time_t pTime,__String*  pUrl);
  void populateConnection(time_t pTime,__String*  pUrl);
  
  time_t getTime();
  __String* getUrl();
  
};

#endif /* defined(__CountlyX__CountlyConnection__) */
