// ҡ��

#ifndef __JOYSTICK_H__
#define __JOYSTICK_H__

#include "cocos2d.h"
USING_NS_CC;

enum Joystick_dir
{
    _BEHIND,
    _FORWARD,
	_LEFT,
	_RIGHT,
	_STOP
};

class Joystick : public Layer
{
public:
	Joystick();
	~Joystick();
    int speed = 4;
	// ����ҡ�ˣ�aPoint��ҡ������ aRadius��ҡ�˰뾶 aJsSprite��ҡ�˿��Ƶ� aJsBg��ҡ�˱���
	static Joystick* create(Vec2 aPoint ,float aRadius ,char* aJsSprite,char* aJsBg);

	// ��ȡҡ�˷���
//	Joystick_dir getDirection();
    Vec2 getDirection();
private:
	float getVelocity();   // ҡ������
	void update(float dt); 
	// ��ʼ����aPoint��ҡ������ aRadius��ҡ�˰뾶 aJsSprite��ҡ�˿��Ƶ� aJsBg��ҡ�˱���
	virtual bool init(Vec2 aPoint ,float aRadius ,char* aJsSprite,char* aJsBg);
	virtual bool onTouchBegan(Touch *pTouch, Event *pEvent);
	virtual void onTouchMoved(Touch *pTouch, Event *pEvent);
	virtual void onTouchEnded(Touch *pTouch, Event *pEvent);

private:
	Vec2 m_centerPoint;    // ҡ������
	Vec2 m_currentPoint;   // ҡ�˵�ǰλ��

	float m_radius;        // ҡ�˰뾶
	Sprite* m_jsSprite;    //ҡ�˿��Ƶ�
};

#endif
