/*
*���ඨ����Ӣ�۾��еĸ�������
*������������ݼ��·�ע��
*/
#ifndef _HERO_H_
#define _HERO_H_

#include<vector>
#include<string>
#include<cocos2d.h>

USING_NS_CC;

class Hero
{
public:
	int attack;//������
	int defect;//������     
	int ap_def;//����       
	int ap;//��ǿ         
	int rehp;//��Ѫ      
	double HP;//Ѫ��
	double FullHP;//��Ѫ��    
	double MP;//��             
	double FullMP;//������         
	int attack_distance;//������Χ
	bool survival;//�Ƿ���
	bool red;//�Ƿ��Ǻ췽��
	bool armour_effect = false;//�����׼���Ч���Ƿ���
	bool can_move = true;//�ܷ��ƶ�
	double move_speed;  //�ƶ��ٶ�
	int attack_speed;   //�����ٶ�
	int time = 61;        //���ڳ�����Ч����ʼʱ��ļ�ʱ  

	int level;//�ȼ�
	int hero_exp;//��ǰ����ֵ
	std::vector<int> level_add_attack;//ÿһ�����������Ĺ�����
	std::vector<double>  level_add_HP;//ÿһ������������Ѫ��
	std::vector<int> level_add_experience;//ÿһ����������������Ҫ�ľ���ֵ

	int killed;      //ɱ����
	int be_killed;   //������
	int money;       //ӵ�еĽ��

	int resurrection_time = 10;  //����ʱ��
	std::vector<Menu*>equipments;//ӵ�е�װ�� 6��װ��6������   
	std::vector<Sprite*> equipmentsImages;//ӵ�е�װ�������ľ���ͼ�� //6��װ������12������    
	std::vector<int> equipmentsImages_2;//���̵�װ������ʾ��װ���Ƿ�Ϊ��   //6������  
};

#endif
