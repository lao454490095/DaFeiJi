#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;


HelloWorld::HelloWorld()
	:_elapseBullet(0),
	_elapseEnemy2(0),
	_elapseEnemy3(0),
	_elapseEnemy(0),
	_enemyArray(NULL),
	_enemyArray2(NULL),
	_enemyArray3(NULL),
	_bonusArray(NULL),
	_bulletArray(NULL),
	_bonusTime(0),
	score(0),
	_bombCount(0),
	_bulletMode(0),
	_animationLock1(0),
	_animationLock2(0)
{
	_enemyArray=CCArray::create();
	_enemyArray->retain();
	_enemyArray2=CCArray::create();
	_enemyArray2->retain();
	_enemyArray3=CCArray::create();
	_enemyArray3->retain();
	_bonusArray=CCArray::create();
	_bonusArray->retain();

	_bulletArray=CCArray::create();
	_bulletArray->retain();
	music=SimpleAudioEngine::sharedEngine();
	music->preloadBackgroundMusic("game_music.mp3");
	CCLOG("game_music loaded successfully");
	music->playBackgroundMusic("game_music",true);
	//score
	_scoreLabel = CCLabelTTF::create("0","sans-serif",40);
	_scoreLabel->setPosition(ccp(100,460));
	_scoreLabel->setColor(ccRED);
	this->addChild(_scoreLabel,110);
	
}




HelloWorld::~HelloWorld()
{
	_enemyArray->release();
	_enemyArray=NULL;
	_enemyArray2->release();
	_enemyArray2=NULL;
	_enemyArray3->release();
	_enemyArray3=NULL;
	_bulletArray->release();
	_bulletArray=NULL;
	music->end();
	//CC_SAFE_RELEASE_NULL(_enemyArray);
}

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
	//load all resource
    //add background
	SimpleAudioEngine::sharedEngine()->preloadEffect("achievement.mp3");
	SimpleAudioEngine::sharedEngine()->preloadEffect("big_spaceship_flying.mp3");
	SimpleAudioEngine::sharedEngine()->preloadEffect("bullet.mp3");
	SimpleAudioEngine::sharedEngine()->preloadEffect("enemy0_down.mp3");
	SimpleAudioEngine::sharedEngine()->preloadEffect("enemy1_down.mp3");
	SimpleAudioEngine::sharedEngine()->preloadEffect("enemy2_down.mp3");
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("game_music.mp3");
	SimpleAudioEngine::sharedEngine()->preloadEffect("game_over.mp3");
	SimpleAudioEngine::sharedEngine()->preloadEffect("get_bomb.mp3");
	SimpleAudioEngine::sharedEngine()->preloadEffect("get_double_laser.mp3");
	SimpleAudioEngine::sharedEngine()->preloadEffect("use_bomb.mp3");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("bg/bg.plist");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("game/shoot.plist");
	CCSprite *bg=CCSprite::createWithSpriteFrameName("background.png");
	CCSize winSize=CCDirector::sharedDirector()->getWinSize();
	bg->setPosition(ccp(winSize.width/2,winSize.height/2));
	bg->setScaleX(320.0/480);
	bg->setScaleY(480.0/852);
	this->addChild(bg);

	CCSprite *pauseButton=CCSprite::createWithSpriteFrameName("game_resume_nor.png");
	pauseButton->setPosition(ccp(pauseButton->boundingBox().size.width/2,winSize.height-pauseButton->boundingBox().size.height/2));
	this->addChild(pauseButton);

	CCSprite *bombNumber=CCSprite::createWithSpriteFrameName("bomb.png");
	bombNumber->setPosition(ccp(bombNumber->boundingBox().size.width/2,bombNumber->boundingBox().size.height/2));
	this->addChild(bombNumber);

	bombLabel = CCLabelTTF::create("0","sans-serif",40);
	bombLabel->setPosition(ccp(80,25));
	bombLabel->setColor(ccBLACK);
	this->addChild(bombLabel,110);

	//add a fighter


	_player=CCSprite::createWithSpriteFrameName("hero1.png");
	_player->setScale(0.4);
	CCSize playerDisplaySize=_player->boundingBox().size;

	_player->setPosition(ccp(winSize.width/2,playerDisplaySize.height/2));
	this->addChild(_player);

	//动画效果！！！
	CCArray *animationFrames = CCArray::create();
	const int animationCount = 2;
	for (int i=1; i <= animationCount; ++i )
	{
		CCString *frameName = CCString::createWithFormat("hero%d.png",i);
		CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(frameName->getCString());
		animationFrames->addObject(frame);
	}

	CCAnimation *animation = CCAnimation::createWithSpriteFrames(animationFrames);
	animation->setDelayPerUnit(0.08);
	CCAnimate* animate = CCAnimate::create(animation);
	CCRepeatForever *ac = CCRepeatForever::create(animate);
	_player->runAction(ac);

	//move player
	//3.1 enable touch
	this->setTouchEnabled(true);

	//3.2 choose touch type
	this->setTouchMode(kCCTouchesOneByOne);

	//3.3 override some virtual method
	
	//4 add an enemy
	//spawnEnemy(0);
	//this->schedule(schedule_selector(HelloWorld::spawnEnemy),1.0/4);
	this->schedule(schedule_selector(HelloWorld::gameLoop),1.0/100);
	
    return true;
}



