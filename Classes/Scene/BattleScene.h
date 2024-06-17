#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
//#include "Joystick.h"
#include "Config.h"
#include "./DoomSlayer.h"
#include "./Evil/ImpDark.h"
#include "./Evil/Shambler.h"
#include "./Evil/CacoClassic.h"
//#include "EndUI.h"

USING_NS_CC;


class BattleScene : public cocos2d::Layer
{
public:
    Scene* scene;


    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene(int level);
    static BattleScene* createWithLevel(int level);
    int level = 1;

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
	void menuRestartCallback(cocos2d::Ref* pSender);

    void gameover();
    void success();
	

	void update(float dt);
    // implement the "static create()" method manually
    CREATE_FUNC(BattleScene);

    // 加载地图
    void loadMap(std::string mapPath);

    // 加载玩家
    void addDoom(Vec2 pos);

    //加载恶魔
    void AddImpDark(float dt);
    void AddShambler(float dt);
    void CacoClassic(float dt);

    //按键控制
    void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
    void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
    bool isKeyPressed(EventKeyboard::KeyCode keyCode);

    //删除恶魔
    void removeDemon(Sprite* demon);

    Vector<Sprite*> DemonVector;
    Vector<Sprite*> bulletVector;
    Vector<Sprite*> FireBallVector;
    DoomSlayer* doom;

    int score = 0;
    int Blood = 200;
    int Ammo = 20;

    Label* scoreboard = Label::createWithTTF("Score Board"+std::to_string(score), "fonts/Marker Felt.ttf", 48);
    Label* doomBlood = Label::createWithTTF(std::to_string(Blood), "fonts/Marker Felt.ttf", 45);
    Label* doomAmmo = Label::createWithTTF(std::to_string(Ammo), "fonts/Marker Felt.ttf", 45);

    void displayTextSequence();


private:
    int Battlelevel;
	Sprite* bean;
	Size visibleSize;
	//Joystick* m_joystick;
	Vector<Sprite*> ballVector;

    float m_speed; // 角色的移动速度
    Vec2 m_direction; // 角色的移动方向

    std::map<EventKeyboard::KeyCode, bool> keys; // 存储每个键的状态

};

#endif // __HELLOWORLD_SCENE_H__
