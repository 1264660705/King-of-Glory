#ifndef _SOLDIER_H_
#define _SOLDIER_H_

class Soldier
{
public:
	int attack;//������
	double HP;//Ѫ��
	int attack_distance;//������Χ
	bool survival;//�Ƿ���
	bool red;//�Ƿ��Ǻ췽��

	bool can_move = true;//�ܷ��ƶ�
	double move_speed;  //�ƶ��ٶ�
	int attack_speed;   //�����ٶ�
};

#endif