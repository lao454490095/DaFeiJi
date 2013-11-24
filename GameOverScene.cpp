#include "GameOverScene.h"
USING_NS_CC;


GameOverScene::GameOverScene()
{

}

GameOverScene::~GameOverScene()
{

}

CCScene* GameOverScene::scene()
{
	// 'scene' is an autorelease object
	CCScene *scene = CCScene::create();

	// 'layer' is an autorelease object
	GameOverScene *layer = GameOverScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool GameOverScene::init()
{
	//////////////////////////////
	// 1. super init first
	if ( !CCLayer::init() )
	{
		return false;
	}
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("bg/bg.plist");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("game/shoot.plist");
	CCSprite* bg=CCSprite::createWithSpriteFrameName("gameover.png");
	CCSize winSize=CCDirector::sharedDirector()->getWinSize();
	bg->setPosition(ccp(winSize.width/2,winSize.height/2));
	bg->setScaleX(320.0/480);
	bg->setScaleY(480.0/852);
	this->addChild(bg);

	CCSprite* next=CCSprite::createWithSpriteFrameName("restart_sel.png");
	next->setPosition(ccp(160,next->getContentSize().height/2));
	this->addChild(next);



	this->setTouchEnabled(true);
	this->setTouchMode(kCCTouchesOneByOne);

	int _score=CCUserDefault::sharedUserDefault()->getIntegerForKey("score");
	CCLabelTTF* scoreLabel = CCLabelTTF::create("0","sans-serif",60);
	scoreLabel->setString(CCString::createWithFormat("%d",_score)->getCString());
	scoreLabel->setPosition(ccp(160,280));
	scoreLabel->setColor(ccRED);
	this->addChild(scoreLabel,110);

	int temp_score=CCUserDefault::sharedUserDefault()->getIntegerForKey("temp_score");
	CCLabelTTF* scoreLabel2 = CCLabelTTF::create("0","sans-serif",60);
	scoreLabel2->setString(CCString::createWithFormat("%d",temp_score)->getCString());
	scoreLabel2->setPosition(ccp(160,120));
	scoreLabel2->setColor(ccRED);
	this->addChild(scoreLabel2,110);

	return true;
}

void GameOverScene::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
	CCPoint position=pTouch->getLocationInView();
	position=CCDirector::sharedDirector()->convertToGL(position);
	if (position.x>60&&position.x<260&&position.y>0&&position.y<50)//leave
	{
		//CCDirector::sharedDirector()->popScene();
		SimpleAudioEngine::sharedEngine()->stopAllEffects();
		CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5,HelloWorld::scene()));
	}

}




void GameOverScene::menuCloseCallback(CCObject* pSender)
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


bool GameOverScene::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	return true;
}

void GameOverScene::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{

}

void GameOverScene::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{

}