#ifndef _ROOM_SCENE_H_
#define _ROOM_SCENE_H_

#include "cocos2d.h"

class RoomScene :public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	//ת����ս����  
	void RoomToGame(Ref* pSender);

	CREATE_FUNC(RoomScene);
};

#endif