void HelloWorld::gameLoop(float dt)
{
	dt+=0.0001;
	this->spawnEnemy(dt);
	this->spawnBonus(dt);
	this->fireBullet(dt);
	this->collideDetection(dt);
}

void HelloWorld::collideDetection(float dt)
{
	CCRect playerRect=_player->boundingBox();
	CCObject *obj1=NULL;
	CCObject *obj2=NULL;
	CCRect enemyRect;
	CCRect bulletRect;
	CCARRAY_FOREACH(_enemyArray,obj1)
	{
		//CCLOG("i am in");
		//CCLOG("i am in2");
		CCSprite *sprite =(CCSprite*)obj1;
		enemyRect=sprite->boundingBox();
		if (enemyRect.intersectsRect(playerRect)&&!_animationLock1)
		{
			this->unschedule(schedule_selector(HelloWorld::gameLoop));
			_animationLock1=1;
			SimpleAudioEngine::sharedEngine()->playEffect("game_over.mp3");
			//玩家爆炸动画
			CCArray *animationFrames = CCArray::create();
			const int animationCount = 4;
			CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("game/shoot.plist");
			for (int i=1; i <= animationCount; ++i )
			{
				CCString *frameName = CCString::createWithFormat("hero_blowup_n%d.png",i);
				CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(frameName->getCString());
				animationFrames->addObject(frame);
			}
			CCAnimation *animation = CCAnimation::createWithSpriteFrames(animationFrames);
			animation->setDelayPerUnit(0.2);
			CCAnimate* animate = CCAnimate::create(animation);

			CCCallFuncO *callback = CCCallFuncO::create(this,callfuncO_selector(HelloWorld::onGameOver),sprite);
			CCDelayTime *delay = CCDelayTime::create(1.0);//可以人为延迟！！！
			CCHide *hide = CCHide::create();
			CCSequence *ac = CCSequence::create(animate, hide,delay,callback, NULL);
			_player->runAction(ac);
		}

		CCARRAY_FOREACH(_bulletArray,obj2)
		{	
			CCSprite* sprite2=(CCSprite*)obj2;
			bulletRect=sprite2->boundingBox();
			if (bulletRect.intersectsRect(enemyRect)&&!_animationLock2)
			{
				SimpleAudioEngine::sharedEngine()->playEffect("enemy0_down.mp3");
				_animationLock2=1;
				score+=1000;
				removeSprite(sprite2);
				_bulletArray->removeObject(sprite2);
				sprite->stopAllActions();
				_scoreLabel->setString(CCString::createWithFormat("%d",score)->getCString());
				CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("game/shoot.plist");
				//敌机爆炸动画
				CCArray *animationFrames = CCArray::create();
				const int animationCount = 4;
				for (int i=1; i <= animationCount; ++i )
				{
					CCString *frameName = CCString::createWithFormat("enemy0_down%d.png",i);
					CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(frameName->getCString());
					animationFrames->addObject(frame);
				}
				CCAnimation *animation = CCAnimation::createWithSpriteFrames(animationFrames);
				animation->setDelayPerUnit(0.05);
				CCAnimate* animate = CCAnimate::create(animation);

				CCCallFuncO *callback = CCCallFuncO::create(this,callfuncO_selector(HelloWorld::onGameOver2),sprite);
				CCHide *hide = CCHide::create();
				CCSequence *ac = CCSequence::create(animate, hide,callback, NULL);
				sprite->runAction(ac);
				//CCLOG("reached!");

				break;
			}
		}
	}
	
	CCARRAY_FOREACH(_enemyArray2,obj1)
	{

		//CCLOG("i am in");
		//CCLOG("i am in2");
		CCSprite *sprite =(CCSprite*)obj1;
		enemyRect=sprite->boundingBox();
		if (enemyRect.intersectsRect(playerRect)&&!_animationLock1)
		{
			this->unschedule(schedule_selector(HelloWorld::gameLoop));
			_animationLock1=1;
			SimpleAudioEngine::sharedEngine()->playEffect("game_over.mp3");
			//玩家爆炸动画
			CCArray *animationFrames = CCArray::create();
			const int animationCount = 4;
			CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("game/shoot.plist");
			for (int i=1; i <= animationCount; ++i )
			{
				CCString *frameName = CCString::createWithFormat("hero_blowup_n%d.png",i);
				CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(frameName->getCString());
				animationFrames->addObject(frame);
			}
			CCAnimation *animation = CCAnimation::createWithSpriteFrames(animationFrames);
			animation->setDelayPerUnit(0.2);
			CCAnimate* animate = CCAnimate::create(animation);

			CCCallFuncO *callback = CCCallFuncO::create(this,callfuncO_selector(HelloWorld::onGameOver),sprite);
			CCDelayTime *delay = CCDelayTime::create(1.0);//可以人为延迟！！！
			CCHide *hide = CCHide::create();
			CCSequence *ac = CCSequence::create(animate, hide,delay,callback, NULL);
			_player->runAction(ac);
		}

		CCARRAY_FOREACH(_bulletArray,obj2)//对每部敌机遍历所有的子弹
		{
			CCSprite* sprite2=(CCSprite*)obj2;//sprite2 子弹精灵
			bulletRect=sprite2->boundingBox();
			if (bulletRect.intersectsRect(enemyRect)&&!_animationLock2)
			{
				SimpleAudioEngine::sharedEngine()->playEffect("enemy1_down.mp3");
				_animationLock2=1;
				score+=6000;
				removeSprite(sprite2);
				_bulletArray->removeObject(sprite2);
				sprite->stopAllActions();
				_scoreLabel->setString(CCString::createWithFormat("%d",score)->getCString());
				CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("game/shoot.plist");
				//敌机爆炸动画
				CCArray *animationFrames = CCArray::create();
				const int animationCount = 4;
				for (int i=1; i <= animationCount; ++i )
				{
					CCString *frameName = CCString::createWithFormat("enemy1_down%d.png",i);
					CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(frameName->getCString());
					animationFrames->addObject(frame);
				}
				CCAnimation *animation = CCAnimation::createWithSpriteFrames(animationFrames);
				animation->setDelayPerUnit(0.05);
				CCAnimate* animate = CCAnimate::create(animation);

				CCCallFuncO *callback = CCCallFuncO::create(this,callfuncO_selector(HelloWorld::onGameOver2),sprite);
				CCHide *hide = CCHide::create();
				CCSequence *ac = CCSequence::create(animate, hide,callback, NULL);
				sprite->runAction(ac);
				//CCLOG("reached!");

				break;
			}
		}
	}
	CCARRAY_FOREACH(_enemyArray3,obj1)
	{
		//CCLOG("i am in");
		//CCLOG("i am in2");
		CCSprite *sprite =(CCSprite*)obj1;
		enemyRect=sprite->boundingBox();
		if (enemyRect.intersectsRect(playerRect)&&!_animationLock1)
		{
			this->unschedule(schedule_selector(HelloWorld::gameLoop));
			_animationLock1=1;
			SimpleAudioEngine::sharedEngine()->playEffect("game_over.mp3");
			//玩家爆炸动画
			CCArray *animationFrames = CCArray::create();
			const int animationCount = 4;
			CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("game/shoot.plist");
			for (int i=1; i <= animationCount; ++i )
			{
				CCString *frameName = CCString::createWithFormat("hero_blowup_n%d.png",i);
				CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(frameName->getCString());
				animationFrames->addObject(frame);
			}
			CCAnimation *animation = CCAnimation::createWithSpriteFrames(animationFrames);
			animation->setDelayPerUnit(0.2);
			CCAnimate* animate = CCAnimate::create(animation);

			CCCallFuncO *callback = CCCallFuncO::create(this,callfuncO_selector(HelloWorld::onGameOver),sprite);
			CCDelayTime *delay = CCDelayTime::create(1.0);//可以人为延迟！！！
			CCHide *hide = CCHide::create();
			CCSequence *ac = CCSequence::create(animate, hide,delay,callback, NULL);
			_player->runAction(ac);
		}

		CCARRAY_FOREACH(_bulletArray,obj2)//对每部敌机遍历所有的子弹
		{
			CCSprite* sprite2=(CCSprite*)obj2;//sprite2 子弹精灵
			bulletRect=sprite2->boundingBox();
			if (bulletRect.intersectsRect(enemyRect)&&!_animationLock2)
			{
				SimpleAudioEngine::sharedEngine()->playEffect("enemy2_down.mp3");
				_animationLock2=1;
				score+=10000;
				removeSprite(sprite2);
				_bulletArray->removeObject(sprite2);
				sprite->stopAllActions();
				_scoreLabel->setString(CCString::createWithFormat("%d",score)->getCString());
				CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("game/shoot.plist");
				//敌机爆炸动画
				CCArray *animationFrames = CCArray::create();
				const int animationCount = 4;
				for (int i=1; i <= animationCount; ++i )
				{
					CCString *frameName = CCString::createWithFormat("enemy2_down%d.png",i);
					CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(frameName->getCString());
					animationFrames->addObject(frame);
				}
				CCAnimation *animation = CCAnimation::createWithSpriteFrames(animationFrames);
				animation->setDelayPerUnit(0.05);
				CCAnimate* animate = CCAnimate::create(animation);

				CCCallFuncO *callback = CCCallFuncO::create(this,callfuncO_selector(HelloWorld::onGameOver2),sprite);
				CCHide *hide = CCHide::create();
				CCSequence *ac = CCSequence::create(animate, hide,callback, NULL);
				sprite->runAction(ac);
				//CCLOG("reached!");

				break;
			}
		}
	}
	CCARRAY_FOREACH(_bonusArray,obj1)
	{

		//CCLOG("i am in");
		//CCLOG("i am in2");
		CCSprite *sprite =(CCSprite*)obj1;
		int tag=sprite->getTag();
		enemyRect=sprite->boundingBox();
		if (enemyRect.intersectsRect(playerRect))
		{
			removeSprite(sprite);
			_bonusArray->removeObject(sprite);
			if (tag==4)//炸弹
			{
				SimpleAudioEngine::sharedEngine()->playEffect("get_bomb.mp3");
				bombLabel->setString(CCString::createWithFormat("%d",++_bombCount)->getCString());
			}
			if (tag==5)//bullet1
			{
				SimpleAudioEngine::sharedEngine()->playEffect("get_double_laser.mp3");
				_bulletMode=1;
			}
		}
	}

	

}


