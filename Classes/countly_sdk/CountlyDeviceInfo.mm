//
//  CountlyDeviceInfo.m
//  CountlyX
//
//  Created by Junaid on 08/07/2015.
//
//

#include "CountlyDeviceInfo.h"
#import <CoreTelephony/CTTelephonyNetworkInfo.h>
#import <CoreTelephony/CTCarrier.h>


const char* CountlyDeviceInfo::getDeviceId() {
  return [[[[UIDevice currentDevice] identifierForVendor] UUIDString] cStringUsingEncoding:NSUTF8StringEncoding];
}

const char* CountlyDeviceInfo::getUserAgent()
{
  UIWebView *webView = [[UIWebView alloc] initWithFrame:CGRectZero];
  NSString *uaString = [webView stringByEvaluatingJavaScriptFromString:@"navigator.userAgent"];
  [[uaString copy] autorelease];
  [webView release];
  return [uaString cStringUsingEncoding:NSUTF8StringEncoding];
}

const char* CountlyDeviceInfo::getDeviceModel()
{
  return [[[UIDevice currentDevice] model] cStringUsingEncoding:NSUTF8StringEncoding];
}

const char* CountlyDeviceInfo::getCarrierName() {
  CTTelephonyNetworkInfo *netinfo = [[CTTelephonyNetworkInfo alloc] init];
  CTCarrier *carrier = [netinfo subscriberCellularProvider];
  return  [[carrier carrierName] cStringUsingEncoding:NSUTF8StringEncoding];
}

const char* CountlyDeviceInfo::getDeviceSystemName()
{
  return [[[UIDevice currentDevice] systemName] cStringUsingEncoding:NSUTF8StringEncoding];
}

const char* CountlyDeviceInfo::getDeviceSystemVersion()
{
  return [[[UIDevice currentDevice] systemVersion] cStringUsingEncoding:NSUTF8StringEncoding];
}


