//
// Created by 蔡忱儒 on 2024/6/11.
//
#include "Bullet.h"
#include "./Scene/BattleScene.h"


Bullet* Bullet::create(const std::string& imagePath, const Vec2& direction, float speed, float lifeTime , Sprite *shooter ,float damage , BattleScene *battleScene) {
    Bullet* pRet = new(std::nothrow) Bullet();
    if (pRet && pRet->init(imagePath, direction, speed, lifeTime ,shooter , damage , battleScene)) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        return nullptr;
    }
}

bool Bullet::init(const std::string& imagePath, const Vec2& direction, float speed, float lifeTime , Sprite *shooter , float damage , BattleScene *battleScene) {
    if (!Sprite::initWithFile(imagePath)) {
        return false;
    }

    m_direction = direction;
    m_speed = speed;
    m_lifeTime = lifeTime;
    this->shooter = shooter;
    this->m_damage = damage;
    this->battleScene = battleScene;
    scheduleUpdate();
    return true;
}

void Bullet::update(float dt) {
    setPosition(getPosition() + m_direction * m_speed * dt);

    // Life time check
    m_lifeTime -= dt;
    if (m_lifeTime <= 0) {
        this->removeFromParent();
        return;
    }

    if (this->battleScene) {
        auto& Demons = this->battleScene->DemonVector; // 使用引用来避免复制向量

        int i = 0;
        while (i < Demons.size()) {
            auto demon = dynamic_cast<BaseObject*>(Demons.at(i)); // 将demon转换为BaseObject类型
            if (demon && this->shooter == this->battleScene->doom) { // 检查指针有效性
                if (this->getBoundingBox().intersectsRect(demon->getBoundingBox())) {
                    CCLOG("Bullet hit demon");
                    demon->setLife(0);
                    this->battleScene->doom->setArmor(this->battleScene->doom->getArmor() + 1);
                    //设定doom的生命值
//                    int blood = this->battleScene->doom->getLife() + 20;
//                    if(blood > 200){
//                        blood = 200;
//                    }
//                    this->battleScene->doom->setLife(blood);
//                    this->battleScene->Blood = blood;
//                    this->battleScene->doomBlood->setString(std::to_string(blood));
                    Demons.eraseObject(demon);
                    continue;
                }
            }
            i++;
        }

        auto Doom = this->battleScene->doom;
        if (Doom && this->shooter != Doom) {
            if (this->getBoundingBox().intersectsRect(Doom->getBoundingBox())) {
                CCLOG("Bullet hit Doom");
                Doom->hurt(this->m_damage);
                this->removeFromParent();
            }
        }
    } else {
        CCLOG("Battle scene not initialized");
    }
}
