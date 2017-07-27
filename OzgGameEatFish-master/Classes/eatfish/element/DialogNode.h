
#ifndef __OzgGameEatFish__DialogNode__
#define __OzgGameEatFish__DialogNode__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"

namespace eatfish
{
    namespace element
    {
        class DialogNode;
    }
}

class eatfish::element::DialogNode : public cocos2d::Node
{
    
private:

	cocos2d::CallFuncN* m_btn1Callback;
	cocos2d::CallFuncN* m_btn2Callback;

    void onButton(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType eventType);
    
	void showActEnd();

public:
    
    enum class ChildTag
    {
        BTN_1 = 10,
		BTN_2 = 11,
		LAB_TITLE = 12,
		LAB_CONTENT = 13
    };
    
    virtual ~DialogNode();
	virtual bool init(std::string title, std::string content, std::string btn1Text, cocos2d::CallFuncN* btn1Callback);
	virtual bool init(std::string title, std::string content, std::string btn1Text, cocos2d::CallFuncN* btn1Callback, std::string btn2Text, cocos2d::CallFuncN* btn2Callback);
    
	static DialogNode* create(std::string title, std::string content, std::string btn1Text, cocos2d::CallFuncN* btn1Callback);
	static DialogNode* create(std::string title, std::string content, std::string btn1Text, cocos2d::CallFuncN* btn1Callback, std::string btn2Text, cocos2d::CallFuncN* btn2Callback);
    
	void setBtn1Text(std::string text);
	void setBtn2Text(std::string text);
	void setLabTitleText(std::string text);
	void setLabTitleContent(std::string text);

	void setBtn1Callback(cocos2d::CallFuncN* callback);
	void setBtn2Callback(cocos2d::CallFuncN* callback);

};

#endif /* defined(__OzgGameEatFish__DialogNode__) */
