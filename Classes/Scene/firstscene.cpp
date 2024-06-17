#include "firstscene.h"
#include "SelectLevel.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "audio/include/AudioEngine.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;
using namespace experimental;

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
	
    /*ps:初始界面关掉
    auto UI = GUIReader::getInstance()->widgetFromJsonFile("Firstscene_1/Firstscene_1.json");
    UI->setPosition(Vec2(VisibleSize.width /10 , VisibleSize.height / 10));
    auto button = (Button *)UI->getChildByTag(2);
	addChild(UI);
	button->addTouchEventListener(CC_CALLBACK_1(firstscene::EnterSecondScene, this));
    */
	AudioEngine::setVolume(AudioEngine::play2d("open.mp3", true), 1.0f);
	
	

	//创建UI菜单
	auto menu_UI = GUIReader::getInstance()->widgetFromJsonFile("InitUI/InitUI.ExportJson");
	menu_UI->setPosition(Vec2(-215, -135));
	addChild(menu_UI);
	auto image_View = (ImageView*)menu_UI->getChildByTag(2);

	//开始游戏按钮
	auto btn_newgame = (Button*)image_View->getChildByTag(4);
	btn_newgame->addTouchEventListener(CC_CALLBACK_1(firstscene::EnterSecondScene, this));
	

	//退出游戏按钮
	auto btn_endgame = (Button*)image_View->getChildByTag(5);
	btn_endgame->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type)
	{
		//退出游戏
		Director::getInstance()->end();

	});
	
	return true;
}

void firstscene::EnterSecondScene(Ref* pSender)
{
	Director::getInstance()->replaceScene(SelectLevel::createScene());
}