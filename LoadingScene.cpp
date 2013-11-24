#include "LoadingScene.h"
#include "simpleAudioEngine.h"
USING_NS_CC;


LoadingScene::LoadingScene()
{

}

LoadingScene::~LoadingScene()
{

}

CCScene* LoadingScene::scene()
{
	// 'scene' is an autorelease object
	CCScene *scene = CCScene::create();

	// 'layer' is an autorelease object
	LoadingScene *layer = LoadingScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool LoadingScene::init()
{
	//////////////////////////////
	// 1. super init first
	if ( !CCLayer::init() )
	{
		return false;
	}
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("bg/bg.plist");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("game/shoot.plist");
	CCSprite* bg=CCSprite::createWithSpriteFrameName("background.png");
	CCSize winSize=CCDirector::sharedDirector()->getWinSize();
	bg->setPosition(ccp(winSize.width/2,winSize.height/2));
	bg->setScaleX(320.0/480);
	bg->setScaleY(480.0/852);
	this->addChild(bg);

	this->setTouchEnabled(true);
	this->setTouchMode(kCCTouchesOneByOne);

	CCSprite* name=CCSprite::createWithSpriteFrameName("name.png");
	name->setPosition(ccp(160,350));
	name->setScale(0.7);
	this->addChild(name,2);

	_plane=CCSprite::createWithSpriteFrameName("game_loading1.png");
	_plane->setPosition(ccp(160,100));
	_plane->setScale(1.5);
	this->addChild(_plane,2);

	//·É»ú·É¹ý¶¯»­
	CCArray *animationFrames = CCArray::create();
	const int animationCount = 2;
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("bg/bg.plist");
	for (int i=1; i <= animationCount; ++i )
	{
		CCString *frameName = CCString::createWithFormat("game_loading%d.png",i);
		CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(frameName->getCString());
		animationFrames->addObject(frame);
	}
	CCAnimation *animation = CCAnimation::createWithSpriteFrames(animationFrames);
	animation->setDelayPerUnit(0.3);
	CCAnimate* animate = CCAnimate::create(animation);
	CCDelayTime *delay = CCDelayTime::create(1.0);
	//CCSequence* seq=CCSequence::create(animate,delay);
	//CCRepeatForever* ac=CCRepeatForever::create(animate);
	_plane->runAction(animate);


	SimpleAudioEngine *effect=SimpleAudioEngine::sharedEngine();
	effect->preloadEffect("achievement.mp3");
	effect->preloadEffect("big_spaceship_flying.mp3");
	effect->preloadEffect("bullet.mp3");
	effect->preloadEffect("enemy0_down.mp3");
	effect->preloadEffect("enemy1_down.mp3");
	effect->preloadEffect("enemy2_down.mp3");
	effect->preloadBackgroundMusic("game_music.mp3");
	effect->preloadEffect("game_over.mp3");
	effect->preloadEffect("get_bomb.mp3");
	effect->preloadEffect("get_double_laser.mp3");
	effect->preloadEffect("use_bomb.mp3");

	//CCDirector::sharedDirector()->replaceScene(HelloWorld::scene());

	return true;
}

void LoadingScene::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
	CCPoint position=pTouch->getLocationInView();
	position=CCDirector::sharedDirector()->convertToGL(position);
	if (position.x>60&&position.x<260&&position.y>0&&position.y<50)//leave
	{
		//CCDirector::sharedDirector()->popScene();
		CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5,HelloWorld::scene()));
	}

}




void LoadingScene::menuCloseCallback(CCObject* pSender)
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


bool LoadingScene::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	return true;
}

void LoadingScene::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{

}

void LoadingScene::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{

}