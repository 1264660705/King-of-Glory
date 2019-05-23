#include"MainScene.h"
#include"SimpleAudioEngine.h"

#include<string>

//��������Ҫ�õ�
using namespace CocosDenshion;
USING_NS_CC;

Scene* MainScene::createScene()
{
	auto scene = Scene::create();
	auto layer = MainScene::create();
	scene->addChild(layer);
	return scene;
}

extern int id;//��ȡ֮ǰѡ��Ӣ������  槼�����ɪ������

bool  MainScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//ѭ����������
	SimpleAudioEngine::getInstance()->playBackgroundMusic("bgm/back.mp3", true);

	//�����ͼ
	map_layer = Layer::create();
	this->addChild(map_layer);
	map = Sprite::create("map/map.png");
	map_layer->addChild(map);
	map_layer->setAnchorPoint(Vec2(0, 0));
	map->setAnchorPoint(Vec2(0, 0));

	//���
	my_hero = HeroSprite::create(id);
	addChild(my_hero);
	my_hero->setPosition(Vec2(960, 540));
	my_hero->setAnchorPoint(Vec2(0.5, 0.5));

	//��ӷ�����
	addTower();

	//����ʱ��
	time_label = Label::create("00:00", "Arial", 50);
	time_label->setColor(Color3B(200, 150, 156));
	this->addChild(time_label);
	time_label->setPosition(Vec2(960, 960));

	//Ӣ������ʱ�ĸ���ʱ�� ����ʱ�ſɼ�
	int time_resurrection = 10;
	std::string resurrection("Resurrection time ");
	resurrection += std::to_string(time_resurrection);
	time_resurrection_label = Label::create(resurrection, "Arial", 50);
	time_resurrection_label->setColor(Color3B(200, 150, 156));
	this->addChild(time_resurrection_label);
	time_resurrection_label->setPosition(Vec2(960, 540));
	time_resurrection_label->setVisible(false);

	//ʤ������ ʤ��ʱ�ɼ�
	win = Sprite::create("win_or_lose/win.png");
	this->addChild(win);
	win->setPosition(Vec2(960, 540));
	win->setVisible(false);

	//ʧ�ܳ��� ʧ��ʱ�ɼ�
	fail = Sprite::create("win_or_lose/lose.png");
	this->addChild(fail);
	fail->setPosition(Vec2(960, 540));
	fail->setVisible(false);

	//���㴥���¼�������
	auto m = EventListenerTouchOneByOne::create();
	//[=]��ֵ���ݲ���ǰ�����ĵ����б���
	m->onTouchBegan = 
	[=](Touch* touch, Event* e)
	{
		//convertToNodeSpace ��Vec2ת��Ϊ�ڵ㣨���أ��ռ�����
		auto touch_position = convertToNodeSpace(touch->getLocation());//�����λ��
		auto my_hero_position = convertToNodeSpace(my_hero->getPosition());//Ӣ��ԭ����λ��
		//��ֹ֮ǰ�Ĳ���
		my_hero->stopAllActions();
		map_layer->stopAllActions();

		/*
		Ӣ���ƶ���ͼ����ƶ�����֤�ٿص�Ӣ��ʼ������Ļx�������
		���������Ӣ�۸��������ƶ���ͼ�㸺�������ƶ�
		MoveBy�Ǵӵ�ǰ������ƶ��������������ô��ľ���
		����ͨ��speed�����ƶ��ٶ�
		������
			duration����������ʱ�䡣
			deltaPosition��ƫ�����ꡣ��������һ�Զ�(x, y)����ֵ
		*/
		int speed = 800;
		my_hero->runAction(MoveBy::create(touch_position.distance(e->getCurrentTarget()->getPosition()) / speed, Vec2(0, touch_position.y - my_hero_position.y))); 
		map_layer->runAction(MoveBy::create(touch_position.distance(e->getCurrentTarget()->getPosition()) / speed, Vec2((my_hero_position.x - touch_position.x), 0)));
		
		return false;
	};

	map_layer->getEventDispatcher()->addEventListenerWithSceneGraphPriority(m, my_hero);

	return true;
}

//��ӷ�����
void MainScene::addTower()
{
	for (int i = 0; i < 4; i++)
	{
		towers.push_back(TowerSprite::create(i));
		map_layer->addChild(towers[i]);
		towers[i]->setZOrder(100);
	}
	towers[0]->setPosition(Vec2(1550, 650));
	towers[1]->setPosition(Vec2(2550, 650));
	towers[2]->setPosition(Vec2(3750, 650));
	towers[3]->setPosition(Vec2(4950, 650));

	//��������Χ��һ��Ȧ
	auto blue_tower_bg = Sprite::create("atk_range/att_a.png");
	map_layer->addChild(blue_tower_bg);
	blue_tower_bg->setPosition(Vec2(2550, 650));
	blue_tower_bg->setColor(Color3B(100, 100, 100));

	auto red_tower_bg = Sprite::create("atk_range/att_a.png");
	map_layer->addChild(red_tower_bg);
	red_tower_bg->setPosition(Vec2(3750, 650));
	red_tower_bg->setColor(Color3B(100, 100, 100));

}

