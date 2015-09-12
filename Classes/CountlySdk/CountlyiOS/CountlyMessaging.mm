//
//  CountlyMessaging.m
//  CountlyX
//
//  Created by Junaid on 17/08/2015.
//
//

#include "CountlyMessaging.h"
#include "CountlyDeviceInfoModel.h"
#include "Countly.h"
#include "CountlyConnectionQueue.h"

@interface CountlyMessaging ()

@property (nonatomic, strong) NSMutableDictionary *messageInfos;
@property (nonatomic, strong) NSDictionary* crashCustom;
@property (nonatomic) BOOL startedWithTest;

@end

@implementation CountlyMessaging

#define kPushToMessage      1
#define kPushToOpenLink     2
#define kPushToUpdate       3
#define kPushToReview       4
#define kPushEventKeyOpen   @"[CLY]_push_open"
#define kPushEventKeyAction @"[CLY]_push_action"
#define kAppIdPropertyKey   @"[CLY]_app_id"
#define kCountlyAppId       @"695261996"

+ (instancetype)sharedInstance {
  static CountlyMessaging *_sharedCountlyMessage = nil;
  static dispatch_once_t onceToken;
  dispatch_once(&onceToken, ^{
    _sharedCountlyMessage = [[CountlyMessaging alloc] init];
  });
  return _sharedCountlyMessage;
  
}

- (instancetype)init
{
  if (self = [super init])
  {
    _startedWithTest = NO;
    self.messageInfos = [NSMutableDictionary new];
  }
  return self;
}


- (NSMutableSet *) countlyNotificationCategories {
  return [self countlyNotificationCategoriesWithActionTitles:@[@"Cancel", @"Open", @"Update", @"Review"]];
}

- (NSMutableSet *) countlyNotificationCategoriesWithActionTitles:(NSArray *)actions {
  UIMutableUserNotificationCategory *url = [UIMutableUserNotificationCategory new],
  *upd = [UIMutableUserNotificationCategory new],
  *rev = [UIMutableUserNotificationCategory new];
  
  url.identifier = @"[CLY]_url";
  upd.identifier = @"[CLY]_update";
  rev.identifier = @"[CLY]_review";
  
  UIMutableUserNotificationAction *cancel = [UIMutableUserNotificationAction new],
  *open = [UIMutableUserNotificationAction new],
  *update = [UIMutableUserNotificationAction new],
  *review = [UIMutableUserNotificationAction new];
  
  cancel.identifier = @"[CLY]_cancel";
  open.identifier   = @"[CLY]_open";
  update.identifier = @"[CLY]_update";
  review.identifier = @"[CLY]_review";
  
  cancel.title = actions[0];
  open.title   = actions[1];
  update.title = actions[2];
  review.title = actions[3];
  
  cancel.activationMode = UIUserNotificationActivationModeBackground;
  open.activationMode   = UIUserNotificationActivationModeForeground;
  update.activationMode = UIUserNotificationActivationModeForeground;
  review.activationMode = UIUserNotificationActivationModeForeground;
  
  cancel.destructive = NO;
  open.destructive   = NO;
  update.destructive = NO;
  review.destructive = NO;
  
  
  [url setActions:@[cancel, open] forContext:UIUserNotificationActionContextMinimal];
  [url setActions:@[cancel, open] forContext:UIUserNotificationActionContextDefault];
  
  [upd setActions:@[cancel, update] forContext:UIUserNotificationActionContextMinimal];
  [upd setActions:@[cancel, update] forContext:UIUserNotificationActionContextDefault];
  
  [rev setActions:@[cancel, review] forContext:UIUserNotificationActionContextMinimal];
  [rev setActions:@[cancel, review] forContext:UIUserNotificationActionContextDefault];
  
  NSMutableSet *set = [NSMutableSet setWithObjects:url, upd, rev, nil];
  
  return set;
}

- (void)startWithMessagingUsing:(NSString *)appKey withHost:(NSString *)appHost andOptions:(NSDictionary *)options
{
  string appKeyStr = [appKey cStringUsingEncoding:NSUTF8StringEncoding];
  string appHostStr = [appHost cStringUsingEncoding:NSUTF8StringEncoding];
  Countly::sharedInstance()->start(appKeyStr, appHostStr);
  
  NSDictionary *notification = [options objectForKey:UIApplicationLaunchOptionsRemoteNotificationKey];
  if (notification) {
    //    COUNTLY_LOG(@"Got notification on app launch: %@", notification);
    [self handleRemoteNotification:notification displayingMessage:NO];
  }
  
//  [self withAppStoreId:^(NSString *appId) {
//    NSLog(@"ID: %@", appId);
//  }];
}

- (void)startWithTestMessagingUsing:(NSString *)appKey withHost:(NSString *)appHost andOptions:(NSDictionary *)options
{
  _startedWithTest = YES;
  [self startWithMessagingUsing:appKey withHost:appHost andOptions:options];
}


