#ifndef __OzgGameEatFish__BaseEnemyFishNode__
#define __OzgGameEatFish__BaseEnemyFishNode__

#include "BaseFishNode.h"

namespace eatfish
{
	namespace element
	{
		class BaseEnemyFishNode;
	}
}

class eatfish::element::BaseEnemyFishNode : public eatfish::element::BaseFishNode
{

protected:
	virtual void paralysisEnd(cocos2d::Node* sender);

public:
		
	float m_moveTime;
	cocos2d::Vec2 m_moveStartPoint;
	cocos2d::Vec2 m_moveEndPoint;

	float m_moveTimeElapsed;

	virtual void paralysis();
	virtual void update(float delta);

};

#endif
