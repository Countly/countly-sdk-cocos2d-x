//
//  CountlyMessaging.h
//  CountlyX
//
//  Created by Junaid on 17/08/2015.
//
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>


@interface CountlyMessaging : NSObject
{
  double unsentSessionLength;
 }

+ (instancetype)sharedInstance;
- (void)startWithMessagingUsing:(NSString *)appKey withHost:(NSString *)appHost andOptions:(NSDictionary *)options;

- (void)startWithTestMessagingUsing:(NSString *)appKey withHost:(NSString *)appHost andOptions:(NSDictionary *)options;

- (void)didRegisterForRemoteNotificationsWithDeviceToken:(NSData *)deviceToken;

- (void)didFailToRegisterForRemoteNotifications;

- (NSMutableSet *) countlyNotificationCategories;

- (NSMutableSet *) countlyNotificationCategoriesWithActionTitles:(NSArray *)actions;

- (BOOL)handleRemoteNotification:(NSDictionary *)info withButtonTitles:(NSArray *)titles;
- (BOOL)handleRemoteNotification:(NSDictionary *)info;

- (void)recordPushOpenForCountlyDictionary:(NSDictionary *)c;

- (void)recordPushActionForCountlyDictionary:(NSDictionary *)c;

@end

