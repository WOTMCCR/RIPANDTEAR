//
// Created by 蔡忱儒 on 2024/6/11.
//

#include "CacoClassic.h"
#include "SimpleAudioEngine.h"
#include "prop/Bullet.h"
#include "./Scene/BattleScene.h"

CacoClassic::CacoClassic() {
    this->setLife(100);
    this->setSpeed(40);
    this->setToward(1);
    this->setKind(1);
    this->setID(0);
    this->setDirection(Vec2(0, 0));
}

CacoClassic::~CacoClassic() {

}

CacoClassic *CacoClassic::create(Vec2 position, BattleScene *battleScene) {
    CacoClassic *pRet = new(std::nothrow) CacoClassic();
    if (pRet && pRet->init(position, battleScene))
    {
        pRet->autorelease();
        return pRet;
    } else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool CacoClassic::init(cocos2d::Vec2 position, BattleScene *battleScene) {
    if (!Sprite::init()) {
        return false;
    }
    this->initWithFile("Cacoclassic/stand.png");
    this->setPosition(position);
    this->setAnchorPoint(Vec2(0.5, 0.5));
    //调整大小
    this->setScale(1);
    this->setTag(1);
    this->setRunning(false);
    this->setAttacking(false);
    this->battleScene = battleScene;
    this->scheduleUpdate();
    this->setLife(100);

    this->setName("CacoClassic"); // 设置名称

    return true;
}

void CacoClassic::move(cocos2d::Vec2 direction) {
    this->setDirection(direction);

    if (direction == Vec2::ZERO) {
        this->stopAllActionsByTag(1);
        this->setRunning(false);

//        if(flyId != 0) {
//            CocosDenshion::SimpleAudioEngine::getInstance()->stopEffect(this->ImpWalkId);
//            flyId = 0;
//        }



        if (this->getToward() == 1) {
            this->initWithFile("Cacoclassic/stand.png");
            this->setFlippedX(false);
        } else {
            this->initWithFile("Cacoclassic/stand.png");
            this->setFlippedX(true);
        }

        return;
    }
//    if(flyId == 0) {
//        flyId = CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("music/fly.mp3", true);
//    }

    if (direction.x > 0) {
        this->setFlippedX(false);
        this->setToward(1);
    } else if (direction.x < 0) {
        this->setFlippedX(true);
        this->setToward(0);
    }

    Vector<SpriteFrame*> animFrames;
    char str[300] = {'\0'};
    for(int i = 1; i <= 3; i++) {
        sprintf(str, "Cacoclassic/fly/fly%d.png", i);
        auto frame = SpriteFrame::create(str, Rect(0, 0, 56, 58));
        animFrames.pushBack(frame);
        memset(str, '\0', sizeof(str));
    }

    auto animation = Animation::createWithSpriteFrames(animFrames, 0.15f);
    auto animate = Animate::create(animation);
    auto repeatAnimate = RepeatForever::create(animate);
    repeatAnimate->setTag(1);

    if(!this->isRunning() && !this->isAttacking()) {
        this->stopAllActionsByTag(1);
        this->setRunning(true);
        this->runAction(repeatAnimate);
    }
    targetPosition = this->getPosition() + direction * this->getSpeed();
}

void CacoClassic::update(float dt) {
    if(getLife() != 0){
        // 获得 Doom Slayer 的位置
        Vec2 doomslayerPosition = Vec2(0,0);
        if(this->battleScene->doom != nullptr)
            doomslayerPosition = this->battleScene->doom->getPosition();
        else{
            return;
        }

        // 计算与 Doom Slayer 的距离
        float distanceToDoom = this->getPosition().distance(doomslayerPosition);

        // 如果距离大于 200，则移动到 200 的距离内
        if (distanceToDoom > 300) {
            Vec2 directionToDoom = (doomslayerPosition - this->getPosition()).getNormalized();
            this->move(directionToDoom);
        } else {
            // 在 200 的距离内，随机移动和攻击
            if (!this->isAttacking()) {
                CCLOG("Cacoclassic attack");
                // 随机移动
                Vec2 randomOffset = Vec2(CCRANDOM_MINUS1_1() * 100, CCRANDOM_MINUS1_1() * 100); // 随机偏移量
                Vec2 targetPosition = this->getPosition() + randomOffset;
                Vec2 randomDirection = (targetPosition - this->getPosition()).getNormalized();
                this->move(randomDirection);


            }
        }
        // 向 Doom Slayer 攻击
        this->setFlippedX(doomslayerPosition.x < this->getPosition().x);
        //停止所有动作
        //一定概率攻击
        if (CCRANDOM_0_1() < 0.3) {
            Vec2 attackDirection = (doomslayerPosition - this->getPosition()).getNormalized();
            this->attack(attackDirection);
        }

        // 移动更新和边界检测
        if (this->getDirection() != Vec2::ZERO) {
            Vec2 newPosition = this->getPosition() + this->getDirection() * this->getSpeed() * dt;
            auto visibleSize = Director::getInstance()->getVisibleSize();
            if (newPosition.y >= 120 && newPosition.y <= visibleSize.height - 130 && newPosition.x >= 30 &&
                newPosition.x <= visibleSize.width - 30) {
                this->setPosition(newPosition);
            }
        }
    }else{
        if(!isDead) {
            this->DropItem();
            isDead = true;
        }
        this->runAction(Sequence::create(
                CallFunc::create([=]() {
                    this->setTexture("dead1.png");

                }),
                DelayTime::create(10.0f), // 修改为合理的延迟时间，确保动画完成
                CallFunc::create([=]() {
                    this->removeFromParentAndCleanup(true);
                }),
                nullptr
        ));
    }
}

void CacoClassic::attack(Vec2 direction) {
    if (!this->isAttacking()) {
        this->setAttacking(true);
        auto setAttackingFalse = CallFunc::create([this]() {
            this->setAttacking(false);
        });
        //动作
        auto fireBulletForward = CallFunc::create([this,direction]() {
            int toward = this->getToward() == 1 ? 1 : -1;
            this->fireBullet(direction, "Cacoclassic/shoot/purpleball.png" , 150.0f , 15.0f , 0.5);
        });

        auto delay = DelayTime::create(1.0f);

//        SimpleAudioEngine::getInstance()->playEffect("Minislayer/shotgun.mp3");
        auto bulletSequence = Sequence::create(
                CallFunc::create([=]() {
                    this->setTexture("Cacoclassic/shoot/shoot.png");
                }),
                fireBulletForward,
                CallFunc::create([=]() {
                    this->setTexture("Cacoclassic/shoot/shoot.png");
                }),
                delay,
                setAttackingFalse,
                nullptr);
        this->runAction(bulletSequence);
    }
}

void CacoClassic::fireBullet(const Vec2& direction, const std::string& imagePath , float speed ,float damage , float scale) {
    auto bullet = Bullet::create(imagePath, direction, speed, 10.0f , this , damage ,this->battleScene);
    if (bullet) {
        bullet->setScale(scale);
        bullet->setPosition(this->getPosition());
        bullet->setFlippedX(this->isFlippedX());
        this->getParent()->addChild(bullet, 5);
    } else {
        CCLOG("Failed to create bullet");
    }
}

void CacoClassic::hurt(int damage) {

}

void CacoClassic::die() {
    CCLOG("CacoClassic die");
}

void CacoClassic::DropItem() {
    // 获得 Doom Slayer 的位置
    Vec2 doomslayerPosition = Vec2(0,0);
    if(this->battleScene->doom != nullptr)
        doomslayerPosition = this->battleScene->doom->getPosition();
    else{
        return;
    }

    Vec2 ItemDirection = (doomslayerPosition - this->getPosition()).getNormalized();
    fireBullet(ItemDirection, "health.png" , 150.0f , -20.0f , 1);

    // 创建一个延迟动作
    auto delay = DelayTime::create(0.3f);

    // 创建一个调用fireBullet的动作
    auto fireBulletAction = CallFunc::create([=]() {
        fireBullet(ItemDirection, "bullet.png" , 150.0f , 0.0f , 0.5);
    });

    // 创建一个序列动作，先延迟，然后调用fireBullet
    auto sequence = Sequence::create(delay, fireBulletAction, nullptr);

    // 运行这个序列动作
    this->runAction(sequence);
}
