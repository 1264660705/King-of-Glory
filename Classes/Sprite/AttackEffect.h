/*
*������Ч��
*���ݲ�ͬ��Ӣ��id�󶨲�ͬ�Ĺ�����Ч
*/
#ifndef  _ATTACK_EFFECT_H_
#define  _ATTACK_EFFECT_H_

#include "cocos2d.h"

USING_NS_CC;

//������Ч
class AttackEffect :public Sprite
{
public:
	static AttackEffect* create(int id);
	void init(int id);
};

#endif