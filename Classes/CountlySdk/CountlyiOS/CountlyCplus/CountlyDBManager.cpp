//
//  CountlyDBManager.cpp
//  CountlyX
//
//  Created by Junaid on 27/08/2015.
//
//

#include "CountlyDBManager.h"
#include "ThirdPartyLibs/Sqlite/sqlite3.h"
#include "Countly.h"
#include "CountlyConnectionQueue.h"

int loadUrls (void * para, int n_column, char ** column_value, char ** column_name);

static CountlyDBManager* instance = NULL;

CountlyDBManager* CountlyDBManager::sharedInstance() {
  if (!instance) {
    instance = new CountlyDBManager();
    
    instance->dbPath = FileUtils::getInstance()->getWritablePath();
    
    instance->dbPath.append("countly.sqlite");
  }
  return instance;
}

void CountlyDBManager::createDB() {
  sqlite3 *pDB = NULL; //for database path
  char* errMsg = NULL; //for error message
  string sqlstr; //for sql query string
  int result;
  result = sqlite3_open(dbPath.c_str(),&pDB);
  if (result != SQLITE_OK)
    log("OPENING WRONG, %d, MSG:%s",result,errMsg);
  log("result %d",result);
  
  result = sqlite3_exec(pDB, "create table urlQueue(urlKey timestamp, url varchar(32))",NULL,NULL,&errMsg);
  if(result != SQLITE_OK)
    log("CREATE TABLE FAIL %d, Msg: %s",result,errMsg);
  
  result = sqlite3_exec(pDB, "create table events(key varchar(32), count INT, sum float, time timestamp)",NULL,NULL,&errMsg);
  if(result != SQLITE_OK)
    log("CREATE TABLE FAIL %d, Msg: %s",result,errMsg);
  
  result = sqlite3_exec(pDB, "create table eventsSegmentation(evenytKey varchar(32), key varchar(32), value varchar(32))",NULL,NULL,&errMsg);
  if(result != SQLITE_OK)
    log("CREATE TABLE FAIL %d, Msg: %s",result,errMsg);

  
  sqlite3_close(pDB);//for closing connection
  populateUrls();
  populateEvents();
}

void CountlyDBManager::insertUrl(time_t ptime, string url) {
  __String *timeStr = __String::createWithFormat("%ld",ptime);
  string time =  timeStr->getCString();
  
  sqlite3 *pDB = NULL; //for database path
  char* errMsg = NULL; //for error message
  string sqlstr; //for sql query string
  int result;
  result = sqlite3_open(dbPath.c_str(),&pDB);
  if (result != SQLITE_OK)
    log("OPENING WRONG, %d, MSG:%s",result,errMsg);
  log("result %d",result);
  
  sqlstr="insert into urlQueue values('"+time+"','"+url+"')";
  result=sqlite3_exec(pDB,sqlstr.c_str(),NULL,NULL,NULL);
  if(result!=SQLITE_OK)
    log("URL insert data failed!");
  
  
  sqlite3_close(pDB);//for closing connection
  
}

void CountlyDBManager::removeUrl(time_t ptime) {
  __String *timeStr = __String::createWithFormat("%ld",ptime);
  string time =  timeStr->getCString();
  
  sqlite3 *pDB = NULL; //for database path
  char* errMsg = NULL; //for error message
  string sqlstr; //for sql query string
  int result;
  result = sqlite3_open(dbPath.c_str(),&pDB);
  if (result != SQLITE_OK)
    log("OPENING WRONG, %d, MSG:%s",result,errMsg);
    log("result %d",result);
  
  sqlstr = "delete from urlQueue where urlKey = '"+time+"'";
  result=sqlite3_exec(pDB,sqlstr.c_str(),NULL,NULL,NULL);
  if(result!=SQLITE_OK)
    log("URL remove data failed!");
  
  
  sqlite3_close(pDB);//for closing connection
  
}

