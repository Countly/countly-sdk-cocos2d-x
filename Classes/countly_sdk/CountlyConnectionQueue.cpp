//
//  CountlyConnectionQueue.cpp
//  CountlyX
//
//  Created by Junaid on 13/07/2015.
//
//

#include "Countly.h"
#include "CountlyUtils.h"
#include "CountlyUserDetails.h"
#include "CountlyCrashDetail.h"
#include "CountlyConnectionQueue.h"
#include "CountlyDeviceInfoModel.h"
#include "external/json/writer.h"
#include "external/json/document.h"
#include "external/json/stringbuffer.h"

using namespace cocos2d::network;

#define COUNTLY_SDK_VERSION "1.0"

static CountlyConnectionQueue* instance = NULL;
CountlyConnectionQueue::CountlyConnectionQueue() {
  bgTask = false;
  locationString = nullptr;
}

CountlyConnectionQueue* CountlyConnectionQueue::sharedInstance() {
  if (!instance) {
    instance = new CountlyConnectionQueue();
  }
  return instance;
}

void CountlyConnectionQueue::setCrashCustom(Map<string, cocos2d::__String *> custom) {
  crashCustom = custom;
}

string CountlyConnectionQueue::getAppKey() {
  return appKey;
}
string CountlyConnectionQueue::getAppHost() {
  return appHost;
}

void CountlyConnectionQueue::setAppKey(string key) {
  appKey = key;
}

void CountlyConnectionQueue::setAppHost(string host) {
  appHost = host;
}

void CountlyConnectionQueue::addToQueue(__String* data) {
  dataQueue.pushBack(data);
}


void CountlyConnectionQueue::beginSession() {
  __String *data = __String::createWithFormat("app_key=%s&device_id=%s&timestamp=%ld&sdk_version=%s&begin_session=1&metrics=%s",this->appKey.c_str(),CountlyDeviceInfoModel::getDeviceId(),time(NULL),COUNTLY_SDK_VERSION,CountlyDeviceInfoModel::metrics());
  
  addToQueue(data);
  tick();
}

void CountlyConnectionQueue::sendUserDetails() {
  
  string str = CountlyUserDetails::sharedInstance()->serialize();
  
  string userData = CountlyUtils::urlencode(str);
  
  __String *data = __String::createWithFormat("app_key=%s&device_id=%s&timestamp=%ld&sdk_version=%s&user_details=%s",this->appKey.c_str(),CountlyDeviceInfoModel::getDeviceId(),time(NULL),COUNTLY_SDK_VERSION,userData.c_str());
  addToQueue(data);
  tick();
  
}
void CountlyConnectionQueue::endSessionWithDuration(int duration) {
  __String *data = __String::createWithFormat("app_key=%s&device_id=%s&timestamp=%ld&end_session=1&session_duration=%d",this->appKey.c_str(),CountlyDeviceInfoModel::getDeviceId(),time(NULL),duration);

  addToQueue(data);
  tick();
}

void CountlyConnectionQueue::updateSessionWithDuration(int duration) {
  __String *data = __String::createWithFormat("app_key=%s&device_id=%s&timestamp=%ld&session_duration=%d",this->appKey.c_str(),CountlyDeviceInfoModel::getDeviceId(),time(NULL),duration);
  if (this->locationString) {
    data->appendWithFormat("&location=%s",locationString);
    locationString = nullptr;
  }
  addToQueue(data);
  tick();
}

void CountlyConnectionQueue::reportCrash(string crashReport) {
  __String *data = __String::createWithFormat("app_key=%s&device_id=%s&timestamp=%ld&crash=%s",this->appKey.c_str(),CountlyDeviceInfoModel::getDeviceId(),time(NULL),crashReport.c_str());
  
  addToQueue(data);
  tick();
}

