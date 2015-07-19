//
//  CountlyDeviceInfoModel.cpp
//  CountlyX
//
//  Created by Junaid on 08/07/2015.
//
//

#include "CountlyDeviceInfoModel.h"
#include "CountlyDeviceInfo.h"
#include "CountlyUtils.h"

#include "external/json/writer.h"
#include "external/json/document.h"
#include "external/json/stringbuffer.h"

//const char* CountlyDeviceInfoModel::deviceId = "";
//const char* CountlyDeviceInfoModel::userAgent = "";
//const char* CountlyDeviceInfoModel::deviceModel = "";
//const char* CountlyDeviceInfoModel::carrierName = "";
//const char* CountlyDeviceInfoModel::deviceMetrics = "";
//const char* CountlyDeviceInfoModel::deviceResolution = "";
//const char* CountlyDeviceInfoModel::deviceSystemName = "";
//const char* CountlyDeviceInfoModel::deviceSystemVersion = "";

//CountlyDeviceInfoModel::CountlyDeviceInfoModel() {
//  populateModel();
//}


void CountlyDeviceInfoModel::populateModel() {
//  deviceId = CountlyDeviceInfo::getDeviceId();
//  userAgent = CountlyDeviceInfo::getUserAgent();
//  deviceModel = CountlyDeviceInfo::getDeviceModel();
//  carrierName = CountlyDeviceInfo::getCarrierName();
//  deviceResolution = CountlyUtils::getDeviceResolution();
//  deviceSystemName = CountlyDeviceInfo::getDeviceSystemName();
//  deviceSystemVersion = CountlyDeviceInfo::getDeviceSystemVersion();
//  
  populateMetrics();
}

const char* CountlyDeviceInfoModel::populateMetrics() {
  rapidjson::StringBuffer s;
  rapidjson::Writer<rapidjson::StringBuffer> writer(s);
  
  writer.StartObject();
  writer.String("_device");
  writer.String(CountlyDeviceInfo::getDeviceId());
  writer.String("_os");
  writer.String(CountlyDeviceInfo::getDeviceSystemName());
  writer.String("_os_version");
  writer.String(CountlyDeviceInfo::getDeviceSystemVersion());
  writer.String("_resolution");
  writer.String(CountlyUtils::getDeviceResolution());
  writer.String("_device_model");
  writer.String(CountlyDeviceInfo::getDeviceModel());
  writer.EndObject();
  
  string strEncode = CountlyUtils::urlencode(s.GetString());
  return strEncode.c_str();
//  deviceMetrics = strEncode.c_str();
}

const char* CountlyDeviceInfoModel::metrics() {
  return populateMetrics();
}

const char* CountlyDeviceInfoModel::getDeviceId() {
  return CountlyDeviceInfo::getDeviceId();
}
const char* CountlyDeviceInfoModel::getUserAgent() {
  return CountlyDeviceInfo::getUserAgent();
}
const char* CountlyDeviceInfoModel::getDeviceModel() {
  return CountlyDeviceInfo::getDeviceId();
}
const char* CountlyDeviceInfoModel::getCarrierName() {
  return CountlyDeviceInfo::getCarrierName();
}
const char* CountlyDeviceInfoModel::getDeviceResolution() {
  return CountlyUtils::getDeviceResolution();
}
const char* CountlyDeviceInfoModel::getDeviceSystemName() {
  return CountlyDeviceInfo::getDeviceSystemName();
}
const char* CountlyDeviceInfoModel::getDeviceSystemVersion() {
  return CountlyDeviceInfo::getDeviceSystemVersion();
}
