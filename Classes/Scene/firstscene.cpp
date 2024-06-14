#include"BattleScene.h"
#include"firstscene.h"
#include"ui/CocosGUI.h"
#include"cocostudio/CocoStudio.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;

Scene* firstscene::createScene()
{
	auto scene = Scene::create();
	auto layer = firstscene::create();
	scene->addChild(layer);		
	return scene;			
}

bool firstscene::init()
{
	if (!Layer::init())
	{
		return false;
	}
	Size VisibleSize = Director::getInstance()->getVisibleSize();

//	auto label = Label::createWithSystemFont("Click to start the Game!", "fonts/arial.ttf", 30);
//	auto menuitem = MenuItemLabel::create(label, CC_CALLBACK_1(firstscene::EnterSecondScene, this));
    //auto menu = Menu::create(menuitem, NULL);
//    this->addChild(menu);

    auto UI = GUIReader::getInstance()->widgetFromJsonFile("Firstscene_1/Firstscene_1.json");
    UI->setPosition(Vec2(VisibleSize.width /10 , VisibleSize.height / 10));
    auto button = (Button *)UI->getChildByTag(2);
	addChild(UI);
	button->addTouchEventListener(CC_CALLBACK_1(firstscene::EnterSecondScene, this));
    
	return true;
}

void firstscene::EnterSecondScene(Ref* pSender)
{
	Director::getInstance()->replaceScene(BattleScene::createScene());
}