void CountlyConnectionQueue::reportCrash(string error, string reason, bool nonfatal) {
  rapidjson::StringBuffer s;
  rapidjson::Writer<rapidjson::StringBuffer> writer(s);
  
  writer.StartObject();

  writer.String("_os");
  writer.String(CountlyDeviceInfoModel::getDeviceSystemName());
  writer.String("_os_version");
  writer.String(CountlyDeviceInfoModel::getDeviceSystemVersion());
  writer.String("_device");
  writer.String(CountlyDeviceInfoModel::getDeviceId());
  writer.String("_resolution");
  writer.String(CountlyDeviceInfoModel::getDeviceResolution());
  writer.String("_app_version");
  writer.String(CountlyDeviceInfoModel::getAppVersion());
  writer.String("_name");
  writer.String(reason.c_str());
  writer.String("_nonfatal");
  writer.Bool(nonfatal);
  writer.String("_ram_current");
  writer.Double(CountlyCrashDetail::getRamCurrent());
  writer.String("_ram_total");
  writer.Double(CountlyCrashDetail::getRamTotal());
  writer.String("_disk_current");
  writer.Double(CountlyCrashDetail::getDiskCurrent());
  writer.String("_disk_total");
  writer.Double(CountlyCrashDetail::getDiskTotal());
  writer.String("_bat");
  writer.Double(CountlyCrashDetail::batteryLevel());
  writer.String("_orientation");
  writer.String(CountlyCrashDetail::orientation());
  writer.String("_online");
  writer.Bool((CountlyCrashDetail::connectionType())? 1 : 0 );
  writer.String("_opengl");
  writer.Double(CountlyCrashDetail::OpenGLESversion());
  writer.String("_root");
  writer.Bool(CountlyCrashDetail::isRooted());
  writer.String("_background");
  writer.Bool(CountlyCrashDetail::isInBackground());
  writer.String("_muted");
  writer.Bool(CountlyCrashDetail::isMuted());
  writer.String("_run");
  writer.Double(Countly::sharedInstance()->timeSinceLaunch());
  writer.String("_cpu");
  writer.String(CountlyCrashDetail::getCPUType());
  writer.String("_manufacture");
  writer.String(CountlyCrashDetail::manufacturer());
  writer.String("_error");
  writer.String(error.c_str());
  
  
  if(!crashCustom.empty()){
    writer.String("_custom");
    writer.StartObject();
    std::vector<std::string> mapKeyVec;
    mapKeyVec = crashCustom.keys();
    for(auto key : mapKeyVec)
    {
      auto value = (__String*)crashCustom.at(key);
      writer.String(key.c_str());
      writer.String(value->getCString());
    }
    writer.EndObject();
  }
  
  writer.EndObject();
  
  
  log("Crash Log >>>> %s",s.GetString());
  
  string strEncode = CountlyUtils::urlencode(s.GetString());
  
  reportCrash(strEncode);
  
//  return strEncode;

}

void CountlyConnectionQueue::recordEvents(string events) {
  __String *data = __String::createWithFormat("app_key=%s&device_id=%s&timestamp=%ld&events=%s",this->appKey.c_str(),CountlyDeviceInfoModel::getDeviceId(),time(NULL),events.c_str());
  
  addToQueue(data);
  tick();
}

void CountlyConnectionQueue::tick() {
  if (!dataQueue.size() || bgTask) {
    return;
  }
  
  bgTask = true;
  __String *request = dataQueue.at(0);
  httpRequestUrl(request);
}

void CountlyConnectionQueue::httpRequestUrl(__String *url, bool isImmidiate) {
  
  __String *urlStr = __String::createWithFormat("%s/i?%s",this->appHost.c_str(),url->getCString());
  HttpRequest* request = new (std::nothrow) HttpRequest();
  request->setUrl(urlStr->getCString());
  request->setRequestType(HttpRequest::Type::GET);
  std::vector<std::string> headers;
  headers.push_back("Content-Type: application/json; charset=utf-8");
  request->setHeaders(headers);
  
//  // write the post data
//  const char* postData = "visitor=cocos2d&TestSuite=Extensions Test/NetworkTest";
//  request->setRequestData(postData, strlen(postData));
  request->setTag("POST");
  if(isImmidiate) {
    
    HttpClient::getInstance()->sendImmediate(request);
  }
  else {
    request->setResponseCallback(CC_CALLBACK_2(CountlyConnectionQueue::onHttpRequestCompleted, this));
    HttpClient::getInstance()->send(request);
  }
  request->release();

}

void CountlyConnectionQueue::onHttpRequestCompleted(HttpClient *sender, HttpResponse *response)
{
  
  if (!response)
  {
    return;
  }
//
  // You can get original request type from: response->request->reqType
  if (0 != strlen(response->getHttpRequest()->getTag()))
  {
    log("%s completed", response->getHttpRequest()->getTag());
  }
  
  long statusCode = response->getResponseCode();
  
  if (statusCode < 200 || statusCode > 300) {
    return;
  }
  
  char statusString[64] = {};
  sprintf(statusString, "HTTP Status Code: %ld, tag = %s", statusCode, response->getHttpRequest()->getTag());
  log("response code: %ld", statusCode);
  
  if (!response->isSucceed())
  {
    log("response failed");
    log("error buffer: %s", response->getErrorBuffer());
    return;
  }
  
  // dump data
  std::vector<char> *buffer = response->getResponseData();
  log("Http Test, dump data: ");
  for (unsigned int i = 0; i < buffer->size(); i++)
  {
    log("%c", (*buffer)[i]);
  }
  log("\n");
  if (response->getHttpRequest()->getReferenceCount() != 2)
  {
    log("request ref count not 2, is %d", response->getHttpRequest()->getReferenceCount());
  }
  
  dataQueue.erase(0);
  bgTask = false;
  tick();
}


