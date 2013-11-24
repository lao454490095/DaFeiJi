#ifndef __GAMEOVER_SCENE_H__
#define __GAMEOVER_SCENE_H__

#include "cocos2d.h"
#include "AppDelegate.h"
#include "GameStartScene.h"
using namespace cocos2d;

class GameOverScene : public cocos2d::CCLayer
{
public:
	GameOverScene();
	~GameOverScene();

	virtual bool init();  


	static cocos2d::CCScene* scene();

	void menuCloseCallback(CCObject* pSender);
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);

	CREATE_FUNC(GameOverScene);

private:
};

#endif // __GAMEOVER_SCENE_H__
