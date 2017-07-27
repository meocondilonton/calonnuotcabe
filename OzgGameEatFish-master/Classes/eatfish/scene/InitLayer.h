#ifndef __OzgGameEatFish__InitLayer__
#define __OzgGameEatFish__InitLayer__

#include "BaseLayer.h"

namespace eatfish
{
	namespace scene
	{
		class InitLayer;
	}
}
class eatfish::scene::InitLayer : public eatfish::scene::BaseLayer
{

private:
	void goToStartLayer(float delta);

public:
	virtual ~InitLayer();
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(InitLayer);

};

#endif
