//
//  CountlyDeviceInfoModel.h
//  CountlyX
//
//  Created by Junaid on 08/07/2015.
//
//

#ifndef __CountlyX__CountlyDeviceInfoModel__
#define __CountlyX__CountlyDeviceInfoModel__


class CountlyDeviceInfoModel {
private:
//  CountlyDeviceInfoModel();
  
//  static const char* deviceId;
//  static const char* userAgent;
//  static const char* deviceModel;
//  static const char* carrierName;
//  static const char* deviceMetrics;
//  static const char* deviceResolution;
//  static const char* deviceSystemName;
//  static const char* deviceSystemVersion;
  
public:
  static void populateModel();
  static const char*  populateMetrics();
  static const char* metrics();
  static const char* getDeviceId();
  static const char* getUserAgent();
  static const char* getDeviceModel();
  static const char* getCarrierName();
  static const char* getDeviceResolution();
  static const char* getDeviceSystemName();
  static const char* getDeviceSystemVersion();
};

#endif /* defined(__CountlyX__CountlyDeviceInfoModel__) */
