//
//  CountlyExceptionHandler.m
//  CountlyX
//
//  Created by Junaid on 28/07/2015.
//
//

#include "Countly.h"
#include <execinfo.h>
#include "CountlyCrashDetail.h"
#include "CountlyDeviceInfoModel.h"
#include "CountlyExceptionHandler.h"
#include "CountlyConnectionQueue.h"


#define kCountlyCrashUserInfoKey @"[CLY]_stack_trace"

NSString* CountlyJSONFromObject(id object);
NSString* CountlyURLEscapedString(NSString* string);
NSString* CountlyURLUnescapedString(NSString* string);
void CountlyUncaughtExceptionHandler(NSException *exception);


void CountlyExceptionHandler::printStacktrace(string reason)
{
  
  void* callstack[128];
  NSInteger frames = backtrace(callstack, 128);
  char **lines = backtrace_symbols(callstack, (int)frames);
  
  const NSInteger startOffset = 1;
  NSMutableArray *backtrace = [NSMutableArray arrayWithCapacity:frames];
  
  for (NSInteger i = startOffset; i < frames; i++)
    [backtrace addObject:[NSString stringWithUTF8String:lines[i]]];
  
  free(lines);
  
  NSMutableDictionary *userInfo =[NSMutableDictionary dictionaryWithObject:@"UnCaught Exception" forKey:@"Exception"];
  [userInfo setObject:backtrace forKey:kCountlyCrashUserInfoKey];
  
  NSString *reasonStr = [NSString stringWithFormat:@"%s",reason.c_str()];
  NSException *e = [NSException exceptionWithName:@"UnCaught Exception" reason:reasonStr userInfo:userInfo];
  
  CountlyUncaughtExceptionHandler(e);

}


void CountlyExceptionHandler::terminateHandler()
{
  string reason = "";
  std::exception_ptr exptr = std::current_exception();
  if (exptr != 0)
  {
    // the only useful feature of std::exception_ptr is that it can be rethrown...
    try
    {
      std::rethrow_exception(exptr);
    }
    catch (std::exception &ex)
    {
     
      reason = "Terminated due to exception: ";
      reason.append(ex.what());
    }
    catch (...)
    {
      reason = "Terminated due to unknown exception";
    }
  }
  else
  {
    reason = "Terminated due to unknown reason :(";
  }
  printStacktrace(reason);
  std::abort();
}


