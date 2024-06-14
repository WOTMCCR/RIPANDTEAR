//
// Created by 蔡忱儒 on 2024/6/11.
//

#ifndef RIPANDTEAR_BULLET_H
#define RIPANDTEAR_BULLET_H

#include "cocos2d.h"
USING_NS_CC;

class Doomslayer;
class BattleScene;
class Bullet : public cocos2d::Sprite {
public:
    Sprite* shooter;
    void setDamage(float damage) { m_damage = damage; }
    float getDamage() const { return m_damage; }

    void setDirection(const cocos2d::Vec2& direction) { m_direction = direction; }
    const cocos2d::Vec2& getDirection() const { return m_direction; }

    void setSpeed(float speed) { m_speed = speed; }
    float getSpeed() const { return m_speed; }

    void setLifeTime(float lifeTime) { m_lifeTime = lifeTime; }
    float getLifeTime() const { return m_lifeTime; }


    static Bullet* create(const std::string& imagePath, const Vec2& direction, float speed, float lifeTime , Sprite* shooter , float damage , BattleScene* battleScene);

    bool init(const std::string& imagePath, const Vec2& direction, float speed, float lifeTime , Sprite* shooter , float damage , BattleScene* battleScene);

    void update(float dt);

private:
    cocos2d::Vec2 m_direction;
    float m_speed;
    float m_lifeTime;
    float m_damage;
    BattleScene* battleScene;
};

#endif //RIPANDTEAR_BULLET_H
