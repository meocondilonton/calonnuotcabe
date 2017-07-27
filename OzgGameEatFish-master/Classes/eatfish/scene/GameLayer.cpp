
#include "GameLayer.h"
#include "StartLayer.h"
#include "EnemyFishNode.h"
#include "JellyfishNode.h"
#include "PlayerFishNode.h"
#include "DialogNode.h"
#include "GameConfig.h"
#include "Strings.h"
#include "Utility.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
using namespace CocosDenshion;
using namespace ui;
using namespace eatfish::scene;
using namespace eatfish::element;
using namespace ozgcc;

GameLayer::~GameLayer()
{
	//停止水泡的粒子系统
	ParticleSystemQuad *blisterLeft = (ParticleSystemQuad*)this->getChildByTag((int)ChildTag::BLISTER_LEFT);
	ParticleSystemQuad *blisterRight = (ParticleSystemQuad*)this->getChildByTag((int)ChildTag::BLISTER_RIGHT);
	blisterLeft->stopSystem();
	blisterRight->stopSystem();

	this->removeAllChildrenWithCleanup(true);

	Director::getInstance()->getTextureCache()->removeTextureForKey("btn2_dw.png");
	Director::getInstance()->getTextureCache()->removeTextureForKey("btn2_up.png");
	Director::getInstance()->getTextureCache()->removeTextureForKey(this->m_bg);

	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("Fishtales.plist");
	Director::getInstance()->getTextureCache()->removeTextureForKey("Fishtales.png");

	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("Fishall.plist");
	Director::getInstance()->getTextureCache()->removeTextureForKey("Fishall.png");

	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("cump.plist");
	Director::getInstance()->getTextureCache()->removeTextureForKey("cump.png");

	Director::getInstance()->getTextureCache()->removeTextureForKey("particleTexture.png");
	Director::getInstance()->getTextureCache()->removeTextureForKey("pause_dw.png");
	Director::getInstance()->getTextureCache()->removeTextureForKey("pause_up.png");
	Director::getInstance()->getTextureCache()->removeTextureForKey("progressk.png");
	
	CCLOG("GameLayer释放");
}

bool GameLayer::init()
{
	if (BaseLayer::init())
	{
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Fishtales.plist");
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Fishall.plist");
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("cump.plist");

		Size winSize = Director::getInstance()->getWinSize();

		this->m_stageNum = 1;
		this->m_score = 0;
		this->m_playerLife = GAME_CONFIG_PLAYERS;

		this->m_eatFish = 0;
		this->m_eatFishTotal = 0;
		this->m_eatFishTotalType1And2 = 0;
		this->m_eatFishTotalType3 = 0;
		this->m_eatFishTotalType4 = 0;

		this->m_stageClear = GAME_CONFIG_STAGE_CLEAR;
		this->m_playerStatusNormal = (int)((float)this->m_stageClear * 0.29 + 0.5);
		this->m_playerStatusBig = (int)((float)this->m_stageClear * 0.61 + 0.5);
				
		//背景
		Sprite *bg = this->createRandomBg();
		bg->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
		this->addChild(bg);

		//水泡
		ParticleSystemQuad *blisterLeft = ParticleSystemQuad::create("particle_sys_blister.plist");
		blisterLeft->setPosition(Vec2(winSize.width / 2 - 300, 120));
		blisterLeft->setTag((int)ChildTag::BLISTER_LEFT);
		this->addChild(blisterLeft);

		ParticleSystemQuad *blisterRight = ParticleSystemQuad::create("particle_sys_blister.plist");
		blisterRight->setPosition(Vec2(winSize.width / 2 + 300, 120));
		blisterRight->setTag((int)ChildTag::BLISTER_RIGHT);
		this->addChild(blisterRight);
		
		//所有的鱼元素都在这个Node
		Node *fishNode = Node::create();
		fishNode->setAnchorPoint(Vec2::ZERO);
		fishNode->setPosition(Vec2::ZERO);
		fishNode->setTag((int)ChildTag::FISH_NODE);
		this->addChild(fishNode);

		//右上角的部分
        Label *stageNumLab = Label::createWithSystemFont(StringUtils::format(this->m_strings["game_scene_lab_stage_num"].c_str(), this->m_stageNum), GAME_CONFIG_GLOBAL_FONTNAME_01, 30);
        stageNumLab->setDimensions(200, 40);
        stageNumLab->setHorizontalAlignment(TextHAlignment::LEFT);
        stageNumLab->setVerticalAlignment(TextVAlignment::CENTER);
		stageNumLab->setPosition(Vec2(winSize.width - 100, winSize.height - 24));
		stageNumLab->setTag((int)ChildTag::LAB_STAGE_NUM);
		stageNumLab->enableOutline(Color4B::BLACK, 2);
		this->addChild(stageNumLab);

		Label *scoreLab = Label::createWithSystemFont(StringUtils::format(this->m_strings["game_scene_lab_score"].c_str(), this->m_score), GAME_CONFIG_GLOBAL_FONTNAME_01, 30);
        scoreLab->setDimensions(200, 40);
        scoreLab->setHorizontalAlignment(TextHAlignment::LEFT);
        scoreLab->setVerticalAlignment(TextVAlignment::CENTER);
		scoreLab->setPosition(Vec2(winSize.width - 100, winSize.height - 56));
		scoreLab->setTag((int)ChildTag::LAB_SCORE);
		scoreLab->enableOutline(Color4B::BLACK, 2);
		this->addChild(scoreLab);

		//暂停按钮
		Button *btnPause = Button::create();
		btnPause->loadTextureNormal("pause_up.png");
		btnPause->loadTexturePressed("pause_dw.png");
		btnPause->setPosition(Vec2(winSize.width - 120, winSize.height - 100));
		btnPause->addTouchEventListener(CC_CALLBACK_2(GameLayer::onButton, this));
		btnPause->setTag((int)ChildTag::BTN_PAUSE);
		this->addChild(btnPause);

		//左上角的部分
		Sprite *progressBg = Sprite::createWithSpriteFrameName("progress.png");
		progressBg->setPosition(Vec2(80, 610));
		progressBg->setTag((int)ChildTag::PROGRESS_BG);
		this->addChild(progressBg);

		//关卡进度条
        ProgressTimer *progress = ProgressTimer::create(Sprite::create("progressk.png"));
        progress->setBarChangeRate(Vec2(1, 0)); //设置进度条的长度和高度开始变化的大小
        progress->setType(ProgressTimer::Type::BAR); //设置进度条为水平
        progress->setMidpoint(Vec2(0, 0));
        progress->setPosition(Vec2(80, 594));
        progress->setTag((int)ChildTag::PROGRESS);
        this->addChild(progress);

		Sprite *fishLife = Sprite::createWithSpriteFrameName("fishlife.png");
		fishLife->setPosition(Vec2(70, 550));
		fishLife->setTag((int)ChildTag::FISH_LIFE);
		this->addChild(fishLife);

        Label *fishLifeLab = Label::createWithSystemFont(StringUtils::format("%i", this->m_playerLife), GAME_CONFIG_GLOBAL_FONTNAME_01, 30);
        fishLifeLab->setDimensions(100, 40);
        fishLifeLab->setHorizontalAlignment(TextHAlignment::LEFT);
        fishLifeLab->setVerticalAlignment(TextVAlignment::CENTER);
		fishLifeLab->setPosition(Vec2(140, 540));
		fishLifeLab->setTag((int)ChildTag::LAB_FISH_LIFE);
		fishLifeLab->enableOutline(Color4B::BLACK, 2);
		this->addChild(fishLifeLab);

		this->enabledTouchEvent(false);
		
		//player
		PlayerFishNode *player = PlayerFishNode::create();
		player->setPosition(Vec2(winSize.width / 2, 800));
		player->setTag((int)ChildTag::FISH_PLAYER);
		fishNode->addChild(player, 99999);
		player->invincible();
		
		return true;
	}
	return false;
}

