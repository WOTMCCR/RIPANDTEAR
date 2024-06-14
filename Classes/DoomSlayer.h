//
// Created by 蔡忱儒 on 2024/6/11.
//

#ifndef RIPANDTEAR_DOOMSLAYER_H
#define RIPANDTEAR_DOOMSLAYER_H

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"
#include "BaseObject.h"
//#include "prop/Bullet.h"
//#include "Scene/BattleScene.h"

USING_NS_CC;
using namespace cocos2d;
using namespace cocostudio;
using namespace cocos2d::ui;
using namespace CocosDenshion;

class Bullet;
class BattleScene;
class DoomSlayer : public BaseObject{
public:
    DoomSlayer();
    ~DoomSlayer();
    static DoomSlayer* create(Vec2 position , BattleScene* battleScene);
    bool init(Vec2 position , BattleScene* battleScene);

    void setArmor(int armor);
    int getArmor();

    void update(float dt);

    //动作
    void move(Vec2 direction);  //(1,0)右 (0,1)上 (-1,0)左 (0,-1)下 (0,0)停止
    void attack();              //toward 0左 1右 攻击

    void fireBullet(const Vec2& direction, const std::string& imagePath);
    void hurt(int damage);
    void die();

    //技能
    void sprint();              //根据方向冲刺

    int doomwalkId = 0;
    bool IsDead = false;

private:
    int armor;
    Vec2 targetPosition; // 用于记录目标位置
    bool isInvincible;
//    bool isRunning;
//    bool isAttacking;
};



#endif //RIPANDTEAR_DOOMSLAYER_H
