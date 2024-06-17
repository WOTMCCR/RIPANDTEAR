#pragma once
#include "cocos2d.h"					
using namespace cocos2d;
class SelectLevel : public Layer
{
public:
	static Scene* createScene();
	virtual bool init();
	CREATE_FUNC(SelectLevel);
	void EnterBattleScene(Ref* pSender);
    void EnterBattleScene2(Ref* pSender);
	void returnfirstscene(cocos2d::Ref* pSender);
}; 
