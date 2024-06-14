//
// Created by 蔡忱儒 on 2024/6/11.
//

#ifndef RIPANDTEAR_IMPDARK_H
#define RIPANDTEAR_IMPDARK_H

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"
#include "./BaseObject.h"
//#include "prop/Bullet.h"

USING_NS_CC;
using namespace cocos2d;
using namespace cocostudio;
using namespace cocos2d::ui;
using namespace CocosDenshion;

class BattleScene;
class ImpDark : public BaseObject{
public:
    ImpDark();
    ~ImpDark();
    static ImpDark* create(Vec2 position,BattleScene* battleScene);

    bool init(Vec2 position,BattleScene* battleScene);
    void update(float dt);

    void move(Vec2 direction);  //(1,0)右 (0,1)上 (-1,0)左 (0,-1)下 (0,0)停止
    void attack(Vec2 direction);              //toward 0左 1右 攻击

    void fireBullet(const Vec2& direction, const std::string& imagePath ,float speed ,float damage ,float scale);
    void DropItem();

    void closeAttack();
    void hurt(int damage);
    void die();

    int ImpWalkId = 0;

    bool isDead = false;
private:
    Vec2 targetPosition; // 用于记录目标位置
};


#endif //RIPANDTEAR_IMPDARK_H
