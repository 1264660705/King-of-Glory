#ifndef _MAIN_SCENE_H_
#define _MAIN_SCENE_H_

#include "cocos2d.h"
#include"../Sprite/TowerSprite.h"
#include"../Sprite/HeroSprite.h"
#include<vector>

class MainScene :public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	CREATE_FUNC(MainScene);

private:
	void addTower();

	cocos2d::Sprite* map;//��ͼ
	cocos2d::Layer* map_layer;//��ͼ��ͼ�㣬������Ҳ��������
	cocos2d::Label* time_label;//��ʾ��Ϸ��ʱ��
	cocos2d::Label* time_resurrection_label;//��ʾӢ�������ĸ���ʱ��
	cocos2d::Sprite* win;//ʤ������
	cocos2d::Sprite* fail;//ʧ�ܳ���
	std::vector<TowerSprite*> towers;
	HeroSprite* my_hero;
};

#endif