void GameLayer::onEnterTransitionDidFinish()
{
    this->gameStart();
}

Scene* GameLayer::createScene()
{
	Scene *s = Scene::create();
	GameLayer *layer = GameLayer::create();
	s->addChild(layer);
	return s;
}

void GameLayer::update(float delay)
{
	Size winSize = Director::getInstance()->getWinSize();
	Node *fishNode = this->getChildByTag((int)ChildTag::FISH_NODE);

	if (!fishNode)
		return;

	float offsetVal = (float)this->m_stageNum * 0.0003;

	//水母
	if (CCRANDOM_0_1() <= GAME_CONFIG_ENEMY_JELLYFISH + offsetVal)
	{
		JellyfishNode *enemyFishNode = JellyfishNode::create();
		
		float minVal = enemyFishNode->getContentSize().width / 2;
		float maxVal = winSize.width - (enemyFishNode->getContentSize().width / 2);

		float srcX = Utility::randomFloat(minVal, maxVal);

		enemyFishNode->setPosition(Vec2(srcX, -enemyFishNode->getContentSize().height / 2));
		fishNode->addChild(enemyFishNode);

		float moveTime = Utility::randomFloat(10.0, 15.0);

		enemyFishNode->runAction(Sequence::createWithTwoActions(MoveTo::create(moveTime, Vec2(srcX, winSize.height + (enemyFishNode->getContentSize().height / 2))), CallFuncN::create(CC_CALLBACK_1(GameLayer::enemyFishMoveEnd, this))));

	}

	//金币
	if (CCRANDOM_0_1() <= GAME_CONFIG_ITEM_GOLD)
	{
		ItemNode *itemNode = ItemNode::create(ItemNode::ItemNodeType::GOLD);

		float minVal = itemNode->getContentSize().width / 2;
		float maxVal = winSize.width - (itemNode->getContentSize().width / 2);

		float srcX = Utility::randomFloat(minVal, maxVal);

		itemNode->setPosition(Vec2(srcX, winSize.height + (itemNode->getContentSize().height / 2)));
		fishNode->addChild(itemNode);

		float moveTime = Utility::randomFloat(15.0, 20.0);

		itemNode->runAction(Sequence::createWithTwoActions(MoveTo::create(moveTime, Vec2(srcX, -itemNode->getContentSize().height / 2)), CallFuncN::create(CC_CALLBACK_1(GameLayer::itemMoveEnd, this))));

	}

	//fish1
	if (CCRANDOM_0_1() <= GAME_CONFIG_ENEMY_FISH1 + offsetVal)
	{
		EnemyFishNode *enemyFishNode = EnemyFishNode::create(EnemyFishNode::EnemyFishType::Fish1);
		this->enemyFishEmergence(enemyFishNode);
	}

	//fish2
	if (CCRANDOM_0_1() <= GAME_CONFIG_ENEMY_FISH2 + offsetVal)
	{
		EnemyFishNode *enemyFishNode = EnemyFishNode::create(EnemyFishNode::EnemyFishType::Fish2);
		this->enemyFishEmergence(enemyFishNode);
	}

	//fish3
	if (CCRANDOM_0_1() <= GAME_CONFIG_ENEMY_FISH3 + offsetVal)
	{
		EnemyFishNode *enemyFishNode = EnemyFishNode::create(EnemyFishNode::EnemyFishType::Fish3);
		this->enemyFishEmergence(enemyFishNode);
	}

	//fish4
	if (CCRANDOM_0_1() <= GAME_CONFIG_ENEMY_FISH4 + offsetVal)
	{
		EnemyFishNode *enemyFishNode = EnemyFishNode::create(EnemyFishNode::EnemyFishType::Fish4);
		this->enemyFishEmergence(enemyFishNode);
	}

	//fish5
	if (CCRANDOM_0_1() <= GAME_CONFIG_ENEMY_FISH5 + offsetVal)
	{
		EnemyFishNode *enemyFishNode = EnemyFishNode::create(EnemyFishNode::EnemyFishType::Fish5);
		this->enemyFishEmergence(enemyFishNode);
	}

	//fish6
	if (CCRANDOM_0_1() <= GAME_CONFIG_ENEMY_FISH6 + offsetVal)
	{
		EnemyFishNode *enemyFishNode = EnemyFishNode::create(EnemyFishNode::EnemyFishType::Fish6);
		this->enemyFishEmergence(enemyFishNode);
	}

	//以下是碰撞

	Vector<Node*> nodeList = fishNode->getChildren();

	for (int i = 0; i < nodeList.size(); i++)
	{
		Node *srcObj = nodeList.at(i);
		Rect srcCenter = ((BaseFishNode*)srcObj)->centerRect();
		for (int j = 0; j < nodeList.size(); j++)
		{
			Node *targetObj = nodeList.at(j);
			Rect targetCenter = ((BaseFishNode*)targetObj)->centerRect();

			if (srcCenter.intersectsRect(targetCenter))
			{
				if (dynamic_cast<EnemyFishNode*>(srcObj) != NULL && dynamic_cast<EnemyFishNode*>(targetObj) != NULL)
				{
					//Enemy鱼跟Enemy鱼的处理
					//大鱼吃小鱼					
					if ((int)((EnemyFishNode*)srcObj)->m_type > (int)EnemyFishNode::EnemyFishType::Fish2 && (int)((EnemyFishNode*)srcObj)->m_type > (int)((EnemyFishNode*)targetObj)->m_type)
					{
						((EnemyFishNode*)srcObj)->cump();
						targetObj->removeFromParentAndCleanup(true);
					}
				}
				else if (dynamic_cast<EnemyFishNode*>(srcObj) != NULL && dynamic_cast<JellyfishNode*>(targetObj) != NULL)
				{
					//鲨鱼不执行				
					if ((int)((EnemyFishNode*)srcObj)->m_type < (int)EnemyFishNode::EnemyFishType::Fish5)
					{
						//Enemy鱼跟水母的处理
						((EnemyFishNode*)srcObj)->paralysis();
					}
				}
				else if (dynamic_cast<PlayerFishNode*>(srcObj) != NULL)
				{
					if (dynamic_cast<JellyfishNode*>(targetObj) != NULL)
					{
						//player与水母碰撞了
						if (((PlayerFishNode*)srcObj)->m_effectStatus != PlayerFishNode::EffectStatus::INVINCIBLE)
						{
							SimpleAudioEngine::getInstance()->playEffect("audios_jellyfish.mp3");
							((PlayerFishNode*)srcObj)->paralysis();
						}
					}
					else if (dynamic_cast<ItemNode*>(targetObj) != NULL)
					{
						//player与item碰撞了

						PlayerFishNode *player = (PlayerFishNode*)srcObj;

						switch (((ItemNode*)targetObj)->m_type)
						{
						default:
							//吃了金币
							player->cump(((ItemNode*)targetObj)->m_type);
							targetObj->removeFromParentAndCleanup(true);

							//加分
							this->changeScore(((ItemNode*)targetObj)->m_type);
							break;
						}

					}
					else if (dynamic_cast<EnemyFishNode*>(targetObj) != NULL)
					{
						//player与Enemy鱼碰撞了

						bool doEat = false;
						PlayerFishNode *player = (PlayerFishNode*)srcObj;

						if (player->m_isMoving)
						{
							switch (player->m_status)
							{
								case PlayerFishNode::Status::NORMAL:
									//中的状态
									if (((EnemyFishNode*)targetObj)->m_type == EnemyFishNode::EnemyFishType::Fish1 || ((EnemyFishNode*)targetObj)->m_type == EnemyFishNode::EnemyFishType::Fish2 || ((EnemyFishNode*)targetObj)->m_type == EnemyFishNode::EnemyFishType::Fish3)
										doEat = true;

									break;
								case PlayerFishNode::Status::BIG:
									//大的状态
									if (((EnemyFishNode*)targetObj)->m_type == EnemyFishNode::EnemyFishType::Fish1 || ((EnemyFishNode*)targetObj)->m_type == EnemyFishNode::EnemyFishType::Fish2 || ((EnemyFishNode*)targetObj)->m_type == EnemyFishNode::EnemyFishType::Fish3 || ((EnemyFishNode*)targetObj)->m_type == EnemyFishNode::EnemyFishType::Fish4)
										doEat = true;

									break;
								default:
									//小的状态
									if (((EnemyFishNode*)targetObj)->m_type == EnemyFishNode::EnemyFishType::Fish1 || ((EnemyFishNode*)targetObj)->m_type == EnemyFishNode::EnemyFishType::Fish2)
										doEat = true;

									break;
							}
						}
						
						if (doEat)
						{
							//吃掉比自己小的鱼
							player->cump(((EnemyFishNode*)targetObj)->m_type);
							targetObj->removeFromParentAndCleanup(true);

							//分数
							this->changeScore(((EnemyFishNode*)targetObj)->m_type);

							//关卡进度条
							float cpProgress = (float)this->m_eatFishTotal / (float)this->m_stageClear;
							ProgressTimer *progress = (ProgressTimer*)this->getChildByTag((int)ChildTag::PROGRESS);
                            progress->setPercentage(cpProgress * 100.0f);

							if (cpProgress >= 1)
							{
								//过关

								if (this->getChildByTag((int)ChildTag::CLEAR_NODE))
									break;
								
								this->unscheduleUpdate();
								SimpleAudioEngine::getInstance()->playEffect("audios_complete.mp3");
								this->enabledTouchEvent(false);
                                
                                fishNode->removeAllChildrenWithCleanup(true);
                                
								//过关界面
                                
                                Sprite *clearBg = Sprite::createWithSpriteFrameName("completebg.png");
                                clearBg->setPosition(clearBg->getContentSize().width / 2, clearBg->getContentSize().height / 2);
                                
                                Node *clearNode = Node::create();
                                clearNode->setAnchorPoint(Vec2(0.5, 0.5));
                                clearNode->setContentSize(clearBg->getContentSize());
                                clearNode->setPosition(Vec2(Director::getInstance()->getWinSize().width / 2, Director::getInstance()->getWinSize().height / 2));
                                clearNode->setTag((int)ChildTag::CLEAR_NODE);
                                this->addChild(clearNode);
                                
                                clearNode->addChild(clearBg);
                                
                                Sprite *fishNum = Sprite::createWithSpriteFrameName("fishnum.png");
                                fishNum->setPosition(clearNode->getContentSize().width / 2, clearNode->getContentSize().height / 2);
                                clearNode->addChild(fishNum);
                                
                                Label *title = Label::createWithSystemFont(this->m_strings["clear_title"], GAME_CONFIG_GLOBAL_FONTNAME_01, 50);
                                title->setPosition(Vec2(clearNode->getContentSize().width / 2, 470));
                                clearNode->addChild(title);
                                
                                Label *gameClearLab1 = Label::createWithSystemFont(StringUtils::format("%i", this->m_eatFishTotalType1And2), GAME_CONFIG_GLOBAL_FONTNAME_01, 30); //::create(, Size(), , );
                                gameClearLab1->setDimensions(500, 40);
                                gameClearLab1->setHorizontalAlignment(TextHAlignment::LEFT);
                                gameClearLab1->setVerticalAlignment(TextVAlignment::CENTER);
                                gameClearLab1->setPosition(Vec2(730, 330));
                                clearNode->addChild(gameClearLab1);
                                
                                Label *gameClearLab2 = Label::createWithSystemFont(StringUtils::format("%i", this->m_eatFishTotalType3), GAME_CONFIG_GLOBAL_FONTNAME_01, 30);
                                gameClearLab2->setDimensions(500, 40);
                                gameClearLab2->setHorizontalAlignment(TextHAlignment::LEFT);
                                gameClearLab2->setVerticalAlignment(TextVAlignment::CENTER);
                                gameClearLab2->setPosition(Vec2(730, 255));
                                clearNode->addChild(gameClearLab2);
                                
                                Label *gameClearLab3 = Label::createWithSystemFont(StringUtils::format("%i", this->m_eatFishTotalType4), GAME_CONFIG_GLOBAL_FONTNAME_01, 30);
                                gameClearLab3->setDimensions(500, 40);
                                gameClearLab3->setHorizontalAlignment(TextHAlignment::LEFT);
                                gameClearLab3->setVerticalAlignment(TextVAlignment::CENTER);
                                gameClearLab3->setPosition(Vec2(730, 180));
                                clearNode->addChild(gameClearLab3);
                                
                                Button *btnExit = Button::create();
                                btnExit->loadTextureNormal("btn1_up.png");
                                btnExit->loadTexturePressed("btn1_dw.png");
                                btnExit->setPosition(Vec2(200, 70));
                                btnExit->addTouchEventListener(CC_CALLBACK_2(GameLayer::onButton, this));
                                btnExit->setTag((int)ChildTag::BTN_EXIT);
                                btnExit->setTitleFontName(GAME_CONFIG_GLOBAL_FONTNAME_01);
                                btnExit->setTitleFontSize(22.0f);
                                btnExit->setTitleText(this->m_strings["clear_exit"]);
                                clearNode->addChild(btnExit);
                                
                                Button *btnNext = Button::create();
                                btnNext->loadTextureNormal("btn1_up.png");
                                btnNext->loadTexturePressed("btn1_dw.png");
                                btnNext->setPosition(Vec2(clearNode->getContentSize().width - 200, 70));
                                btnNext->addTouchEventListener(CC_CALLBACK_2(GameLayer::onButton, this));
                                btnNext->setTag((int)ChildTag::BTN_NEXT);
                                btnNext->setTitleFontName(GAME_CONFIG_GLOBAL_FONTNAME_01);
                                btnNext->setTitleFontSize(22.0f);
                                btnNext->setTitleText(this->m_strings["clear_next"]);
                                clearNode->addChild(btnNext);
                                
							}

							//变大的判断
							if (player->m_status == PlayerFishNode::Status::NORMAL && this->m_eatFish >= this->m_playerStatusBig)
							{
								SimpleAudioEngine::getInstance()->playEffect("audios_growth.mp3");
								player->changeStatus(PlayerFishNode::Status::BIG);
							}
							else if (player->m_status == PlayerFishNode::Status::SMALL && this->m_eatFish >= this->m_playerStatusNormal)
							{
								SimpleAudioEngine::getInstance()->playEffect("audios_growth.mp3");
								player->changeStatus(PlayerFishNode::Status::NORMAL);
							}

						}
						else
						{
							//如果在可控制状态下，不是无敌状态的话，就会被比自己大的鱼吃了
							
							if (player->m_isMoving && ((PlayerFishNode*)srcObj)->m_effectStatus != PlayerFishNode::EffectStatus::INVINCIBLE)
							{
								((EnemyFishNode*)targetObj)->cump();
								player->removeFromParentAndCleanup(true);
								
                                this->enabledTouchEvent(false);
                                
								if (this->m_playerLife == 0)
								{
                                    this->unscheduleUpdate();
                                    
									//没有了生命值就game over
									SimpleAudioEngine::getInstance()->playEffect("audios_complete.mp3");
									this->enabledTouchEvent(false);

									fishNode->removeAllChildrenWithCleanup(true);

									//game over界面
                                    
                                    Sprite *gameoverBg = Sprite::createWithSpriteFrameName("completebg.png");
                                    gameoverBg->setPosition(gameoverBg->getContentSize().width / 2, gameoverBg->getContentSize().height / 2);
                                    
                                    Node *gameoverNode = Node::create();
                                    gameoverNode->setAnchorPoint(Vec2(0.5, 0.5));
                                    gameoverNode->setContentSize(gameoverBg->getContentSize());
                                    gameoverNode->setPosition(Vec2(Director::getInstance()->getWinSize().width / 2, Director::getInstance()->getWinSize().height / 2));
                                    gameoverNode->setTag((int)ChildTag::GAMEOVER_NODE);
                                    this->addChild(gameoverNode);
                                    
                                    gameoverNode->addChild(gameoverBg);
                                    
                                    Label *title = Label::createWithSystemFont(this->m_strings["gameover_title"], GAME_CONFIG_GLOBAL_FONTNAME_01, 50);
                                    title->setPosition(Vec2(gameoverNode->getContentSize().width / 2, 430));
                                    gameoverNode->addChild(title);
                                    
                                    Label *content = Label::createWithSystemFont(this->m_strings["gameover_content"], GAME_CONFIG_GLOBAL_FONTNAME_01, 30);
                                    content->setPosition(Vec2(gameoverNode->getContentSize().width / 2, 350));
                                    gameoverNode->addChild(content);
                                    
                                    Button *btnExit = Button::create();
                                    btnExit->loadTextureNormal("btn1_up.png");
                                    btnExit->loadTexturePressed("btn1_dw.png");
                                    btnExit->setPosition(Vec2(200, 70));
                                    btnExit->addTouchEventListener(CC_CALLBACK_2(GameLayer::onButton, this));
                                    btnExit->setTag((int)ChildTag::BTN_EXIT);
                                    btnExit->setTitleFontName(GAME_CONFIG_GLOBAL_FONTNAME_01);
                                    btnExit->setTitleFontSize(22.0f);
                                    btnExit->setTitleText(this->m_strings["gameover_exit"]);
                                    gameoverNode->addChild(btnExit);
                                    
                                    Button *btnRestart = Button::create();
                                    btnRestart->loadTextureNormal("btn1_up.png");
                                    btnRestart->loadTexturePressed("btn1_dw.png");
                                    btnRestart->setPosition(Vec2(gameoverNode->getContentSize().width - 200, 70));
                                    btnRestart->addTouchEventListener(CC_CALLBACK_2(GameLayer::onButton, this));
                                    btnRestart->setTag((int)ChildTag::BTN_RESTART);
                                    btnRestart->setTitleFontName(GAME_CONFIG_GLOBAL_FONTNAME_01);
                                    btnRestart->setTitleFontSize(22.0f);
                                    btnRestart->setTitleText(this->m_strings["gameover_restart"]);
                                    gameoverNode->addChild(btnRestart);
                                    
								}
								else
								{
									this->m_eatFish = 0;

									SimpleAudioEngine::getInstance()->playEffect("audios_playbyeat.mp3");
									this->changePlayerLife(this->m_playerLife - 1);
									this->scheduleOnce(schedule_selector(GameLayer::gameRestart), 2.5);
								}
								
								break;
							}
							
						}

					}
				}

			}
		}
	}	

}

