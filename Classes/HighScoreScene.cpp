#include "HighScoreScene.h"
#include "MenuScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* HighScore::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HighScore::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HighScore::init()
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
    this->addChild(_spr_background, 0);

    auto highscoreLabel = Label::createWithTTF("HighScore", "fonts/Marker Felt.ttf", 24);
    highscoreLabel->setPosition(Vec2(origin.x + visibleSize.width/2,
                                origin.y + visibleSize.height - highscoreLabel->getContentSize().height));
    highscoreLabel->enableShadow();
    highscoreLabel->setTextColor(Color4B::RED);
    this->addChild(highscoreLabel, 1);
    
    getHighScore();
    
    
    auto backItem = MenuItemFont::create("BACK", CC_CALLBACK_1(HighScore::goToMenu, this));
    
    backItem->setPosition(Vec2(origin.x + visibleSize.width - backItem->getContentSize().width/2 ,
                                origin.y + backItem->getContentSize().height/2));
    backItem->setFontSize(16);
    backItem->setColor(Color3B::RED);
    

    auto menu = Menu::create(backItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    
    return true;
}

void HighScore::getHighScore()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    UserDefault *def = UserDefault::getInstance( );
    
    int highScore = def->getIntegerForKey("HIGHSCORE", 0);
    
    __String *value = __String::createWithFormat( "%i", highScore - 1 );
    
    auto highScoreLabel = Label::createWithTTF(value->getCString(), "fonts/Marker Felt.ttf", 24);
    highScoreLabel->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    highScoreLabel->setTextColor(Color4B::RED);
    highScoreLabel->enableShadow();
    this->addChild(highScoreLabel, 1);

}

void HighScore::goToMenu(Ref* pSender)
{
    auto scene = MenuScene::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene, Color3B(0,255,255)));
}
