#include "MenuScene.h"
#include "GameScene.h"
#include "HighScoreScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* MenuScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = MenuScene::create();

    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MenuScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    auto windowsSize = Director::getInstance()->getWinSize();

    
    _spr_background = Sprite::create("background.png");
    _spr_background->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    _spr_background->setScale(visibleSize.width / _spr_background->getContentSize().width,
                             visibleSize.height / _spr_background->getContentSize().height);
    this->addChild(_spr_background);
    
    auto nameLabel = Label::createWithTTF("Mouse Escape", "fonts/Marker Felt.ttf", 24);
    nameLabel->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - nameLabel->getContentSize().height));
    nameLabel->enableShadow();
    nameLabel->setTextColor(Color4B::RED);
    this->addChild(nameLabel, 1);
    
    auto menu_item_1 = MenuItemFont::create("Play", CC_CALLBACK_1(MenuScene::play, this));
    auto menu_item_2 = MenuItemFont::create("HighScore", CC_CALLBACK_1(MenuScene::highscores, this));
    menu_item_1->setColor(Color3B::BLACK);
    menu_item_1->setFontSize(16);
    menu_item_2->setColor(Color3B::BLACK);
    menu_item_2->setFontSize(16);

    
    auto menu = Menu::create(menu_item_1, menu_item_2, NULL);
    menu->alignItemsVertically();
    this->addChild(menu);
    
    auto bg_sound = CocosDenshion::SimpleAudioEngine::getInstance();
    bg_sound->playBackgroundMusic("sounds/background_song.mp3", true);
    
    return true;
}

void MenuScene::play(cocos2d::Ref *pSender)
{

    auto scene = GameScene::createScene();
    
    Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene, Color3B(0,255,255)));
}

void MenuScene::highscores(cocos2d::Ref *psender)
{
    auto scene = HighScore::createScene();
    
    Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene, Color3B(0,255,255)));
}