void GameLayer::enemyFishMoveEnd(cocos2d::Node* sender)
{
	sender->removeFromParentAndCleanup(true);

}

void GameLayer::itemMoveEnd(cocos2d::Node* sender)
{
	sender->removeFromParentAndCleanup(true);

}

void GameLayer::scenePause()
{
    if(this->getChildByTag((int)ChildTag::PAUSE_NODE) || this->getChildByTag((int)ChildTag::GAMEOVER_NODE) || this->getChildByTag((int)ChildTag::CLEAR_NODE))
        return;
    
    Node *fishNode = this->getChildByTag((int)ChildTag::FISH_NODE);
    
    //如果player不存在也不会执行暂停
    if(!fishNode->getChildByTag((int)ChildTag::FISH_PLAYER))
        return;
    
    if(fishNode)
    {
        Vector<Node*> nodeList = fishNode->getChildren();
        Vector<Node*>::iterator it = nodeList.begin();
        while(it != nodeList.end())
        {
            Node *fish = *it;
            Vector<Node*> fishChildren = fish->getChildren();
            Vector<Node*>::iterator it2 = fishChildren.begin();
            while(it2 != fishChildren.end())
            {
                Node *fishChild = *it2;
                fishChild->pause();
                
                it2++;
            }
            
            fish->pause();
            
            it++;
        }
    }
    
    this->unscheduleUpdate();
    
    Size winSize = Director::getInstance()->getWinSize();
    
    this->enabledTouchEvent(false);
    
    //暂停界面
    
    Sprite *pauseBg = Sprite::createWithSpriteFrameName("pausebg.png");
    pauseBg->setPosition(Vec2(pauseBg->getContentSize().width / 2, pauseBg->getContentSize().height / 2));
    
    Node *pauseNode = Node::create();
    pauseNode->setAnchorPoint(Vec2(0.5, 0.5));
    pauseNode->setContentSize(pauseBg->getContentSize());
    pauseNode->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
    pauseNode->setTag((int)ChildTag::PAUSE_NODE);
    this->addChild(pauseNode);
    
    pauseNode->addChild(pauseBg);
    
    Button *btnResume = Button::create();
    btnResume->loadTextureNormal("btn1_up.png");
    btnResume->loadTexturePressed("btn1_dw.png");
    btnResume->setPosition(Vec2(180, 370));
    btnResume->addTouchEventListener(CC_CALLBACK_2(GameLayer::onButton, this));
    btnResume->setTag((int)ChildTag::BTN_RESUME);
    btnResume->setTitleFontName(GAME_CONFIG_GLOBAL_FONTNAME_01);
    btnResume->setTitleFontSize(22.0f);
    btnResume->setTitleText(this->m_strings["pause_resume"]);
    pauseNode->addChild(btnResume);
    
    Button *btnSound = Button::create();
    btnSound->loadTextureNormal("btn1_up.png");
    btnSound->loadTexturePressed("btn1_dw.png");
    btnSound->setPosition(Vec2(180, 264));
    btnSound->addTouchEventListener(CC_CALLBACK_2(GameLayer::onButton, this));
    btnSound->setTag((int)ChildTag::BTN_SOUND);
    btnSound->setTitleFontName(GAME_CONFIG_GLOBAL_FONTNAME_01);
    btnSound->setTitleFontSize(22.0f);
    
    if(UserDefault::getInstance()->getBoolForKey(GAME_CONFIG_BGSOUND, true))
        btnSound->setTitleText(StringUtils::format("%s(%s)", this->m_strings["pause_sound"].c_str(), this->m_strings["pause_off"].c_str()));
    else
		btnSound->setTitleText(StringUtils::format("%s(%s)", this->m_strings["pause_sound"].c_str(), this->m_strings["pause_on"].c_str()));
    
    pauseNode->addChild(btnSound);
    
    Button *btnEffect = Button::create();
    btnEffect->loadTextureNormal("btn1_up.png");
    btnEffect->loadTexturePressed("btn1_dw.png");
    btnEffect->setPosition(Vec2(180, 157));
    btnEffect->addTouchEventListener(CC_CALLBACK_2(GameLayer::onButton, this));
    btnEffect->setTag((int)ChildTag::BTN_EFFECT);
    btnEffect->setTitleFontName(GAME_CONFIG_GLOBAL_FONTNAME_01);
    btnEffect->setTitleFontSize(22.0f);
    
    if(UserDefault::getInstance()->getBoolForKey(GAME_CONFIG_EFFECT, true))
		btnEffect->setTitleText(StringUtils::format("%s(%s)", this->m_strings["pause_effect"].c_str(), this->m_strings["pause_off"].c_str()));
    else
		btnEffect->setTitleText(StringUtils::format("%s(%s)", this->m_strings["pause_effect"].c_str(), this->m_strings["pause_on"].c_str()));
    
    pauseNode->addChild(btnEffect);
    
    Button *btnExit = Button::create();
    btnExit->loadTextureNormal("btn1_up.png");
    btnExit->loadTexturePressed("btn1_dw.png");
    btnExit->setPosition(Vec2(180, 50));
    btnExit->addTouchEventListener(CC_CALLBACK_2(GameLayer::onButton, this));
    btnExit->setTag((int)ChildTag::BTN_EXIT);
    btnExit->setTitleFontName(GAME_CONFIG_GLOBAL_FONTNAME_01);
    btnExit->setTitleFontSize(22.0f);
    btnExit->setTitleText(this->m_strings["pause_exit"]);
    pauseNode->addChild(btnExit);
    
    Label *labGithub = Label::createWithSystemFont("github:https://github.com/ouzhigang/OzgGameEatFish", GAME_CONFIG_GLOBAL_FONTNAME_01, 20);
    labGithub->setPosition(Vec2(650, 210));
    pauseNode->addChild(labGithub);
    
}

