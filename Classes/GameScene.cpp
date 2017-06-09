#include <string.h>
#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "MenuScene.h"

USING_NS_CC;

Scene* GameScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setGravity(Vec2(0,0));
    
    // 'layer' is an autorelease object
    auto layer = GameScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
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
    
    _timerlbl = Label::createWithTTF("Timer: 0", "fonts/Marker Felt.ttf", 16);
    _timerlbl->enableBold();
    _timerlbl->setPosition(Vec2(origin.x + visibleSize.width/2,
                               origin.y + visibleSize.height - _timerlbl->getContentSize().height - 10));
    _timerlbl->setTextColor(Color4B::RED);
    this->addChild(_timerlbl, 2);

    
    _mouse = Sprite::create("raton_parado.png");
    _mouse->setPosition(windowsSize.width / 2, (windowsSize.height * 0.20));
    _mouse->setScale(2);
    setMousePhysics(_mouse);
    this->addChild(_mouse, 1);
    touchListener();
    contactListener();
    schedule(schedule_selector(GameScene::createTimeCounter), 1.0f);
    schedule(schedule_selector(GameScene::updateLogic), 3.0f);
    
    return true;
}

void GameScene::setEnemyPhysics(cocos2d::Sprite* sprite)
{
    auto body = PhysicsBody::createCircle(sprite -> getContentSize().width / 2.5);
    body-> setCollisionBitmask(ENEMY_COLLISION_BITMASK);
    body-> setContactTestBitmask(true);
    body-> setDynamic(true);
    sprite->setPhysicsBody(body);
}

void GameScene::setMousePhysics(cocos2d::Sprite* sprite)
{
    auto body = PhysicsBody::createCircle(sprite->getContentSize().width/3);
    body-> setCollisionBitmask(MOUSE_COLLISION_BITMASK);
    body -> setContactTestBitmask(true);
    body -> setDynamic(true);
    sprite->setPhysicsBody(body);
}

void GameScene::touchListener()
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener -> onTouchBegan = [] (Touch* touch, Event* event)
    {
        return true;
    };
    listener -> onTouchMoved = CC_CALLBACK_2(GameScene::movePlayer, this);
    listener -> onTouchEnded = [=] (Touch* touch, Event* event) {};
    _eventDispatcher -> addEventListenerWithSceneGraphPriority(listener, this);
}

void GameScene::contactListener()
{
    auto contactListener = EventListenerPhysicsContact::create( );
    contactListener->onContactBegin = CC_CALLBACK_1(GameScene::onContactBegin, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority( contactListener, this);
    
}

void GameScene::movePlayer(cocos2d::Touch* touch, cocos2d::Event* event)
{
    auto touchPosition = touch->getLocation();
    if(_mouse->getBoundingBox().containsPoint(touchPosition)){
        if(_mouse->getPositionX() > touchPosition.x){
            _mouse->setTexture("raton_izquierda.png");
        } else if (_mouse->getPositionX() < touchPosition.x){
            _mouse->setTexture("raton_derecha.png");
        }
        _mouse -> setPositionX(touchPosition.x);
        _mouse -> setPositionY(touchPosition.y);
    }
}

void GameScene::updateLogic(float dt)
{
    auto director = Director::getInstance();
    auto windowsSize = director->getWinSize();
    Sprite* enemy = nullptr;
    for(int i = 0 ; i < 10 ; i++)
    {
        enemy = Sprite::create("objeto_laser.png");
        enemy -> setAnchorPoint(Vec2::ZERO);
        enemy -> setPosition(CCRANDOM_0_1() * windowsSize.width , windowsSize.height);
        setEnemyPhysics(enemy);
        enemy ->getPhysicsBody()->setVelocity(Vect(0, ( (CCRANDOM_0_1() + 0.2f) * -250) ));
        _enemy.pushBack(enemy);
        this -> addChild(enemy, 1);
    }
}

void GameScene::createTimeCounter(float dt){
    
    std::string timer = "Timer: " + std::to_string(_count);
    _timerlbl->setString(timer);
    _count++;

}

bool GameScene::onContactBegin(cocos2d::PhysicsContact& contact)
{
    
    auto nodeA = contact.getShapeA()->getBody();
    auto nodeB = contact.getShapeB()->getBody();
    
    auto windowsSize = Director::getInstance()->getWinSize();
    
    
    if((MOUSE_COLLISION_BITMASK == nodeA->getCollisionBitmask() && ENEMY_COLLISION_BITMASK == nodeB->getCollisionBitmask()) || ( MOUSE_COLLISION_BITMASK == nodeB->getCollisionBitmask() && ENEMY_COLLISION_BITMASK == nodeA->getCollisionBitmask()))
    {
        auto boomSound = CocosDenshion::SimpleAudioEngine::getInstance();
        unschedule(schedule_selector(GameScene::updateLogic));
        boomSound->pauseBackgroundMusic();
        unschedule(schedule_selector(GameScene::createTimeCounter));
        Director::getInstance()->getRunningScene()->getPhysicsWorld()->setSpeed(0);
        Director::getInstance()->getEventDispatcher()->pauseEventListenersForTarget(this);
        
        
        boomSound->playEffect("sounds/boom.wav");
        
        _mouse->setTexture("raton_muerto.png");
        auto rotate = RotateTo::create(0.2f, 405);
        _mouse->runAction(rotate);
        
        
        _gameOverLbl = Label::createWithTTF("GAME OVER", "fonts/Marker Felt.ttf", 24);
        _gameOverLbl->setPosition(windowsSize.width/2, windowsSize.height);
        _gameOverLbl->enableShadow();
        _gameOverLbl->setTextColor(Color4B::RED);
        this->addChild(_gameOverLbl, 1);
        
        auto move = MoveTo::create(0.3f, Vec2(windowsSize.width/2, (windowsSize.height - 100)));
        _gameOverLbl->runAction(move);
        
        saveHighScore();
        scheduleOnce(schedule_selector(GameScene::goToMenuScene), 4.0f);
    }
    
    return true;
}
                  
void GameScene::goToMenuScene(float dt)
{
    auto scene = MenuScene::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene, Color3B(0,255,255)));
}

void GameScene::saveHighScore(){
    
    auto windowsSize = Director::getInstance()->getWinSize();

    UserDefault *def = UserDefault::getInstance( );
    
    auto highScore = def->getIntegerForKey("HIGHSCORE", 0);
    
    if (_count > highScore)
    {
        highScore = _count;
        
        auto newHighScoreLabel = Label::createWithTTF("New HighScore!!!", "fonts/Marker Felt.ttf", 24);
        newHighScoreLabel->setPosition(windowsSize.width/2, windowsSize.height/2);
        newHighScoreLabel->enableGlow(Color4B::RED);
        newHighScoreLabel->enableShadow();
        addChild(newHighScoreLabel,100);
        
        auto highScoreSound = CocosDenshion::SimpleAudioEngine::getInstance();
        highScoreSound->playEffect("sounds/highscore.wav");

        
        def->setIntegerForKey("HIGHSCORE", highScore);
    }
    def->flush( );
}