void HelloWorld::onGameOver2(CCObject* sprite)
{
	//removeSprite(sprite);
	removeSprite(sprite);
	_enemyArray->removeObject(sprite);
	_animationLock2=0;
}

void HelloWorld::onGameOver(CCObject* sprite)
{
	removeSprite(sprite);
	_enemyArray->removeObject(sprite);
	removeSprite(_player);
	int _score=CCUserDefault::sharedUserDefault()->getIntegerForKey("score");
	if (score>_score)
	{
		SimpleAudioEngine::sharedEngine()->playEffect("achievement.mp3",true);
		CCUserDefault::sharedUserDefault()->setIntegerForKey("score",score);
	}
	CCUserDefault::sharedUserDefault()->setIntegerForKey("temp_score",score);
	_animationLock1=0;
	CCDirector::sharedDirector()->replaceScene(GameOverScene::scene());
}

void HelloWorld::fireBullet(float dt)
{
	_elapseBullet+=dt;
	if (_bulletMode==0)
	{
	if (_elapseBullet>=0.2)
	{
		CCSprite *bullet=CCSprite::createWithSpriteFrameName("bullet1.png");
		bullet->setScaleX(0.7);
		bullet->setScaleY(0.8);
		CCPoint playerPosition=_player->getPosition();
		CCPoint pos=playerPosition+ccp(0,_player->boundingBox().size.height/2+10);
		bullet->setPosition(pos); 
		this->addChild(bullet);
		bullet->setTag(2);
		_bulletArray->addObject(bullet);
		SimpleAudioEngine::sharedEngine()->playEffect("bullet.mp3",false);

		CCMoveBy *move=CCMoveBy::create(1.0,ccp(0,500));
		CCCallFuncO *callback=CCCallFuncO::create(this,callfuncO_selector(HelloWorld::removeSprite),bullet);
		CCSequence *seq=CCSequence::create(move,callback,NULL);
		bullet->runAction(seq);
		_elapseBullet=0;
	}
	}
	if (_bulletMode==1)
	{
		if (_elapseBullet>=0.5)
		{
			CCSprite *bullet1=CCSprite::createWithSpriteFrameName("bullet1.png");
			CCSprite *bullet2=CCSprite::createWithSpriteFrameName("bullet1.png");
			bullet1->setScaleX(0.7);
			bullet1->setScaleY(0.8);
			bullet2->setScaleX(0.7);
			bullet2->setScaleY(0.8);
			CCPoint playerPosition=_player->getPosition();
			CCPoint pos1=playerPosition+ccp(-10,_player->boundingBox().size.height/2+10);
			CCPoint pos2=playerPosition+ccp(10,_player->boundingBox().size.height/2+10);
			bullet1->setPosition(pos1); 
			bullet2->setPosition(pos2);
			this->addChild(bullet1);
			this->addChild(bullet2);
			bullet1->setTag(2);
			bullet2->setTag(2);
			_bulletArray->addObject(bullet1);
			_bulletArray->addObject(bullet2);

			CCMoveBy *move=CCMoveBy::create(1.0,ccp(0,500));
			CCCallFuncO *callback=CCCallFuncO::create(this,callfuncO_selector(HelloWorld::removeSprite),bullet1);
			CCSequence *seq=CCSequence::create(move,callback,NULL);
			bullet1->runAction(seq);

			CCMoveBy *move2=CCMoveBy::create(1.0,ccp(0,500));
			CCCallFuncO *callback2=CCCallFuncO::create(this,callfuncO_selector(HelloWorld::removeSprite),bullet2);
			CCSequence *seq2=CCSequence::create(move2,callback2,NULL);
			bullet2->runAction(seq2);
			_elapseBullet=0;
		}
	}
}


