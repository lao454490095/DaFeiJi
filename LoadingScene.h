#ifndef __LOADING_SCENE_H__
#define __LOADING_SCENE_H__

#include "cocos2d.h"
#include "AppDelegate.h"
using namespace cocos2d;

class LoadingScene : public cocos2d::CCLayer
{
public:
	LoadingScene();
	~LoadingScene();

	virtual bool init();  


	static cocos2d::CCScene* scene();

	void menuCloseCallback(CCObject* pSender);
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);

	CREATE_FUNC(LoadingScene);

private:
	CCSprite * _plane;
};

#endif // __GAMEOVER_SCENE_H__
