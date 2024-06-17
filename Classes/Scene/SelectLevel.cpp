#include "SelectLevel.h"
#include "SimpleAudioEngine.h"
#include "BattleScene.h"
#include "firstscene.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "audio/include/AudioEngine.h"
//#include "EndUI.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;
using namespace experimental;

Scene* SelectLevel::createScene()
{
	auto scene = Scene::create();


	auto layer = SelectLevel::create();
	scene->addChild(layer);

	return scene;
}

bool SelectLevel::init()
{
	
	if (!Layer::init())
	{
		return false;
	}
	Size VisibleSize = Director::getInstance()->getVisibleSize();
	
	//创建UI菜单
	auto menu_UI = GUIReader::getInstance()->widgetFromJsonFile("SelectLevel/SelectLevel.ExportJson");
	menu_UI->setPosition(Vec2(20, 15));
	addChild(menu_UI);
	auto image_View = (ImageView*)menu_UI->getChildByTag(15);

	//LEVEL1
	auto btn_level1 = (Button*)image_View->getChildByTag(16);
	btn_level1->addTouchEventListener(CC_CALLBACK_1(SelectLevel::EnterBattleScene, this));

	//LEVEL2
	auto btn_level2 = (Button*)image_View->getChildByTag(17);
	btn_level2->addTouchEventListener(CC_CALLBACK_1(SelectLevel::EnterBattleScene2, this));
	

	//BACK，返回初始界面
	auto btn_back = (Button*)image_View->getChildByTag(20);
	btn_back->addTouchEventListener(CC_CALLBACK_1(SelectLevel::returnfirstscene, this));
	return true;
	
	
}

void SelectLevel::EnterBattleScene(Ref* pSender)
{
	AudioEngine::stopAll();
	Director::getInstance()->replaceScene(BattleScene::createScene(1));

}

void SelectLevel::EnterBattleScene2(Ref* pSender)
{
    AudioEngine::stopAll();
    Director::getInstance()->replaceScene(BattleScene::createScene(2));

}


void SelectLevel::returnfirstscene(cocos2d::Ref* pSender)
{
	AudioEngine::stopAll();
	Director::getInstance()->replaceScene(firstscene::createScene());
	Director::getInstance()->resume();
}