void HelloWorld::spawnBonus(float dt)
{
	_bonusTime+=dt;//bonus每15秒一次
	if (_bonusTime>=15)
	{
		int randomnumber_temp=(int)(CCRANDOM_0_1()*2);
		int randomnumber=2*randomnumber_temp;
		if (randomnumber>0&&randomnumber<=1)//炸弹
		{
			CCSize winSize=CCDirector::sharedDirector()->getWinSize();
			CCSprite *bomb=CCSprite::createWithSpriteFrameName("prop_type_1.png");
			bomb->setScale(0.5);
			int minX=bomb->boundingBox().size.width/2;
			int maxX=winSize.width-bomb->boundingBox().size.width/2;
			int randomX=(CCRANDOM_0_1()*(maxX-minX))+minX;
			bomb->setPosition(ccp(randomX,winSize.height+bomb->boundingBox().size.height));
			//enemy->setZOrder(30);
			//bomb->setScale(1.5);
			this->addChild(bomb);
			bomb->setTag(4);
			_bonusArray->addObject(bomb);
			CCMoveBy *move=CCMoveBy::create(2,ccp(0,-winSize.height-bomb->boundingBox().size.height));
			CCCallFuncO *callback=CCCallFuncO::create(this,callfuncO_selector(HelloWorld::removeSprite),bomb);
			CCSequence *seq=CCSequence::create(move,callback,NULL);
			bomb->runAction(seq);
		}
		if (randomnumber>1&&randomnumber<=2)//子弹
		{
			CCSize winSize=CCDirector::sharedDirector()->getWinSize();
			CCSprite *bomb=CCSprite::createWithSpriteFrameName("prop_type_0.png");
			bomb->setScale(0.2);
			int minX=bomb->boundingBox().size.width/2;
			int maxX=winSize.width-bomb->boundingBox().size.width/2;
			int randomX=(CCRANDOM_0_1()*(maxX-minX))+minX;
			bomb->setPosition(ccp(randomX,winSize.height+bomb->boundingBox().size.height));
			//enemy->setZOrder(30);
			bomb->setScale(1.5);//子弹体积较小
			this->addChild(bomb);
			bomb->setTag(5);
			_bonusArray->addObject(bomb);
			CCMoveBy *move=CCMoveBy::create(2,ccp(0,-winSize.height-bomb->boundingBox().size.height));
			CCCallFuncO *callback=CCCallFuncO::create(this,callfuncO_selector(HelloWorld::removeSprite),bomb);
			CCSequence *seq=CCSequence::create(move,callback,NULL);
			bomb->runAction(seq);
		}
		_bonusTime=0;
}
}
void HelloWorld::spawnEnemy(float dt)
{
	_elapseEnemy+=dt;
	_elapseEnemy2+=dt;
	_elapseEnemy3+=dt;
	float smalltime=CCRANDOM_0_1()*3+0.3;
	float middletime=CCRANDOM_0_1()*3+4;
	float bigtime=CCRANDOM_0_1()*3+8;
	if (_elapseEnemy>=smalltime)
	{
		CCSize winSize=CCDirector::sharedDirector()->getWinSize();
		CCSprite *enemy=CCSprite::createWithSpriteFrameName("enemy0.png");
		enemy->setScale(0.8);
		int minX=enemy->boundingBox().size.width/2;
		int maxX=winSize.width-enemy->boundingBox().size.width/2;
		int randomX=(CCRANDOM_0_1()*(maxX-minX))+minX;
		enemy->setPosition(ccp(randomX,winSize.height+enemy->boundingBox().size.height));
		//enemy->setZOrder(30);
		//enemy->setScale(0.6);
		this->addChild(enemy);

		enemy->setTag(1);
		_enemyArray->addObject(enemy);
		

		//design a move
		CCMoveBy *move=CCMoveBy::create(3.0,ccp(0,-winSize.height-enemy->boundingBox().size.height));
		CCCallFuncO *callback=CCCallFuncO::create(this,callfuncO_selector(HelloWorld::removeSprite),enemy);
		CCSequence *seq=CCSequence::create(move,callback,NULL);
		enemy->runAction(seq);

		_elapseEnemy=0;
	}
	if (_elapseEnemy2>=middletime)
	{
		CCSize winSize=CCDirector::sharedDirector()->getWinSize();
		CCSprite *enemy2=CCSprite::createWithSpriteFrameName("enemy1.png");
		enemy2->setScale(0.8);
		int minX2=enemy2->boundingBox().size.width/2;
		int maxX2=winSize.width-enemy2->boundingBox().size.width/2;
		int randomX2=(CCRANDOM_0_1()*(maxX2-minX2))+minX2;
		enemy2->setPosition(ccp(randomX2,winSize.height+enemy2->boundingBox().size.height));
		//enemy2->setZOrder(30);
		this->addChild(enemy2,30);

		enemy2->setTag(2);
		_enemyArray2->addObject(enemy2);

		CCMoveBy *move2=CCMoveBy::create(5,ccp(0,-winSize.height-enemy2->boundingBox().size.height));
		CCCallFuncO *callback2=CCCallFuncO::create(this,callfuncO_selector(HelloWorld::removeSprite),enemy2);
		CCSequence *seq2=CCSequence::create(move2,callback2,NULL);
		enemy2->runAction(seq2);

		_elapseEnemy2=0;
	}
	if (_elapseEnemy3>=bigtime)
	{
		CCSize winSize=CCDirector::sharedDirector()->getWinSize();
		CCSprite *enemy3=CCSprite::createWithSpriteFrameName("enemy2.png");
		int minX3=enemy3->boundingBox().size.width/2;
		int maxX3=winSize.width-enemy3->boundingBox().size.width/2;
		int randomX3=(CCRANDOM_0_1()*(maxX3-minX3))+minX3;
		enemy3->setPosition(ccp(randomX3,winSize.height+enemy3->boundingBox().size.height/2));
		//enemy2->setZOrder(30);
		this->addChild(enemy3,30);
		SimpleAudioEngine::sharedEngine()->playEffect("big_spaceship_flying.mp3",true);

		enemy3->setTag(3);
		_enemyArray3->addObject(enemy3);

		CCMoveBy *move3=CCMoveBy::create(6,ccp(0,-winSize.height-enemy3->boundingBox().size.height));
		CCCallFuncO *callback3=CCCallFuncO::create(this,callfuncO_selector(HelloWorld::removeSprite),enemy3);
		CCSequence *seq3=CCSequence::create(move3,callback3,NULL);
		enemy3->runAction(seq3);

		_elapseEnemy3=0;
	}
	
}


