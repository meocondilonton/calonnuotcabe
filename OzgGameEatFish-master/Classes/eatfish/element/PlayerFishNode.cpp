
#include "PlayerFishNode.h"
#include "AnimData.h"
#include "GameConfig.h"

USING_NS_CC;
using namespace eatfish::element;
using namespace CocosDenshion;

PlayerFishNode::~PlayerFishNode()
{

	CCLOG("PlayerFishNode释放");
}

bool PlayerFishNode::init()
{
	if (BaseFishNode::init())
	{
		this->m_status = Status::SMALL;
		this->m_animSpriteList = PLAYER_FISH;
		this->m_effectStatus = EffectStatus::NORMAL;
		this->m_isMoving = false;

		Sprite *fish = Sprite::createWithSpriteFrameName(this->m_animSpriteList.at(0).c_str());
		fish->setAnchorPoint(Vec2::ZERO);
		fish->setPosition(Vec2::ZERO);
		fish->setTag((int)BaseFishNode::ChildTag::MAIN_OBJ);
		this->addChild(fish);

		this->setAnchorPoint(Vec2(0.5, 0.5));
		this->setContentSize(fish->getContentSize());

		Node *center = Node::create();
		center->setAnchorPoint(Vec2(0.5, 0.5));
		center->setPosition(Vec2(this->getContentSize().width / 2, 21));
		center->setTag((int)BaseFishNode::ChildTag::CENTER_POINT);
		center->setContentSize(Size(16, 16));
		this->addChild(center);

		//test
		/*Sprite *test = Sprite::create("test-32.png");
		test->setPosition(Vec2(this->getContentSize().width / 2, 21));
		test->setScale(0.5);
		test->setContentSize(Size(16, 16));
		test->setTag(9999);
		this->addChild(test);*/

		this->changeStatus(this->m_status);
		return true;
	}
	return false;
}

void PlayerFishNode::changeStatus(Status status)
{
	this->m_status = status;

	Sprite *water = (Sprite*)this->getChildByTag((int)ChildTag::WATER);
	Node *center = this->getChildByTag((int)BaseFishNode::ChildTag::CENTER_POINT);

	//Sprite *test = (Sprite*)this->getChildByTag(9999);

	switch (this->m_status)
	{
		case Status::NORMAL:
			this->m_animSpriteList = PLAYER_M_FISH;
			this->m_animKey = GAME_CONFIG_ANIMKEY_PLAYER_M_FISH;

			if (water)
				water->setScale(10.0);

			center->setPosition(Vec2(56, 40));
			center->setContentSize(Size(56, 56));

			//test->setPosition(Vec2(56, 40));
			//test->setContentSize(Size(56, 56));
			//test->setScale(1.75);

			break;
		case Status::BIG:
			this->m_animSpriteList = PLAYER_B_FISH;
			this->m_animKey = GAME_CONFIG_ANIMKEY_PLAYER_B_FISH;

			if (water)
				water->setScale(15.0);

			center->setPosition(Vec2(120, 96));
			center->setContentSize(Size(96, 96));

			//test->setPosition(Vec2(120, 96));
			//test->setContentSize(Size(96, 96));
			//test->setScale(3);

			break;
		default:
			this->m_animSpriteList = PLAYER_FISH;
			this->m_animKey = GAME_CONFIG_ANIMKEY_PLAYER_FISH;

			if (water)
				water->setScale(5.0);

			center->setPosition(Vec2(28, 21));
			center->setContentSize(Size(16, 16));

			//test->setPosition(Vec2(28, 21));
			//test->setContentSize(Size(16, 16));
			//test->setScale(0.5);

			break;
	}

	this->playAnim();

}

void PlayerFishNode::invincible()
{
	this->m_effectStatus = EffectStatus::INVINCIBLE;

	//水泡
	Sprite *water = Sprite::createWithSpriteFrameName("water1.png");
	water->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height / 2));
	water->setScale(5.0);
	water->setTag((int)ChildTag::WATER);
	this->addChild(water);

	//自动取消无敌时间
	this->scheduleOnce(schedule_selector(PlayerFishNode::invincibleCallback), GAME_CONFIG_PLAYER_INVINCIBLE);
}

void PlayerFishNode::invincibleCallback(float delay)
{
	this->m_effectStatus = EffectStatus::NORMAL;

	Sprite *water = (Sprite*)this->getChildByTag((int)ChildTag::WATER);
	if (water)
		water->removeFromParentAndCleanup(true);

}

void PlayerFishNode::invincible2()
{
	this->m_effectStatus = EffectStatus::INVINCIBLE;

	//水泡
	Sprite *water = Sprite::createWithSpriteFrameName("water1.png");
	water->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height / 2));
	water->setScale(5.0);
	water->setTag((int)ChildTag::WATER);
	this->addChild(water);

	//跟随的粒子效果
	ParticleSystemQuad *flower = ParticleSystemQuad::create("particle_sys_flower.plist");
	flower->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height / 2));
	flower->setTag((int)ChildTag::FLOWER);
	this->addChild(flower);

	//自动取消无敌时间
	this->scheduleOnce(schedule_selector(PlayerFishNode::invincible2Callback1), GAME_CONFIG_PLAYER_INVINCIBLE2 - 3.0);
	this->scheduleOnce(schedule_selector(PlayerFishNode::invincible2Callback2), GAME_CONFIG_PLAYER_INVINCIBLE2);
}