//private
void GameLayer::onButton(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType eventType)
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
                case ((int)ChildTag::BTN_PAUSE):
                {
                    //暂停游戏
                    SimpleAudioEngine::getInstance()->playEffect("audios_btn.wav");
                    
                    this->scenePause();                    
                }
                    break;
                case ((int)ChildTag::BTN_RESUME):
                {
                    //继续游戏
                    
                    SimpleAudioEngine::getInstance()->playEffect("audios_btn.wav");
                    
                    Node *fishNode = this->getChildByTag((int)ChildTag::FISH_NODE);
                    if(fishNode)
                    {
                        Vector<Node*> nodeList = fishNode->getChildren();
                        Vector<Node*>::iterator it = nodeList.begin();
                        while(it != nodeList.end())
                        {
                            Node *fish = *it;
                            Vector<Node*> fishChildren = fish->getChildren();
                            Vector<Node*>::iterator it2 = fishChildren.begin();
                            while(it2 != fishChildren.end())
                            {
                                Node *fishChild = *it2;
                                fishChild->resume();
                                
                                it2++;
                            }
                            
                            fish->resume();
                            
                            it++;
                        }
                    }
                    
                    this->scheduleUpdate();
                    this->enabledTouchEvent(true);
                    
                    Node *pauseNode = this->getChildByTag((int)ChildTag::PAUSE_NODE);
                    if(pauseNode)
                        pauseNode->removeFromParentAndCleanup(true);
                    
                }
                    break;
                case ((int)ChildTag::BTN_SOUND):
                {
                    //背景音乐
                    if(!UserDefault::getInstance()->getBoolForKey(GAME_CONFIG_BGSOUND, true))
                    {
                        SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(1.0);
                        UserDefault::getInstance()->setBoolForKey(GAME_CONFIG_BGSOUND, true);
						btn->setTitleText(StringUtils::format("%s(%s)", this->m_strings["pause_sound"].c_str(), this->m_strings["pause_off"].c_str()));
                    }
                    else
                    {
                        SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.0);
                        UserDefault::getInstance()->setBoolForKey(GAME_CONFIG_BGSOUND, false);
						btn->setTitleText(StringUtils::format("%s(%s)", this->m_strings["pause_sound"].c_str(), this->m_strings["pause_on"].c_str()));
                    }
                    
                }
                    break;
                case ((int)ChildTag::BTN_EFFECT):
                {
                    //效果声音
                    if(!UserDefault::getInstance()->getBoolForKey(GAME_CONFIG_EFFECT, true))
                    {
                        SimpleAudioEngine::getInstance()->setEffectsVolume(1.0);
                        UserDefault::getInstance()->setBoolForKey(GAME_CONFIG_EFFECT, true);
						btn->setTitleText(StringUtils::format("%s(%s)", this->m_strings["pause_effect"].c_str(), this->m_strings["pause_off"].c_str()));
                    }
                    else
                    {
                        SimpleAudioEngine::getInstance()->setEffectsVolume(0.0);
                        UserDefault::getInstance()->setBoolForKey(GAME_CONFIG_EFFECT, false);
						btn->setTitleText(StringUtils::format("%s(%s)", this->m_strings["pause_effect"].c_str(), this->m_strings["pause_on"].c_str()));
                    }
                    
                }
                    break;
                case ((int)ChildTag::BTN_EXIT):
                {
                    //退出游戏
                    LayerColor *maskLayer = LayerColor::create(Color4B(0, 0, 0, 180), Director::getInstance()->getWinSize().width, Director::getInstance()->getWinSize().height);
					maskLayer->setTag((int)ChildTag::MASK);
					maskLayer->setPosition(Vec2::ZERO);
					this->addChild(maskLayer);
                    
                    Node *pauseNode = this->getChildByTag((int)ChildTag::PAUSE_NODE);
                    Node *clearNode = this->getChildByTag((int)ChildTag::CLEAR_NODE);
                    Node *gameoverNode = this->getChildByTag((int)ChildTag::GAMEOVER_NODE);
                    
                    if(pauseNode)
                    {
                        Button *btnResume = (Button*)pauseNode->getChildByTag((int)ChildTag::BTN_RESUME);
                        btnResume->setEnabled(false);
                        
                        Button *btnSound = (Button*)pauseNode->getChildByTag((int)ChildTag::BTN_SOUND);
                        btnSound->setEnabled(false);
                        
                        Button *btnEffect = (Button*)pauseNode->getChildByTag((int)ChildTag::BTN_EFFECT);
                        btnEffect->setEnabled(false);
                        
                        Button *btnExit = (Button*)pauseNode->getChildByTag((int)ChildTag::BTN_EXIT);
                        btnExit->setEnabled(false);
                    }
                    
                    if(clearNode)
                    {
                        Button *btnExit = (Button*)clearNode->getChildByTag((int)ChildTag::BTN_EXIT);
                        btnExit->setEnabled(false);
                        
                        Button *btnNext = (Button*)clearNode->getChildByTag((int)ChildTag::BTN_NEXT);
                        btnNext->setEnabled(false);
                    }
                    
                    if(gameoverNode)
                    {
                        Button *btnExit = (Button*)gameoverNode->getChildByTag((int)ChildTag::BTN_EXIT);
                        btnExit->setEnabled(false);
                        
                        Button *btnRestart = (Button*)gameoverNode->getChildByTag((int)ChildTag::BTN_RESTART);
                        btnRestart->setEnabled(false);
                    }
                    
                    DialogNode *dialog = DialogNode::create(this->m_strings["title"], this->m_strings["settings_exit_dialog"], this->m_strings["dialog_btn_no"], CallFuncN::create(CC_CALLBACK_1(GameLayer::onDialogBtnNo, this)), this->m_strings["dialog_btn_yes"], CallFuncN::create(CC_CALLBACK_1(GameLayer::onDialogBtnYes, this)));
					dialog->setPosition(this->getContentSize().width / 2, this->getContentSize().height / 2);
					this->addChild(dialog);
                }
                    break;
                case ((int)ChildTag::BTN_NEXT):
                {
                    //下一关
                    
					//更新背景
					this->removeChildByTag((int)ChildTag::BG);
					Director::getInstance()->getTextureCache()->removeTextureForKey(this->m_bg);
					
					Size winSize = Director::getInstance()->getWinSize();

					Sprite *bg = this->createRandomBg();
					bg->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
					this->insertChild(bg, 0);
					//更新背景 end

                    this->m_stageNum += 1;
                    if(this->m_stageNum > GAME_CONFIG_MAX_STAGE)
                        this->m_stageNum = GAME_CONFIG_MAX_STAGE;
                    
                    Label *stageNumLab = (Label*)this->getChildByTag((int)ChildTag::LAB_STAGE_NUM);
                    stageNumLab->setString(StringUtils::format(this->m_strings["game_scene_lab_stage_num"].c_str(), this->m_stageNum));
                    
                    this->m_eatFish = 0;
                    this->m_eatFishTotal = 0;
                    this->m_eatFishTotalType1And2 = 0;
                    this->m_eatFishTotalType3 = 0;
                    this->m_eatFishTotalType4 = 0;
                    
					this->m_stageClear += GAME_CONFIG_STAGE_CLEAR_STEP;
					this->m_playerStatusNormal = (int)((float)this->m_stageClear * 0.29 + 0.5);
					this->m_playerStatusBig = (int)((float)this->m_stageClear * 0.61 + 0.5);

                    ProgressTimer *progress = (ProgressTimer*)this->getChildByTag((int)ChildTag::PROGRESS);
                    progress->setPercentage(0);
                    
                    Node *clearNode = this->getChildByTag((int)ChildTag::CLEAR_NODE);
                    clearNode->removeFromParentAndCleanup(true);
                    
                    Node *fishNode = this->getChildByTag((int)ChildTag::FISH_NODE);
                    PlayerFishNode *player = PlayerFishNode::create();
                    player->setPosition(Vec2(Director::getInstance()->getWinSize().width / 2, 800));
                    player->setTag((int)ChildTag::FISH_PLAYER);
                    fishNode->addChild(player, 99999);
                    player->invincible();
                    
                    this->gameStart();
                }
                    break;
                case ((int)ChildTag::BTN_RESTART):
                {
                    //重新开始
                    
                    this->m_score = 0;
                    this->m_stageNum = 1;
                    this->m_playerLife = GAME_CONFIG_PLAYERS;
                    this->m_eatFish = 0;
                    this->m_eatFishTotal = 0;
                    this->m_eatFishTotalType1And2 = 0;
                    this->m_eatFishTotalType3 = 0;
                    this->m_eatFishTotalType4 = 0;

					this->m_stageClear = GAME_CONFIG_STAGE_CLEAR;
					this->m_playerStatusNormal = (int)((float)this->m_stageClear * 0.29 + 0.5);
					this->m_playerStatusBig = (int)((float)this->m_stageClear * 0.61 + 0.5);
                    
                    Label *stageNumLab = (Label*)this->getChildByTag((int)ChildTag::LAB_STAGE_NUM);
                    stageNumLab->setString(StringUtils::format(this->m_strings["game_scene_lab_stage_num"].c_str(), this->m_stageNum));
                    
                    Label *scoreLab = (Label*)this->getChildByTag((int)ChildTag::LAB_SCORE);
                    scoreLab->setString(StringUtils::format(this->m_strings["game_scene_lab_score"].c_str(), this->m_score).c_str());
                    
                    Label *fishLifeLab = (Label*)this->getChildByTag((int)ChildTag::LAB_FISH_LIFE);
                    fishLifeLab->setString(StringUtils::format("%i", this->m_playerLife).c_str());
                    
                    ProgressTimer *progress = (ProgressTimer*)this->getChildByTag((int)ChildTag::PROGRESS);
                    progress->setPercentage(0);
                    
                    Node *gameoverNode = this->getChildByTag((int)ChildTag::GAMEOVER_NODE);
                    gameoverNode->removeFromParentAndCleanup(true);
                    
                    Node *fishNode = this->getChildByTag((int)ChildTag::FISH_NODE);
                    PlayerFishNode *player = PlayerFishNode::create();
                    player->setPosition(Vec2(Director::getInstance()->getWinSize().width / 2, 800));
                    player->setTag((int)ChildTag::FISH_PLAYER);
                    fishNode->addChild(player, 99999);
                    player->invincible();
                    
                    this->gameStart();
                }
                    break;
                default:
                    break;
            }
            
        }
			break;
		case ui::Widget::TouchEventType::CANCELED:

			break;
	}
}

