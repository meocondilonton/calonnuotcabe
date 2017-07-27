#ifndef __OzgGameEatFish__PlayerFishNode__
#define __OzgGameEatFish__PlayerFishNode__

#include "BaseFishNode.h"
#include "EnemyFishNode.h"
#include "ItemNode.h"

namespace eatfish
{
	namespace element
	{
		class PlayerFishNode;
	}
}

class eatfish::element::PlayerFishNode : public eatfish::element::BaseFishNode
{

private:
	void scoreEffectMoveEnd(cocos2d::Node* sender);

public:

	//大中小的状态
	enum class Status
	{
		SMALL = 1,
		NORMAL = 2,
		BIG = 3
	};
		
	enum class ChildTag
	{
		WATER = 10,
		FLOWER = 11
	};

	Status m_status;
	
	virtual ~PlayerFishNode();
	virtual bool init();

	CREATE_FUNC(PlayerFishNode);

	void changeStatus(Status status);

	void invincible();
	void invincibleCallback(float delay);

	void invincible2();
	void invincible2Callback1(float delay);
	void invincible2Callback2(float delay);

	virtual void pause();
	virtual void resume();
	
	virtual void cump(eatfish::element::EnemyFishNode::EnemyFishType type);
	virtual void cump(eatfish::element::ItemNode::ItemNodeType type);
};

#endif
