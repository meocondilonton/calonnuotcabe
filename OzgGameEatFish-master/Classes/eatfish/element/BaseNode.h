#ifndef __OzgGameEatFish__BaseNode__
#define __OzgGameEatFish__BaseNode__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

namespace eatfish
{
	namespace element
	{
		class BaseNode;
	}
}

class eatfish::element::BaseNode : public cocos2d::Node
{

protected:

	std::vector<std::string> m_animSpriteList;
	std::string m_animKey;

	void playAnim();
	void playAnim(float frameDelay);
public:

	enum class ChildTag
	{
		MAIN_OBJ = 1,
		CENTER_POINT = 2
	};

	bool m_isMoving;

	virtual ~BaseNode();
	virtual bool init();

	cocos2d::Rect centerRect();

	virtual void pause();
	virtual void resume();

};

#endif
