
#include "ItemNode.h"
#include "AnimData.h"
#include "GameConfig.h"

USING_NS_CC;
using namespace std;
using namespace eatfish::element;

ItemNode::~ItemNode()
{

	CCLOG("ItemNode释放");
}

bool ItemNode::init(ItemNodeType type)
{
	if (BaseNode::init())
	{
		this->m_type = type;

		switch (this->m_type)
		{
		default:
			this->m_animSpriteList = ITEM_GOLD;
			this->m_animKey = GAME_CONFIG_ANIMKEY_ITEM_GOLD;

			break;
		}

		Sprite *itemNode = Sprite::createWithSpriteFrameName(this->m_animSpriteList.at(0).c_str());
		itemNode->setAnchorPoint(Vec2::ZERO);
		itemNode->setPosition(Vec2::ZERO);
		itemNode->setTag((int)ChildTag::MAIN_OBJ);
		this->addChild(itemNode);

		this->setAnchorPoint(Vec2(0.5, 0.5));
		this->setContentSize(itemNode->getContentSize());

		Node *center = Node::create();
		center->setAnchorPoint(Vec2(0.5, 0.5));
		center->setTag((int)ChildTag::CENTER_POINT);
		switch (this->m_type)
		{
		default:

			center->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height / 2));
			center->setContentSize(itemNode->getContentSize());

			break;
		}

		this->addChild(center);

		this->playAnim(0.15);
		this->scheduleUpdate();
		return true;
	}
	return false;
}

ItemNode* ItemNode::create(ItemNodeType type)
{
	ItemNode *obj = new ItemNode();
	if (obj && obj->init(type))
	{
		obj->autorelease();
		return obj;
	}

	CC_SAFE_DELETE(obj);
	return NULL;
}
