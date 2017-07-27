
#include "BaseEnemyFishNode.h"
#include "GameLayer.h"

USING_NS_CC;
using namespace std;
using namespace eatfish::scene;
using namespace eatfish::element;

void BaseEnemyFishNode::paralysis()
{
	BaseFishNode::paralysis();
	
	this->unscheduleUpdate(); //停止update里面的计数

}

void BaseEnemyFishNode::update(float delta)
{
	this->m_moveTimeElapsed += delta;
}

//protected
void BaseEnemyFishNode::paralysisEnd(cocos2d::Node* sender)
{
	BaseFishNode::paralysisEnd(sender);
	
	//继续移动
	this->scheduleUpdate();	
	this->runAction(Sequence::createWithTwoActions(MoveTo::create(this->m_moveTime - this->m_moveTimeElapsed, this->m_moveEndPoint), CallFuncN::create(CC_CALLBACK_1(GameLayer::enemyFishMoveEnd, (GameLayer*)this->getParent()->getParent()))));
}