void HelloWorld::removeSprite(CCObject* obj)
{
	CCSprite* sprite=(CCSprite*)obj;
	int tag=sprite->getTag();
	if (tag==1)
	{
		_enemyArray->removeObject(sprite);

	}
	if (tag==2)
	{
		_enemyArray2->removeObject(sprite);
	}
	if (tag==3)
	{
		_enemyArray3->removeObject(sprite);
	}
	if (tag==4||tag==5)
	{
		_bonusArray->removeObject(sprite);
	}
	//remove from parent layer
	sprite->removeFromParentAndCleanup(true);
}


void HelloWorld::menuCloseCallback(CCObject* pSender)
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

bool HelloWorld::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	//CCLOG("screen touched");
	CCPoint pt=this->convertTouchToNodeSpace(pTouch);
	CCPoint position=pTouch->getLocationInView();
	position=CCDirector::sharedDirector()->convertToGL(position);
	//_player->setPosition(pt);
	_lastPostion=pt;
	if (_bombCount>=1)
	{
	if (position.x>0&&position.x<63&&position.y>0&&position.y<60)//leave
	{
		//CCDirector::sharedDirector()->popScene();
		CCObject* obj1;
		CCARRAY_FOREACH(_enemyArray,obj1)
		{
			CCSprite *sprite =(CCSprite*)obj1;
			int tag=sprite->getTag();
			if (tag==1)
				score+=1000;
			if (tag==2)
				score+=6000;
			if (tag==3)
				score==10000;
			removeSprite(obj1);
			_bonusArray->removeObject(sprite);
			SimpleAudioEngine::sharedEngine()->playEffect("use_bomb.mp3");
			bombLabel->setString(CCString::createWithFormat("%d",--_bombCount)->getCString());
		}
	}
	}
	if (position.x>0&&position.x<60&&position.y>435&&position.y<480)
	{
		CCLOG("pressed");
		if (CCDirector::sharedDirector()->isPaused())
		{
			SimpleAudioEngine::sharedEngine()->playEffect("button.mp3");
			SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
			CCDirector::sharedDirector()->resume();
			return true;
		}
		else
		{
			SimpleAudioEngine::sharedEngine()->playEffect("button.mp3");
			SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
			CCDirector::sharedDirector()->pause();
			return true;
		}


	}
	
	return true;
}



void HelloWorld::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
	CCPoint pt=this->convertTouchToNodeSpace(pTouch);
	CCPoint offset=pt-_lastPostion;
	CCPoint pos=_player->getPosition()+offset;
	if (pos.x>=_player->boundingBox().size.width*0.6/2 && pos.x<=320 - _player->boundingBox().size.width*0.6/2 && pos.y>=_player->boundingBox().size.height*0.6/2 && pos.y<=480- _player->boundingBox().size.height*0.6/2)
	{
		_player->setPosition(pos);
	}
	_lastPostion=pt;
}
void HelloWorld::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{

}
void HelloWorld::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{

}