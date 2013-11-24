#include "GameStartScene.h"
USING_NS_CC;


GameStartScene::GameStartScene()
{
	
}

GameStartScene::~GameStartScene()
{

}

CCScene* GameStartScene::scene()
{
	// 'scene' is an autorelease object
	CCScene *scene = CCScene::create();

	// 'layer' is an autorelease object
	GameStartScene *layer = GameStartScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool GameStartScene::init()
{
	//////////////////////////////
	// 1. super init first
	if ( !CCLayer::init() )
	{
		return false;
	}
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("bg/bg.plist");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("game/shoot.plist");
	CCSprite* start=CCSprite::createWithSpriteFrameName("restart_nor.png");
	start->setPosition(ccp(160,260));
	start->setScale(1.5);
	this->addChild(start,2);

	CCSprite *bg=CCSprite::createWithSpriteFrameName("background.png");
	CCSize winSize=CCDirector::sharedDirector()->getWinSize();
	bg->setPosition(ccp(winSize.width/2,winSize.height/2));
	bg->setScaleX(320.0/480);
	bg->setScaleY(480.0/852);
	this->addChild(bg);

	CCSprite* name=CCSprite::createWithSpriteFrameName("name.png");
	name->setPosition(ccp(160,400));
	name->setScale(0.7);
	this->addChild(name,2);

	CCSprite* leave=CCSprite::createWithSpriteFrameName("quit_nor.png");
	leave->setPosition(ccp(160,180));
	leave->setScale(1.5);
	this->addChild(leave,2);

	CCSprite* loading1=CCSprite::createWithSpriteFrameName("game_loading1.png");
	loading1->setPosition(ccp(160,100));
	loading1->setScale(1.5);
	this->addChild(loading1,2);

	CCSprite* goon=CCSprite::createWithSpriteFrameName("resume_nor.png");
	goon->setPosition(ccp(160,320));
	goon->setScale(1.5);
	this->addChild(goon,2);

	CCSprite* loading2=CCSprite::createWithSpriteFrameName("game_loading2.png");
	loading2->setPosition(ccp(240,80));
	loading2->setScale(1.5);
	this->addChild(loading2,2);

	this->setTouchEnabled(true);
	this->setTouchMode(kCCTouchesOneByOne);


	return true;
}

void GameStartScene::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
	CCPoint position=pTouch->getLocationInView();
	position=CCDirector::sharedDirector()->convertToGL(position);
	if (position.x>60&&position.x<260&&position.y>155&&position.y<205)//leave
	{
		//CCDirector::sharedDirector()->popScene();
		CCDirector::sharedDirector()->end();
	}

	if (position.x>60&&position.x<260&&position.y>295&&position.y<345)//go-on   (160,320)   200,50
	{
		//CCDirector::sharedDirector()->replaceScene()
		CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5,HelloWorld::scene()));

	}
	if (position.x>60&&position.x<260&&position.y>235&&position.y<285)//start    (160,260)    200,50
	{
		CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5,HelloWorld::scene()));
	}
	
}




void GameStartScene::menuCloseCallback(CCObject* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
	CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
#endif
}


bool GameStartScene::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	return true;
}

void GameStartScene::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
	
}

void GameStartScene::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{

}