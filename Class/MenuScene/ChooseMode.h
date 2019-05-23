#ifndef _CHOOSE_MODE_H_
#define _CHOOSE_MODE_H_

#include "cocos2d.h"

class ChooseMode : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	//ת��ѡ��Ӣ�۽���  
	void onPushSceneChooseHero(Ref* pSender);
	//ת�������������  
	void onPushSceneCreateRoom(Ref* pSender);
	//ת�����뷿�����
	void onPushSceneJoinRoom(Ref* pSender);

	CREATE_FUNC(ChooseMode);

};
#endif 
