//
//  CountlyBackTrace.h
//  CountlyX
//
//  Created by Junaid on 09/08/2015.
//
//

#ifndef __CountlyX__CountlyBackTrace__
#define __CountlyX__CountlyBackTrace__

#include "cocos2d.h"
#include <stdio.h>
#include <iostream>
#include <iomanip>

#include <unwind.h>
#include <dlfcn.h>

class CountlyBackTrace {
  
public:
  static std::string getBacktrace();
};

#endif /* defined(__CountlyX__CountlyBackTrace__) */
