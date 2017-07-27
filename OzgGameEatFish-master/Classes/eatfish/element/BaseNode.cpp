
#include "BaseNode.h"

USING_NS_CC;
using namespace std;
using namespace eatfish::element;

BaseNode::~BaseNode()
{

	CCLOG("BaseNode释放");
}

bool BaseNode::init()
{
	if (Node::init())
	{

		return true;
	}
	return false;
}

Rect BaseNode::centerRect()
{
	Node *center = this->getChildByTag((int)ChildTag::CENTER_POINT);
	
	if (!center)		
		return Rect::ZERO;

	Vec2 point = center->boundingBox().origin;
	point = this->convertToWorldSpace(point);

	return Rect(point.x, point.y, center->getContentSize().width, center->getContentSize().height);
}

void BaseNode::pause()
{
	if (this->getChildByTag((int)ChildTag::MAIN_OBJ))
		this->getChildByTag((int)ChildTag::MAIN_OBJ)->pause();
	
	Node::pause();
}

void BaseNode::resume()
{
	if (this->getChildByTag((int)ChildTag::MAIN_OBJ))
		this->getChildByTag((int)ChildTag::MAIN_OBJ)->resume();
	
	Node::resume();
}

//protected
void BaseNode::playAnim()
{
	this->playAnim(0.1);
}

//protected
void BaseNode::playAnim(float frameDelay)
{
	Animation *anim = AnimationCache::getInstance()->getAnimation(this->m_animKey);

	Sprite *mainObj = (Sprite*)this->getChildByTag((int)ChildTag::MAIN_OBJ);

	if (!anim)
	{
		Vector<SpriteFrame*> frames;

		vector<string>::iterator it = this->m_animSpriteList.begin();
		while (it != this->m_animSpriteList.end())
		{
			string item = *it;
			frames.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName(item.c_str()));

			it++;
		}
		
		anim = Animation::createWithSpriteFrames(frames);
		anim->setDelayPerUnit(frameDelay);
		anim->setRestoreOriginalFrame(false);

		AnimationCache::getInstance()->addAnimation(anim, this->m_animKey);

		this->setContentSize(frames.at(0)->getOriginalSize());
	}
		
	mainObj->stopAllActions();

	RepeatForever *animate = RepeatForever::create(Animate::create(anim));
	mainObj->runAction(animate);
}
