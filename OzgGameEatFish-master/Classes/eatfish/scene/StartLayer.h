#ifndef __OzgGameEatFish__StartLayer__
#define __OzgGameEatFish__StartLayer__

#include <iostream>
#include "BaseLayer.h"

namespace eatfish
{
	namespace scene
	{
		class StartLayer;
	}
}

class eatfish::scene::StartLayer : public eatfish::scene::BaseLayer
{

private:
	void onButton(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType eventType);

	void mainVisible(bool visible);
	void helpVisible(bool visible);

public:

	enum class ChildTag
	{
		BG = 99,
		TITLE = 100,
		BTN_HELP = 101,
		BTN_START = 102,
		HELP = 103,
        HELP_TITLE = 104,
        HELP_LAB1 = 105,
        HELP_LAB2 = 106,
        HELP_LAB3 = 107,
        HELP_BTN_BACK = 108
	};

	virtual ~StartLayer();
	virtual bool init();

	CREATE_FUNC(StartLayer);
	static cocos2d::Scene* createScene();
	
};

#endif
