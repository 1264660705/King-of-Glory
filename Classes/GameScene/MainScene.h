/*
*�����汾�ģ���Ϸ������
*���ݼ���ḻ��ȫ�°汾���������棡
*��Ҫ�����˵�ͼ������Ӣ�۲������з�Ӣ��AI��С��AI��������AI
*�����̵�װ���߼��������߼�����Ҿ���ȼ�ϵͳ�ȵ�
*��ϸ�ľͲ�˵�ˣ���������ע�ͺܷḻ��
*/
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
	void ai_hero_attack(float);//ai�Զ�����
	void gameOver(float);//�ж��Ƿ���Ϸ����
	void tower_attack(float);//����������
	void resurrection(float);	//�������
	void MainScene::display_resurrection(float); //������ʾ�����ǩ
	void MainScene::ai_soldiers_attack(float); //С��ai

	//�̵��һЩ����
	void OpenShop(Ref* pSender);		//���̵����            
	void CloseShop(Ref* pSender);       //�ر��̵����       
	void CloseShop_1(Ref* pSender);       //�ӵڶ�ҳ�ر��̵����       
	void second_page(Ref* pSender);     //�̵�ڶ�ҳ
	void first_page(Ref* pSender);     //�̵��һҳ
	bool create_shop();//�����̵갴ť

	/*
	�������أ���������ֵ�Ѫ������ҷ�������С����Ӣ�۵�
	��һ������Ϊ��Ѫ�Ķ��󣬵ڶ�������Ϊ��Ѫֵ��������)
	*/
	void dropBlood(HeroSprite* hero, int attack);//Ӣ�۵�Ѫ         
	void dropBlood(TowerSprite* tower, int attack, int id);//����Ѫ              
	void dropBlood(SoldierSprite* soldier, int attack, int id);//С����Ѫ          
	void reBlood(HeroSprite* hero);//Ӣ��Ȫˮ��Ѫ    

	//����ȼ�ϵͳ
	bool hero_survive;//�з�Ӣ���Ƿ���
	bool tower_survive_1;//�з�һ���Ƿ���
	bool tower_survive_2;//�з������Ƿ���
	Label* hero_name_label;//��ʾӢ������
	Label* experience_label;//��ʾ����
	Label* level_label;//��ʾ�ȼ�
	void MainScene::level(float);
	void MainScene::experience(float);
	void MainScene::money(float);

	//��Ҫ���ڵ�ͼ�������ƣ�װ���̵�ϵͳ
	cocos2d::Sprite* map;//��ͼ
	cocos2d::Layer* map_layer;//��ͼ��ͼ�㣬������Ҳ��������
	cocos2d::Layer* shop_second_layer;//�̵�ڶ�ҳͼ��  
	cocos2d::Sprite* shop;//�̵�       
	cocos2d::Sprite* shop_second_sprite;//�̵�ڶ�ҳ   
	cocos2d::Sprite* equipments_introduction;//װ������  
	cocos2d::Layer* shop_layer;//�̵�ͼ��             
	cocos2d::Menu* shop_close_menu;//�ر��̵�˵�   
	cocos2d::Menu* shop_close_menu_1;//�ر��̵�˵��ڶ�ҳ��   
	cocos2d::Menu* shop_open_menu;//���̵�˵�   
	cocos2d::Menu* second_page_menu;//�ڶ�ҳ�̵�˵�  
	cocos2d::Menu* first_page_menu;//�ڶ�ҳ�̵�˵�  
	cocos2d::Menu* buy_it_menu;//����װ��                 
	cocos2d::Menu* red_diamond_menu;//�챦ʯ�˵�   
	cocos2d::Menu* blue_diamond_menu;//����ʯ�˵�    
	cocos2d::Menu* mp_book_menu;//��ǿ��˵�          
	cocos2d::Menu* iron_sword_menu;//�����˵�        
	cocos2d::Menu* def_clothes_menu;//���²˵�        
	cocos2d::Menu* def_ap_cloak_menu;//��ħ����˵�        
	cocos2d::Menu* green_diamond_menu;//�̱�ʯ�˵�   
	cocos2d::Menu* basic_shoes_menu;//��Ь�˵�   
	cocos2d::Menu* belt_strength_menu;//���������˵�   
	cocos2d::Menu* belt_strength_after_red_menu;//����������˵�   
	cocos2d::Menu* big_stick_menu;//���   
	cocos2d::Menu* big_stick_after_book_menu;//�����˵�   
	cocos2d::Menu* def_shoes_menu;//����Ь   
	cocos2d::Menu* def_shoes_after_basic_menu;//����Ь��˵�   
	cocos2d::Menu* def_armour_menu; //������   
	cocos2d::Menu* def_armour_after_clothes_menu;//�����׺�˵�   
	cocos2d::Menu* big_sword_menu;//�󽣲˵�        
	cocos2d::Label* time_label;//��ʾ��Ϸ��ʱ��
	cocos2d::Label* money_label;//��ʾ�ҷ�Ӣ�۽��           
	cocos2d::Label* money_label_shop;//���̵�����ʾ�ҷ�Ӣ�۽��           
	cocos2d::Label* time_resurrection_label;//��ʾӢ�������ĸ���ʱ��
	cocos2d::Sprite* win;//ʤ������
	cocos2d::Sprite* fail;//ʧ�ܳ���
	cocos2d::Sprite* passive;//����ͼƬ
	cocos2d::Sprite* passive_describe;//��������ͼƬ
	std::vector<TowerSprite*> towers;//������
	HeroSprite* my_hero;//�ҷ�Ӣ��
	HeroSprite* ai_hero;//aiӢ��
	int ai_hero_id = HeroSprite::myrand();//�������Ӣ��ai��id    
	int time_minute = 0;//ʱ�� ��
	int time_second = 0;//ʱ�� ��
	int red_diamond_num = 0;//����ĺ�ˮ�������������ж��Ƿ���ϳ���������   
	int mp_book_num = 0;
	int basic_shoes_num = 0;
	int def_clothes_num = 0;
	int iron_sword_num = 0;
	int big_sword_num = 0;
	std::vector<SoldierSprite*>blue_soldier_vec;//��ɫ����vec
	std::vector<SoldierSprite*>red_soldier_vec;//��ɫ����vec
	std::vector<int>vecx;//װ����λ��x            
	std::vector<int>vecy;//װ����λ��y   

	//��Ҫ����ս��ҳ��鿴
	bool myhero_survive;
	bool aihero_survive;
	int killed_hero_result;//��ɱӢ����
	int killed_soldier_result;//��ɱС����
	int destroyed_tower_result;//�ݻ�����
	int be_killed_result;//����ɱ��
	void MainScene::myhero_be_killed(float);//����Ӣ�۱���ɱ��
	void CheckResult(Ref* pSender);
	void CloseResult(Ref* pSender);
	bool create_CombatResult();
	cocos2d::Menu* open_combat_result;
	cocos2d::Menu* close_combat_result;
	cocos2d::Layer* result_layer;
	cocos2d::Sprite* result;
	cocos2d::Sprite* avatar_image;
	cocos2d::Layer* hero_image;
	cocos2d::Label* HP_label;//��ʾ����ֵ
	cocos2d::Label* level_result_label;//��ʾ�ȼ�
	cocos2d::Label* attack_label;//��ʾ������
	cocos2d::Label* killed_hero_label;//��ʾ��ɱӢ����
	cocos2d::Label* killed_soldier_label;//��ʾ��ɱС����
	cocos2d::Label* destroyed_tower_label;//��ʾ�ݻ�����
	cocos2d::Label* be_killed_label;//��ʾ����ɱ��
};

#endif
