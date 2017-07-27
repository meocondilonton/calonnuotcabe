
#include "DialogNode.h"
#include "GameConfig.h"

USING_NS_CC;
using namespace ui;
using namespace eatfish::element;

DialogNode::~DialogNode()
{
	if (this->m_btn1Callback)
	{
		this->m_btn1Callback->release();
		this->m_btn1Callback = NULL;
	}	

	if (this->m_btn2Callback)
	{		
		this->m_btn2Callback->release();
		this->m_btn2Callback = NULL;
	}	
    
	Director::getInstance()->getTextureCache()->removeTextureForKey("dialog_bg.png");
    CCLOG("DialogNode释放");
}

bool DialogNode::init(std::string title, std::string content, std::string btn1Text, cocos2d::CallFuncN* btn1Callback)
{
	return this->init(title, content, btn1Text, btn1Callback, "", NULL);
}

bool DialogNode::init(std::string title, std::string content, std::string btn1Text, cocos2d::CallFuncN* btn1Callback, std::string btn2Text, cocos2d::CallFuncN* btn2Callback)
{
    if(Node::init())
    {
		this->m_btn1Callback = NULL;
		this->m_btn2Callback = NULL;

		if (btn1Callback)
		{			
			this->m_btn1Callback = btn1Callback;
			this->m_btn1Callback->retain();
		}
		
		if (btn2Callback)
		{			
			this->m_btn2Callback = btn2Callback;
			this->m_btn2Callback->retain();
		}
		
        this->setScale(0);
        
        Sprite *bgSprite = Sprite::create("dialog_bg.png");        
        this->addChild(bgSprite);
		this->setContentSize(bgSprite->getContentSize());
		this->setAnchorPoint(Vec2(0.5, 0.5));
		bgSprite->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height / 2));

		Label *labTitle = Label::createWithSystemFont(title.c_str(), GAME_CONFIG_GLOBAL_FONTNAME_01, 32);
		labTitle->setPosition(Vec2(this->getContentSize().width / 2, 170));
		labTitle->setTag((int)ChildTag::LAB_TITLE);
		this->addChild(labTitle);

        Label *labContent = Label::createWithSystemFont(content, GAME_CONFIG_GLOBAL_FONTNAME_01, 25);
        labContent->setDimensions(300, 70);
        labContent->setHorizontalAlignment(TextHAlignment::LEFT);
        labContent->setVerticalAlignment(TextVAlignment::TOP);
		labContent->setPosition(Vec2(this->getContentSize().width / 2, 115));
		labContent->setTag((int)ChildTag::LAB_CONTENT);
		this->addChild(labContent);

		if (btn2Callback)
		{
			//2个按钮

			Button *btn1 = Button::create();
			btn1->loadTextureNormal("dialog_btn_yes_1.png");
			btn1->loadTexturePressed("dialog_btn_yes_2.png");
			btn1->setPosition(Vec2(this->getContentSize().width / 2 - 90, 45));
			btn1->setEnabled(false);
			btn1->addTouchEventListener(CC_CALLBACK_2(DialogNode::onButton, this));
			btn1->setTag((int)ChildTag::BTN_1);
			btn1->setTitleFontName(GAME_CONFIG_GLOBAL_FONTNAME_01);
			btn1->setTitleFontSize(32.0f);
			btn1->setTitleText(btn1Text.c_str());
			btn1->setScale(0.75);
			this->addChild(btn1);

			Button *btn2 = Button::create();
			btn2->loadTextureNormal("dialog_btn_no_1.png");
			btn2->loadTexturePressed("dialog_btn_no_2.png");
			btn2->setPosition(Vec2(this->getContentSize().width / 2 + 90, 45));
			btn2->setEnabled(false);
			btn2->addTouchEventListener(CC_CALLBACK_2(DialogNode::onButton, this));
			btn2->setTag((int)ChildTag::BTN_2);
			btn2->setTitleFontName(GAME_CONFIG_GLOBAL_FONTNAME_01);
			btn2->setTitleFontSize(32.0f);
			btn2->setTitleText(btn2Text.c_str());
			btn2->setScale(0.75);
			this->addChild(btn2);
		}
		else
		{
			//1个按钮

			Button *btn1 = Button::create();
			btn1->loadTextureNormal("dialog_btn_yes_1.png");
			btn1->loadTexturePressed("dialog_btn_yes_2.png");
			btn1->setPosition(Vec2(this->getContentSize().width / 2, 45));
			btn1->setEnabled(false);
			btn1->addTouchEventListener(CC_CALLBACK_2(DialogNode::onButton, this));
			btn1->setTag((int)ChildTag::BTN_1);
			btn1->setTitleFontName(GAME_CONFIG_GLOBAL_FONTNAME_01);
			btn1->setTitleFontSize(32.0f);
			btn1->setTitleText(btn1Text.c_str());
			btn1->setScale(0.75);
			this->addChild(btn1);
		}
		
        this->runAction(Sequence::createWithTwoActions(EaseElasticOut::create(ScaleTo::create(1, 1)), CallFunc::create(CC_CALLBACK_0(DialogNode::showActEnd, this))));
        
        return true;
    }
    return false;
}

