#ifndef __ROOM_SCENE_H__
#define __ROOM_SCENE_H__

#include "cocos2d.h"
#include"SimpleAudioEngine.h"

using namespace CocosDenshion;

class RoomScene :public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	SimpleAudioEngine* Sound = SimpleAudioEngine::getInstance();

	virtual bool init();

	void backToMainScene(cocos2d::Ref* pSender);

	//ת����ս����  
	void RoomToMap(Ref* pSender);

	CREATE_FUNC(RoomScene);
};

#endif