//private
void GameLayer::gameStart()
{
	SimpleAudioEngine::getInstance()->playEffect("audios_fishstart.mp3");

	Node *fishNode = this->getChildByTag((int)ChildTag::FISH_NODE);

	PlayerFishNode *player = (PlayerFishNode*)fishNode->getChildByTag((int)ChildTag::FISH_PLAYER);
	player->runAction(Sequence::createWithTwoActions(MoveBy::create(1.0, Vec2(0, -400)), CallFunc::create(CC_CALLBACK_0(GameLayer::gameStartCallback, this))));
	
}

//private
void GameLayer::gameStartCallback()
{
	this->enabledTouchEvent(true);
	
	Node *fishNode = this->getChildByTag((int)ChildTag::FISH_NODE);
	PlayerFishNode *player = (PlayerFishNode*)fishNode->getChildByTag((int)ChildTag::FISH_PLAYER);
	player->m_isMoving = true;

	//随机性质的事件和AI都在这里计算
	this->scheduleUpdate();
}

//private
void GameLayer::gameRestart(float delay)
{
	SimpleAudioEngine::getInstance()->playEffect("audios_fishstart.mp3");

	Size winSize = Director::getInstance()->getWinSize();
	Node *fishNode = this->getChildByTag((int)ChildTag::FISH_NODE);

	PlayerFishNode *player = PlayerFishNode::create();
	player->setPosition(Vec2(winSize.width / 2, 800));
	player->setTag((int)ChildTag::FISH_PLAYER);
	fishNode->addChild(player, 99999);
	player->invincible();

	//鱼掉下来
	player->runAction(Sequence::createWithTwoActions(MoveBy::create(1.0, Vec2(0, -400)), CallFunc::create(CC_CALLBACK_0(GameLayer::gameRestartCallback, this))));

	this->enabledTouchEvent(false);
}

