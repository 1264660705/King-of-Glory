#ifndef _MAIN_SCENE_H_
#define _MAIN_SCENE_H_

#include "cocos2d.h"
#include"../Sprite/TowerSprite.h"
#include"../Sprite/HeroSprite.h"
#include"../Sprite/SoldierSprite.h"
#include"../Sprite/AttackEffect.h"
#include<vector>

class MainScene :public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	CREATE_FUNC(MainScene);

private:
	void addTower();       //��ӷ�����
	void time(float);      //��ʾʱ�䣬������
	void sport(float);     //�Ƿ��ܹ��˶�
	void ai_hero_attack(float);
	void gameOver(float);
	void tower_attack(float);
	void resurrection(float);	//�������
	void MainScene::display_resurrection(float); //������ʾ�����ǩ
	void MainScene::ai_soldiers_attack(float); //С��ai
	void MainScene::onPushSceneCheckResult(float dt);

	/*
		�������أ���������ֵ�Ѫ������ҷ�������С����Ӣ�۵�
		��һ������Ϊ��Ѫ�Ķ��󣬵ڶ�������Ϊ��Ѫֵ��������)
	*/
	void me_dropBlood(HeroSprite* hero, int attack);//�ҷ�Ӣ�۵�Ѫ
	void ai_dropBlood(HeroSprite* hero, int attack);//�з�Ӣ�۵�Ѫ
	void dropBlood(TowerSprite* tower,int attack);//����Ѫ
	void dropBlood(SoldierSprite* soldier,int attack);//С����Ѫ

	cocos2d::Sprite* map;//��ͼ
	cocos2d::Layer* map_layer;//��ͼ��ͼ�㣬������Ҳ��������
	cocos2d::Label* time_label;//��ʾ��Ϸ��ʱ��
	cocos2d::Label* time_resurrection_label;//��ʾӢ�������ĸ���ʱ��
	//////
	cocos2d::Label* experience_label;//��ʾ����
	cocos2d::Label* level_label;//��ʾ�ȼ�
	cocos2d::Label* money_label;//��ʾ���
	void MainScene::level(float);
	void MainScene::experience(float);
	void MainScene::money(float);
	////////////
	cocos2d::Sprite* win;//ʤ������
	cocos2d::Sprite* fail;//ʧ�ܳ���
	std::vector<TowerSprite*> towers;//������
	HeroSprite* my_hero;//�ҷ�Ӣ��
	HeroSprite* ai_hero;//aiӢ��
	int time_minute = 0;//ʱ�� ��
	int time_second = 0;//ʱ�� ��
	std::vector<SoldierSprite*>blue_soldier_vec;//��ɫ����vec
	std::vector<SoldierSprite*>red_soldier_vec;//��ɫ����vec
	bool hero_survive;
	bool tower_survive_1;
	bool tower_survive_2;
	std::vector<int>soldier_survive;
};

#endif