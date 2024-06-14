//
// Created by 蔡忱儒 on 2024/6/11.
//

#include "DoomSlayer.h"
#include "SimpleAudioEngine.h"
#include "./Scene/BattleScene.h"
#include "prop/Bullet.h"

DoomSlayer :: DoomSlayer() {
    this->armor = 6;
    this->setLife(200);
    this->setSpeed(200);
    this->setToward(1);
    this->setKind(0);
    this->setID(0);
    this->setDirection(Vec2(0, 0));
}

DoomSlayer :: ~DoomSlayer() {

}

//create DoomSlayer
DoomSlayer* DoomSlayer::create(Vec2 position ,BattleScene* battleScene) {
    DoomSlayer* pRet = new(std::nothrow) DoomSlayer();
    if (pRet && pRet->init(position ,battleScene))
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

//init DoomSlayer
bool DoomSlayer::init(Vec2 position ,BattleScene* battleScene) {
    if (!Sprite::init()) {
        return false;
    }
    this->initWithFile("/Minislayer/stop.png");
    this->setPosition(position);
    this->setAnchorPoint(Vec2(0.5, 0.5));
    this->setName("DoomSlayer");
    //调整大小
    this->setScale(1.5);
    this->setTag(0);
    this->setRunning(false);
    this->setAttacking(false);
    this->scheduleUpdate();
    this->isInvincible = false;

    this->battleScene = battleScene;

    // 预加载音效
//    SimpleAudioEngine::getInstance()->preloadEffect("Minislayer/shotgun.mp3");

    return true;
}


void DoomSlayer::move(Vec2 direction) {
//    CCLOG("MOVE");
    // 设置方向
    if(!IsDead)
        this->setDirection(direction);

    if (direction == Vec2::ZERO) {
        this->stopAllActionsByTag(1);
        this->setRunning(false);

        if(doomwalkId != 0)
        {
            CocosDenshion::SimpleAudioEngine::getInstance()->stopEffect(this->doomwalkId);
            doomwalkId = 0;
        }

//        CCLOG("%s" , this->isAttacking() ? "true" : "false");

        if(this->getToward() == 1) {
            this->initWithFile("Minislayer/stop.png");
            this->setFlippedX(false);
        }else{
            this->initWithFile("Minislayer/stop.png");
            this->setFlippedX(true);
        }

        return;
    }
    if(doomwalkId == 0) {
        //设置音量
        CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(0.1);
        doomwalkId = CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Minislayer/walk.mp3", true);
    }

    // 设置朝向
    if (direction.x > 0) {
        //水平翻转 ， 所以动作帧不用左右两个文件夹
        this->setFlippedX(false);
        this->setToward(1);
    } else if (direction.x < 0) {
        this->setFlippedX(true);
        this->setToward(0);
    }

    // 创建动画帧
    Vector<SpriteFrame*> animFrames;
    char str[300] = {'\0'};
    for(int i = 1; i <= 6; i++) {
        sprintf(str, "Minislayer/walk/walk%d.png", i);
        //动作帧，第一个参数是图片，第二个参数是图片的大小
        auto frame = SpriteFrame::create(str, Rect(0, 0, 49, 58)); //宽  长  前两个
        animFrames.pushBack(frame);
        memset(str, '\0', sizeof(str));
    }

    // 创建动画
    auto animation = Animation::createWithSpriteFrames(animFrames, 0.15f);
    auto animate = Animate::create(animation);
    auto repeatAnimate = RepeatForever::create(animate);
    repeatAnimate->setTag(1); // 设置移动动画的标签

    // 停止所有动作，然后开始动画
    if(!this->isRunning()) {
        this->stopAllActionsByTag(1); // 停止之前的移动动画
        this->setRunning(true);
        this->runAction(repeatAnimate);
    }
    // 移动
    targetPosition = this->getPosition() + direction * this->getSpeed();
}




void DoomSlayer::update(float dt) {
    // 每帧更新位置

    if (this->getDirection() != Vec2::ZERO) {
        Vec2 newPosition = this->getPosition() + this->getDirection() * this->getSpeed() * dt;
        //边界检测
        // y >= 120 && y <=visibleSize.height - 130
        //.x >= 30 && x <= visibleSize.width - 30
        auto visibleSize = Director::getInstance()->getVisibleSize();
        if (newPosition.y >= 120 && newPosition.y <= visibleSize.height - 130 && newPosition.x >= 30 && newPosition.x <= visibleSize.width - 30) {
            this->setPosition(newPosition);
        }
    }
    
}

void DoomSlayer::attack() {

    if (!this->isAttacking() && this->getArmor() > 0) {
        this->setArmor(this->getArmor() - 1);
        this->setAttacking(true);
        // 设置攻击结束后的回调
        auto setAttackingFalse = CallFunc::create([this]() {
            this->setAttacking(false);
        });
        // 创建发射子弹动作
        auto fireBulletForward = CallFunc::create([this]() {
            int toward = this->getToward() == 1 ? 1 : -1;
            this->fireBullet(Vec2(toward, 0), "Minislayer/bullet.png");
        });
        //delay
        auto delay = DelayTime::create(1.0f);

//      CCLOG("Volume: %f", SimpleAudioEngine::getInstance()->getEffectsVolume());
        // 创建动作序列
        auto bulletSequence = Sequence::create(fireBulletForward,delay, setAttackingFalse, nullptr);
        this->runAction(bulletSequence);
    }
}

void DoomSlayer::fireBullet(const Vec2& direction, const std::string& imagePath) {
    auto bullet = Bullet::create(imagePath, direction, 500.0f, 0.3f , this , 50 , this->battleScene);  // 子弹速度500, 生命周期2秒
    if (bullet) {
        bullet->setScale(0.5);
        bullet->setPosition(this->getPosition());
        bullet->setFlippedX(this->isFlippedX());  // 设置子弹的朝向和DoomSlayer的朝向一致
        this->getParent()->addChild(bullet,5);
    } else {
        CCLOG("Failed to create bullet");
    }
}


void DoomSlayer::hurt(int damage) {
    if(this->isInvincible && damage >= 0){
        CCLOG("DoomSlayer is invincible");
        return ;
    }
    //如果伤害为0，增加弹药
    if(damage==0){
        setArmor(this->getArmor() + 1);
    }
    this->setLife(this->getLife() - damage);
    if (this->getLife() <= 0) {
        //停止update
        this->unscheduleUpdate();
        this->setLife(this->getLife() - damage);
        this->battleScene->Blood = 0;
        this->battleScene->doomBlood->setString(std::to_string(this->getLife()));
        this->IsDead = true;
        this->die();
    }
    else {
        if(this->getLife() > 200)
            this->setLife(200);
        this->battleScene->Blood = this->getLife();
        this->battleScene->doomBlood->setString(std::to_string(this->getLife()));
    }
}

void DoomSlayer::die() {
    CCLOG("DoomSlayer died");
    this->stopAllActions();
    //关闭音效
    if(doomwalkId != 0)
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->stopEffect(this->doomwalkId);
        doomwalkId = 0;
    }
    this->runAction(Sequence::create(
            CallFunc::create([=]() {
                this->setTexture("dead2.png");
            }),
//            DelayTime::create(10.0f), // 修改为合理的延迟时间，确保动画完成
            CallFunc::create([=]() {
                this->removeFromParentAndCleanup(true);
                // Assuming battleScene->doom is the pointer to this DoomSlayer
                if (this->battleScene->doom == this) {
                    this->battleScene->doom = nullptr;
                }
            }),

            nullptr
    ));
}

