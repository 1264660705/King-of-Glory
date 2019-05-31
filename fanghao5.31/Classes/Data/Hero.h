#ifndef _HERO_H_
#define _HERO_H_

#include<vector>
#include<string>
#include<cocos2d.h>//��
USING_NS_CC;//��

class Hero
{
public:
	int attack;//������
	int defect;//������     //��
	int ap_def;//����       //��
	int ap;//��ǿ         //��
	double HP;//Ѫ��
	double FullHP;//��Ѫ��    //��
	double MP;//��             //��
	double FullMP;//������         //��
	int attack_distance;//������Χ
	bool survival;//�Ƿ���
	bool red;//�Ƿ��Ǻ췽��

	bool can_move = true;//�ܷ��ƶ�
	double move_speed;  //�ƶ��ٶ�
	int attack_speed;   //�����ٶ�

	int experience;  //����
	int killed;      //ɱ����
	int be_killed;   //������
	int money;       //ӵ�еĽ��
	int resurrection_time=10;  //����ʱ��
	std::vector<Menu*>equipments;//ӵ�е�װ�� 6��װ��6������   //��
    std::vector<Sprite*> equipmentsImages;//ӵ�е�װ�������ľ���ͼ�� //6��װ������12������    //��
};

#endif