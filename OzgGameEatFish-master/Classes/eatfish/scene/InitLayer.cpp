#include "InitLayer.h"
#include "StartLayer.h"
#include "GameConfig.h"

USING_NS_CC;
using namespace eatfish;
using namespace eatfish::scene;
using namespace CocosDenshion;

InitLayer::~InitLayer()
{
	CCLOG("InitLayer释放");
}

bool InitLayer::init()
{
	if (BaseLayer::init())
	{
		//这个界面停掉背景音乐
		SimpleAudioEngine::getInstance()->stopBackgroundMusic();

		Sprite *logo = Sprite::create("Default.png");
		logo->setPosition(Vec2(Director::getInstance()->getWinSize().width / 2, Director::getInstance()->getWinSize().height / 2));
		logo->setRotation(-90);
		this->addChild(logo);
		this->scheduleOnce(schedule_selector(InitLayer::goToStartLayer), 3.0);
		return true;
	}
	return false;
}

Scene* InitLayer::createScene()
{
	Scene *s = Scene::create();
	InitLayer *layer = InitLayer::create();
	s->addChild(layer);
	return s;
}

//private
void InitLayer::goToStartLayer(float delta)
{
	Scene *s = StartLayer::createScene();
	TransitionFade *t = TransitionFade::create(GAME_CONFIG_TRANSITION, s);
	Director::getInstance()->replaceScene(t);
}
