
#include "BaseFishNode.h"

USING_NS_CC;
using namespace std;
using namespace eatfish::element;

BaseFishNode::~BaseFishNode()
{

	CCLOG("BaseFishNode释放");
}

bool BaseFishNode::init()
{
	if (Node::init())
	{
		this->m_orientation = Orientation::LEFT;

		return true;
	}
	return false;
}

void BaseFishNode::orientationLeft()
{
	this->m_orientation = Orientation::LEFT;

	Sprite *fish = (Sprite*)this->getChildByTag((int)ChildTag::MAIN_OBJ);
	fish->setFlippedX(false);
}

void BaseFishNode::orientationRight()
{
	this->m_orientation = Orientation::RIGHT;

	Sprite *fish = (Sprite*)this->getChildByTag((int)ChildTag::MAIN_OBJ);
	fish->setFlippedX(true);
}

void BaseFishNode::cump()
{
	Sprite *chumSprite = (Sprite*)this->getChildByTag((int)ChildTag::CUMP);
	if (chumSprite)
	{
		chumSprite->stopAllActions();
		chumSprite->removeFromParentAndCleanup(true);
	}

	//随机的cump精灵
	vector<string> cumpList = { "cump1.png", "cump2.png", "cump3.png", "cump4.png", "cump5.png" };
	
	int i = CCRANDOM_0_1() * (cumpList.size() - 1);
	chumSprite = Sprite::createWithSpriteFrameName(cumpList.at(i).c_str());

	//定义左边或右边的位置
	if (this->m_orientation == Orientation::LEFT)	
		chumSprite->setPosition(Vec2(-chumSprite->getContentSize().width / 2, this->getContentSize().height / 2));
	else
		chumSprite->setPosition(Vec2(this->getContentSize().width + (chumSprite->getContentSize().width / 2), this->getContentSize().height / 2));

	chumSprite->setTag((int)ChildTag::CUMP);
	this->addChild(chumSprite);

	chumSprite->runAction(Sequence::createWithTwoActions(DelayTime::create(0.2), CallFuncN::create(CC_CALLBACK_1(BaseFishNode::cumpAutoHide, this))));

}

void BaseFishNode::paralysis()
{
	if (this->m_effectStatus == EffectStatus::INVINCIBLE || this->m_effectStatus == EffectStatus::PARALYSIS)
		return;

	this->m_effectStatus = EffectStatus::PARALYSIS;
	this->stopAllActions();

	Sprite *fish = (Sprite*)this->getChildByTag((int)ChildTag::MAIN_OBJ);
	if (fish)
		fish->stopAllActions();

	MoveBy *act1 = MoveBy::create(0.01, Vec2(-3, 0));
	MoveBy *act2 = MoveBy::create(0.02, Vec2(6, 0));
	ActionInterval *act3 = act2->reverse();
	MoveBy *act4 = MoveBy::create(0.01, Vec2(3, 0));

	//麻痹5秒后恢复正常
	this->runAction(Sequence::create(act1, act2, act3, act4, DelayTime::create(5.0), CallFuncN::create(CC_CALLBACK_1(BaseFishNode::paralysisEnd, this)), NULL));
}

void BaseFishNode::pause()
{
	if (this->getChildByTag((int)ChildTag::CUMP))
		this->getChildByTag((int)ChildTag::CUMP)->pause();

	BaseNode::pause();
}

void BaseFishNode::resume()
{
	if (this->getChildByTag((int)ChildTag::CUMP))
		this->getChildByTag((int)ChildTag::CUMP)->resume();

	BaseNode::resume();
}

//protected
void BaseFishNode::cumpAutoHide(cocos2d::Node* sender)
{
	//吃了鱼之后的cump图片自动消失
	sender->stopAllActions();
	sender->removeFromParentAndCleanup(true);

}

//protected
void BaseFishNode::paralysisEnd(cocos2d::Node* sender)
{
	this->playAnim();
	this->m_effectStatus = BaseFishNode::EffectStatus::NORMAL;

}