//private
void GameLayer::gameRestartCallback()
{
	Node *fishNode = this->getChildByTag((int)ChildTag::FISH_NODE);
	PlayerFishNode *player = (PlayerFishNode*)fishNode->getChildByTag((int)ChildTag::FISH_PLAYER);
	player->m_isMoving = true;

	this->enabledTouchEvent(true);
}

//private
void GameLayer::enabledTouchEvent(bool enabled)
{
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	if (enabled)
	{
		auto listener = EventListenerTouchOneByOne::create();
		listener->setSwallowTouches(true);
		listener->onTouchBegan = CC_CALLBACK_2(GameLayer::onLayerTouchBegan, this);
		listener->onTouchMoved = CC_CALLBACK_2(GameLayer::onLayerTouchMoved, this);
		listener->onTouchEnded = CC_CALLBACK_2(GameLayer::onLayerTouchEnded, this);
		dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	}
	else
		dispatcher->removeEventListenersForTarget(this);

	Button *btnPause = (Button*)this->getChildByTag((int)ChildTag::BTN_PAUSE);
	btnPause->setEnabled(enabled);
}

//private
bool GameLayer::onLayerTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{

	return true;
}

//private
void GameLayer::onLayerTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{
	Size winSize = Director::getInstance()->getWinSize();

	Node *fishNode = this->getChildByTag((int)ChildTag::FISH_NODE);

	PlayerFishNode *player = (PlayerFishNode*)fishNode->getChildByTag((int)ChildTag::FISH_PLAYER);
	if (player && player->m_isMoving && player->m_effectStatus != BaseFishNode::EffectStatus::PARALYSIS)
	{
		Point beginPoint = touch->getLocation();
		Point endPoint = touch->getPreviousLocation();
		Point offSet = beginPoint - endPoint;
		Point toPoint = player->getPosition() + offSet;

		float toX = player->getPosition().x;
		float toY = player->getPosition().y;

		Rect rect = player->centerRect();
		Rect moveRect = Rect(rect.size.width / 2, rect.size.height / 2, winSize.width - (rect.size.width / 2), winSize.height - (rect.size.height / 2));

		//如果toPoint的x存在moveRect的宽度范围里面则x为可移动，y的情况一样
		if (toPoint.x >= moveRect.origin.x && toPoint.x <= moveRect.size.width)
			toX = toPoint.x;
		if (toPoint.y >= moveRect.origin.y && toPoint.y <= moveRect.size.height)
			toY = toPoint.y;

		player->setPosition(Vec2(toX, toY));

		if (offSet.x > 0)
			player->orientationRight(); //向右移动则转向右边
		else if (offSet.x < 0)
			player->orientationLeft(); //向左移动则转向左边
	}
		

}

