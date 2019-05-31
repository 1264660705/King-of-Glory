#ifndef _MAIN_SCENE_H_
#define _MAIN_SCENE_H_

#include "cocos2d.h"
#include"../Sprite/TowerSprite.h"
#include"../Sprite/HeroSprite.h"
#include"../Sprite/SoldierSprite.h"
#include"../Sprite/AttackEffect.h"
#include"../Sprite/ShopMenu.h"
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
	void OpenShop(Ref* pSender);		//���̵����            //�ġ�
	void CloseShop(Ref*pSender);       //�ر��̵����          
	bool create_shop();//�����̵갴ť

	                                                              //�ġ�
	/*
		�������أ���������ֵ�Ѫ������ҷ�������С����Ӣ�۵�
		��һ������Ϊ��Ѫ�Ķ��󣬵ڶ�������Ϊ��Ѫֵ��������)
	*/
	void dropBlood(HeroSprite* hero,int attack);//Ӣ�۵�Ѫ         //��
	void dropBlood(TowerSprite* tower,int attack,int id);//����Ѫ              //��
	void dropBlood(SoldierSprite* soldier,int attack,int id);//С����Ѫ          //��
	void reBlood(HeroSprite* hero);//Ӣ��Ȫˮ��Ѫ          //��

	cocos2d::Sprite* map;//��ͼ
	cocos2d::Layer* map_layer;//��ͼ��ͼ�㣬������Ҳ��������
	cocos2d::Sprite* shop;//�̵�       //��
	cocos2d::Layer* shop_layer;//�̵�ͼ��             //��
	cocos2d::Menu* shop_close_menu;//�ر��̵�˵�   //��
	cocos2d::Menu* shop_open_menu;//���̵�˵�   //��
	cocos2d::Menu* red_diamond_menu;//����챦ʯ�˵�   //��
	cocos2d::Menu* blue_diamond_menu;//��������ʯ�˵�    //��
	cocos2d::Menu* mp_book_menu;//����ǿ��˵�          //��
	cocos2d::Menu* iron_sword_menu;//���������˵�        //��
	cocos2d::Menu* def_clothes_menu;//�����²˵�        //��
	cocos2d::Menu* def_ap_cloak_menu;//����ħ����˵�        //��
	cocos2d::Label* time_label;//��ʾ��Ϸ��ʱ��
	cocos2d::Label* money_label;//��ʾ�ҷ�Ӣ�۽��           //��
	cocos2d::Label* time_resurrection_label;//��ʾӢ�������ĸ���ʱ��
	cocos2d::Sprite* win;//ʤ������
	cocos2d::Sprite* fail;//ʧ�ܳ���
	std::vector<TowerSprite*> towers;//������
	HeroSprite* my_hero;//�ҷ�Ӣ��
	HeroSprite* ai_hero;//aiӢ��
	int ai_hero_id = HeroSprite::myrand();//�������Ӣ��ai��id    //��
	int time_minute = 0;//ʱ�� ��
	int time_second = 0;//ʱ�� ��
	std::vector<SoldierSprite*>blue_soldier_vec;//��ɫ����vec
	std::vector<SoldierSprite*>red_soldier_vec;//��ɫ����vec
	std::vector<int>vecx ;//װ����λ��x            //��
	std::vector<int>vecy ;//װ����λ��y            //��
};

#endif