- (BOOL) handleRemoteNotification:(NSDictionary *)info withButtonTitles:(NSArray *)titles {
  return [self handleRemoteNotification:info displayingMessage:YES withButtonTitles:titles];
}

- (BOOL) handleRemoteNotification:(NSDictionary *)info {
  return [self handleRemoteNotification:info displayingMessage:YES];
}

- (BOOL) handleRemoteNotification:(NSDictionary *)info displayingMessage:(BOOL)displayMessage {
  return [self handleRemoteNotification:info displayingMessage:displayMessage
                       withButtonTitles:@[@"Cancel", @"Open", @"Update", @"Review"]];
}

- (BOOL) handleRemoteNotification:(NSDictionary *)info displayingMessage:(BOOL)displayMessage withButtonTitles:(NSArray *)titles {
//  COUNTLY_LOG(@"Handling remote notification (display? %d): %@", displayMessage, info);
  
  NSDictionary *aps = info[@"aps"];
  NSDictionary *countly = info[@"c"];
  
  if (countly[@"i"]) {
//    COUNTLY_LOG(@"Message id: %@", countly[@"i"]);
    
    [self recordPushOpenForCountlyDictionary:countly];
    NSString *appName = [[[NSBundle mainBundle] infoDictionary] objectForKey:(NSString*)kCFBundleNameKey];
    NSString *message = [aps objectForKey:@"alert"];
    
    int type = 0;
    NSString *action = nil;
    
    if ([aps objectForKey:@"content-available"]) {
      return NO;
    } else if (countly[@"l"]) {
      type = kPushToOpenLink;
      action = titles[1];
    } else if (countly[@"r"] != nil) {
      type = kPushToReview;
      action = titles[3];
    } else if (countly[@"u"] != nil) {
      type = kPushToUpdate;
      action = titles[2];
    } else if (displayMessage) {
      type = kPushToMessage;
      action = nil;
    }
    
    if (type && [message length]) {
      UIAlertView *alert;
      if (action) {
        alert = [[UIAlertView alloc] initWithTitle:appName message:message delegate:self
                                 cancelButtonTitle:titles[0] otherButtonTitles:action, nil];
      } else {
        alert = [[UIAlertView alloc] initWithTitle:appName message:message delegate:self
                                 cancelButtonTitle:titles[0] otherButtonTitles:nil];
      }
      alert.tag = type;
      
      _messageInfos[alert.description] = info;
      
      [alert show];
      return YES;
    }
  }
  
  return NO;
}

- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex {
  NSDictionary *info = [_messageInfos[alertView.description] copy];
  [_messageInfos removeObjectForKey:alertView.description];
  
  if (alertView.tag == kPushToMessage) {
    // do nothing
  } else if (buttonIndex != alertView.cancelButtonIndex) {
    if (alertView.tag == kPushToOpenLink) {
      [self recordPushActionForCountlyDictionary:info[@"c"]];
      [[UIApplication sharedApplication] openURL:[NSURL URLWithString:info[@"c"][@"l"]]];
    } else if (alertView.tag == kPushToUpdate) {
      if ([info[@"c"][@"u"] length]) {
        [self openUpdate:info[@"c"][@"u"] forInfo:info];
      } else {
        [self withAppStoreId:^(NSString *appStoreId) {
          [self openUpdate:appStoreId forInfo:info];
        }];
      }
    } else if (alertView.tag == kPushToReview) {
      if ([info[@"c"][@"r"] length]) {
        [self openReview:info[@"c"][@"r"] forInfo:info];
      } else {
        [self withAppStoreId:^(NSString *appStoreId) {
          [self openReview:appStoreId forInfo:info];
        }];
      }
    }
  }
}

