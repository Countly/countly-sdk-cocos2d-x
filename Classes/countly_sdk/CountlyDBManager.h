//
//  CountlyDBManager.h
//  CountlyX
//
//  Created by Junaid on 27/08/2015.
//
//

#ifndef __CountlyX__CountlyDBManager__
#define __CountlyX__CountlyDBManager__

#include <stdio.h>
#include "cocos2d.h"
#include "CountlyEvent.h"

using namespace std;

USING_NS_CC;

class CountlyDBManager {
  string dbPath;
public:
  static CountlyDBManager* sharedInstance();
  
  void createDB();
  void insertUrl(time_t time, string url);
  
  void removeAllEvents();
  void insertEvent(CountlyEvent* event);
  void updateEvent(CountlyEvent* event);
  
  void removeUrl(time_t time);
  void populateUrls();
  void populateEvents();
//  int loadUrls (void * para, int n_column, char ** column_value, char ** column_name);
};

#endif /* defined(__CountlyX__CountlyDBManager__) */
