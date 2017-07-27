#ifndef __OzgGameEatFish__BaseLayer__
#define __OzgGameEatFish__BaseLayer__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"
#include "Strings.h"

namespace eatfish
{
	namespace scene
	{
		class BaseLayer;
	}
}

class eatfish::scene::BaseLayer : public cocos2d::Layer
{

protected:
	std::map<std::string, std::string> m_strings;

public:

	virtual ~BaseLayer();
	virtual bool init();

};

#endif
