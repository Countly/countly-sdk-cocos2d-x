#include "HelloWorldScene.h"
#include "Countly.h"
#include "CountlyUtils.h"
#include "CountlyExceptionHandler.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
  
  this->testCountlyEvents();
  
    return true;
}


void HelloWorld::testCountlyEvents() {
  
  auto sp0 = __String::create("testing");
  Map<std::string, __String*> map0;
  std::string mapKey0 = "MAP_KEY_0";
  map0.insert(mapKey0, sp0);
  Map<std::string, __String*> map1;
  
  map1.insert("MAP_KEY_1", __String::create("testing"));
  map1.insert("MAP_KEY_2", __String::create("testing2"));
  
  
  Countly::sharedInstance()->recordEvent("Key_2_a", 1);
  Countly::sharedInstance()->recordEvent("Key_1_a", 3);
  Countly::sharedInstance()->recordEvent("Key_4_a", 2);
  Countly::sharedInstance()->recordEvent("Key_3_a", 5);
  
  Countly::sharedInstance()->recordEvent("Key_2_a", 1);
  Countly::sharedInstance()->recordEvent("Key_1_a", 3);
  Countly::sharedInstance()->recordEvent("Key_4_a", 2);
  Countly::sharedInstance()->recordEvent("Key_3_a", 5);
  
  Countly::sharedInstance()->recordEvent("Key_2_b", 1,2);
  Countly::sharedInstance()->recordEvent("Key_1_b", 3,4);
  Countly::sharedInstance()->recordEvent("Key_4_b", 2,6);
  Countly::sharedInstance()->recordEvent("Key_3_b", 5,8);
  
  
  Countly::sharedInstance()->recordEvent("Key_2_b", 1,2);
  Countly::sharedInstance()->recordEvent("Key_1_b", 3,4);
  Countly::sharedInstance()->recordEvent("Key_4_b", 2,6);
  Countly::sharedInstance()->recordEvent("Key_3_b", 5,8);
  
  
  Countly::sharedInstance()->recordEvent("Key_2_c", map0);
  Countly::sharedInstance()->recordEvent("Key_1_c", map1);
  Countly::sharedInstance()->recordEvent("Key_4_c", map0);
  Countly::sharedInstance()->recordEvent("Key_3_c", map1);
  
  
  Countly::sharedInstance()->recordEvent("Key_2_c", map0, 1);
  Countly::sharedInstance()->recordEvent("Key_1_c", map1, 3);
  Countly::sharedInstance()->recordEvent("Key_4_c", map0, 2);
  Countly::sharedInstance()->recordEvent("Key_3_c", map1, 5);
  
  Countly::sharedInstance()->recordEvent("Key_2_d", map0, 1.99f);
  Countly::sharedInstance()->recordEvent("Key_1_d", map1, 3.99f);
  Countly::sharedInstance()->recordEvent("Key_4_d", map0, 2.99f);
  Countly::sharedInstance()->recordEvent("Key_3_d", map1, 5.99f);
  
  Countly::sharedInstance()->recordEvent("Key_2_d", map0, 1.99f, 1);
  Countly::sharedInstance()->recordEvent("Key_1_d", map1, 3.99f, 2);
  Countly::sharedInstance()->recordEvent("Key_4_d", map0, 2.99f, 3);
  Countly::sharedInstance()->recordEvent("Key_3_d", map1, 5.99f, 4);
  
  Countly::sharedInstance()->recordEvent("Key_2_d", map0, 1.99f);
  Countly::sharedInstance()->recordEvent("Key_1_d", map1, 3.99f);
  Countly::sharedInstance()->recordEvent("Key_4_d", map0, 2.99f);
  Countly::sharedInstance()->recordEvent("Key_3_d", map1, 5.99f);
  
  }


void HelloWorld::menuCloseCallback(Ref* pSender)
{
  
  Map<std::string, __String*> data;
  data.insert(kCLYUserName, __String::create("Junaid"));
  data.insert(kCLYUserUsername, __String::create("ijunaid"));
  data.insert(kCLYUserEmail, __String::create("mjunaid.akram@yahoo.com"));
  data.insert(kCLYUserOrganization, __String::create("Countly"));
  data.insert(kCLYUserPhone, __String::create("+923349963029"));
  data.insert(kCLYUserGender, __String::create("M"));
  data.insert(kCLYUserPicture, __String::create("https://upload.wikimedia.org/wikipedia/commons/4/4f/Polistes_May_2013-2.jpg"));
//  data.insert(kCLYUserPicturePath, __String::create("ijunaid"));
  data.insert(kCLYUserBirthYear, __String::create("1987"));
  
  
  Map<std::string, __String*> custom;
  custom.insert("key1", __String::create("Value1"));
  custom.insert("key2", __String::create("Value2"));
  
  Countly::sharedInstance()->recordUserDetails(data, custom);
  
//  throw std::runtime_error("Hello, world!");(SIGABRT);
//  CountlyExceptionHandler::testCrash();
//    Director::getInstance()->end();
//
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//    exit(0);
//#endif
}
