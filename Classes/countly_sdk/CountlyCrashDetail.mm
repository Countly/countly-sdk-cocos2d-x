//
//  CountlyCrashDetail.m
//  CountlyX
//
//  Created by Junaid on 01/08/2015.
//
//

#import <Foundation/Foundation.h>
#include "CountlyCrashDetail.h"

#import <AudioToolbox/AudioToolbox.h>
#include <sys/types.h>
#include <sys/sysctl.h>
#import <mach/mach.h>
#import <mach/mach_host.h>
#import <arpa/inet.h>
#import <ifaddrs.h>
#include <libkern/OSAtomic.h>
#include <execinfo.h>
#include "Countly.h"

unsigned long long freeRAM() {
  vm_statistics_data_t vms;
  mach_msg_type_number_t ic = HOST_VM_INFO_COUNT;
  kern_return_t kr = host_statistics(mach_host_self(), HOST_VM_INFO, (host_info_t)&vms, &ic);
  if(kr != KERN_SUCCESS)
    return -1;
  
  return vm_page_size * (vms.free_count);
}

unsigned long long totalRAM() {
  return NSProcessInfo.processInfo.physicalMemory;
}

unsigned long long freeDisk() {
  return [[NSFileManager.defaultManager attributesOfFileSystemForPath:NSHomeDirectory() error:nil][NSFileSystemFreeSize] longLongValue];
}

unsigned long long totalDisk() {
  return [[NSFileManager.defaultManager attributesOfFileSystemForPath:NSHomeDirectory() error:nil][NSFileSystemSize] longLongValue];
}


long CountlyCrashDetail::getRamCurrent() {
  return ((totalRAM()-freeRAM())/1048576);
}
long CountlyCrashDetail::getRamTotal() {
  return (totalRAM()/1048576);
}
long CountlyCrashDetail::getDiskCurrent() {
  return ((totalDisk()-freeDisk())/1048576);
}
long CountlyCrashDetail::getDiskTotal() {
  return (totalDisk()/1048576);
}

int CountlyCrashDetail::batteryLevel() {
  UIDevice.currentDevice.batteryMonitoringEnabled = YES;
  return abs((int)(UIDevice.currentDevice.batteryLevel*100));
}

const char* CountlyCrashDetail::orientation() {
  NSArray *orientations = @[@"Unknown", @"Portrait", @"PortraitUpsideDown", @"LandscapeLeft", @"LandscapeRight", @"FaceUp", @"FaceDown"];
  NSString *orientationStr = orientations[UIDevice.currentDevice.orientation];
  return [orientationStr cStringUsingEncoding:NSUTF8StringEncoding];
}

long CountlyCrashDetail::connectionType() {
  typedef enum:NSInteger {CLYConnectionNone, CLYConnectionCellNetwork, CLYConnectionWiFi} CLYConnectionType;
  CLYConnectionType connType = CLYConnectionNone;
  
  @try
  {
    struct ifaddrs *interfaces, *i;
    
    if (!getifaddrs(&interfaces))
    {
      i = interfaces;
      
      while(i != NULL)
      {
        if(i->ifa_addr->sa_family == AF_INET)
        {
          if([[NSString stringWithUTF8String:i->ifa_name] isEqualToString:@"pdp_ip0"])
          {
            connType = CLYConnectionCellNetwork;
          }
          else if([[NSString stringWithUTF8String:i->ifa_name] isEqualToString:@"en0"])
          {
            connType = CLYConnectionWiFi;
            break;
          }
        }
        
        i = i->ifa_next;
      }
    }
    
    freeifaddrs(interfaces);
  }
  @catch (NSException *exception)
  {
    
  }
  
  return connType;
}

float CountlyCrashDetail::OpenGLESversion() {
  EAGLContext *aContext;
  
  aContext = [EAGLContext.alloc initWithAPI:kEAGLRenderingAPIOpenGLES3];
  if(aContext)
    return 3.0;
  
  aContext = [EAGLContext.alloc initWithAPI:kEAGLRenderingAPIOpenGLES2];
  if(aContext)
    return 2.0;
  
  return 1.0;
}


bool CountlyCrashDetail::isRooted() {
  FILE *f = fopen("/bin/bash", "r");
  BOOL isJailbroken = (f != NULL);
  fclose(f);
  return isJailbroken;
}

bool CountlyCrashDetail::isInBackground() {
  return UIApplication.sharedApplication.applicationState == UIApplicationStateBackground;
}

bool CountlyCrashDetail::isMuted() {
#if TARGET_IPHONE_SIMULATOR
  // return NO in simulator. Code causes crashes for some reason.
  return NO;
#endif
  
  CFStringRef state;
  UInt32 propertySize = sizeof(CFStringRef);
  AudioSessionInitialize(NULL, NULL, NULL, NULL);
  AudioSessionGetProperty(kAudioSessionProperty_AudioRoute, &propertySize, &state);
  if(CFStringGetLength(state) > 0)
    return NO;
  else
    return YES;
  
}

const char* CountlyCrashDetail::manufacturer() {
  return "Apple";
}

const char* CountlyCrashDetail::getCPUType() {
  NSMutableString *cpu = [[NSMutableString alloc] init];
  size_t size;
  cpu_type_t type;
  cpu_subtype_t subtype;
  size = sizeof(type);
  sysctlbyname("hw.cputype", &type, &size, NULL, 0);
  
  size = sizeof(subtype);
  sysctlbyname("hw.cpusubtype", &subtype, &size, NULL, 0);
  
  // values for cputype and cpusubtype defined in mach/machine.h
  if (type == CPU_TYPE_X86)
  {
    [cpu appendString:@"x86 "];
    // check for subtype ...
    
  } else if (type == CPU_TYPE_ARM)
  {
    [cpu appendString:@"ARM"];
    switch(subtype)
    {
      case CPU_SUBTYPE_ARM_V7:
        [cpu appendString:@"V7"];
        break;
        // ...
    }
  }
  const char* cpuStr = [cpu cStringUsingEncoding:NSUTF8StringEncoding];
  [cpu release];
  return cpuStr;
}