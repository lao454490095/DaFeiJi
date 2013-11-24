#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "AppDelegate.h"
#include "GameStartScene.h"
#include "GameOverScene.h""
using namespace cocos2d;
using namespace CocosDenshion;

class HelloWorld : public cocos2d::CCLayer
{
public:
	HelloWorld();
	~HelloWorld();
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();

	void spawnEnemy(float dt);
	void fireBullet(float dt);
	void removeSprite(CCObject *obj);
	void gameLoop(float dt);
	void spawnBonus(float dt);
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
    
    // implement the "static node()" method manually
    CREATE_FUNC(HelloWorld);

	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);

	void onGameOver(CCObject* sprite);
	void onGameOver2(CCObject* sprite);
	void enemy0Animation();

	void collideDetection(float dt);
	int _bulletMode;
private:
	CCSprite *_player;
	CCPoint _lastPostion;
	float _elapseEnemy;
	float _elapseBullet;
	CCArray * _enemyArray;
	CCArray * _bulletArray;
	SimpleAudioEngine *music;
	CCLabelTTF* _scoreLabel;
	CCLabelTTF* bombLabel;
	int score;
	CCArray * _enemyArray2;
	CCArray * _enemyArray3;
	float _elapseEnemy2;
	float _elapseEnemy3;
	float _bonusTime;
	CCArray * _bonusArray;
	int _bombCount;
	int _animationLock1;
	int _animationLock2;
	
};

#endif // __HELLOWORLD_SCENE_H__
