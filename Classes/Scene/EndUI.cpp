#include "EndUI.h"
#include "SimpleAudioEngine.h"
#include "BattleScene.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "SelectLevel.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;

Scene* EndUI::createScene()
{
	auto scene = Scene::create();


	auto layer = EndUI::create();
	scene->addChild(layer);

	return scene;
}

bool EndUI::init()
{
	if (!Layer::init())
	{
		return false;
	}
	Size VisibleSize = Director::getInstance()->getVisibleSize();

	//创建UI菜单
	auto menu_UI = GUIReader::getInstance()->widgetFromJsonFile("EndUI/EndUI.ExportJson");
	menu_UI->setPosition(Vec2(20, 18));
	addChild(menu_UI);
	auto image_View = (ImageView*)menu_UI->getChildByTag(51);

	//重启游戏按钮
	auto btn_newgame = (Button*)image_View->getChildByTag(53);
	btn_newgame->addTouchEventListener(CC_CALLBACK_1(EndUI::EnterBattleScene, this));


	//退出游戏按钮
	auto btn_endgame = (Button*)image_View->getChildByTag(54);
	btn_endgame->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type)
	{
		//退出游戏
		Director::getInstance()->end();

	});
	return true;
}

void EndUI::EnterBattleScene(Ref* pSender)
{
	SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	Director::getInstance()->replaceScene(SelectLevel::createScene());
}