void DoomSlayer::sprint() {
    CCLOG("sprint");
    this->isInvincible = true;

    auto setInvincibleFalse = CallFunc::create([this]() {
        this->isInvincible = false;
    });

    auto sprintDistance = this->getDirection() * this->getSpeed()/10;
    auto visibleSize = Director::getInstance()->getVisibleSize();

    // Calculate the new position after sprint
    Vec2 newPosition = this->getPosition() + sprintDistance;

    // Check if the new position is within the visible area
    if (newPosition.y > 125 && newPosition.y < visibleSize.height - 125 && newPosition.x > 35 && newPosition.x < visibleSize.width - 35) {
        auto sprintMove = MoveBy::create(0.01f, sprintDistance);
        auto sprintSequence = Sequence::create(sprintMove, setInvincibleFalse, nullptr);
        this->runAction(sprintSequence);
    } else {
        // Adjust the sprint distance to keep the object within the visible area
        if (newPosition.y <= 130) {
            sprintDistance.y = 125 - this->getPosition().y;
        } else if (newPosition.y >= visibleSize.height - 150) {
            sprintDistance.y = visibleSize.height - 135 - this->getPosition().y;
        }
        if (newPosition.x <= 35) {
            sprintDistance.x = 35 - this->getPosition().x;
        } else if (newPosition.x >= visibleSize.width - 35) {
            sprintDistance.x = visibleSize.width - 35 - this->getPosition().x;
        }
        auto sprintMove = MoveBy::create(0.01f, sprintDistance);
        auto sprintSequence = Sequence::create(sprintMove, setInvincibleFalse, nullptr);
        this->runAction(sprintSequence);
    }

}

void DoomSlayer::setArmor(int armor) {
    if(armor > 20)
        armor = 20;
    this->armor = armor;
    this->battleScene->Ammo = armor;
    this->battleScene->doomAmmo->setString(std::to_string(armor));
}

int DoomSlayer::getArmor() {
    return this->armor;
}


