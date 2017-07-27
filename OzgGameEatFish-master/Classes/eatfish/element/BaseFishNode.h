#ifndef __OzgGameEatFish__BaseFishNode__
#define __OzgGameEatFish__BaseFishNode__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "BaseNode.h"

namespace eatfish
{
	namespace element
	{
		class BaseFishNode;
	}
}

class eatfish::element::BaseFishNode : public eatfish::element::BaseNode
{

protected:
	
	void cumpAutoHide(cocos2d::Node* sender);
	virtual void paralysisEnd(cocos2d::Node* sender);
	
public:

	enum class ChildTag
	{
		MAIN_OBJ = 1,
		CENTER_POINT = 2,
		CUMP = 3
	};

	enum class Orientation
	{
		LEFT = 1,
		RIGHT = 2
	};

	//效果状态，例如无敌状态、通常状态、麻痹状态
	enum class EffectStatus
	{
		NORMAL = 1, //通常状态
		INVINCIBLE = 2, //无敌状态
		PARALYSIS = 3 //麻痹状态
	};

	Orientation m_orientation; //朝向
	EffectStatus m_effectStatus;

	virtual ~BaseFishNode();
	virtual bool init();
	
	void orientationRight();
	void orientationLeft();

	virtual void cump();
	virtual void paralysis();

	virtual void pause();
	virtual void resume();

};

#endif
