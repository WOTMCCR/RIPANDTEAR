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

	//����UI�˵�
	auto menu_UI = GUIReader::getInstance()->widgetFromJsonFile("EndUI/EndUI.ExportJson");
	menu_UI->setPosition(Vec2(20, 18));
	addChild(menu_UI);
	auto image_View = (ImageView*)menu_UI->getChildByTag(51);

	//������Ϸ��ť
	auto btn_newgame = (Button*)image_View->getChildByTag(53);
	btn_newgame->addTouchEventListener(CC_CALLBACK_1(EndUI::EnterBattleScene, this));


	//�˳���Ϸ��ť
	auto btn_endgame = (Button*)image_View->getChildByTag(54);
	btn_endgame->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type)
	{
		//�˳���Ϸ
		Director::getInstance()->end();

	});
	return true;
}

void EndUI::EnterBattleScene(Ref* pSender)
{
	SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	Director::getInstance()->replaceScene(SelectLevel::createScene());
}

