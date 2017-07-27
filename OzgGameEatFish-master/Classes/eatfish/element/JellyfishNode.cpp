
#include "JellyfishNode.h"
#include "AnimData.h"
#include "GameConfig.h"

USING_NS_CC;
using namespace eatfish::element;

JellyfishNode::~JellyfishNode()
{

	CCLOG("JellyfishNode释放");
}

bool JellyfishNode::init()
{
	if (BaseFishNode::init())
	{
		this->m_animSpriteList = JELLYFISH;
		this->m_animKey = GAME_CONFIG_ANIMKEY_JELLY_FISH;
		this->m_moveTimeElapsed = 0;

		Sprite *jellyfish = Sprite::createWithSpriteFrameName(this->m_animSpriteList.at(0).c_str());
		jellyfish->setAnchorPoint(Vec2::ZERO);
		jellyfish->setPosition(Vec2::ZERO);
		jellyfish->setTag((int)BaseFishNode::ChildTag::MAIN_OBJ);
		this->addChild(jellyfish);

		this->setAnchorPoint(Vec2(0.5, 0.5));
		this->setContentSize(jellyfish->getContentSize());

		Node *center = Node::create();
		center->setAnchorPoint(Vec2(0.5, 0.5));
		center->setPosition(Vec2(this->getContentSize().width / 2, 110));
		center->setTag((int)ChildTag::CENTER_POINT);
		center->setContentSize(Size(56, 64));
		this->addChild(center);

		//test
		/*Sprite *test = Sprite::create("test-32.png");
		test->setPosition(Vec2(this->getContentSize().width / 2, 110));
		test->setScaleX(1.75);
		test->setScaleY(2);
		this->addChild(test);*/

		this->playAnim();
		this->scheduleUpdate();
		return true;
	}
	return false;
}
