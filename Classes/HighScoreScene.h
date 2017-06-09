#ifndef __HIGHSCORE_SCENE_H__
#define __HIGHSCORE_SCENE_H__

#include "cocos2d.h"

class HighScore : public cocos2d::Layer
{
private:
    
    void getHighScore();
    
public:
    
    cocos2d::Sprite* _spr_background;
    
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void goToMenu(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HighScore);
};

#endif // __HIGHSCORE_SCENE_H__
