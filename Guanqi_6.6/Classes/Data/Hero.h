#ifndef _HERO_H_
#define _HERO_H_

#include<vector>
#include<string>

class Hero
{
public:
	std::vector<int> attack;//������
	std::vector<double>  HP;//Ѫ��
	std::vector<int> experience;//ÿһ�ȼ���������

	int attack_distance;//������Χ
	bool survival;//�Ƿ���
	bool red;//�Ƿ��Ǻ췽��

	bool can_move = true;//�ܷ��ƶ�
	double move_speed;  //�ƶ��ٶ�
	int attack_speed;   //�����ٶ�
	int level;//�ȼ�
	int hero_exp;//��ǰ����
	int killed;      //ɱ����
	int be_killed;   //������
	int money;       //ӵ�еĽ��
	int resurrection_time=10;  //����ʱ��
	std::vector<std::string> equipments;//ӵ�е�װ��
};

#endif