void CountlyDBManager::populateUrls() {
  
  sqlite3 *pDB = NULL; //for database path
  char* errMsg = NULL; //for error message
  string sqlstr; //for sql query string
  int result;
  result = sqlite3_open(dbPath.c_str(),&pDB);
  if (result != SQLITE_OK)
    log("OPENING WRONG, %d, MSG:%s",result,errMsg);
  log("result %d",result);
  
  
  sqlstr = "select * from urlQueue";
  
  sqlite3_stmt *ppStmt1;
  result=sqlite3_prepare_v2(pDB,sqlstr.c_str() , -1, &ppStmt1, NULL);
  
  if(result!=SQLITE_OK)
    log("URL's select data failed!");
  
  __String *time;
  __String *url;
  for (;;) {
    result = sqlite3_step(ppStmt1);
    
    if (result == SQLITE_DONE)
      break;
    if (result != SQLITE_ROW) {
      printf("error: %s!\n", sqlite3_errmsg(pDB));
      break;
    }
    time = __String::create((const char*)sqlite3_column_text(ppStmt1, 0));
    url = __String::create((const char*)sqlite3_column_text(ppStmt1, 1));
    
    CountlyConnectionQueue::sharedInstance()->addConnection(url, time->doubleValue());
  }
  
  
  
  sqlite3_close(pDB);//for closing connection
  
}

int loadUrls (void * para, int n_column, char ** column_value, char ** column_name) {
  log ("time = %s, url = %s", column_value [0], column_value [1]);
  __String *time = __String::createWithFormat("%s",column_value [0]);
  __String *url = __String::createWithFormat("%s",column_value [1]);
  CountlyConnectionQueue::sharedInstance()->addConnection(url, time->doubleValue());
  return 0;
}

void CountlyDBManager::populateEvents() {
    sqlite3 *pDB = NULL; //for database path
    char* errMsg = NULL; //for error message
    string sqlstr; //for sql query string
    int result;
    result = sqlite3_open(dbPath.c_str(),&pDB);
    if (result != SQLITE_OK)
      log("OPENING WRONG, %d, MSG:%s",result,errMsg);
    log("result %d",result);
    
    
    sqlstr = "select * from events";
    //  result=sqlite3_exec(pDB,sqlstr.c_str(),NULL,NULL,NULL);
    
    sqlite3_stmt *ppStmt1;
    result=sqlite3_prepare_v2(pDB,sqlstr.c_str() , -1, &ppStmt1, NULL);
  
  
  if(result!=SQLITE_OK)
    log("Event select data failed!");
  
    string evenykey;
    __String *count;
    __String *sum;
    __String *time;
    for (;;) {
      result = sqlite3_step(ppStmt1);
      
      if (result == SQLITE_DONE)
        break;
      if (result != SQLITE_ROW) {
        printf("error: %s!\n", sqlite3_errmsg(pDB));
        break;
      }
      evenykey = (const char*)sqlite3_column_text(ppStmt1, 0);
      count = __String::create((const char*)sqlite3_column_text(ppStmt1, 1));
      sum = __String::create((const char*)sqlite3_column_text(ppStmt1, 2));
      time = __String::create((const char*)sqlite3_column_text(ppStmt1, 3));
      
      sqlstr = "select * from eventsSegmentation where evenytKey = '"+evenykey+"'";
      //  result=sqlite3_exec(pDB,sqlstr.c_str(),NULL,NULL,NULL);
      
      sqlite3_stmt *ppStmt1;
      result=sqlite3_prepare_v2(pDB,sqlstr.c_str() , -1, &ppStmt1, NULL);
      
      
      if(result!=SQLITE_OK)
        log("Event Seg select data failed!");
      
      Map<string, __String*> pSegmentation;
      string key;
      __String *value;
      for (;;) {
        result = sqlite3_step(ppStmt1);
        
        if (result == SQLITE_DONE)
          break;
        if (result != SQLITE_ROW) {
          printf("error: %s!\n", sqlite3_errmsg(pDB));
          break;
        }
        
        
        key = (const char*)sqlite3_column_text(ppStmt1, 1);
        value = __String::create((const char*)sqlite3_column_text(ppStmt1, 2));
        
        pSegmentation.insert(key, value);
        
      }
      
      
      Countly::sharedInstance()->addDBEvent(evenykey, pSegmentation, sum->floatValue(), time->doubleValue(), count->intValue());
      
    }
    
    sqlite3_close(pDB);//for closing connection
  
}

