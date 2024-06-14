//
// Created by 蔡忱儒 on 2024/6/11.
//

#ifndef RIPANDTEAR_BASEOBJECT_H
#define RIPANDTEAR_BASEOBJECT_H


#pragma once

#include "cocos2d.h"
#include "Config.h"

USING_NS_CC;
using namespace cocos2d;

class BattleScene;
class BaseObject : public Sprite
{
public:
    BaseObject() {};
    virtual ~BaseObject() {};

    CREATE_FUNC(BaseObject);
    virtual bool init();

    BattleScene * battleScene;

    // set and get
    int		getLife() { return m_life; };
    float	getSpeed() { return m_speed; };
    int		getKind() { return m_kind; };
    Vec2    getDirection() { return m_direction; };
    int     getID() { return m_ID; };
    int     getToward() { return m_toward; };

    void	setLife(int l) { m_life = l; };
    void	setSpeed(float s) { m_speed = s; }; // 设置移动速度
    void    setDirection(Vec2 d) { m_direction = d; };
    void    setToward(int t) { m_toward = t; };

    void    setKind(int k) { m_kind = k; };
    void    setID(int id) { m_ID = id; };

    // 移动
    bool    isRunning() { return Running; };
    bool    isAttacking() { return Attacking; };

    void   setRunning(bool b) { Running = b; };
    void   setAttacking(bool b) { Attacking = b; };

private:
    int     m_ID;            // OBJECT的ID
    float   m_speed;         // OBJECT的移动速度
    int     m_life;          // OBJECT的生命值(或有效值)
    Vec2    m_direction;     // OBJECT移动的方向
    int     m_toward;        // OBJECT的朝向
    int     m_kind;          // OBJECT所属的类型,在不同的派生类中有不同的意思

    bool    Running;
    bool    Attacking;
};



#endif //RIPANDTEAR_BASEOBJECT_H
