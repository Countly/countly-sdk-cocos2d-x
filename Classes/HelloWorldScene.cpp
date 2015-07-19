#include "HelloWorldScene.h"
#include "Countly.h"
#include "CountlyUtils.h"

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
  
  
  Countly::sharedInstance()->recordEvent("Key_2", 1);
  Countly::sharedInstance()->recordEvent("Key_1", 3);
  Countly::sharedInstance()->recordEvent("Key_4", 2);
  Countly::sharedInstance()->recordEvent("Key_3", 5);
  
  Countly::sharedInstance()->recordEvent("Key_2", 1,2);
  Countly::sharedInstance()->recordEvent("Key_1", 3,4);
  Countly::sharedInstance()->recordEvent("Key_4", 2,6);
  Countly::sharedInstance()->recordEvent("Key_3", 5,8);
  
  Countly::sharedInstance()->recordEvent("Key_2", 1,map0);
  Countly::sharedInstance()->recordEvent("Key_1", 3,map1);
  Countly::sharedInstance()->recordEvent("Key_4", 2,map0);
  Countly::sharedInstance()->recordEvent("Key_3", 5,map1);
  
  Countly::sharedInstance()->recordEvent("Key_2", 1,2,map0);
  Countly::sharedInstance()->recordEvent("Key_1", 3,4,map1);
  Countly::sharedInstance()->recordEvent("Key_4", 2,6,map0);
  Countly::sharedInstance()->recordEvent("Key_3", 5,8,map1);
  
  
  Countly::sharedInstance()->recordEvent("Key_2", 1);
  Countly::sharedInstance()->recordEvent("Key_1", 3);
  Countly::sharedInstance()->recordEvent("Key_4", 2);
  Countly::sharedInstance()->recordEvent("Key_3", 5);
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
