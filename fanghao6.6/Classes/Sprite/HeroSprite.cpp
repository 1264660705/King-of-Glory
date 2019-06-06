#include"HeroSprite.h"
#include<string>
#include<vector>

//��ȡ槼���ɪ�����ʼ�﹥����                   //�ġ�
int HeroSprite::get_hero_atk(int id)
{
	std::vector<int>hero_atk = { 170,172,188 };
	return hero_atk[id % 3];
}

//��ȡ槼���ɪ�����ʼ������                  
int HeroSprite::get_hero_def(int id)           
{
	std::vector<int>hero_def = { 86,109,86 };
	return hero_def[id % 3];
}                                            

//��ȡ槼���ɪ�����ʼ��                  
int HeroSprite::get_hero_mp(int id)
{
	std::vector<int>hero_mp = { 560,500,500 };
	return hero_mp[id % 3];
}

//��ȡ槼���ɪ�����ʼ��ǿ                 
int HeroSprite::get_hero_ap(int id)
{
	std::vector<int>hero_ap = { 17,2,0 };
	return hero_ap[id % 3];
}

//��ȡ槼���ɪ�����ʼ����                  
int HeroSprite::get_hero_ap_def(int id)
{
	std::vector<int>hero_ap_def = { 8,8,8 };
	return hero_ap_def[id % 3];
}

//��ȡ槼���ɪ�����ʼ����ֵ
int HeroSprite::get_hero_hp(int id)
{
	std::vector<int>hero_hp = { 3383,3776,3182 };
	return hero_hp[id % 3];
}                                             //�ġ�          


HeroSprite::HeroSprite(int id)//��
{
	hero.attack = get_hero_atk(id);//��
	hero.HP = get_hero_hp(id);//��
	hero.FullHP = get_hero_hp(id);//��
	hero.defect = get_hero_def(id);//��
	hero.MP = get_hero_def(id);//��
	hero.FullMP = get_hero_def(id);//��
	hero.ap = get_hero_ap(id);//��
	hero.ap_def = get_hero_ap_def(id);//��
	hero.attack_distance = 400;
	hero.rehp = 20;//��
	hero.money = 10000;//��
	hero.move_speed = 500;//��
	hero.survival = true;

	//����������ͬ��ɫ��Ѫ��
	if (id>=0 && id<=2)
	{
		blood = Sprite::create("blood_bar&blue_bar/bule_h_x.png");
		this->addChild(blood);
		blood->setScaleY(0.8);
		blood->setScaleX(0.5);
		blood->setPosition(Vec2(50, 200));
		hero.red = false;
	}
	else
	{
		blood = Sprite::create("blood_bar&blue_bar/red_h_x.png");
		this->addChild(blood);
		blood->setScaleY(0.8);
		blood->setScaleX(0.5);
		blood->setPosition(Vec2(50, 300));
		hero.red = true;
	}
}

HeroSprite* HeroSprite::create(int id)
{
	HeroSprite* heroSprite = new HeroSprite(id);
	//Ϊ�������ﲻ���ֺ���
	if (id > 2)
	{
		id -= 3;
	}
	heroSprite->init(id);
	heroSprite->autorelease();
	return heroSprite;
}
bool HeroSprite::init(int id)
{
	std::vector<std::string> hero_image =
	{
		"characters/daji/daji_down.png",
		"characters/yase/yase_down.png",
		"characters/houyi/houyi_down.png"
	};

	//���Ӣ��
	Sprite::initWithFile(hero_image[id]);
	return true;
}

 int HeroSprite::myrand()          //�ġ�
{
	srand((unsigned)time(NULL));
	int ai_id = rand() % 3 + 3;
	return ai_id;
}                                  //�ġ�


