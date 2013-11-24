#ifndef __GAMESTART_SCENE_H__
#define __GAMESTART_SCENE_H__

#include "cocos2d.h"
#include "AppDelegate.h"
#include "GameOverScene.h""
using namespace cocos2d;

class GameStartScene : public cocos2d::CCLayer
{
public:
	GameStartScene();
	~GameStartScene();

	virtual bool init();  


	static cocos2d::CCScene* scene();

	void menuCloseCallback(CCObject* pSender);
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);

	CREATE_FUNC(GameStartScene);
};

#endif // _GAMESTART_SCENE_H__
