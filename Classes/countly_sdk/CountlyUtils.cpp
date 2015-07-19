//
//  CountlyUtils.cpp
//  CountlyX
//
//  Created by Junaid on 08/07/2015.
//
//

#include "CountlyUtils.h"

USING_NS_CC;

//RFC 3986 section 2.3 Unreserved Characters (January 2005)
string CountlyUtils::unreserved = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_.~";

const char* CountlyUtils::getDeviceResolution()  {
  auto director = Director::getInstance();
  auto glview = director->getOpenGLView();
  Size frameSize = glview->getFrameSize();
  __String *frameSizeStr = __String::createWithFormat("%dx%d",(int)frameSize.width,(int)frameSize.height);
  
  return frameSizeStr->getCString();
}

string CountlyUtils::urlencode(const string &s)
{
  string escaped="";
  size_t length = s.length();
  for(size_t i=0; i<length; i++)
  {
    if (unreserved.find_first_of(s[i]) != string::npos)
    {
      escaped.push_back(s[i]);
    }
    else
    {
      escaped.append("%");
      char buf[3];
      sprintf(buf, "%.2X", s[i]);
      escaped.append(buf);
    }
  }
  return escaped;
}

bool CountlyUtils::isMapEquals(Map<string, __String*> first, Map<string, __String*> second) {
  auto pred = [] (decltype(*first.begin()) a, decltype(a) b)
  { return a.first == b.first; };
  
  bool  isEqual = first.size() == second.size()
  && std::equal(first.begin(), first.end(), second.begin(), pred);
  return isEqual;
}