- (void) withAppStoreId:(void (^)(NSString *))block{
  NSString *appStoreId = [[NSUserDefaults standardUserDefaults] stringForKey:kAppIdPropertyKey];
  if (appStoreId) {
    block(appStoreId);
  } else {
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^{
      NSString *appStoreId = nil;
      NSString *bundle = [[NSBundle mainBundle] bundleIdentifier];
      
      NSString *appStoreCountry = [(NSLocale *)[NSLocale currentLocale] objectForKey:NSLocaleCountryCode];
      if ([appStoreCountry isEqualToString:@"150"]) {
        appStoreCountry = @"eu";
      } else if ([[appStoreCountry stringByReplacingOccurrencesOfString:@"[A-Za-z]{2}" withString:@"" options:NSRegularExpressionSearch range:NSMakeRange(0, 2)] length]) {
        appStoreCountry = @"us";
      }
      
      NSString *iTunesServiceURL = [NSString stringWithFormat:@"http://itunes.apple.com/%@/lookup", appStoreCountry];
      iTunesServiceURL = [iTunesServiceURL stringByAppendingFormat:@"?bundleId=%@", bundle];
      
      NSError *error = nil;
      NSURLResponse *response = nil;
      NSURLRequest *request = [NSURLRequest requestWithURL:[NSURL URLWithString:iTunesServiceURL] cachePolicy:NSURLRequestUseProtocolCachePolicy timeoutInterval:10];
      NSData *data = [NSURLConnection sendSynchronousRequest:request returningResponse:&response error:&error];
      NSInteger statusCode = ((NSHTTPURLResponse *)response).statusCode;
      if (data && statusCode == 200) {
        
        id json = [[NSJSONSerialization JSONObjectWithData:data options:(NSJSONReadingOptions)0 error:&error][@"results"] lastObject];
        
        if (!error && [json isKindOfClass:[NSDictionary class]]) {
          NSString *bundleID = json[@"bundleId"];
          if (bundleID && [bundleID isEqualToString:bundle]) {
            appStoreId = [json[@"trackId"] stringValue];
          }
        }
      }
      
      dispatch_async(dispatch_get_main_queue(), ^{
        [[NSUserDefaults standardUserDefaults] setObject:appStoreId forKey:kAppIdPropertyKey];
        [[NSUserDefaults standardUserDefaults] synchronize];
        block(appStoreId);
      });
    });
  }
  
}

- (void) openUpdate:(NSString *)appId forInfo:(NSDictionary *)info {
  if (!appId) appId = kCountlyAppId;
    
    NSString *urlFormat = nil;
#if TARGET_OS_IPHONE
    urlFormat = @"itms-apps://itunes.apple.com/app/id%@";
#else
    urlFormat = @"macappstore://itunes.apple.com/app/id%@";
#endif
    
    [self recordPushActionForCountlyDictionary:info[@"c"]];
  
  NSURL *url = [NSURL URLWithString:[NSString stringWithFormat:urlFormat, appId]];
  [[UIApplication sharedApplication] openURL:url];
}

- (void) openReview:(NSString *)appId forInfo:(NSDictionary *)info{
  if (!appId) appId = kCountlyAppId;
    
    NSString *urlFormat = nil;
#if TARGET_OS_IPHONE
    float iOSVersion = [[UIDevice currentDevice].systemVersion floatValue];
    if (iOSVersion >= 7.0f && iOSVersion < 7.1f) {
      urlFormat = @"itms-apps://itunes.apple.com/app/id%@";
    } else {
      urlFormat = @"itms-apps://itunes.apple.com/WebObjects/MZStore.woa/wa/viewContentsUserReviews?type=Purple+Software&id=%@";
    }
#else
  urlFormat = @"macappstore://itunes.apple.com/app/id%@";
#endif
  
  [self recordPushActionForCountlyDictionary:info[@"c"]];
  
  NSURL *url = [NSURL URLWithString:[NSString stringWithFormat:urlFormat, appId]];
  [[UIApplication sharedApplication] openURL:url];
}

- (void)recordPushOpenForCountlyDictionary:(NSDictionary *)c {
  NSString *messageId = c[@"i"];
  string messageIdStr = [messageId cStringUsingEncoding:NSUTF8StringEncoding];
  string keyStr = [kPushEventKeyOpen cStringUsingEncoding:NSUTF8StringEncoding];
  Countly::sharedInstance()->recordPushEvent(keyStr, messageIdStr, 1);;
}

- (void)recordPushActionForCountlyDictionary:(NSDictionary *)c {
  NSString *messageId = c[@"i"];
  string messageIdStr = [messageId cStringUsingEncoding:NSUTF8StringEncoding];
  string keyStr = [kPushEventKeyAction cStringUsingEncoding:NSUTF8StringEncoding];
  Countly::sharedInstance()->recordPushEvent(keyStr, messageIdStr, 1);
}

- (void)didRegisterForRemoteNotificationsWithDeviceToken:(NSData *)deviceToken {
  const unsigned *tokenBytes = (const unsigned *)[deviceToken bytes];
  NSString *token = [NSString stringWithFormat:@"%08x%08x%08x%08x%08x%08x%08x%08x",
                     ntohl(tokenBytes[0]), ntohl(tokenBytes[1]), ntohl(tokenBytes[2]),
                     ntohl(tokenBytes[3]), ntohl(tokenBytes[4]), ntohl(tokenBytes[5]),
                     ntohl(tokenBytes[6]), ntohl(tokenBytes[7])];
  CountlyConnectionQueue::sharedInstance()->tokenSession([token cStringUsingEncoding:NSUTF8StringEncoding], "ios_token", _startedWithTest);
}

- (void)didFailToRegisterForRemoteNotifications {
  CountlyConnectionQueue::sharedInstance()->tokenSession("", "ios_token", _startedWithTest);
}

@end

