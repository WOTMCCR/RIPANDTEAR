#include "BattleScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

bool isAttackKeyPressed = false;

Scene* BattleScene::createScene()
{
    auto scene = Scene::create();
    auto layer = BattleScene::create();
    scene->addChild(layer);
    return scene;
}

bool BattleScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    // Set the initial scene to black
    auto blackLayer = LayerColor::create(Color4B(0, 0, 0, 255));
    this->addChild(blackLayer, 10, "blackLayer");

    // Play background music
//    CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("back.mp3");
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("demon-chant.mp3", true);
//    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("back2.mp3", true);


    // Display text sequence
    this->displayTextSequence();


    //���ص�ͼ
    loadMap(MAP_TMX_FILE_PATH);

    //Ԥ������Ч
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("Minislayer/shotgun.mp3");
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("Minislayer/walk.mp3");
    
    visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // ��������
    m_speed = 5;
    m_direction = Vec2(0, 0);

    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(BattleScene::onKeyPressed, this);
    listener->onKeyReleased = CC_CALLBACK_2(BattleScene::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    /*
     * �˵����
     */
    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create("CloseNormal.png","CloseSelected.png",
                                           CC_CALLBACK_1(BattleScene::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    auto restartItem = MenuItemImage::create("restart.jpg","restart.jpg",CC_CALLBACK_1(BattleScene::menuRestartCallback,this));
    restartItem->setPosition(Vec2(origin.x + visibleSize.width - restartItem->getContentSize().width / 2 - 100,
                                  origin.y + restartItem->getContentSize().height / 2));
    restartItem->setScale(0.5);
    auto menu2 = Menu::create(restartItem,NULL);
    menu2->setPosition(Vec2::ZERO);
    this->addChild(menu2,2);

    //�Ʒְ�
    if(scoreboard == nullptr)
    {
        CCLOG("scoreboard is null");
    }
    else
    {
        scoreboard->setPosition(Vec2(origin.x + visibleSize.width - scoreboard->getContentSize().width,
                                     origin.y + visibleSize.height - scoreboard->getContentSize().height));
        scoreboard->setName("scoreboard");
        this->addChild(scoreboard, 1);
    }

    //doomѪ����ʾ
    if(doomBlood == nullptr)
    {
        CCLOG("doomBlood is null");
    }
    else
    {
        doomBlood->setPosition(Vec2(origin.x + doomBlood->getContentSize().width + 285,
                                    doomBlood->getContentSize().height/3));
        doomBlood->setName("doomBlood");
        doomBlood->setColor(Color3B(255, 0, 0));
        this->addChild(doomBlood, 1);
    }

    //doom��ҩ��ʾ
    if(doomAmmo == nullptr)
    {
        CCLOG("doomAmmo is null");
    }
    else
    {
        doomAmmo->setPosition(Vec2(origin.x + doomAmmo->getContentSize().width+585,
                                   doomBlood->getContentSize().height/3));
        doomAmmo->setName("doomAmmo");
        //��ɫ
        doomAmmo->setColor(Color3B(255,69,19));
        this->addChild(doomAmmo, 1);
    }

    /*
     * �������
     */
    //doom
    doom = DoomSlayer::create(Point(visibleSize.width/2, visibleSize.height/2) , this);
    this->addChild(doom, 5);


    /*
     * ��ʱ�����
     */
    //��ʱ����
    //������ɵ���

    this->scheduleOnce([this](float) {

        // Remove the black layer
//        this->removeChildByName("blackLayer");
        AddShambler(0.5f);
        AddImpDark(0.5f);
        CacoClassic(0.5f);
        AddShambler(0.5f);
        AddImpDark(0.5f);
        CacoClassic(0.5f);
        AddImpDark(0);
        // ����һ��1��6֮������������
        float randomIntervalImpDark = 3.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX/(6.0f-1.0f)));
        this->schedule(schedule_selector(BattleScene::AddImpDark), randomIntervalImpDark);

        float randomIntervalCacoClassic = 3.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX/(6.0f-1.0f)));
        this->schedule(schedule_selector(BattleScene::CacoClassic), randomIntervalCacoClassic);

        float randomIntervalAddShambler = 3.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX/(6.0f-1.0f)));
        this->schedule(schedule_selector(BattleScene::AddShambler), randomIntervalAddShambler);
    }, 38.0f, "startGame");

//	this->scheduleUpdate();

    return true;
}
void BattleScene::AddImpDark(float dt) {
    //���λ��
    auto pos = Point(random(0, 1000), random(100, 550));
    auto imp = ImpDark::create(pos, this);
    this->addChild(imp, 4);
    this->DemonVector.pushBack(imp);
}
//
void BattleScene::AddShambler(float dt){
    auto pos = Point(random(100, 800), random(100, 550));
    auto Shambler = Shambler::create(pos, this);
    this->addChild(Shambler, 4);
    this->DemonVector.pushBack(Shambler);
}