void CountlyDBManager::insertEvent(CountlyEvent* event) {
//  __String *timeStr = __String::createWithFormat("%ld",ptime);
//  string time =  timeStr->getCString();
  
  
//  result = sqlite3_exec(pDB, "create table events(key varchar(32), count INT, sum float, time timestamp)",NULL,NULL,&errMsg);
  __String *str =  __String::createWithFormat("insert into events values('%s','%d','%f','%ld')",event->getKey().c_str(),event->getCount(),event->getSum(),event->getTime());
  
  sqlite3 *pDB = NULL; //for database path
  char* errMsg = NULL; //for error message
  string sqlstr; //for sql query string
  int result;
  result = sqlite3_open(dbPath.c_str(),&pDB);
  if (result != SQLITE_OK)
    log("OPENING WRONG, %d, MSG:%s",result,errMsg);
  log("result %d",result);
  
  sqlstr= str->getCString();
  result=sqlite3_exec(pDB,sqlstr.c_str(),NULL,NULL,NULL);
  if(result!=SQLITE_OK)
    log("Event insert data failed!");
  Map<string, __String*> pSegmentation = event->getSegmentation();
  if(!pSegmentation.empty()){
    //    'tablename' ('column1', 'column2') VALUES
    __String *str =  __String::createWithFormat("insert into 'eventsSegmentation' values");
    std::vector<std::string> mapKeyVec;
    mapKeyVec = pSegmentation.keys();
    int count = (int)mapKeyVec.size();
    int i = 0;
    for(auto key : mapKeyVec)
    {
      i++;
      auto value = (__String*)pSegmentation.at(key);
      str->appendWithFormat("('%s','%s','%s')",event->getKey().c_str(),key.c_str(),value->getCString());
      if(i == count) {
        str->append(";");
      }
      else {
        str->append(",");
      }
    }
    sqlstr= str->getCString();
    result=sqlite3_exec(pDB,sqlstr.c_str(),NULL,NULL,NULL);
    if(result!=SQLITE_OK)
      log("Event Segmentation insert data failed!");
  }
  
  sqlite3_close(pDB);//for closing connection
  
}

void CountlyDBManager::updateEvent(CountlyEvent* event) {
  
  
//  (key varchar(32), count INT, sum float, time timestamp)
  __String *str =  __String::createWithFormat("update events set count = '%d', sum = '%f', time = '%ld' where key = '%s'",event->getCount(),event->getSum(),event->getTime(),event->getKey().c_str());
  
  
  sqlite3 *pDB = NULL; //for database path
  char* errMsg = NULL; //for error message
  string sqlstr; //for sql query string
  int result;
  result = sqlite3_open(dbPath.c_str(),&pDB);
  if (result != SQLITE_OK)
    log("OPENING WRONG, %d, MSG:%s",result,errMsg);
  log("result %d",result);
  
  sqlstr = str->getCString();
  result=sqlite3_exec(pDB,sqlstr.c_str(),NULL,NULL,NULL);
  if(result!=SQLITE_OK)
    log("Event update data failed!");
  
  
  sqlite3_close(pDB);//for closing connection
  
}


void CountlyDBManager::removeAllEvents() {
//  __String *timeStr = __String::createWithFormat("%ld",ptime);
//  string time =  timeStr->getCString();
  
  sqlite3 *pDB = NULL; //for database path
  char* errMsg = NULL; //for error message
  string sqlstr; //for sql query string
  int result;
  result = sqlite3_open(dbPath.c_str(),&pDB);
  if (result != SQLITE_OK)
    log("OPENING WRONG, %d, MSG:%s",result,errMsg);
  log("result %d",result);
  
  sqlstr = "delete from events";
  result=sqlite3_exec(pDB,sqlstr.c_str(),NULL,NULL,NULL);
  if(result!=SQLITE_OK)
    log("Event remove data failed!");
  
  sqlstr = "delete from eventsSegmentation";
  result=sqlite3_exec(pDB,sqlstr.c_str(),NULL,NULL,NULL);
  if(result!=SQLITE_OK)
    log("Event Segmentation remove data failed!");
  
  
  sqlite3_close(pDB);//for closing connection
  
}

