//
//  CountlyConnectionQueue.cpp
//  CountlyX
//
//  Created by Junaid on 13/07/2015.
//
//

#include "CountlyConnectionQueue.h"
#include "CountlyDeviceInfoModel.h"

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

void CountlyConnectionQueue::httpRequestUrl(__String *url) {
  
  __String *urlStr = __String::createWithFormat("%s/i?%s",this->appHost.c_str(),url->getCString());
  HttpRequest* request = new (std::nothrow) HttpRequest();
  request->setUrl(urlStr->getCString());
  request->setRequestType(HttpRequest::Type::GET);
  std::vector<std::string> headers;
  headers.push_back("Content-Type: application/json; charset=utf-8");
  request->setHeaders(headers);
  request->setResponseCallback(CC_CALLBACK_2(CountlyConnectionQueue::onHttpRequestCompleted, this));
  
//  // write the post data
//  const char* postData = "visitor=cocos2d&TestSuite=Extensions Test/NetworkTest";
//  request->setRequestData(postData, strlen(postData));
  request->setTag("POST");
  HttpClient::getInstance()->send(request);
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