NSString* CountlyJSONFromObject(id object)
{
  NSError *error = nil;
  NSData *data = [NSJSONSerialization dataWithJSONObject:object options:0 error:&error];
  
  if (error)
  {}
//    COUNTLY_LOG(@"%@", [error description]);
  
  return [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
}


NSString* CountlyURLEscapedString(NSString* string)
{
  // Encode all the reserved characters, per RFC 3986
  // (<http://www.ietf.org/rfc/rfc3986.txt>)
  CFStringRef escaped =
  CFURLCreateStringByAddingPercentEscapes(kCFAllocatorDefault,
                                          (CFStringRef)string,
                                          NULL,
                                          (CFStringRef)@"!*'();:@&=+$,/?%#[]",
                                          kCFStringEncodingUTF8);
  return (NSString*)CFBridgingRelease(escaped);
}

NSString* CountlyURLUnescapedString(NSString* string)
{
  NSMutableString *resultString = [NSMutableString stringWithString:string];
  [resultString replaceOccurrencesOfString:@"+"
                                withString:@" "
                                   options:NSLiteralSearch
                                     range:NSMakeRange(0, [resultString length])];
  return [resultString stringByReplacingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
}


void CountlyExceptionHandler(NSException *exception, bool nonfatal)
{
//  if(Countly.sharedInstance.crashCustom)
//    crashReport[@"_custom"] = Countly.sharedInstance.crashCustom;
  
//  if(CountlyCustomCrashLogs)
//    crashReport[@"_logs"] = [CountlyCustomCrashLogs componentsJoinedByString:@"\n"];
  
  NSArray* stackArray = exception.userInfo[kCountlyCrashUserInfoKey];
  if(!stackArray) stackArray = exception.callStackSymbols;
  
  NSMutableString* stackString = NSMutableString.string;
  for (NSString* line in stackArray)
  {
    NSRegularExpression *regex = [NSRegularExpression regularExpressionWithPattern:@"\\s+\\s" options:0 error:nil];
    NSString *cleanLine = [regex stringByReplacingMatchesInString:line options:0 range:(NSRange){0,line.length} withTemplate:@"  "];
    [stackString appendString:cleanLine];
    [stackString appendString:@"\n"];
  }
  
  CountlyConnectionQueue::sharedInstance()->reportCrash([stackString cStringUsingEncoding:NSUTF8StringEncoding], [exception.debugDescription cStringUsingEncoding:NSUTF8StringEncoding], nonfatal);
  
  NSSetUncaughtExceptionHandler(NULL);
  signal(SIGABRT, SIG_DFL);
  signal(SIGILL, SIG_DFL);
  signal(SIGSEGV, SIG_DFL);
  signal(SIGFPE, SIG_DFL);
  signal(SIGBUS, SIG_DFL);
  signal(SIGPIPE, SIG_DFL);
}

void CountlyUncaughtExceptionHandler(NSException *exception)
{
  CountlyExceptionHandler(exception, false);
}

void CountlySignalHandler(uintptr_t signalCode)
{
  void* callstack[128];
  NSInteger frames = backtrace(callstack, 128);
  char **lines = backtrace_symbols(callstack, (int)frames);
  
  const NSInteger startOffset = 1;
  NSMutableArray *backtrace = [NSMutableArray arrayWithCapacity:frames];
  
  for (NSInteger i = startOffset; i < frames; i++)
    [backtrace addObject:[NSString stringWithUTF8String:lines[i]]];
  
  free(lines);
  
  NSMutableDictionary *userInfo =[NSMutableDictionary dictionaryWithObject:@(signalCode) forKey:@"signal_code"];
  [userInfo setObject:backtrace forKey:kCountlyCrashUserInfoKey];
  NSString *reason = [NSString stringWithFormat:@"App terminated by SIG%@",[NSString stringWithUTF8String:sys_signame[signalCode]].uppercaseString];
  
  NSException *e = [NSException exceptionWithName:@"Fatal Signal" reason:reason userInfo:userInfo];
  
  CountlyUncaughtExceptionHandler(e);
}

void registerSignalHandler() {
  
  dispatch_source_t source = dispatch_source_create(DISPATCH_SOURCE_TYPE_SIGNAL, SIGABRT, 0, dispatch_get_global_queue(0, 0));
  
  dispatch_source_set_event_handler(source, ^{
    CountlySignalHandler(SIGABRT);
  });
  dispatch_resume(source);
  
  source = dispatch_source_create(DISPATCH_SOURCE_TYPE_SIGNAL, SIGILL, 0, dispatch_get_global_queue(0, 0));
  
  dispatch_source_set_event_handler(source, ^{
    CountlySignalHandler(SIGILL);
  });
  dispatch_resume(source);
  
  source = dispatch_source_create(DISPATCH_SOURCE_TYPE_SIGNAL, SIGSEGV, 0, dispatch_get_global_queue(0, 0));
  
  dispatch_source_set_event_handler(source, ^{
    CountlySignalHandler(SIGSEGV);
  });
  dispatch_resume(source);
  
  source = dispatch_source_create(DISPATCH_SOURCE_TYPE_SIGNAL, SIGFPE, 0, dispatch_get_global_queue(0, 0));
  
  dispatch_source_set_event_handler(source, ^{
    CountlySignalHandler(SIGFPE);
  });
  dispatch_resume(source);
  
  source = dispatch_source_create(DISPATCH_SOURCE_TYPE_SIGNAL, SIGBUS, 0, dispatch_get_global_queue(0, 0));
  
  dispatch_source_set_event_handler(source, ^{
    CountlySignalHandler(SIGBUS);
  });
  dispatch_resume(source);
  
  source = dispatch_source_create(DISPATCH_SOURCE_TYPE_SIGNAL, SIGPIPE, 0, dispatch_get_global_queue(0, 0));
  
  dispatch_source_set_event_handler(source, ^{
    CountlySignalHandler(SIGPIPE);
  });
  dispatch_resume(source);
  
}

void CountlyExceptionHandler::startCrashReporting() {
  
  NSSetUncaughtExceptionHandler(&CountlyUncaughtExceptionHandler);
  registerSignalHandler();
  std::set_terminate(terminateHandler);
  
}

void CountlyExceptionHandler::testCrash() {
  
}
