# Cocos大作业游戏开发开发文档

shenzhen university 计算机游戏开发 期末

大体实现思路，总体框架使用，游戏交互的模版进行修改

1. **游戏交互**，要看一下，按键的操作，好像有个按R重新开始
2. slayer运动使用，英雄快跑
3. 增添恶魔，套用游戏交互
4. 敌人ai，
5. 伤害，套用游戏交互





2024-06-12任务
- [x] 地图切换成自己的
- [x] 完成了wsad控制上下左右
	- [x] 边界控制
- [x] 完成doom slayer的模型交互控制
	- [x] 上下左右
		- [x] 冲刺 
		- [x] 开火
- [x] 可以随机添加恶魔

2024-06-13
感觉应该先伤害系统然后再移动
- [x] 伤害系统
- [x] 恶魔的动作，
	- [x] 恶魔可以走
	- [x] 恶魔可以攻击
- [x] 枪械音效
- [x] 背景音乐
- [x] 道具系统
	- [x] 掉落
	- [x] 恢复
2024-06-14
- [x] 做增加恶魔
- [x] 增加界面
- [x] 增加音乐

## 累次实验的用处
英雄快跑：
- 人物动作
- 地图加载
交互设计，最主要的模版
- 恶魔生成，以及死亡
- slayer控制
- 伤害系统
二维动画合成
- ai
游戏中的人工智能
- 按键交互。

## Cocos
一些使用方法，不很完善
cocos以后估计也不会再使用，所以以后也应该不会再补完
代码中有很多设计不足的地方，许多逻辑没有复用，如果再进行设计应该会更加谨慎
以下，是一部分解释，但其实看代码也很好理解和使用

如果有人想使用本项目进行期末大作业，可以方便的替换人物模型，地图，交互等，几乎可以完成一整类的2D游戏设计。

### Schedule 定时器
按一定时间执行
```C++
    //定时任务
	this->schedule(schedule_selector(BattleScene::addBall1), 1.0f);
	this->schedule(schedule_selector(BattleScene::addBall2), 2.0f);
	this->schedule(schedule_selector(BattleScene::addBall3), 3.0f);
```
开启update函数
```C++
	this->scheduleUpdate();
```


## 文档
### 创建对象
```C++
DoomSlayer* DoomSlayer::create(Vec2 position) {
    DoomSlayer* pRet = new(std::nothrow) DoomSlayer();
    if (pRet && pRet->init(position))
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
bool DoomSlayer::init(Vec2 position) {
    if (!Sprite::init()) {
        return false;
    }
    this->initWithFile("/Minislayer/stop.png");
    this->setPosition(position);
    this->setAnchorPoint(Vec2(0.5, 0.5));
    //调整大小
    this->setScale(1.5);
    this->setTag(0);
    isRunning = false;
    this->scheduleUpdate();

    return true;
}
```
### 添加动作
#### 移动
##### 参数
```C++
    Vec2 targetPosition; // 用于记录目标位置
    bool isRunning;    //用来控制动画的播放，否则会循环第一帧
```
##### 逻辑
###### 停止
使用`this->setFlippedX(false);`  镜像素材
**注意，**对于`this->initWithFile("Minislayer/stop.png");` 需要先Init再set init会覆盖
```C++
void DoomSlayer::move(Vec2 direction) {
    this->setDirection(direction);

    if (direction == Vec2::ZERO) {
        this->stopAllActionsByTag(1);
        isRunning = false;
        //加载静止帧
        if(this->getToward() == 1) {
            this->initWithFile("Minislayer/stop.png");
            this->setFlippedX(false);
        }else{
            this->initWithFile("Minislayer/stop.png");
            this->setFlippedX(true);
        }
        this->stopAllActions();
        return;
    }
```

###### 左右移动
设置朝向
```C++
    // 设置朝向
    if (direction.x > 0) {
        //水平翻转 ， 所以动作帧不用左右两个文件夹
        this->setFlippedX(false);
        this->setToward(1);
    } else if (direction.x < 0) {
        this->setFlippedX(true);
        this->setToward(0);
    }
```
设置动画
```C++
    // 读取动画帧
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
```
当没有在跑的时候才播放动画
```C++
    // 停止所有动作，然后开始动画
    if(!isRunning) {
        this->stopAllActionsByTag(1); // 停止之前的移动动画
        isRunning = true;
        this->runAction(repeatAnimate);
    }

```
更新位置
```C++
    // 移动
    targetPosition = this->getPosition() + direction * this->getSpeed();
    CCLOG("targetPosition: %f, %f", targetPosition.x, targetPosition.y);

}
```

#### 攻击
动作序列可以依次执行动作delay以及最后状态的修改。
非常有用
最后需要是nullptr
```C++
void DoomSlayer::attack() {
    CCLOG("attack");

    if (!this->isAttacking()) {
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
        
        CCLOG("Volume: %f", SimpleAudioEngine::getInstance()->getEffectsVolume());
        SimpleAudioEngine::getInstance()->playEffect("Minislayer/shotgun.mp3");
        // 创建动作序列
        auto bulletSequence = Sequence::create(fireBulletForward,delay, setAttackingFalse, nullptr);
        this->runAction(bulletSequence);
    }
}
```

子弹实体
反转可以直接设置
```C++
void DoomSlayer::fireBullet(const Vec2& direction, const std::string& imagePath) {
    auto bullet = Bullet::create(imagePath, direction, 500.0f, 2.0f);  // 子弹速度500, 生命周期2秒
    if (bullet) {
        bullet->setScale(0.5);
        bullet->setPosition(this->getPosition());
        bullet->setFlippedX(this->isFlippedX());  // 设置子弹的朝向和DoomSlayer的朝向一致
        this->getParent()->addChild(bullet,5);
    } else {
        CCLOG("Failed to create bullet");
    }
}

```
