#include"SoldierSprite.h"
#include<string>
#include<vector>

//��ȡ����Զ���ڳ���ʼ�﹥���ݲ���ʱ����е���                             //�ġ�
int SoldierSprite::get_soldier_atk(int id,int time_minute)
{
	std::vector<int>soldier_atk = { 273,260,350 };
	return (soldier_atk[id % 3] + time_minute * 10);
	
}

//��ȡ����Զ���ڳ���ʼ����������ʱ����е���
int SoldierSprite::get_soldier_def(int id,int time_minute)
{
	std::vector<int>soldier_def = { 45,40,50 };
	return (soldier_def[id % 3] + time_minute * 5);
}

//��ȡ����Զ���ڳ���ʼ����ֵ����ʱ����е���
int SoldierSprite::get_soldier_hp(int id,int time_minute)
{
	std::vector<int>soldier_hp = { 1800,1600,3200 };
	return (soldier_hp[id % 3] + time_minute * 100);
}                                                                          //�ġ�

SoldierSprite::SoldierSprite(int id,int time_minute)//��
{
	soldier.attack = get_soldier_atk(id,time_minute);//��
	soldier.HP = get_soldier_hp(id,time_minute);//��
	soldier.defect = get_soldier_def(id, time_minute);//��
	soldier.attack_distance = 100;
	soldier.survival = true;

	//����������ͬ��ɫ��Ѫ��
	if (id == 0 || id == 1|| id == 2 ) //��
	{
		blood = Sprite::create("blood_bar&blue_bar/bule_h_x.png");
		this->addChild(blood);
		blood->setScaleY(0.8);
		blood->setScaleX(0.5);
		blood->setPosition(Vec2(50, 200));
		soldier.red = false;
	}
	else
	{
		blood = Sprite::create("blood_bar&blue_bar/red_h_x.png");
		this->addChild(blood);
		blood->setScaleY(0.8);
		blood->setScaleX(0.5);
		blood->setPosition(Vec2(50, 200));
		soldier.red = true;
	}

	//�ڳ�������Χ��
	if (id == 2 || id == 5)//��
	{
		soldier.attack_distance = 300;
	}
}

SoldierSprite* SoldierSprite::create(int id,int time_minute)//��
{
	SoldierSprite* soldierSprite = new SoldierSprite(id,time_minute);//��
	soldierSprite->init(id);
	soldierSprite->autorelease();
	return soldierSprite;
}
bool SoldierSprite::init(int id)
{
	//����֣�soldier3���������ڳ��ᴴ����Զ��
	std::vector<std::string> soldier_image =
	{
		"soldiers\\soldier1_blue.png"  ,
		"soldiers\\soldier2_blue.png"  ,//��   Զ��
		"soldiers\\soldier4_blue.png"  ,//��   �ڳ�
		"soldiers\\soldier1_red.png"   ,
		"soldiers\\soldier2_red.png"   ,//��
		"soldiers\\soldier4_red.png"    //��
	};

	//���С��
	Sprite::initWithFile(soldier_image[id]);
	return true;
}