DialogNode* DialogNode::create(std::string title, std::string content, std::string btn1Text, cocos2d::CallFuncN* btn1Callback)
{
	DialogNode *obj = new DialogNode();
	if (obj && obj->init(title, content, btn1Text, btn1Callback))
	{
		obj->autorelease();
		return obj;
	}
	CC_SAFE_DELETE(obj);
	return NULL;
}

DialogNode* DialogNode::create(std::string title, std::string content, std::string btn1Text, cocos2d::CallFuncN* btn1Callback, std::string btn2Text, cocos2d::CallFuncN* btn2Callback)
{
	DialogNode *obj = new DialogNode();
	if (obj && obj->init(title, content, btn1Text, btn1Callback, btn2Text, btn2Callback))
	{
		obj->autorelease();
		return obj;
	}
	CC_SAFE_DELETE(obj);
	return NULL;
}

void DialogNode::setBtn1Text(std::string text)
{
	Button *btn1 = (Button*)this->getChildByTag((int)ChildTag::BTN_1);
	btn1->setTitleText(text);
}

void DialogNode::setBtn2Text(std::string text)
{
	Button *btn2 = (Button*)this->getChildByTag((int)ChildTag::BTN_2);
	btn2->setTitleText(text);
}

void DialogNode::setLabTitleText(std::string text)
{
	Label *labTitle = (Label*)this->getChildByTag((int)ChildTag::LAB_TITLE);
	labTitle->setString(text);
}

void DialogNode::setLabTitleContent(std::string text)
{
	Label *labContent = (Label*)this->getChildByTag((int)ChildTag::LAB_CONTENT);
	labContent->setString(text);
}

void DialogNode::setBtn1Callback(cocos2d::CallFuncN* callback)
{
	if (this->m_btn1Callback)
	{
		this->m_btn1Callback->release();
		this->m_btn1Callback = NULL;
	}

	if (callback)
	{
		this->m_btn1Callback = callback;
		this->m_btn1Callback->retain();
	}
}

void DialogNode::setBtn2Callback(cocos2d::CallFuncN* callback)
{
	if (this->m_btn2Callback)
	{
		this->m_btn2Callback->release();
		this->m_btn2Callback = NULL;
	}

	if (callback)
	{
		this->m_btn2Callback = callback;
		this->m_btn2Callback->retain();
	}
}

//private
void DialogNode::showActEnd()
{
	Button *btnClose = (Button*)this->getChildByTag((int)ChildTag::BTN_1);
	if (btnClose)
		btnClose->setEnabled(true);

	Button *btnYES = (Button*)this->getChildByTag((int)ChildTag::BTN_2);
	if (btnYES)
		btnYES->setEnabled(true);

}

//private
void DialogNode::onButton(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType eventType)
{
	Button *btn = (Button*)pSender;

	switch (eventType)
	{
		case ui::Widget::TouchEventType::BEGAN:

			break;
		case ui::Widget::TouchEventType::MOVED:

			break;
		case ui::Widget::TouchEventType::ENDED:
		{
			switch (btn->getTag())
			{
				case (int)ChildTag::BTN_2:
				{
					if (this->m_btn2Callback)
						this->runAction(this->m_btn2Callback);
					//this->removeFromParentAndCleanup(true);
				}
					break;
				default:
				{
					if (this->m_btn1Callback)
						this->runAction(this->m_btn1Callback);
                    
				}
					break;
			}

		}
			break;
		case ui::Widget::TouchEventType::CANCELED:

			break;
	}
}
