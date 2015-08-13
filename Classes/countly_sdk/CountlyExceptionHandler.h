//
//  CountlyExceptionHandler.h
//  CountlyX
//
//  Created by Junaid on 24/07/2015.
//
//

#ifndef __CountlyX__CountlyExceptionHandler__
#define __CountlyX__CountlyExceptionHandler__


#include "cocos2d.h"
#include <stdio.h>

class CountlyExceptionHandler {
  static char* p;
  
public:
  static void testCrash();
 static void printStacktrace(std::string reason);
 static void signalHandler(int sig);
 static void terminateHandler();
 static void startCrashReporting();
  static void exitSignalHandler(int _ignored);

  
};

#endif /* defined(__CountlyX__CountlyExceptionHandler__) */