//private
void GameLayer::onLayerTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
	
}

//private
void GameLayer::enemyFishEmergence(BaseFishNode* enemyFishNode)
{
	Vec2 startPoint;
	Vec2 endPoint;

	Node *fishNode = this->getChildByTag((int)ChildTag::FISH_NODE);

	//0.5为左边右边的机率各为50%
	if (CCRANDOM_0_1() <= 0.5)
	{
		//左边出现
		startPoint = this->enemyFishRandomLeftPoint(enemyFishNode);
		endPoint = this->enemyFishRandomRightPoint(enemyFishNode);
		enemyFishNode->orientationRight(); //左边出现需要面向右边
	}
	else
	{
		//右边出现
		startPoint = this->enemyFishRandomRightPoint(enemyFishNode);
		endPoint = this->enemyFishRandomLeftPoint(enemyFishNode);
		enemyFishNode->orientationLeft(); //右边出现需要面向左边
	}
	enemyFishNode->setPosition(startPoint);
	fishNode->addChild(enemyFishNode);

	float moveTime = Utility::randomFloat(10.0, 20.0);

	((BaseEnemyFishNode*)enemyFishNode)->m_isMoving = true; //执行action需要强制设置成YES
	((BaseEnemyFishNode*)enemyFishNode)->m_moveTime = moveTime;
	((BaseEnemyFishNode*)enemyFishNode)->m_moveStartPoint = startPoint;
	((BaseEnemyFishNode*)enemyFishNode)->m_moveEndPoint = endPoint;

	enemyFishNode->runAction(Sequence::createWithTwoActions(MoveTo::create(moveTime, endPoint), CallFuncN::create(CC_CALLBACK_1(GameLayer::enemyFishMoveEnd, this))));
	
}

