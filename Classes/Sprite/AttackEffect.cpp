#include"AttackEffect.h"
#include<string>
#include<vector>

AttackEffect* AttackEffect::create(int id)
{
	AttackEffect* effect = new AttackEffect();
	effect->init(id);// 槼� ��ɪ ���� С�� ������ �Ĺ�����Ч
	effect->autorelease();
	return effect;
}
void AttackEffect::init(int id)
{
	//��ɪ������Χ��С������槼��Ĺ�����Чռλ
	const std::vector<std::string> effects = 
	{
		"atk_way/daji_atk.png",
		"atk_way/daji_atk.png",
		"atk_way/houyi_atk.png",
		"atk_way/soldier_atk.png",
		"atk_way/tower_atk.png"
	};

	Sprite::initWithFile(effects[id]);
}


