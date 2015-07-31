##What's Countly?
[Countly](http://count.ly) is an innovative, real-time, open source mobile analytics and push notifications platform. It collects data from mobile devices, and visualizes this information to analyze mobile application 
usage and end-user behavior. There are two parts of Countly: [the server that collects and analyzes data](http://github.com/countly/countly-server), and mobile SDK that sends this data. Both parts are open source with different licensing terms.

##About

This repository includes the Countly Cocos2d-x SDK. 

### Other Github resources ###

This SDK needs one of Countly mobile analytics & push notifications server editions. For more information about different editions, see [comparison table](https://count.ly/compare/).

There are also other Countly SDK repositories (both official and community supported) on [Countly resources](http://resources.count.ly/v1.0/docs/downloading-sdks).

### Implemented features 

* General analytics features (iOS & Android) 
* Crash reporting: Crash reporting is implemented for iOS (objective-c) and C++. Main features of crash reporting.
** Uncaught Exception Handler: Its responsibilities would be handling of uncaught exception, creation of crash report on the basis of exception and device information (some information of devices is still in progress like RAM, Disk etc).
** Signal Handler: It handles signals raised through code like SIGABRT, SIGSEGV etc, backtrace the symbols, create an exception on the basis of signal, backtrace it and send this exception to uncaught exception handler.
** Terminate Handler for C++: It will handle the uncaught exception, re-throw that exception, backtrace the symbols, create an exception on the basis of symbols & reason and send that exception to uncaught exception handler.

## In-progress work 

* Push notifications
* User profiles
* Referral tracking