//private
Vec2 GameLayer::enemyFishRandomLeftPoint(BaseFishNode* enemyFishNode)
{
	Size winSize = Director::getInstance()->getWinSize();
	float x = -enemyFishNode->getContentSize().width / 2;
	float minY = enemyFishNode->centerRect().size.height / 2;
	float maxY = winSize.height - minY;

	float y = Utility::randomFloat(minY, maxY);
	return Vec2(x, y);
}

//private
Vec2 GameLayer::enemyFishRandomRightPoint(BaseFishNode* enemyFishNode)
{
	Size winSize = Director::getInstance()->getWinSize();
	float x = winSize.width + (enemyFishNode->getContentSize().width / 2);
	float minY = enemyFishNode->centerRect().size.height / 2;
	float maxY = winSize.height - minY;
		
	float y = Utility::randomFloat(minY, maxY);
	return Vec2(x, y);
}

//private
void GameLayer::changeScore(ItemNode::ItemNodeType type)
{
	switch (type)
	{
	default:
		//金币
		this->m_score += GAME_CONFIG_SCORE_ITEM_GOLD;

		break;
	}

	if (this->m_score > GAME_CONFIG_MAX_SCORE)
		this->m_score = GAME_CONFIG_MAX_SCORE;

	Label *scoreLab = (Label*)this->getChildByTag((int)ChildTag::LAB_SCORE);
	scoreLab->setString(StringUtils::format(this->m_strings["game_scene_lab_score"].c_str(), this->m_score).c_str());
}

//private
void GameLayer::changeScore(EnemyFishNode::EnemyFishType type)
{
	switch (type)
	{
		case EnemyFishNode::EnemyFishType::Fish2:
			this->m_score += GAME_CONFIG_SCORE_FISH2;
			this->m_eatFish += GAME_CONFIG_SCORE_FISH2;
			this->m_eatFishTotal += GAME_CONFIG_SCORE_FISH2;
			this->m_eatFishTotalType1And2 += 1;

			break;
		case EnemyFishNode::EnemyFishType::Fish3:
			this->m_score += GAME_CONFIG_SCORE_FISH3;
			this->m_eatFish += GAME_CONFIG_SCORE_FISH3;
			this->m_eatFishTotal += GAME_CONFIG_SCORE_FISH3;
			this->m_eatFishTotalType3 += 1;

			break;
		case EnemyFishNode::EnemyFishType::Fish4:
			this->m_score += GAME_CONFIG_SCORE_FISH4;
			this->m_eatFish += GAME_CONFIG_SCORE_FISH4;
			this->m_eatFishTotal += GAME_CONFIG_SCORE_FISH4;
			this->m_eatFishTotalType4 += 1;

			break;
		default:
			this->m_score += GAME_CONFIG_SCORE_FISH1;
			this->m_eatFish += GAME_CONFIG_SCORE_FISH1;
			this->m_eatFishTotal += GAME_CONFIG_SCORE_FISH1;
			this->m_eatFishTotalType1And2 += 1;

			break;
	}
    
	if (this->m_score > GAME_CONFIG_MAX_SCORE)
		this->m_score = GAME_CONFIG_MAX_SCORE;
    
	if (this->m_eatFish > GAME_CONFIG_MAX_SCORE)
		this->m_eatFish = GAME_CONFIG_MAX_SCORE;
    
	if (this->m_eatFishTotal > GAME_CONFIG_MAX_SCORE)
		this->m_eatFishTotal = GAME_CONFIG_MAX_SCORE;
    
	Label *scoreLab = (Label*)this->getChildByTag((int)ChildTag::LAB_SCORE);
	scoreLab->setString(StringUtils::format(this->m_strings["game_scene_lab_score"].c_str(), this->m_score).c_str());
}

//private
void GameLayer::changePlayerLife(int playerLife)
{
	this->m_playerLife = playerLife;

	Label *fishLifeLab = (Label*)this->getChildByTag((int)ChildTag::LAB_FISH_LIFE);
	fishLifeLab->setString(StringUtils::format("%i", this->m_playerLife).c_str());
}

//private
void GameLayer::onDialogBtnNo(cocos2d::Node* sender)
{
    //取消退出游戏
    
    sender->removeFromParentAndCleanup(true);
    
    LayerColor *maskLayer = (LayerColor*)this->getChildByTag((int)ChildTag::MASK);
    maskLayer->removeFromParentAndCleanup(true);
    
    Node *pauseNode = this->getChildByTag((int)ChildTag::PAUSE_NODE);
    Node *clearNode = this->getChildByTag((int)ChildTag::CLEAR_NODE);
    Node *gameoverNode = this->getChildByTag((int)ChildTag::GAMEOVER_NODE);
    
    if(pauseNode)
    {
        Button *btnResume = (Button*)pauseNode->getChildByTag((int)ChildTag::BTN_RESUME);
        btnResume->setEnabled(true);
        
        Button *btnSound = (Button*)pauseNode->getChildByTag((int)ChildTag::BTN_SOUND);
        btnSound->setEnabled(true);
        
        Button *btnEffect = (Button*)pauseNode->getChildByTag((int)ChildTag::BTN_EFFECT);
        btnEffect->setEnabled(true);
        
        Button *btnExit = (Button*)pauseNode->getChildByTag((int)ChildTag::BTN_EXIT);
        btnExit->setEnabled(true);
    }
    
    if(clearNode)
    {
        Button *btnExit = (Button*)clearNode->getChildByTag((int)ChildTag::BTN_EXIT);
        btnExit->setEnabled(true);
        
        Button *btnNext = (Button*)clearNode->getChildByTag((int)ChildTag::BTN_NEXT);
        btnNext->setEnabled(true);
    }
    
    if(gameoverNode)
    {
        Button *btnExit = (Button*)gameoverNode->getChildByTag((int)ChildTag::BTN_EXIT);
        btnExit->setEnabled(true);
        
        Button *btnRestart = (Button*)gameoverNode->getChildByTag((int)ChildTag::BTN_RESTART);
        btnRestart->setEnabled(true);
    }
    
}

//private
void GameLayer::onDialogBtnYes(cocos2d::Node* sender)
{
    //退出游戏
    this->unscheduleUpdate();
    
    Scene *s = StartLayer::createScene();
    TransitionFade *t = TransitionFade::create(GAME_CONFIG_TRANSITION, s);
    Director::getInstance()->replaceScene(t);
}

//private
Sprite* GameLayer::createRandomBg()
{
	vector<string> bgList;
	bgList.push_back("bg1.png");
	bgList.push_back("bg2.png");
	bgList.push_back("bg3.png");

	//背景
	int i = Utility::randomInt(0, (int)bgList.size() - 1);
	this->m_bg = bgList.at(i);
	Sprite *bg = Sprite::create(this->m_bg.c_str());
	bg->setTag((int)ChildTag::BG);
	return bg;
}