void BattleScene::CacoClassic(float dt){
    auto pos = Point(random(0, 1000), random(100, 550));
    auto CacoClassic = CacoClassic::create(pos, this);
    this->addChild(CacoClassic, 5);
    this->DemonVector.pushBack(CacoClassic);
}



void BattleScene::removeDemon(cocos2d::Sprite *demon) {
    this->removeChild(demon);
    this->DemonVector.eraseObject(demon);
}


//update ����������Ϸ״̬
void BattleScene::update(float dt){

    //ʤ���߼�
    if(score < 0)
    {
        gameover();
    }
    if(score>=300)
    {
        success();
    }
    if(doom == nullptr){
        CCLOG("doom is dead");
        gameover();
    }

	// ���ƽ�ɫ�ƶ�
    //��ɫλ��
    // 50 * 35 ��
    //1000 * 700 ����
    //��������
    //С��
    if (isKeyPressed(EventKeyboard::KeyCode::KEY_W)) {
        m_direction.y = 1;
    }
    if (isKeyPressed(EventKeyboard::KeyCode::KEY_S)) {
        m_direction.y = -1;
    }
    if (isKeyPressed(EventKeyboard::KeyCode::KEY_A)) {
        m_direction.x = -1;
    }
    if (isKeyPressed(EventKeyboard::KeyCode::KEY_D)) {
        m_direction.x = 1;
    }
    if (!isKeyPressed(EventKeyboard::KeyCode::KEY_W) && !isKeyPressed(EventKeyboard::KeyCode::KEY_S)) {
        m_direction.y = 0;
    }
    if (!isKeyPressed(EventKeyboard::KeyCode::KEY_A) && !isKeyPressed(EventKeyboard::KeyCode::KEY_D)) {
        m_direction.x = 0;
    }
    //shift���
    if(doom != nullptr) {
        if (isKeyPressed(EventKeyboard::KeyCode::KEY_SHIFT)) {
            doom->sprint();
        }
        doom->move(m_direction);
    }
    if(doom == nullptr){
        CCLOG("doom is dead");
        gameover();
    }
}

/*
 * ��Ϸ���鲿��
 */
//ʧ��
//void BattleScene::gameover()
//{
//    CCLOG("gameover");
//}
void BattleScene::gameover()
{
    // Add a black background layer
    auto blackLayer = LayerColor::create(Color4B(0, 0, 0, 255));
    this->addChild(blackLayer, 10, "blackLayer");

    //You Die
    Label* label = Label::createWithTTF("You Die", "fonts/Marker Felt.ttf", 48);
    label->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + 130));
    label->setTextColor(Color4B::WHITE);
    blackLayer->addChild(label, 1);

    // Add the game over image
    auto gameOverSprite = Sprite::create("doomdifficulty.png");
    gameOverSprite->setScale(0.5);
    gameOverSprite->setPosition(visibleSize.width / 2, visibleSize.height / 2);
    blackLayer->addChild(gameOverSprite);

    // Create the restart menu item
    auto restartItem = MenuItemImage::create("restart.png", "restart.png", CC_CALLBACK_1(BattleScene::menuRestartCallback, this));
    restartItem->setPosition(Vec2(gameOverSprite->getPositionX()   , gameOverSprite->getPositionY() - 180));
    restartItem->setScale(0.2);

    // Create the exit menu item
    auto exitItem = MenuItemImage::create("CloseNormal.png","CloseSelected.png", CC_CALLBACK_1(BattleScene::menuCloseCallback, this));
    exitItem->setPosition(Vec2(gameOverSprite->getPositionX()  , gameOverSprite->getPositionY() - 230));
    exitItem->setScale(1.5);

    // Create a menu and add the items to it
    auto menu = Menu::create(restartItem, exitItem, nullptr);
    menu->setPosition(Vec2::ZERO);
    blackLayer->addChild(menu, 1);

    CCLOG("gameover");
}

//ʤ��
void BattleScene::success()
{
    CCLOG("success");
}

//���ص�ͼ
void BattleScene::loadMap(std::string imagePath)
{
    // ����ͼƬ��Ϊ����
    auto backgroundSprite = Sprite::create(imagePath);
    if (backgroundSprite != NULL)
    {
        // ��ȡ��Ļ��С
        Size visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();

        // ���þ����С����Ӧ��Ļ
        backgroundSprite->setContentSize(visibleSize);

        backgroundSprite->setName("background");

        // �����������Ļ�м�
        backgroundSprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

        // ��Ӿ��鵽������
        this->addChild(backgroundSprite, 1);
    }
    else
    {
        CCLOG("load image failed!");
        return;
    }
}

/*
 * ���̿���
 */
//�������
void BattleScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event) {
    keys[keyCode] = true;
    if (keyCode == EventKeyboard::KeyCode::KEY_J) {

        if (doom!=nullptr && !isAttackKeyPressed && !doom->isAttacking()) {
            SimpleAudioEngine::getInstance()->playEffect("Minislayer/shotgun.mp3");
            doom->attack();
            isAttackKeyPressed = true;
        }
    }
}

void BattleScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event) {
    keys[keyCode] = false;
    if (keyCode == EventKeyboard::KeyCode::KEY_J) {
        isAttackKeyPressed = false;
    }
}

bool BattleScene::isKeyPressed(EventKeyboard::KeyCode keyCode) {
    if (keys.find(keyCode) != keys.end())
        return keys[keyCode];
    return false;
}


//���潻��
//restart and close
void BattleScene::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
    MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void BattleScene::menuRestartCallback(cocos2d::Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
    MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif
    //score =0;
    Director::getInstance()->replaceScene(BattleScene::createScene());
    Director::getInstance()->resume();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}


void BattleScene::displayTextSequence()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    // Display an initial image for 5 seconds
    auto initialImage = Sprite::create("startFace.png");
    initialImage->setScale(1);
    initialImage->setPosition(Vec2(origin.x + visibleSize.width / 2,
                                   origin.y + visibleSize.height / 2));
    this->addChild(initialImage, 10);

    auto initialImageDelay = DelayTime::create(5.0f);
    auto removeInitialImage = RemoveSelf::create();
    //����ʧ
    auto FadeImageOut = FadeOut::create(1.0f);
    auto initialSequence = Sequence::create(initialImageDelay,FadeImageOut, removeInitialImage, nullptr);
    initialImage->runAction(initialSequence);

    // Define the text sequence as pairs of strings
    std::vector<std::pair<std::string, std::string>> textSequence = {
            {"TEAM 10 REAP AND TEAR", ""},
            {"Against all the evil that Hell can conjure.", "��Ե��������ٻ���һ��а��"},
            {"all the wickedness that mankind can produce.", "�������������ȫ�����"},
            {"we will send unto them...only you.", "��������ȥ�Կ����ǵ�...Ψ����һ��"},
            {"Rip and tear, until it is done!", "�����ƶǣ�ֱ���������"}
    };

    float delay = 5.0f;  // Start after the initial image
    float displayTime = 5.0f; // Time each text stays on the screen before fading out

    for (const auto& textPair : textSequence)
    {
        // Create the label for the English text
        auto labelEng = Label::createWithTTF(textPair.first, "fonts/Marker Felt.ttf", 48);
        labelEng->setPosition(Vec2(origin.x + visibleSize.width / 2,
                                   origin.y + visibleSize.height / 2 ));
        labelEng->setTextColor(Color4B::WHITE);
        labelEng->setOpacity(0);  // Initially invisible
        labelEng->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER); // Center align the text
        this->addChild(labelEng, 11);

//        // Create the label for the Chinese text
//        auto labelChi = Label::createWithTTF(textPair.second, "fonts/Marker Felt.ttf", 24);
//        labelChi->setPosition(Vec2(origin.x + visibleSize.width / 2,
//                                   origin.y + visibleSize.height / 2 - 30));
//        labelChi->setTextColor(Color4B::WHITE);
//        labelChi->setOpacity(0);  // Initially invisible
//        labelChi->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER); // Center align the text
//        this->addChild(labelChi, 11);

        // Define the fade in, wait, fade out, and remove actions for the English text
        auto fadeInEng = FadeIn::create(1.0f);
        auto waitEng = DelayTime::create(displayTime);
        auto fadeOutEng = FadeOut::create(1.0f);
        auto removeLabelEng = RemoveSelf::create();

        auto delayActionEng = DelayTime::create(delay);
        auto sequenceEng = Sequence::create(delayActionEng, fadeInEng, waitEng, fadeOutEng, removeLabelEng, nullptr);

        labelEng->runAction(sequenceEng);

        // Define the fade in, wait, fade out, and remove actions for the Chinese text
//        auto fadeInChi = FadeIn::create(1.0f);
//        auto waitChi = DelayTime::create(displayTime);
//        auto fadeOutChi = FadeOut::create(1.0f);
//        auto removeLabelChi = RemoveSelf::create();
//
//        auto delayActionChi = DelayTime::create(delay);
//        auto sequenceChi = Sequence::create(delayActionChi, fadeInChi, waitChi, fadeOutChi, removeLabelChi, nullptr);
//
//        labelChi->runAction(sequenceChi);

        delay += displayTime + 1.0f;  // Increase delay for next text
    }

    // Schedule the transition to the main scene after the last text
    auto transitionDelay = DelayTime::create(delay + 1.0f);
    auto transitionCallFunc = CallFunc::create([this]() {
        CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("back2.mp3", true);
        this->removeChildByName("blackLayer");
        this->scheduleUpdate();  // Resume the update loop
    });
    auto transitionSequence = Sequence::create(transitionDelay, transitionCallFunc, nullptr);

    this->runAction(transitionSequence);
}


