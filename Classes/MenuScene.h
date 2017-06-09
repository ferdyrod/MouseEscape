#ifndef __MENU_SCENE_H__
#define __MENU_SCENE_H__

#include "cocos2d.h"

class MenuScene : public cocos2d::Layer
{
    
private:
    cocos2d::Sprite* _spr_background;
    
    
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(MenuScene);
    
    void play(cocos2d::Ref* pSender);
    void settings(cocos2d::Ref* pSender);
    void highscores(cocos2d::Ref* psender);
};

#endif // __MENU_SCENE_H__
