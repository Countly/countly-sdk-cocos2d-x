//
//  CountlyUtils.h
//  CountlyX
//
//  Created by Junaid on 08/07/2015.
//
//

#ifndef __CountlyX__CountlyUtils__
#define __CountlyX__CountlyUtils__

#include "cocos2d.h"
using namespace std;
USING_NS_CC;
class CountlyUtils {
private:
  static string unreserved;
public:
  static const char* getDeviceResolution();
  static string urlencode(const string &s);
  static bool isMapEquals(Map<string, __String*> first, Map<string, __String*> second);
};

#endif /* defined(__CountlyX__CountlyUtils__) */
