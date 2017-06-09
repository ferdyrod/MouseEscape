#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#define MOUSE_COLLISION_BITMASK 0x000001
#define ENEMY_COLLISION_BITMASK 0x000002

#include "cocos2d.h"

class GameScene : public cocos2d::Layer
{
private:
    cocos2d::Sprite* _spr_background;
    cocos2d::Sprite* _mouse;
    cocos2d::Vector<cocos2d::Sprite*> _enemy;
    cocos2d::Label* _gameOverLbl;
    cocos2d::Label* _timerlbl;
    
    int _count;
    
    void setEnemyPhysics(cocos2d::Sprite* sprite);
    void setMousePhysics(cocos2d::Sprite* sprite);
    
    void touchListener();
    void movePlayer(cocos2d::Touch* touch, cocos2d::Event* event);
    void contactListener();
    bool onContactBegin(cocos2d::PhysicsContact& contact);
    void goToMenuScene(float dt);
    void updateLogic(float dt);
    void createTimeCounter(float dt);
    void saveHighScore();

    
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);
    
    };

#endif // __GAME_SCENE_H__