void PlayerFishNode::invincible2Callback1(float delay)
{
	Sprite *water = (Sprite*)this->getChildByTag((int)ChildTag::WATER);
	Blink *blink = Blink::create(3.0, 5);
	if (water)
		water->runAction(blink);
}

void PlayerFishNode::invincible2Callback2(float delay)
{
	this->m_effectStatus = EffectStatus::NORMAL;

	Sprite *water = (Sprite*)this->getChildByTag((int)ChildTag::WATER);
	if (water)
		water->removeFromParentAndCleanup(true);

	ParticleSystemQuad *flower = (ParticleSystemQuad*)this->getChildByTag((int)ChildTag::FLOWER);
	if (flower)
	{
		flower->stopSystem();
		flower->removeFromParentAndCleanup(true);
	}
}

void PlayerFishNode::pause()
{
	if (this->getChildByTag((int)ChildTag::WATER))
		this->getChildByTag((int)ChildTag::WATER)->pause();

	if (this->getChildByTag((int)ChildTag::FLOWER))
		this->getChildByTag((int)ChildTag::FLOWER)->pause();

	BaseFishNode::pause();
}

void PlayerFishNode::resume()
{
	if (this->getChildByTag((int)ChildTag::WATER))
		this->getChildByTag((int)ChildTag::WATER)->resume();

	if (this->getChildByTag((int)ChildTag::FLOWER))
		this->getChildByTag((int)ChildTag::FLOWER)->resume();

	BaseFishNode::resume();
}

void PlayerFishNode::cump(EnemyFishNode::EnemyFishType type)
{	
	if (CCRANDOM_0_1() <= 0.2)
		SimpleAudioEngine::getInstance()->playEffect("audios_eatfish2.mp3");
	else
		SimpleAudioEngine::getInstance()->playEffect("audios_eatfish1.mp3");

	Label *scoreEffect = NULL;
	switch (type)
	{
		case EnemyFishNode::EnemyFishType::Fish2:
            scoreEffect = Label::createWithSystemFont(StringUtils::format("+%i", GAME_CONFIG_SCORE_FISH2), GAME_CONFIG_GLOBAL_FONTNAME_01, 24);
			break;
		case EnemyFishNode::EnemyFishType::Fish3:
			scoreEffect = Label::createWithSystemFont(StringUtils::format("+%i", GAME_CONFIG_SCORE_FISH3), GAME_CONFIG_GLOBAL_FONTNAME_01, 24);
			break;
		case EnemyFishNode::EnemyFishType::Fish4:
			scoreEffect = Label::createWithSystemFont(StringUtils::format("+%i", GAME_CONFIG_SCORE_FISH4), GAME_CONFIG_GLOBAL_FONTNAME_01, 24);
			break;
		default:
			scoreEffect = Label::createWithSystemFont(StringUtils::format("+%i", GAME_CONFIG_SCORE_FISH1), GAME_CONFIG_GLOBAL_FONTNAME_01, 24);
			break;
	}
    scoreEffect->setDimensions(this->getContentSize().width, 30);
    scoreEffect->setHorizontalAlignment(TextHAlignment::CENTER);
    scoreEffect->setVerticalAlignment(TextVAlignment::CENTER);
	scoreEffect->setColor(Color3B(255, 255, 0));
	scoreEffect->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height));
	this->addChild(scoreEffect);
	scoreEffect->runAction(Sequence::createWithTwoActions(MoveBy::create(1.0, Vec2(0, 20)), CallFuncN::create(CC_CALLBACK_1(PlayerFishNode::scoreEffectMoveEnd, this))));

	BaseFishNode::cump();
}

void PlayerFishNode::cump(ItemNode::ItemNodeType type)
{
	SimpleAudioEngine::getInstance()->playEffect("audios_eatgold.mp3");

	Label *scoreEffect = NULL;
	switch (type)
	{
        default:
            //金币
            scoreEffect = Label::createWithSystemFont(StringUtils::format("+%i", GAME_CONFIG_SCORE_ITEM_GOLD), GAME_CONFIG_GLOBAL_FONTNAME_01, 24);
            break;
	}
    scoreEffect->setDimensions(this->getContentSize().width, 30);
    scoreEffect->setHorizontalAlignment(TextHAlignment::CENTER);
    scoreEffect->setVerticalAlignment(TextVAlignment::CENTER);
    
	scoreEffect->setColor(Color3B(50, 220, 50));
	scoreEffect->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height));
	this->addChild(scoreEffect);
	scoreEffect->runAction(Sequence::createWithTwoActions(MoveBy::create(1.0, Vec2(0, 20)), CallFuncN::create(CC_CALLBACK_1(PlayerFishNode::scoreEffectMoveEnd, this))));

}

//private
void PlayerFishNode::scoreEffectMoveEnd(cocos2d::Node* sender)
{
	sender->removeFromParentAndCleanup(true);

}
