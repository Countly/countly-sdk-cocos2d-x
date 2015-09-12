//
//  CountlyUserDetails.h
//  CountlyX
//
//  Created by Junaid on 13/08/2015.
//
//

#ifndef __CountlyX__CountlyUserDetails__
#define __CountlyX__CountlyUserDetails__

#include <stdio.h>
#include "cocos2d.h"

USING_NS_CC;
using namespace std;

class CountlyUserDetails {
  Map<string, __String*> userMap;
  Map<string, __String*> userCustom;
  
public:

  static CountlyUserDetails* sharedInstance ();
  void setUserData(Map<string, __String*> pUserMap);
  void setUserData(Map<string, __String*> pUserMap, Map<string, __String*> pUserCustom);
  string serialize();
  
};

#endif /* defined(__CountlyX__CountlyUserDetails__) */
