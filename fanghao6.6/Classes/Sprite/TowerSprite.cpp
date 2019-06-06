#include"TowerSprite.h"
#include<string>
#include<vector>

//��ȡˮ��������ʼ����               //�ġ�
int TowerSprite::get_tower_atk(int id)
{
	std::vector<int>tower_atk = { 724 ,540 };
	return tower_atk[id % 2];
}

//��ȡˮ��������ʼ������ 
int TowerSprite::get_tower_def(int id)
{
	std::vector<int>tower_def = { 200,300 };
	return tower_def[id % 2];
}

//��ȡˮ��������ʼ����ֵ 
int TowerSprite::get_tower_hp(int id)
{
	std::vector<int>tower_hp = { 9000,6000 };
	return tower_hp[id % 2];
}                                             //�ġ�          


//4����0,1,��������2��3�Ǻ췽
TowerSprite::TowerSprite(int id) 
{
	tower.attack = get_tower_atk(id);//��
	tower.HP = get_tower_hp(id);//��
	tower.defect = get_tower_def(id);//��
	tower.attack_distance = 500;
	tower.survival = true;

	//����������ͬ��ɫ��Ѫ��
	if (id == 0 || id == 1)
	{
		blood = Sprite::create("blood_bar&blue_bar/bule_h_x.png");
		this->addChild(blood);
		blood->setScaleY(0.8);
		blood->setScaleX(0.5);
		tower.red = false;
	}
	else
	{
		blood = Sprite::create("blood_bar&blue_bar/red_h_x.png");
		this->addChild(blood);
		blood->setScaleY(0.8);
		blood->setScaleX(0.5);
		tower.red = true;
	}

	//΢��Ѫ����λ��
	switch (id)
	{
	case 0:
		blood->setPosition(Vec2(100, 250));
		break;
	case 1:
		blood->setPosition(Vec2(50, 300));//��
		break;
	case 2:
		blood->setPosition(Vec2(100, 300));
		break;
	case 3:
		blood->setPosition(Vec2(150, 300));
		break;
	default:
		break;
	}
}

TowerSprite* TowerSprite::create(int id)
{
	TowerSprite* towerSprite = new TowerSprite(id);
	towerSprite->init(id);
	towerSprite->autorelease();
	return towerSprite;
}
bool TowerSprite::init(int id)
{
	std::vector<std::string> tower_image =
	{
		"defending_tower/basement_blue.png",
		"defending_tower/tower_blue.png",
		"defending_tower/tower_red.png",
		"defending_tower/basement_red.png"

	};

	//��ӷ�����
	Sprite::initWithFile(tower_image[id]);
	return true;
}


