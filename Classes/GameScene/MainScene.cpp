#include"MainScene.h"
#include"SimpleAudioEngine.h"
#include<iostream>
#include<vector>
#include<string>

using namespace CocosDenshion;//��������Ҫ�õ�

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

	//�����̵���װ������λ�ò���                     
	vecx = { 572,749,926,1103,1280,1457 };
	vecy = { 113 };

	//����̵갴ť         
	create_shop();

	//���
	my_hero = HeroSprite::create(id);
	addChild(my_hero);
	my_hero->setPosition(Vec2(960, 540));
	my_hero->setAnchorPoint(Vec2(0.5, 0.5));

	//��ӷ�����
	addTower();

	//��ʾս������
	create_CombatResult();

	//��ɱ����Ϊ0
	int killed_hero_result=0;//��ɱӢ����
	int killed_soldier_result=0;//��ɱС����
	int destroyed_tower_result=0;//�ݻ�����
	int be_killed_result = 0;//����ɱ��

	//����ʱ��
	time_label = Label::create("00:00", "Arial", 50);
	time_label->setColor(Color3B(200, 150, 156));
	this->addChild(time_label);
	time_label->setPosition(Vec2(960, 960));

	//������                                                                               
	money_label = Label::create(std::to_string(my_hero->hero.money), "Arial", 50);
	money_label->setColor(Color3B(255, 215, 0));
	this->addChild(money_label);
	money_label->setPosition(Vec2(120, 460));

	//��ʾ�ȼ�
	level_label = Label::create("LEVEL: 1", "Arial", 40);
	level_label->setColor(Color3B(50, 200, 200));
	this->addChild(level_label);
	level_label->setPosition(Vec2(120, 960));

	//��ʾ����
	experience_label = Label::create("EXP: 0 / 100", "Arial", 40);
	experience_label->setColor(Color3B(50, 200, 200));
	this->addChild(experience_label);
	experience_label->setPosition(Vec2(120, 880));

	//Ӣ������
	std::string hero_name;
	if (id == 0)
	{
		hero_name = "DAJI";
	}
	else if(id == 1)
	{
		hero_name = "YASE";
	}
	else
	{
		hero_name = "HOUYI";
	}

	hero_name_label = Label::create(hero_name, "fonts/Marker Felt.ttf", 40);
	hero_name_label->setColor(Color3B(50, 200, 200));
	this->addChild(hero_name_label);
	hero_name_label->setPosition(Vec2(120, 780));

	//Ӣ������ʱ�ĸ���ʱ�� ����ʱ�ſɼ�
	int time_resurrection = 10;
	std::string resurrection("Resurrection time: ");
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

	//����ͼƬ
	if (id == 0)
	{
		passive = Sprite::create("passive/daji_passive_png.png");
	}
	else if (id == 1)
	{
		passive = Sprite::create("passive/yase_passive_png.png");
	}
	else
	{
		passive = Sprite::create("passive/houyi_passive_png.png");
	}
	this->addChild(passive);
	passive->setPosition(Vec2(1650, 500));
	
	passive->setVisible(true);

	//��������ͼƬ
	if (id == 0)
	{
		my_hero->hero.attack += my_hero->hero.ap;
		passive_describe = Sprite::create("passive/daji_passive.png");
	}
	else if (id == 1)
	{
		my_hero->hero.defect += 100;
		passive_describe = Sprite::create("passive/yase_passive.png");
	}
	else
	{
		my_hero->hero.attack += 50;
		passive_describe = Sprite::create("passive/houyi_passive.png");
	}
	this->addChild(passive_describe);
	passive_describe->setAnchorPoint(Vec2(1, 0));
	passive_describe->setPosition(Vec2(1900, 80));
	passive_describe->setVisible(true);

	//����
	int ai_id = MainScene::ai_hero_id;//���������,���aiӢ��    
	ai_hero = HeroSprite::create(ai_id);
	map_layer->addChild(ai_hero);
	ai_hero->setPosition(Vec2(6000, 700));

	//���㴥���¼������� �����ҷ�Ӣ��
	auto m = EventListenerTouchOneByOne::create();
	//[=]��ֵ���ݲ���ǰ�����ĵ����б���
	m->onTouchBegan =
		[=](Touch* touch, Event* e)
	{
		//convertToNodeSpace ��Vec2ת��Ϊ�ڵ㣨���أ��ռ�����
		auto touch_position_change = convertToNodeSpace(touch->getLocation());//�����λ��
		auto my_hero_position_change = convertToNodeSpace(my_hero->getPosition());//Ӣ��ԭ����λ��
		//��ֹ֮ǰ�Ĳ���
		my_hero->stopAllActions();
		map_layer->stopAllActions();

		auto my_hero_position = my_hero->getPosition();
		auto ai_hero_position = ai_hero->getPosition();
		auto map_layer_position = map_layer->getPosition();
		auto touch_position = Vec2(touch->getLocation().x - map_layer_position.x, touch->getLocation().y);

		//��������
		//��Ϊ��һ�´���һ�Σ����Թ����ٶȿ��������..
		if (my_hero->hero.survival)
		{
			//�������к�ɫ��
			for (auto& i : red_soldier_vec)
			{
				/*
				��������С������ɫС������ڹ�����Χ֮�� ����С���Ǵ���
				����������Ч��������С����Ѫ
				��һ��listener�У��������жϴ������Ƿ��ھ�������ʱ�����õķ�ʽΪ��
				it->boundingBox().containsPoint(t->getLocation())
				����itΪ����ָ�룬tΪlistener��touch����ָ�룬ͨ�����ַ�ʽ�Ϳ����жϵ����Χ�Ƿ��ھ��鷶Χ�ڡ�
				*/
				if (i->boundingBox().containsPoint(touch_position) && i->getPosition().distance(my_hero_position - map_layer_position) <= my_hero->hero.attack_distance && i->soldier.survival == true)
				{
					int soldier_id = 2;//��ʼΪ2�����ڳ�     
					if (i == red_soldier_vec[0])
					{
						soldier_id = 0;
					}
					if (i == red_soldier_vec[1])
					{
						soldier_id = 1;
					}
					soldier_id += 3;//���ֵ���С��������ʱ�����ж�
					auto hero_attack_effect = AttackEffect::create(id);
					map_layer->addChild(hero_attack_effect);
					hero_attack_effect->setScale(0.5);
					hero_attack_effect->setPosition(my_hero->getPosition() - map_layer_position);
					auto call = CallFuncN::create([=](Node* t)
						{
							if (i->soldier.survival == true)
							{
								dropBlood(i, my_hero->hero.attack, soldier_id);
							}
						});
					//����Ч��
					//��������(Sequence) ��һ�ַ�װ��������Ķ��󣬵��������ִ��ʱ����װ�Ķ�����˳��ִ��
					//�ȷ��У��ٵ�Ѫ��������ʧ
					hero_attack_effect->runAction(
						Sequence::create(MoveTo::create((my_hero_position - map_layer_position).distance(i->getPosition()) / 200, i->getPosition()),
							call, FadeOut::create(0.1), 0));
					return false;
				}
			}

			//�����з�Ӣ��
			if (ai_hero->boundingBox().containsPoint(touch_position) && ai_hero->getPosition().distance(my_hero_position - map_layer_position) <= my_hero->hero.attack_distance && ai_hero->hero.survival == true)
			{
				auto hero_attack_effect = AttackEffect::create(id);
				map_layer->addChild(hero_attack_effect);
				hero_attack_effect->setScale(0.5);
				hero_attack_effect->setPosition(my_hero->getPosition() - map_layer_position);
				if (ai_hero->hero.armour_effect == true)                       //�����׼��ٹ�����Ч��
				{
					my_hero->hero.move_speed = my_hero->hero.move_speed * 0.85;
					my_hero->hero.time = time_second;
				}
				auto call = CallFuncN::create([=](Node* t)
					{
						dropBlood(ai_hero, my_hero->hero.attack);
					});
				hero_attack_effect->runAction(
					Sequence::create(MoveTo::create((my_hero_position - map_layer_position).distance(ai_hero->getPosition()) / 200, ai_hero->getPosition()),
						call, FadeOut::create(0.1), 0));
				return false;
			}

			//������
			for (int i = 2; i <= 3; ++i)
			{
				if (towers[i]->boundingBox().containsPoint(touch_position) && towers[i]->getPosition().distance(my_hero_position - map_layer_position) <= my_hero->hero.attack_distance && towers[i]->tower.survival == true)
				{
					auto hero_attack_effect = AttackEffect::create(id);
					map_layer->addChild(hero_attack_effect);
					hero_attack_effect->setScale(0.5);
					hero_attack_effect->setPosition(my_hero->getPosition() - map_layer_position);
					auto call = CallFuncN::create([=](Node* t)
						{
							dropBlood(towers[i], my_hero->hero.attack, i);
						});
					hero_attack_effect->runAction(
						Sequence::create(MoveTo::create((my_hero_position - map_layer_position).distance(towers[i]->getPosition()) / 200, towers[i]->getPosition()),
							call, FadeOut::create(0.1), 0));
					return false;
				}
			}
		}
		/*
		Ӣ���ƶ���ͼ����ƶ�����֤�ٿص�Ӣ��ʼ������Ļx�������
		���������Ӣ�۸��������ƶ���ͼ�㸺�������ƶ�
		MoveBy�Ǵӵ�ǰ������ƶ��������������ô��ľ���
		����ͨ��speed�����ƶ��ٶ�
		������
			duration����������ʱ�䡣
			deltaPosition��ƫ�����ꡣ��������һ�Զ�(x, y)����ֵ
		*/

		my_hero->runAction(MoveBy::create(touch_position_change.distance(e->getCurrentTarget()->getPosition()) / my_hero->hero.move_speed, Vec2(0, touch_position_change.y - my_hero_position_change.y)));
		map_layer->runAction(MoveBy::create(touch_position_change.distance(e->getCurrentTarget()->getPosition()) / my_hero->hero.move_speed, Vec2((my_hero_position_change.x - touch_position_change.x), 0)));

		return false;
	};
	map_layer->getEventDispatcher()->addEventListenerWithSceneGraphPriority(m, my_hero);

	//�ڶ�������Ϊ�ص��ļ��ʱ�䣬�������������Ĭ����ÿһ֡
	schedule(schedule_selector(MainScene::time), 1);
	schedule(schedule_selector(MainScene::sport));
	schedule(schedule_selector(MainScene::ai_hero_attack), 1.0f);//�����ٶ�д����
	schedule(schedule_selector(MainScene::gameOver));
	schedule(schedule_selector(MainScene::tower_attack), 1.0f);
	schedule(schedule_selector(MainScene::resurrection), 1.0f);
	schedule(schedule_selector(MainScene::display_resurrection));
	schedule(schedule_selector(MainScene::ai_soldiers_attack));//�����ٶ�д����
	schedule(schedule_selector(MainScene::level));
	schedule(schedule_selector(MainScene::experience));
	schedule(schedule_selector(MainScene::money));
	schedule(schedule_selector(MainScene::myhero_be_killed));

	return true;
}

//С��ai
void MainScene::ai_soldiers_attack(float)
{
	//��������ÿ��С��
	for (auto blue_iter = blue_soldier_vec.begin(); blue_iter != blue_soldier_vec.end(); ++blue_iter)
	{
		bool blue_flag = true;
		for (auto red_iter = red_soldier_vec.begin(); red_iter != red_soldier_vec.end(); ++red_iter)
		{
			if ((*blue_iter)->getPosition().distance((*red_iter)->getPosition()) < (*blue_iter)->soldier.attack_distance && (*red_iter)->soldier.survival == true && (*blue_iter)->soldier.survival == true)
			{
				int soldier_id = 2;//Ĭ��ֵΪ2�����ڳ�                           
				if ((red_iter - red_soldier_vec.begin()) % 3 == 0)
				{
					soldier_id = 0;
				}
				if ((red_iter - red_soldier_vec.begin()) % 3 == 1)
				{
					soldier_id = 1;
				}
				blue_flag = false;
				if ((*blue_iter)->soldier.attack_speed == 100)//��ΪΪÿ��ˢ�£����Ծ�b_num�͸�һ��
				{
					auto tower_attack_effect = AttackEffect::create(3);//С��������Ч
					map_layer->addChild(tower_attack_effect);
					tower_attack_effect->setPosition((*blue_iter)->getPosition());
					auto call = CallFuncN::create([=](Node* t)
						{
							dropBlood(*red_iter, (*blue_iter)->soldier.attack, soldier_id);
						});
					tower_attack_effect->runAction(Sequence::create(MoveTo::create(1.2f, (*red_iter)->getPosition()), call, FadeOut::create(0.1), 0));
					(*blue_iter)->soldier.attack_speed = 0;
				}
				else
				{
					++(*blue_iter)->soldier.attack_speed;
				}
			}
		}
		if ((*blue_iter)->getPosition().distance(towers[3]->getPosition()) <= (*blue_iter)->soldier.attack_distance && towers[3]->tower.survival == true && (*blue_iter)->soldier.survival == true)
		{
			blue_flag = false;
			if ((*blue_iter)->soldier.attack_speed == 100)//��ΪΪÿ��ˢ�£����Ծ�b_num�͸�һ��
			{
				auto tower_attack_effect = AttackEffect::create(3);//С��������Ч
				map_layer->addChild(tower_attack_effect);
				tower_attack_effect->setPosition((*blue_iter)->getPosition());
				auto call = CallFuncN::create([=](Node* t)
					{
						dropBlood(towers[3], (*blue_iter)->soldier.attack, 3);
					});
				tower_attack_effect->runAction(Sequence::create(MoveTo::create(1.2f, towers[3]->getPosition()), call, FadeOut::create(0.1), 0));
				(*blue_iter)->soldier.attack_speed = 0;
			}
			else
			{
				++(*blue_iter)->soldier.attack_speed;
			}
		}
		else if ((*blue_iter)->getPosition().distance(towers[2]->getPosition()) <= (*blue_iter)->soldier.attack_distance && towers[2]->tower.survival == true && (*blue_iter)->soldier.survival == true)
		{
			blue_flag = false;
			if ((*blue_iter)->soldier.attack_speed == 100)//��ΪΪÿ��ˢ�£����Ծ�b_num�͸�һ��
			{
				auto tower_attack_effect = AttackEffect::create(3);//С��������Ч
				map_layer->addChild(tower_attack_effect);
				tower_attack_effect->setPosition((*blue_iter)->getPosition());
				auto call = CallFuncN::create([=](Node* t)
					{
						dropBlood(towers[2], (*blue_iter)->soldier.attack, 2);
					});
				tower_attack_effect->runAction(Sequence::create(MoveTo::create(1.2f, towers[2]->getPosition()), call, FadeOut::create(0.1), 0));
				(*blue_iter)->soldier.attack_speed = 0;
			}
			else
			{
				++(*blue_iter)->soldier.attack_speed;
			}
		}
		else if ((*blue_iter)->getPosition().distance(ai_hero->getPosition()) <= (*blue_iter)->soldier.attack_distance && ai_hero->hero.survival == true && (*blue_iter)->soldier.survival == true)
		{
			blue_flag = false;
			if ((*blue_iter)->soldier.attack_speed == 100)//��ΪΪÿ��ˢ�£����Ծ�b_num�͸�һ��
			{
				auto tower_attack_effect = AttackEffect::create(3);//С��������Ч
				map_layer->addChild(tower_attack_effect);
				tower_attack_effect->setPosition((*blue_iter)->getPosition());
				if (ai_hero->hero.armour_effect == true)                       //�����׼��ٹ�����Ч��
				{
					(*blue_iter)->soldier.move_speed = (*blue_iter)->soldier.move_speed * 0.85;
					(*blue_iter)->soldier.time = time_second;
				}
				auto call = CallFuncN::create([=](Node* t)
					{
						dropBlood(ai_hero, (*blue_iter)->soldier.attack);
					});
				tower_attack_effect->runAction(Sequence::create(MoveTo::create(1.2f, ai_hero->getPosition()), call, FadeOut::create(0.1), 0));
				(*blue_iter)->soldier.attack_speed = 0;
			}
			else
			{
				++(*blue_iter)->soldier.attack_speed;
			}
		}

		if (blue_flag == true)
		{
			(*blue_iter)->runAction(MoveBy::create(6 / (*blue_iter)->soldier.move_speed, Vec2(1.5, 0)));
		}
	}

	//���ں췽ÿ��С��
	for (auto red_iter = red_soldier_vec.begin(); red_iter != red_soldier_vec.end(); ++red_iter)
	{
		bool red_flag = true;
		for (auto blue_iter = blue_soldier_vec.begin(); blue_iter != blue_soldier_vec.end(); ++blue_iter)
		{
			if ((*red_iter)->getPosition().distance((*blue_iter)->getPosition()) < (*red_iter)->soldier.attack_distance && (*blue_iter)->soldier.survival == true && (*red_iter)->soldier.survival == true)
			{
				int soldier_id = 2;//Ĭ��ֵΪ2�����ڳ�                            
				if ((blue_iter - blue_soldier_vec.begin()) % 3 == 0)
				{
					soldier_id = 0;
				}
				if ((blue_iter - blue_soldier_vec.begin()) % 3 == 1)
				{
					soldier_id = 1;
				}
				red_flag = false;
				if ((*red_iter)->soldier.attack_speed == 100)//��ΪΪÿ��ˢ�£����Ծ�b_num�͸�һ��
				{
					auto tower_attack_effect = AttackEffect::create(3);//С��������Ч
					map_layer->addChild(tower_attack_effect);
					tower_attack_effect->setPosition((*red_iter)->getPosition());
					auto call = CallFuncN::create([=](Node* t)
						{
							dropBlood(*blue_iter, (*red_iter)->soldier.attack, soldier_id);
						});
					tower_attack_effect->runAction(Sequence::create(MoveTo::create(1.2f, (*blue_iter)->getPosition()), call, FadeOut::create(0.1), 0));
					(*red_iter)->soldier.attack_speed = 0;
				}
				else
				{
					++(*red_iter)->soldier.attack_speed;
				}
			}
		}
		if ((*red_iter)->getPosition().distance(towers[1]->getPosition()) <= (*red_iter)->soldier.attack_distance && towers[1]->tower.survival == true && (*red_iter)->soldier.survival == true)
		{
			red_flag = false;
			if ((*red_iter)->soldier.attack_speed == 100)//��ΪΪÿ��ˢ�£����Ծ�b_num�͸�һ��
			{
				auto tower_attack_effect = AttackEffect::create(3);//С��������Ч
				map_layer->addChild(tower_attack_effect);
				tower_attack_effect->setPosition((*red_iter)->getPosition());
				auto call = CallFuncN::create([=](Node* t)
					{
						dropBlood(towers[1], (*red_iter)->soldier.attack, 1);
					});
				tower_attack_effect->runAction(Sequence::create(MoveTo::create(1.2f, towers[1]->getPosition()), call, FadeOut::create(0.1), 0));
				(*red_iter)->soldier.attack_speed = 0;
			}
			else
			{
				++(*red_iter)->soldier.attack_speed;
			}
		}
		else if ((*red_iter)->getPosition().distance(towers[0]->getPosition()) <= (*red_iter)->soldier.attack_distance && towers[0]->tower.survival == true && (*red_iter)->soldier.survival == true)
		{
			red_flag = false;
			if ((*red_iter)->soldier.attack_speed == 100)//��ΪΪÿ��ˢ�£����Ծ�b_num�͸�һ��
			{
				auto tower_attack_effect = AttackEffect::create(3);//С��������Ч
				map_layer->addChild(tower_attack_effect);
				tower_attack_effect->setPosition((*red_iter)->getPosition());
				auto call = CallFuncN::create([=](Node* t)
					{
						dropBlood(towers[0], (*red_iter)->soldier.attack, 0);
					});
				tower_attack_effect->runAction(Sequence::create(MoveTo::create(1.2f, towers[0]->getPosition()), call, FadeOut::create(0.1), 0));
				(*red_iter)->soldier.attack_speed = 0;
			}
			else
			{
				++(*red_iter)->soldier.attack_speed;
			}
		}
		else if ((*red_iter)->getPosition().distance(my_hero->getPosition() - map_layer->getPosition()) <= (*red_iter)->soldier.attack_distance && my_hero->hero.survival == true && (*red_iter)->soldier.survival == true)
		{
			red_flag = false;
			if ((*red_iter)->soldier.attack_speed == 100)//��ΪΪÿ��ˢ�£����Ծ�b_num�͸�һ��
			{
				auto tower_attack_effect = AttackEffect::create(3);//С��������Ч
				map_layer->addChild(tower_attack_effect);
				tower_attack_effect->setPosition((*red_iter)->getPosition());
				if (my_hero->hero.armour_effect == true)                       //�����׼��ٹ�����Ч��
				{
					(*red_iter)->soldier.move_speed = (*red_iter)->soldier.move_speed * 0.85;
					(*red_iter)->soldier.time = time_second;
				}
				auto call = CallFuncN::create([=](Node* t)
					{
						dropBlood(my_hero, (*red_iter)->soldier.attack);
					});
				tower_attack_effect->runAction(Sequence::create(MoveTo::create(1.2f, my_hero->getPosition() - map_layer->getPosition()), call, FadeOut::create(0.1), 0));
				(*red_iter)->soldier.attack_speed = 0;
			}
			else
			{
				++(*red_iter)->soldier.attack_speed;
			}
		}
		if (red_flag == true)
		{
			(*red_iter)->runAction(MoveBy::create(6 / (*red_iter)->soldier.move_speed, Vec2(-1.5, 0)));
		}
	}

}

//Ϊ���Ǽ�����ʾ�����ǩ�����õ���һ��
void MainScene::display_resurrection(float)
{
	if (my_hero->hero.survival == false)
	{
		time_resurrection_label->setVisible(true);
		map->setColor(Color3B(128, 128, 128));
	}
	else
	{
		time_resurrection_label->setVisible(false);
		map->setColor(Color3B(255, 255, 255));
	}
}

//�������
void MainScene::resurrection(float)
{
	if (my_hero->hero.survival == false && my_hero->hero.resurrection_time == 0)//����ҷ�Ӣ���������Ҹ���ʱ��Ϊ0
	{
		map_layer->setPosition(Vec2(0, 0));
		my_hero->setVisible(true);
		my_hero->hero.HP = my_hero->hero.FullHP;
		my_hero->blood->setScaleX(0.5);
		my_hero->hero.survival = true;
		my_hero->setPosition(Vec2(960, 540));
		my_hero->hero.resurrection_time = 10;
	}

	std::string resurrection_time("Resurrection time: ");
	resurrection_time += std::to_string(my_hero->hero.resurrection_time);
	time_resurrection_label->setString(resurrection_time);
	if (my_hero->hero.survival == false)
	{
		--my_hero->hero.resurrection_time;
	}
	if (ai_hero->hero.survival == false && ai_hero->hero.resurrection_time == 0)
	{
		ai_hero->setVisible(true);
		ai_hero->hero.survival = true;
		ai_hero->hero.HP = ai_hero->hero.FullHP;
		ai_hero->blood->setScaleX(0.5);
		ai_hero->setPosition(Vec2(6000, 700));
		ai_hero->hero.resurrection_time = 10;
	}
	if (ai_hero->hero.survival == false)
	{
		--ai_hero->hero.resurrection_time;
	}
}

//����������
void MainScene::tower_attack(float)
{
	if (my_hero->hero.survival == true)
	{
		reBlood(my_hero);
	}
	if (ai_hero->hero.survival == true)
	{
		reBlood(ai_hero);
	}
	bool flag0 = true;
	if (towers[0]->tower.survival == true)
	{
		for (auto iter = red_soldier_vec.begin(); iter != red_soldier_vec.end() && flag0 == true; ++iter)
		{
			if ((*iter)->getPosition().distance(towers[0]->getPosition()) < towers[0]->tower.attack_distance && (*iter)->soldier.survival == true)
			{
				int soldier_id = 2;//Ĭ��ֵΪ2�����ڳ�                         
				if ((iter - red_soldier_vec.begin()) % 3 == 0)
				{
					soldier_id = 0;
				}
				if ((iter - red_soldier_vec.begin()) % 3 == 1)
				{
					soldier_id = 1;
				}
				flag0 = false;
				auto tower_attack_effect = AttackEffect::create(4);//������������Ч
				map_layer->addChild(tower_attack_effect);
				tower_attack_effect->setScale(0.5);
				tower_attack_effect->setPosition(towers[0]->getPosition());
				auto call = CallFuncN::create([=](Node* t)
					{
						dropBlood((*iter), towers[0]->tower.attack, soldier_id);
					});
				//����Ч��
				tower_attack_effect->runAction(
					Sequence::create(MoveTo::create(1.2f, (*iter)->getPosition()),
						call, FadeOut::create(0.1), 0));
				break;//ֻ����һ��
			}
		}

		if (ai_hero->getPosition().distance(towers[0]->getPosition()) < towers[0]->tower.attack_distance && ai_hero->hero.survival == true && flag0 == true)
		{
			flag0 = false;
			auto tower_attack_effect = AttackEffect::create(4);//������������Ч
			map_layer->addChild(tower_attack_effect);
			tower_attack_effect->setScale(0.5);
			tower_attack_effect->setPosition(towers[0]->getPosition());
			auto call = CallFuncN::create([=](Node* t)
				{
					dropBlood(ai_hero, towers[0]->tower.attack);
				});
			tower_attack_effect->runAction(Sequence::create(MoveTo::create(1.2f, ai_hero->getPosition()), call, FadeOut::create(0.1), 0));
		}
	}

	bool flag1 = true;
	if (towers[1]->tower.survival == true)
	{
		for (auto iter = red_soldier_vec.begin(); iter != red_soldier_vec.end() && flag1 == true; ++iter)
		{
			if ((*iter)->getPosition().distance(towers[1]->getPosition()) < towers[1]->tower.attack_distance && (*iter)->soldier.survival == true)
			{
				int soldier_id = 2;//Ĭ��ֵΪ2�����ڳ�                             
				if ((iter - red_soldier_vec.begin()) % 3 == 0)
				{
					soldier_id = 0;
				}
				if ((iter - red_soldier_vec.begin()) % 3 == 1)
				{
					soldier_id = 1;
				}
				flag1 = false;
				auto tower_attack_effect = AttackEffect::create(4);//������������Ч
				map_layer->addChild(tower_attack_effect);
				tower_attack_effect->setScale(0.5);
				tower_attack_effect->setPosition(towers[1]->getPosition());
				auto call = CallFuncN::create([=](Node* t)
					{
						dropBlood((*iter), towers[1]->tower.attack, soldier_id);
					});
				//����Ч��
				tower_attack_effect->runAction(
					Sequence::create(MoveTo::create(1.2f, (*iter)->getPosition()),
						call, FadeOut::create(0.1), 0));
				break;//ֻ����һ��
			}
		}

		if (ai_hero->getPosition().distance(towers[1]->getPosition()) < towers[1]->tower.attack_distance && ai_hero->hero.survival == true && flag1 == true)
		{
			flag1 = false;
			auto tower_attack_effect = AttackEffect::create(4);//������������Ч
			map_layer->addChild(tower_attack_effect);
			tower_attack_effect->setScale(0.5);
			tower_attack_effect->setPosition(towers[1]->getPosition());
			auto call = CallFuncN::create([=](Node* t)
				{
					dropBlood(ai_hero, towers[1]->tower.attack);
				});
			tower_attack_effect->runAction(Sequence::create(MoveTo::create(1.2f, ai_hero->getPosition()), call, FadeOut::create(0.1), 0));
		}
	}

	bool flag2 = true;
	if (towers[2]->tower.survival == true)
	{
		for (auto iter = blue_soldier_vec.begin(); iter != blue_soldier_vec.end() && flag2 == true; ++iter)
		{
			if ((*iter)->getPosition().distance(towers[2]->getPosition()) < towers[2]->tower.attack_distance && (*iter)->soldier.survival == true)
			{
				int soldier_id = 2;//Ĭ��ֵΪ2�����ڳ�                  
				if ((iter - blue_soldier_vec.begin()) % 3 == 0)
				{
					soldier_id = 0;
				}
				if ((iter - blue_soldier_vec.begin()) % 3 == 1)
				{
					soldier_id = 1;
				}
				flag2 = false;
				auto tower_attack_effect = AttackEffect::create(4);//������������Ч
				map_layer->addChild(tower_attack_effect);
				tower_attack_effect->setScale(0.5);
				tower_attack_effect->setPosition(towers[2]->getPosition());
				auto call = CallFuncN::create([=](Node* t)
					{
						dropBlood((*iter), towers[2]->tower.attack, soldier_id);
					});
				//����Ч��
				tower_attack_effect->runAction(
					Sequence::create(MoveTo::create(1.2f, (*iter)->getPosition()),
						call, FadeOut::create(0.1), 0));
				break;//ֻ����һ��
			}
		}

		if ((my_hero->getPosition() - map_layer->getPosition()).distance(towers[2]->getPosition()) < towers[2]->tower.attack_distance && my_hero->hero.survival == true && flag2 == true)
		{
			flag2 = false;
			auto tower_attack_effect = AttackEffect::create(4);//������������Ч
			map_layer->addChild(tower_attack_effect);
			tower_attack_effect->setScale(0.5);
			tower_attack_effect->setPosition(towers[2]->getPosition());
			auto call = CallFuncN::create([=](Node* t)
				{
					dropBlood(my_hero, towers[2]->tower.attack);
				});
			tower_attack_effect->runAction(Sequence::create(MoveTo::create(1.2f, my_hero->getPosition() - map_layer->getPosition()), call, FadeOut::create(0.1), 0));
		}
	}

	bool flag3 = true;
	if (towers[3]->tower.survival == true)
	{
		for (auto iter = blue_soldier_vec.begin(); iter != blue_soldier_vec.end() && flag3 == true; ++iter)
		{
			if ((*iter)->getPosition().distance(towers[3]->getPosition()) < towers[3]->tower.attack_distance && (*iter)->soldier.survival == true)
			{
				int soldier_id = 2;//Ĭ��ֵΪ2�����ڳ�                          
				if ((iter - blue_soldier_vec.begin()) % 3 == 0)
				{
					soldier_id = 0;
				}
				if ((iter - blue_soldier_vec.begin()) % 3 == 1)
				{
					soldier_id = 1;
				}
				flag3 = false;
				auto tower_attack_effect = AttackEffect::create(4);//������������Ч
				map_layer->addChild(tower_attack_effect);
				tower_attack_effect->setScale(0.5);
				tower_attack_effect->setPosition(towers[3]->getPosition());
				auto call = CallFuncN::create([=](Node* t)
					{
						dropBlood((*iter), towers[3]->tower.attack, soldier_id);
					});
				//����Ч��
				tower_attack_effect->runAction(
					Sequence::create(MoveTo::create(1.2f, (*iter)->getPosition()),
						call, FadeOut::create(0.1), 0));
				break;//ֻ����һ��
			}
		}

		if ((my_hero->getPosition() - map_layer->getPosition()).distance(towers[3]->getPosition()) < towers[3]->tower.attack_distance && my_hero->hero.survival == true && flag3 == true)
		{
			flag3 = false;
			auto tower_attack_effect = AttackEffect::create(4);//������������Ч
			map_layer->addChild(tower_attack_effect);
			tower_attack_effect->setScale(0.5);
			tower_attack_effect->setPosition(towers[3]->getPosition());
			auto call = CallFuncN::create([=](Node* t)
				{
					dropBlood(my_hero, towers[3]->tower.attack);
				});
			tower_attack_effect->runAction(Sequence::create(MoveTo::create(1.2f, my_hero->getPosition() - map_layer->getPosition()), call, FadeOut::create(0.1), 0));
		}
	}
}

//�ж���Ϸ��������
void MainScene::gameOver(float)
{
	if (towers[0]->tower.survival == false)
	{
		fail->setVisible(true);
		Director::getInstance()->stopAnimation();
	}
	if (towers[3]->tower.survival == false)
	{
		win->setVisible(true);
		Director::getInstance()->stopAnimation();
	}
}

//�з�Ӣ�۹���
void MainScene::ai_hero_attack(float)
{
	//����һ��ֻ����һ��
	bool flag = true;
	//����������ɫ�� ��ai��Ҫ�Ǵ���
	for (auto iter = blue_soldier_vec.begin(); iter != blue_soldier_vec.end() && ai_hero->hero.survival == true; ++iter)
	{
		if (ai_hero->getPosition().distance((*iter)->getPosition()) < ai_hero->hero.attack_distance && (*iter)->soldier.survival == true)
		{
			int soldier_id = 2;//Ĭ��ֵΪ2�����ڳ�                           
			if ((iter - blue_soldier_vec.begin()) % 3 == 0)
			{
				soldier_id = 0;
			}
			if ((iter - blue_soldier_vec.begin()) % 3 == 1)
			{
				soldier_id = 1;
			}
			//����Ч���Ѱ�Ӣ��
			flag = false;
			auto hero_attack_effect = AttackEffect::create(MainScene::ai_hero_id - 3);
			map_layer->addChild(hero_attack_effect);
			hero_attack_effect->setScale(0.5);
			hero_attack_effect->setPosition(ai_hero->getPosition());
			auto call = CallFuncN::create([=](Node* t)
				{
					dropBlood((*iter), ai_hero->hero.attack, soldier_id);
				});
			//����Ч��
			hero_attack_effect->runAction(
				Sequence::create(MoveTo::create(1.2f, (*iter)->getPosition()),
					call, FadeOut::create(0.1), 0));
			break;
		}
	}

	//����Ӣ��
	if (ai_hero->getPosition().distance(my_hero->getPosition() - map_layer->getPosition()) < ai_hero->hero.attack_distance
		&& ai_hero->hero.survival == true && my_hero->hero.survival == true && flag == true)
	{
		//����Ч���Ѱ�Ӣ��
		flag = false;
		auto hero_attack_effect = AttackEffect::create(MainScene::ai_hero_id - 3);
		map_layer->addChild(hero_attack_effect);
		hero_attack_effect->setScale(0.5);
		hero_attack_effect->setPosition(ai_hero->getPosition());
		if (my_hero->hero.armour_effect == true)                       //�����׼��ٹ�����Ч��
		{
			ai_hero->hero.move_speed = ai_hero->hero.move_speed * 0.85;
			ai_hero->hero.time = time_second;
		}
		auto call = CallFuncN::create([=](Node* t)
			{
				dropBlood(my_hero, ai_hero->hero.attack);
			});
		//����Ч��
		hero_attack_effect->runAction(
			Sequence::create(MoveTo::create(1.2f, my_hero->getPosition() - map_layer->getPosition()),
				call, FadeOut::create(0.1), 0));
	}

	//������
	for (int i = 0; i <= 1 && ai_hero->hero.survival == true; i++)
	{
		if (ai_hero->getPosition().distance(towers[i]->getPosition()) < ai_hero->hero.attack_distance && towers[i]->tower.survival == true && flag == true)
		{
			flag = false;
			//����Ч���Ѱ�Ӣ��
			auto hero_attack_effect = AttackEffect::create(MainScene::ai_hero_id - 3);
			map_layer->addChild(hero_attack_effect);
			hero_attack_effect->setScale(0.5);
			hero_attack_effect->setPosition(ai_hero->getPosition());
			auto call = CallFuncN::create([=](Node* t)
				{
					dropBlood(towers[i], ai_hero->hero.attack, i);
				});
			//����Ч��
			hero_attack_effect->runAction(
				Sequence::create(MoveTo::create(1.2f, towers[i]->getPosition()),
					call, FadeOut::create(0.1), 0));
			break;
		}
	}
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
	towers[0]->setPosition(Vec2(1550, 550));
	towers[1]->setPosition(Vec2(2550, 550));
	towers[2]->setPosition(Vec2(3750, 550));
	towers[3]->setPosition(Vec2(4950, 550));

	//��������Χ��һ��Ȧ
	auto blue_tower_bg = Sprite::create("atk_range/att_a.png");
	map_layer->addChild(blue_tower_bg);
	blue_tower_bg->setPosition(Vec2(2550, 550));
	blue_tower_bg->setColor(Color3B(100, 100, 100));

	auto blue_basement_bg = Sprite::create("atk_range/att_a.png");
	map_layer->addChild(blue_basement_bg);
	blue_basement_bg->setPosition(Vec2(1550, 550));
	blue_basement_bg->setColor(Color3B(100, 100, 100));

	auto red_tower_bg = Sprite::create("atk_range/att_a.png");
	map_layer->addChild(red_tower_bg);
	red_tower_bg->setPosition(Vec2(3750, 550));
	red_tower_bg->setColor(Color3B(100, 100, 100));

	auto red_basement_bg = Sprite::create("atk_range/att_a.png");
	map_layer->addChild(red_basement_bg);
	red_basement_bg->setPosition(Vec2(4950, 550));
	red_basement_bg->setColor(Color3B(100, 100, 100));

}

//������ʾ��ʱ�䣬���ҳ�����
void MainScene::time(float)
{
	//ÿ���1���
	++my_hero->hero.money;

	std::string time;
	if (time_second == 60)
	{
		++time_minute;
		time_second = 0;
	}
	if (time_minute < 10)
	{
		time += "0";
	}
	time += std::to_string(time_minute);
	if (time_second < 10)
	{
		time += ":0";
	}
	else
	{
		time += ":";
	}
	time += std::to_string(time_second++);
	if (my_hero->hero.armour_effect == true)
	{
		for (int i = 0; i < red_soldier_vec.size(); ++i)
		{
			if (red_soldier_vec[i]->soldier.time != 61)
			{
				red_soldier_vec[i]->soldier.move_speed = red_soldier_vec[i]->soldier.move_speed / 0.85;
				red_soldier_vec[i]->soldier.time = 61;
			}
		}
		if (ai_hero->hero.time != 61)
		{
			ai_hero->hero.move_speed = ai_hero->hero.move_speed / 0.85;
		}
	}
	if (ai_hero->hero.armour_effect == true)
	{
		for (int i = 0; i < blue_soldier_vec.size(); ++i)
		{
			if (blue_soldier_vec[i]->soldier.time != 61)
			{
				blue_soldier_vec[i]->soldier.move_speed = blue_soldier_vec[i]->soldier.move_speed / 0.85;
				blue_soldier_vec[i]->soldier.time = 61;
			}
		}
		if (my_hero->hero.time != 61)
		{
			my_hero->hero.move_speed = my_hero->hero.move_speed / 0.85;
		}
	}
	time_label->setString(time);//��ʾʱ��

	//ÿһ���ӳ�һ����
	if (time_second == 1)//��ս                           
	{
		auto blue_soldier = SoldierSprite::create(0, time_minute);
		map_layer->addChild(blue_soldier);
		blue_soldier_vec.push_back(blue_soldier);
		blue_soldier->setPosition(Vec2(1600, 650));
		//blue_soldier->setPosition(Vec2(2600, 650));

		auto red_soldier = SoldierSprite::create(3, time_minute);
		map_layer->addChild(red_soldier);
		red_soldier_vec.push_back(red_soldier);
		red_soldier->setPosition(Vec2(4900, 650));
	}
	if (time_second == 3)//Զ��                                
	{
		auto blue_soldier = SoldierSprite::create(1, time_minute);
		map_layer->addChild(blue_soldier);
		blue_soldier_vec.push_back(blue_soldier);
		blue_soldier->setPosition(Vec2(1600, 650));

		auto red_soldier = SoldierSprite::create(4, time_minute);
		map_layer->addChild(red_soldier);
		red_soldier_vec.push_back(red_soldier);
		red_soldier->setPosition(Vec2(4900, 650));
	}
	if (time_second == 5)//�ڳ�                     
	{
		auto blue_soldier = SoldierSprite::create(2, time_minute);
		map_layer->addChild(blue_soldier);
		blue_soldier_vec.push_back(blue_soldier);
		blue_soldier->setPosition(Vec2(1600, 650));

		auto red_soldier = SoldierSprite::create(5, time_minute);
		map_layer->addChild(red_soldier);
		red_soldier_vec.push_back(red_soldier);
		red_soldier->setPosition(Vec2(4900, 650));
	}

}

//Ӣ�۵�Ѫ
void MainScene::dropBlood(HeroSprite* hero, int attack)
{
	hero->hero.HP -= (attack * 602 / (602 + hero->hero.defect));

	//�����ţ���Ѫ����Ӧ�仯
	if (hero->hero.HP > 0)
	{
		hero->blood->setScaleX((hero->hero.HP / hero->hero.FullHP) * 0.5);
	}
	//���ˣ�Ѫ����ʧ��ͼƬ��ʧ���߼�Ϊ����
	else
	{
		hero->blood->setScaleX(0);
		hero->setVisible(false);
		hero->hero.survival = false;
	}
}

//Ӣ�������Ѫ                                              
void MainScene::reBlood(HeroSprite* hero)
{
	if (hero->hero.HP < hero->hero.FullHP)
	{
		hero->hero.HP += hero->hero.rehp;
		if (hero->hero.HP > hero->hero.FullHP)
		{
			hero->hero.HP = hero->hero.FullHP;
		}
		hero->blood->setScaleX((hero->hero.HP / hero->hero.FullHP) * 0.5);
	}

}

//����Ѫ
void MainScene::dropBlood(TowerSprite* tower, int attack, int id)
{
	tower->tower.HP -= (attack * 602 / (602 + tower->tower.defect));

	//�����ţ���Ѫ����Ӧ�仯
	if (tower->tower.HP > 0)
	{
		tower->blood->setScaleX((tower->tower.HP / TowerSprite::get_tower_hp(id)) * 0.5);
	}
	//���ˣ�Ѫ����ʧ��ͼƬ��ʧ���߼�Ϊ����
	else
	{
		tower->blood->setScaleX(0);
		tower->setVisible(false);
		tower->tower.survival = false;
	}
}

//С����Ѫ
void MainScene::dropBlood(SoldierSprite* soldier, int attack, int id)
{
	soldier->soldier.HP -= (attack * 602 / (602 + soldier->soldier.defect));

	//�����ţ���Ѫ����Ӧ�仯
	if (soldier->soldier.HP > 0)
	{
		soldier->blood->setScaleX((soldier->soldier.HP / SoldierSprite::get_soldier_hp(id, time_minute)) * 0.5);
	}
	//���ˣ�Ѫ����ʧ��ͼƬ��ʧ���߼�Ϊ����
	else
	{
		if (id >= 3 && attack==my_hero->hero.attack)//�з�С������ �������Ҵ�����
		{
			//��Ǯ�ӽ��
			my_hero->hero.money += 25;
			my_hero->hero.hero_exp += 25;

			++killed_soldier_result;
		}

		soldier->blood->setScaleX(0);
		soldier->setVisible(false);
		soldier->soldier.survival = false;
	}
}

//�Ƿ��ܹ��˶�
void MainScene::sport(float)
{
	//�ҷ�Ӣ������
	if (my_hero->hero.survival == false)
	{
		my_hero->stopAllActions();
	}
	ai_hero->hero.can_move = true;
	for (auto iter = blue_soldier_vec.begin(); iter != blue_soldier_vec.end(); ++iter)
	{
		//���������Χ������ɫ�������򲻿��ƶ�
		if (ai_hero->getPosition().distance((*iter)->getPosition()) < ai_hero->hero.attack_distance && (*iter)->soldier.survival == true)
		{
			ai_hero->hero.can_move = false;
		}
	}
	//������ɫ����1�������߿���ˮ��ʱ
	if (towers[1]->getPosition().distance(ai_hero->getPosition()) < ai_hero->hero.attack_distance && towers[1]->tower.survival == true
		|| towers[0]->getPosition().distance(ai_hero->getPosition()) < ai_hero->hero.attack_distance)
	{
		ai_hero->hero.can_move = false;
	}
	//����Ӣ��ʱ
	if (ai_hero->getPosition().distance(my_hero->getPosition() - map_layer->getPosition()) <= ai_hero->hero.attack_distance && my_hero->hero.survival == true)
	{
		ai_hero->hero.can_move = false;
	}
	//��������ƶ�����ô���ƶ�
	if (ai_hero->hero.can_move == true && ai_hero->hero.survival == true)
	{
		ai_hero->runAction(MoveBy::create(8 / (ai_hero)->hero.move_speed, Vec2(-3, 0)));
	}
}

//�����̵갴ť                    
bool  MainScene::create_shop()
{

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	//��ӽ����̵���Ϸ��ť  
	auto item1 = MenuItemImage::create("shop/shop.png", "shop/shopselected.png", CC_CALLBACK_1(MainScene::OpenShop, this));
	shop_open_menu = Menu::create(item1, NULL);
	shop_open_menu->alignItemsVerticallyWithPadding(50);
	//��ť��������Ļ���м�
	item1->setPosition(Vec2(origin.x - 850, origin.y));
	this->addChild(shop_open_menu, 1);

	return true;
}

//���̵�ص�����
void MainScene::OpenShop(Ref* pSender)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	if (money_label_shop != NULL)//�̵�����Ľ����ʾ������һ��
	{
		money_label_shop->setString(std::to_string(my_hero->hero.money));
	}

	if (shop_layer == NULL)
	{
		//���������̵�
		shop_layer = Layer::create();
		this->addChild(shop_layer, 2);
		shop = Sprite::create("shop/shopopened_first.png");
		shop_layer->addChild(shop);
		shop_layer->setAnchorPoint(Vec2(0, 0));
		shop->setAnchorPoint(Vec2(0, 0));
		shop_open_menu->removeFromParent();//���̵�ʱ��ɾ�����̵갴ť����ֹ��㵼�³���ر��̵�ʱ����

		if (money_label_shop == NULL)
		{
			//������                                                                               
			money_label_shop = Label::create(std::to_string(my_hero->hero.money), "Arial", 50);
			money_label_shop->setColor(Color3B(255, 215, 0));
			this->addChild(money_label_shop, 4);
			money_label_shop->setPosition(Vec2(140, 920));

		}

		//��ӹر��̵���Ϸ��ť  
		auto item1 = MenuItemImage::create("shop/close.png", "shop/closeselected.png", CC_CALLBACK_1(MainScene::CloseShop, this));
		shop_close_menu = Menu::create(item1, NULL);
		shop_close_menu->alignItemsVerticallyWithPadding(50);
		//���ùر��̵갴ť
		item1->setPosition(Vec2(origin.x + 850, origin.y + 425));
		this->addChild(shop_close_menu, 2);

		//����л��̵�ҳ����ť
		auto item_2 = MenuItemImage::create("shop/second_page.png", "shop/second_pageselected.png", CC_CALLBACK_1(MainScene::second_page, this));
		second_page_menu = Menu::create(item_2, NULL);
		second_page_menu->alignItemsVerticallyWithPadding(50);
		//���õڶ�ҳ�̵갴ť
		item_2->setPosition(Vec2(origin.x - 850, origin.y + 130));
		this->addChild(second_page_menu, 2);

		//���װ���챦ʯ
		auto item2 = MenuItemImage::create("shop/red_diamond.png", "shop/red_diamond.png", [&](Ref* sender)
			{
				auto item = MenuItemImage::create("shop/buy_it.png", "shop/buy_it_selected.png", [&](Ref* sender)
					{
						if (my_hero->hero.money > 300 && my_hero->hero.equipments.size() < 6)
						{
							++red_diamond_num;
							auto visibleSize = Director::getInstance()->getVisibleSize();
							Vec2 origin = Director::getInstance()->getVisibleOrigin();
							my_hero->hero.money -= 300;
							money_label->setString(std::to_string(my_hero->hero.money));
							money_label_shop->setString(std::to_string(my_hero->hero.money));
							my_hero->hero.FullHP += 300;
							my_hero->hero.HP += 300;
							my_hero->hero.equipments.push_back(red_diamond_menu);
							int i = my_hero->hero.equipments.size();
							my_hero->hero.equipmentsImages.push_back(Sprite::create("shop/red_diamond1.png"));
							my_hero->hero.equipmentsImages[i * 2 - 2]->setPosition(Vec2(origin.x + vecx[i - 1], origin.y + vecy[0]));
							this->addChild(my_hero->hero.equipmentsImages[i * 2 - 2], 2);
							my_hero->hero.equipmentsImages.push_back(Sprite::create("shop/red_diamond2.png"));
							my_hero->hero.equipmentsImages[i * 2 - 1]->setPosition(Vec2(origin.x + 423, origin.y + 790));
							this->addChild(my_hero->hero.equipmentsImages[i * 2 - 1], 3);

							if (belt_strength_after_red_menu != NULL)   //�������˸��챦ʯ�ʹ���һ�����˺챦ʯ��������˵���������ÿ�ΰ�ǰһ������Ϊ���ɼ�
							{
								belt_strength_after_red_menu->setVisible(false);
							}
							//���װ����������   
							auto item10 = MenuItemImage::create("shop/belt_strength_after_red.png", "shop/belt_strength_after_red.png", [&](Ref* sender)
								{
									auto visibleSize = Director::getInstance()->getVisibleSize();
									Vec2 origin = Director::getInstance()->getVisibleOrigin();
									auto item = MenuItemImage::create("shop/buy_it.png", "shop/buy_it_selected.png", [&](Ref* sender)
										{
											if (my_hero->hero.money > 600)
											{
												my_hero->hero.money -= 600;
												money_label->setString(std::to_string(my_hero->hero.money));
												money_label_shop->setString(std::to_string(my_hero->hero.money));
												my_hero->hero.FullHP += 700;
												my_hero->hero.HP += 700;
												auto visibleSize = Director::getInstance()->getVisibleSize();
												Vec2 origin = Director::getInstance()->getVisibleOrigin();
												int size = my_hero->hero.equipments.size();
												int j = 0;
												for (j = 0; j < size; j++)
												{
													if (my_hero->hero.equipments[j] == red_diamond_menu)
													{
														my_hero->hero.equipments[j] = belt_strength_after_red_menu;
														red_diamond_num--;
														break;
													}
												}
												my_hero->hero.equipmentsImages[j * 2]->setVisible(false);
												my_hero->hero.equipmentsImages[j * 2] = (Sprite::create("shop/belt_strength1.png"));
												my_hero->hero.equipmentsImages[j * 2]->setPosition(Vec2(origin.x + vecx[j], origin.y + vecy[0]));
												this->addChild(my_hero->hero.equipmentsImages[j * 2], 2);
												my_hero->hero.equipmentsImages[j * 2 + 1]->setVisible(false);
												my_hero->hero.equipmentsImages[j * 2 + 1] = (Sprite::create("shop/belt_strength_after_red.png"));
												my_hero->hero.equipmentsImages[j * 2 + 1]->setPosition(Vec2(origin.x + 1358, origin.y + 430));
												this->addChild(my_hero->hero.equipmentsImages[j * 2 + 1], 2);
												if (red_diamond_num == 0)
												{
													if (equipments_introduction != NULL)
													{
														equipments_introduction->removeFromParent();
														equipments_introduction = NULL;
														buy_it_menu->removeFromParent();
														buy_it_menu = NULL;
													}
													if (belt_strength_menu != NULL)            //�˴������Ѵ��̵�ʱ���������������˵���Ϊ���ɼ�����Ȼָ��ָ����֮���ִ����������������˵���ԭ�˵������������
													{
														belt_strength_menu->setVisible(false);
													}
													//���װ����������   
													auto item10 = MenuItemImage::create("shop/belt_strength2.png", "shop/belt_strength2.png", [&](Ref* sender)
														{
															auto visibleSize = Director::getInstance()->getVisibleSize();
															Vec2 origin = Director::getInstance()->getVisibleOrigin();
															auto item = MenuItemImage::create("shop/buy_it.png", "shop/buy_it_selected.png", [&](Ref* sender)
																{
																	if (my_hero->hero.money > 900 && my_hero->hero.equipments.size() < 6)
																	{
																		my_hero->hero.money -= 900;
																		money_label->setString(std::to_string(my_hero->hero.money));
																		money_label_shop->setString(std::to_string(my_hero->hero.money));
																		my_hero->hero.FullHP += 1000;
																		my_hero->hero.HP += 1000;
																		my_hero->hero.equipments.push_back(belt_strength_menu);
																		int i = my_hero->hero.equipments.size();
																		auto visibleSize = Director::getInstance()->getVisibleSize();
																		Vec2 origin = Director::getInstance()->getVisibleOrigin();
																		my_hero->hero.equipmentsImages.push_back(Sprite::create("shop/belt_strength1.png"));
																		my_hero->hero.equipmentsImages[i * 2 - 2]->setPosition(Vec2(origin.x + vecx[i - 1], origin.y + vecy[0]));
																		this->addChild(my_hero->hero.equipmentsImages[i * 2 - 2], 2);
																		my_hero->hero.equipmentsImages.push_back(Sprite::create("shop/belt_strength2.png"));
																		my_hero->hero.equipmentsImages[i * 2 - 1]->setPosition(Vec2(origin.x + 1358, origin.y + 430));
																		this->addChild(my_hero->hero.equipmentsImages[i * 2 - 1], 2);

																	}
																});
															if (equipments_introduction != NULL)
															{
																equipments_introduction->removeFromParent();
															}
															equipments_introduction = Sprite::create("shop/belt_strength3.png");
															equipments_introduction->setPosition(Vec2(1730, 630));
															this->addChild(equipments_introduction, 2);
															if (buy_it_menu != NULL)
															{
																buy_it_menu->removeFromParent();
															}
															buy_it_menu = Menu::create(item, NULL);
															buy_it_menu->alignItemsVerticallyWithPadding(50);
															item->setPosition(Vec2(origin.x + 750, origin.y - 250));
															this->addChild(buy_it_menu, 2);

														});
													belt_strength_menu = Menu::create(item10, NULL);
													belt_strength_menu->alignItemsVerticallyWithPadding(50);
													//������������װ����ť
													item10->setPosition(Vec2(origin.x + 400, origin.y - 110));
													this->addChild(belt_strength_menu, 2);
												}

											}
										});
									if (equipments_introduction != NULL)
									{
										equipments_introduction->removeFromParent();
									}
									equipments_introduction = Sprite::create("shop/belt_strength3.png");
									equipments_introduction->setPosition(Vec2(1730, 630));
									this->addChild(equipments_introduction, 2);
									if (buy_it_menu != NULL)
									{
										buy_it_menu->removeFromParent();
									}
									buy_it_menu = Menu::create(item, NULL);
									buy_it_menu->alignItemsVerticallyWithPadding(50);
									item->setPosition(Vec2(origin.x + 750, origin.y - 250));
									this->addChild(buy_it_menu, 2);

								});
							belt_strength_after_red_menu = Menu::create(item10, NULL);
							belt_strength_after_red_menu->alignItemsVerticallyWithPadding(50);
							//������������װ����ť
							item10->setPosition(Vec2(origin.x + 400, origin.y - 110));
							this->addChild(belt_strength_after_red_menu, 2);
						}
					});
				if (equipments_introduction != NULL)
				{
					equipments_introduction->removeFromParent();
				}
				equipments_introduction = Sprite::create("shop/red_diamond3.png");
				equipments_introduction->setPosition(Vec2(1730, 630));
				this->addChild(equipments_introduction, 2);
				if (buy_it_menu != NULL)
				{
					buy_it_menu->removeFromParent();
				}
				buy_it_menu = Menu::create(item, NULL);
				buy_it_menu->alignItemsVerticallyWithPadding(50);
				item->setPosition(Vec2(origin.x + 750, origin.y - 250));
				this->addChild(buy_it_menu, 2);
			});
		red_diamond_menu = Menu::create(item2, NULL);
		red_diamond_menu->alignItemsVerticallyWithPadding(50);
		//���ú챦ʯװ����ť
		item2->setPosition(Vec2(origin.x - 500, origin.y + 250));
		this->addChild(red_diamond_menu, 3);//zorder��Ϊ3������������ܻᵼ��������//���⣺�������һ���ϳɵĴ󽣺󣬹ر��̵����´�ʱ�챦ʯ����ʾ�Ĳ˵�ΪΪ����ʱ�Ĳ˵�

		//���װ������ʯ
		auto item3 = MenuItemImage::create("shop/blue_diamond.png", "shop/blue_diamond.png", [&](Ref* sender)
			{
				auto item = MenuItemImage::create("shop/buy_it.png", "shop/buy_it_selected.png", [&](Ref* sender)
					{
						if (my_hero->hero.money > 220 && my_hero->hero.equipments.size() < 6)
						{
							my_hero->hero.money -= 220;
							money_label->setString(std::to_string(my_hero->hero.money));
							money_label_shop->setString(std::to_string(my_hero->hero.money));
							my_hero->hero.FullMP += 300;
							my_hero->hero.MP += 300;
							my_hero->hero.equipments.push_back(blue_diamond_menu);
							int i = my_hero->hero.equipments.size();
							auto visibleSize = Director::getInstance()->getVisibleSize();
							Vec2 origin = Director::getInstance()->getVisibleOrigin();
							my_hero->hero.equipmentsImages.push_back(Sprite::create("shop/blue_diamond1.png"));
							my_hero->hero.equipmentsImages[i * 2 - 2]->setPosition(Vec2(origin.x + vecx[i - 1], origin.y + vecy[0]));
							this->addChild(my_hero->hero.equipmentsImages[i * 2 - 2], 2);
							my_hero->hero.equipmentsImages.push_back(Sprite::create("shop/blue_diamond2.png"));
							my_hero->hero.equipmentsImages[i * 2 - 1]->setPosition(Vec2(origin.x + 883, origin.y + 790));
							this->addChild(my_hero->hero.equipmentsImages[i * 2 - 1], 2);

						}
					});
				if (equipments_introduction != NULL)
				{
					equipments_introduction->removeFromParent();
				}
				equipments_introduction = Sprite::create("shop/blue_diamond3.png");
				equipments_introduction->setPosition(Vec2(1730, 630));
				this->addChild(equipments_introduction, 2);
				if (buy_it_menu != NULL)
				{
					buy_it_menu->removeFromParent();
				}
				buy_it_menu = Menu::create(item, NULL);
				buy_it_menu->alignItemsVerticallyWithPadding(50);
				item->setPosition(Vec2(origin.x + 750, origin.y - 250));
				this->addChild(buy_it_menu, 2);
			});
		blue_diamond_menu = Menu::create(item3, NULL);
		blue_diamond_menu->alignItemsVerticallyWithPadding(50);
		//��������ʯװ����ť
		item3->setPosition(Vec2(origin.x - 100, origin.y + 250));
		this->addChild(blue_diamond_menu, 2);

		//���װ����ǿ��
		auto item4 = MenuItemImage::create("shop/mp_book.png", "shop/mp_book.png", [&](Ref* sender)
			{
				auto item = MenuItemImage::create("shop/buy_it.png", "shop/buy_it_selected.png", [&](Ref* sender)
					{
						if (my_hero->hero.money > 300 && my_hero->hero.equipments.size() < 6)
						{
							++mp_book_num;
							my_hero->hero.money -= 300;
							money_label->setString(std::to_string(my_hero->hero.money));
							money_label_shop->setString(std::to_string(my_hero->hero.money));
							my_hero->hero.ap += 40;
							my_hero->hero.equipments.push_back(mp_book_menu);
							int i = my_hero->hero.equipments.size();
							auto visibleSize = Director::getInstance()->getVisibleSize();
							Vec2 origin = Director::getInstance()->getVisibleOrigin();
							my_hero->hero.equipmentsImages.push_back(Sprite::create("shop/mp_book1.png"));
							my_hero->hero.equipmentsImages[i * 2 - 2]->setPosition(Vec2(origin.x + vecx[i - 1], origin.y + vecy[0]));
							this->addChild(my_hero->hero.equipmentsImages[i * 2 - 2], 2);
							my_hero->hero.equipmentsImages.push_back(Sprite::create("shop/mp_book2.png"));
							my_hero->hero.equipmentsImages[i * 2 - 1]->setPosition(Vec2(origin.x + 1333, origin.y + 790));
							this->addChild(my_hero->hero.equipmentsImages[i * 2 - 1], 2);
							if (big_stick_after_book_menu != NULL)   //�������˸���ǿ��ʹ���һ�����˷�ǿ���������˵���������ÿ�ΰ�ǰһ������Ϊ���ɼ�
							{
								big_stick_after_book_menu->setVisible(false);
							}
							//���װ�����   
							auto item11 = MenuItemImage::create("shop/big_stick_after_book.png", "shop/big_stick_after_book.png", [&](Ref* sender)
								{
									auto visibleSize = Director::getInstance()->getVisibleSize();
									Vec2 origin = Director::getInstance()->getVisibleOrigin();
									auto item = MenuItemImage::create("shop/buy_it.png", "shop/buy_it_selected.png", [&](Ref* sender)
										{
											if (my_hero->hero.money > 520)
											{
												my_hero->hero.money -= 520;
												money_label->setString(std::to_string(my_hero->hero.money));
												money_label_shop->setString(std::to_string(my_hero->hero.money));
												my_hero->hero.ap += 80;
												auto visibleSize = Director::getInstance()->getVisibleSize();
												Vec2 origin = Director::getInstance()->getVisibleOrigin();
												int size = my_hero->hero.equipments.size();
												int j = 0;
												for (j = 0; j < size; j++)
												{
													if (my_hero->hero.equipments[j] == mp_book_menu)
													{
														my_hero->hero.equipments[j] = big_stick_after_book_menu;
														mp_book_num--;
														break;
													}
												}
												my_hero->hero.equipmentsImages[j * 2]->setVisible(false);
												my_hero->hero.equipmentsImages[j * 2] = (Sprite::create("shop/big_stick1.png"));
												my_hero->hero.equipmentsImages[j * 2]->setPosition(Vec2(origin.x + vecx[j], origin.y + vecy[0]));
												this->addChild(my_hero->hero.equipmentsImages[j * 2], 2);
												my_hero->hero.equipmentsImages[j * 2 + 1]->setVisible(false);
												my_hero->hero.equipmentsImages[j * 2 + 1] = (Sprite::create("shop/big_stick_after_book.png"));
												my_hero->hero.equipmentsImages[j * 2 + 1]->setPosition(Vec2(origin.x + 438, origin.y + 250));
												this->addChild(my_hero->hero.equipmentsImages[j * 2 + 1], 2);
												if (mp_book_num == 0)
												{
													if (equipments_introduction != NULL)
													{
														equipments_introduction->removeFromParent();
														equipments_introduction = NULL;
														buy_it_menu->removeFromParent();
														buy_it_menu = NULL;
													}
													if (big_stick_menu != NULL)            //�˴������Ѵ��̵�ʱ�����Ĵ���˵���Ϊ���ɼ�����Ȼָ��ָ����֮���ִ������´���˵���ԭ�˵������������
													{
														big_stick_menu->setVisible(false);
													}
													//���װ����� 
													auto item11 = MenuItemImage::create("shop/big_stick2.png", "shop/big_stick2.png", [&](Ref* sender)
														{
															auto visibleSize = Director::getInstance()->getVisibleSize();
															Vec2 origin = Director::getInstance()->getVisibleOrigin();
															auto item = MenuItemImage::create("shop/buy_it.png", "shop/buy_it_selected.png", [&](Ref* sender)
																{
																	if (my_hero->hero.money > 820 && my_hero->hero.equipments.size() < 6)
																	{
																		my_hero->hero.money -= 820;
																		money_label->setString(std::to_string(my_hero->hero.money));
																		money_label_shop->setString(std::to_string(my_hero->hero.money));
																		my_hero->hero.ap += 120;
																		my_hero->hero.equipments.push_back(big_stick_menu);
																		int i = my_hero->hero.equipments.size();
																		auto visibleSize = Director::getInstance()->getVisibleSize();
																		Vec2 origin = Director::getInstance()->getVisibleOrigin();
																		my_hero->hero.equipmentsImages.push_back(Sprite::create("shop/big_stick1.png"));
																		my_hero->hero.equipmentsImages[i * 2 - 2]->setPosition(Vec2(origin.x + vecx[i - 1], origin.y + vecy[0]));
																		this->addChild(my_hero->hero.equipmentsImages[i * 2 - 2], 2);
																		my_hero->hero.equipmentsImages.push_back(Sprite::create("shop/big_stick2.png"));
																		my_hero->hero.equipmentsImages[i * 2 - 1]->setPosition(Vec2(origin.x + 438, origin.y + 250));
																		this->addChild(my_hero->hero.equipmentsImages[i * 2 - 1], 2);

																	}
																});
															if (equipments_introduction != NULL)
															{
																equipments_introduction->removeFromParent();
															}
															equipments_introduction = Sprite::create("shop/big_stick3.png");
															equipments_introduction->setPosition(Vec2(1730, 630));
															this->addChild(equipments_introduction, 2);
															if (buy_it_menu != NULL)
															{
																buy_it_menu->removeFromParent();
															}
															buy_it_menu = Menu::create(item, NULL);
															buy_it_menu->alignItemsVerticallyWithPadding(50);
															item->setPosition(Vec2(origin.x + 750, origin.y - 250));
															this->addChild(buy_it_menu, 2);

														});
													big_stick_menu = Menu::create(item11, NULL);
													big_stick_menu->alignItemsVerticallyWithPadding(50);
													//���ô��װ����ť
													item11->setPosition(Vec2(origin.x - 520, origin.y - 290));
													this->addChild(big_stick_menu, 2);
												}

											}
										});
									if (equipments_introduction != NULL)
									{
										equipments_introduction->removeFromParent();
									}
									equipments_introduction = Sprite::create("shop/big_stick3.png");
									equipments_introduction->setPosition(Vec2(1730, 630));
									this->addChild(equipments_introduction, 2);
									if (buy_it_menu != NULL)
									{
										buy_it_menu->removeFromParent();
									}
									buy_it_menu = Menu::create(item, NULL);
									buy_it_menu->alignItemsVerticallyWithPadding(50);
									item->setPosition(Vec2(origin.x + 750, origin.y - 250));
									this->addChild(buy_it_menu, 2);

								});
							big_stick_after_book_menu = Menu::create(item11, NULL);
							big_stick_after_book_menu->alignItemsVerticallyWithPadding(50);
							//���ô��װ����ť
							item11->setPosition(Vec2(origin.x - 520, origin.y - 290));
							this->addChild(big_stick_after_book_menu, 2);
						}
					});
				if (equipments_introduction != NULL)
				{
					equipments_introduction->removeFromParent();
				}
				equipments_introduction = Sprite::create("shop/mp_book3.png");
				equipments_introduction->setPosition(Vec2(1730, 630));
				this->addChild(equipments_introduction, 2);
				if (buy_it_menu != NULL)
				{
					buy_it_menu->removeFromParent();
				}
				buy_it_menu = Menu::create(item, NULL);
				buy_it_menu->alignItemsVerticallyWithPadding(50);
				item->setPosition(Vec2(origin.x + 750, origin.y - 250));
				this->addChild(buy_it_menu, 2);
			});
		mp_book_menu = Menu::create(item4, NULL);
		mp_book_menu->alignItemsVerticallyWithPadding(50);
		//���÷�ǿ��װ����ť
		item4->setPosition(Vec2(origin.x + 380, origin.y + 250));
		this->addChild(mp_book_menu, 2);

		//���װ������
		auto item5 = MenuItemImage::create("shop/iron_sword.png", "shop/iron_sword.png", [&](Ref* sender)
			{
				auto item = MenuItemImage::create("shop/buy_it.png", "shop/buy_it_selected.png", [&](Ref* sender)
					{
						if (my_hero->hero.money > 250 && my_hero->hero.equipments.size() < 6)
						{
							iron_sword_num++;
							my_hero->hero.money -= 250;
							money_label->setString(std::to_string(my_hero->hero.money));
							money_label_shop->setString(std::to_string(my_hero->hero.money));
							my_hero->hero.attack += 20;
							my_hero->hero.equipments.push_back(iron_sword_menu);
							int i = my_hero->hero.equipments.size();
							auto visibleSize = Director::getInstance()->getVisibleSize();
							Vec2 origin = Director::getInstance()->getVisibleOrigin();
							my_hero->hero.equipmentsImages.push_back(Sprite::create("shop/iron_sword1.png"));
							my_hero->hero.equipmentsImages[i * 2 - 2]->setPosition(Vec2(origin.x + vecx[i - 1], origin.y + vecy[0]));
							this->addChild(my_hero->hero.equipmentsImages[i * 2 - 2], 2);
							my_hero->hero.equipmentsImages.push_back(Sprite::create("shop/iron_sword2.png"));
							my_hero->hero.equipmentsImages[i * 2 - 1]->setPosition(Vec2(origin.x + 438, origin.y + 610));
							this->addChild(my_hero->hero.equipmentsImages[i * 2 - 1], 2);

						}
					});
				if (equipments_introduction != NULL)
				{
					equipments_introduction->removeFromParent();
				}
				equipments_introduction = Sprite::create("shop/iron_sword3.png");
				equipments_introduction->setPosition(Vec2(1730, 630));
				this->addChild(equipments_introduction, 2);
				if (buy_it_menu != NULL)
				{
					buy_it_menu->removeFromParent();
				}
				buy_it_menu = Menu::create(item, NULL);
				buy_it_menu->alignItemsVerticallyWithPadding(50);
				item->setPosition(Vec2(origin.x + 750, origin.y - 250));
				this->addChild(buy_it_menu, 2);
			});
		iron_sword_menu = Menu::create(item5, NULL);
		iron_sword_menu->alignItemsVerticallyWithPadding(50);
		//��������װ����ť
		item5->setPosition(Vec2(origin.x - 520, origin.y + 70));
		this->addChild(iron_sword_menu, 2);

		//���װ������
		auto item6 = MenuItemImage::create("shop/def_clothes.png", "shop/def_clothes.png", [&](Ref* sender)
			{
				auto item = MenuItemImage::create("shop/buy_it.png", "shop/buy_it_selected.png", [&](Ref* sender)
					{
						if (my_hero->hero.money > 220 && my_hero->hero.equipments.size() < 6)
						{
							++def_clothes_num;
							my_hero->hero.money -= 220;
							money_label->setString(std::to_string(my_hero->hero.money));
							money_label_shop->setString(std::to_string(my_hero->hero.money));
							my_hero->hero.defect += 60;
							my_hero->hero.equipments.push_back(def_clothes_menu);
							int i = my_hero->hero.equipments.size();
							auto visibleSize = Director::getInstance()->getVisibleSize();
							Vec2 origin = Director::getInstance()->getVisibleOrigin();
							my_hero->hero.equipmentsImages.push_back(Sprite::create("shop/def_clothes1.png"));
							my_hero->hero.equipmentsImages[i * 2 - 2]->setPosition(Vec2(origin.x + vecx[i - 1], origin.y + vecy[0]));
							this->addChild(my_hero->hero.equipmentsImages[i * 2 - 2], 2);
							my_hero->hero.equipmentsImages.push_back(Sprite::create("shop/def_clothes2.png"));
							my_hero->hero.equipmentsImages[i * 2 - 1]->setPosition(Vec2(origin.x + 858, origin.y + 610));
							this->addChild(my_hero->hero.equipmentsImages[i * 2 - 1], 2);
							if (def_armour_after_clothes_menu != NULL)   //�������˸����¾ʹ���һ�����˲��º�ķ����ײ˵���������ÿ�ΰ�ǰһ������Ϊ���ɼ�
							{
								def_armour_after_clothes_menu->setVisible(false);
							}
							//���װ��������
							auto item13 = MenuItemImage::create("shop/def_armour_after_clothes.png", "shop/def_armour_after_clothes.png", [&](Ref* sender)
								{
									auto visibleSize = Director::getInstance()->getVisibleSize();
									Vec2 origin = Director::getInstance()->getVisibleOrigin();
									auto item = MenuItemImage::create("shop/buy_it.png", "shop/buy_it_selected.png", [&](Ref* sender)
										{
											if (my_hero->hero.money > 510)
											{
												my_hero->hero.money -= 510;
												money_label->setString(std::to_string(my_hero->hero.money));
												money_label_shop->setString(std::to_string(my_hero->hero.money));
												my_hero->hero.defect += 120;
												my_hero->hero.armour_effect = true;
												auto visibleSize = Director::getInstance()->getVisibleSize();
												Vec2 origin = Director::getInstance()->getVisibleOrigin();
												int size = my_hero->hero.equipments.size();
												int j = 0;
												for (j = 0; j < size; j++)
												{
													if (my_hero->hero.equipments[j] == def_clothes_menu)
													{
														my_hero->hero.equipments[j] = def_armour_after_clothes_menu;
														def_clothes_num--;
														break;
													}
												}
												my_hero->hero.equipmentsImages[j * 2]->setVisible(false);
												my_hero->hero.equipmentsImages[j * 2] = (Sprite::create("shop/def_armour1.png"));
												my_hero->hero.equipmentsImages[j * 2]->setPosition(Vec2(origin.x + vecx[j], origin.y + vecy[0]));
												this->addChild(my_hero->hero.equipmentsImages[j * 2], 2);
												my_hero->hero.equipmentsImages[j * 2 + 1]->setVisible(false);
												my_hero->hero.equipmentsImages[j * 2 + 1] = (Sprite::create("shop/def_armour_after_clothes.png"));
												my_hero->hero.equipmentsImages[j * 2 + 1]->setPosition(Vec2(origin.x + 1358, origin.y + 250));
												this->addChild(my_hero->hero.equipmentsImages[j * 2 + 1], 2);
												if (def_clothes_num == 0)
												{
													if (equipments_introduction != NULL)
													{
														equipments_introduction->removeFromParent();
														equipments_introduction = NULL;
														buy_it_menu->removeFromParent();
														buy_it_menu = NULL;
													}
													if (def_armour_menu != NULL)            //�˴������Ѵ��̵�ʱ�����ķ����ײ˵���Ϊ���ɼ�����Ȼָ��ָ����֮���ִ������·����ײ˵���ԭ�˵������������
													{
														def_armour_menu->setVisible(false);
													}
													//���װ��������
													auto item13 = MenuItemImage::create("shop/def_armour2.png", "shop/def_armour2.png", [&](Ref* sender)
														{
															auto visibleSize = Director::getInstance()->getVisibleSize();
															Vec2 origin = Director::getInstance()->getVisibleOrigin();
															auto item = MenuItemImage::create("shop/buy_it.png", "shop/buy_it_selected.png", [&](Ref* sender)
																{
																	if (my_hero->hero.money > 730 && my_hero->hero.equipments.size() < 6)
																	{
																		my_hero->hero.money -= 730;
																		money_label->setString(std::to_string(my_hero->hero.money));
																		money_label_shop->setString(std::to_string(my_hero->hero.money));
																		my_hero->hero.defect += 210;
																		my_hero->hero.armour_effect = true;
																		my_hero->hero.equipments.push_back(def_armour_menu);
																		int i = my_hero->hero.equipments.size();
																		auto visibleSize = Director::getInstance()->getVisibleSize();
																		Vec2 origin = Director::getInstance()->getVisibleOrigin();
																		my_hero->hero.equipmentsImages.push_back(Sprite::create("shop/def_armour1.png"));
																		my_hero->hero.equipmentsImages[i * 2 - 2]->setPosition(Vec2(origin.x + vecx[i - 1], origin.y + vecy[0]));
																		this->addChild(my_hero->hero.equipmentsImages[i * 2 - 2], 2);
																		my_hero->hero.equipmentsImages.push_back(Sprite::create("shop/def_armour2.png"));
																		my_hero->hero.equipmentsImages[i * 2 - 1]->setPosition(Vec2(origin.x + 1358, origin.y + 250));
																		this->addChild(my_hero->hero.equipmentsImages[i * 2 - 1], 2);

																	}
																});
															if (equipments_introduction != NULL)
															{
																equipments_introduction->removeFromParent();
															}
															equipments_introduction = Sprite::create("shop/def_armour3.png");
															equipments_introduction->setPosition(Vec2(1730, 630));
															this->addChild(equipments_introduction, 2);
															if (buy_it_menu != NULL)
															{
																buy_it_menu->removeFromParent();
															}
															buy_it_menu = Menu::create(item, NULL);
															buy_it_menu->alignItemsVerticallyWithPadding(50);
															item->setPosition(Vec2(origin.x + 750, origin.y - 250));
															this->addChild(buy_it_menu, 2);

														});
													def_armour_menu = Menu::create(item13, NULL);
													def_armour_menu->alignItemsVerticallyWithPadding(50);
													//���÷�����װ����ť
													item13->setPosition(Vec2(origin.x + 400, origin.y - 290));
													this->addChild(def_armour_menu, 2);
												}

											}
										});
									if (equipments_introduction != NULL)
									{
										equipments_introduction->removeFromParent();
									}
									equipments_introduction = Sprite::create("shop/def_armour3.png");
									equipments_introduction->setPosition(Vec2(1730, 630));
									this->addChild(equipments_introduction, 2);
									if (buy_it_menu != NULL)
									{
										buy_it_menu->removeFromParent();
									}
									buy_it_menu = Menu::create(item, NULL);
									buy_it_menu->alignItemsVerticallyWithPadding(50);
									item->setPosition(Vec2(origin.x + 750, origin.y - 250));
									this->addChild(buy_it_menu, 2);

								});
							def_armour_after_clothes_menu = Menu::create(item13, NULL);
							def_armour_after_clothes_menu->alignItemsVerticallyWithPadding(50);
							//���÷�����װ����ť
							item13->setPosition(Vec2(origin.x + 400, origin.y - 290));
							this->addChild(def_armour_after_clothes_menu, 2);

						}
					});
				if (equipments_introduction != NULL)
				{
					equipments_introduction->removeFromParent();
				}
				equipments_introduction = Sprite::create("shop/def_clothes3.png");
				equipments_introduction->setPosition(Vec2(1730, 630));
				this->addChild(equipments_introduction, 2);
				if (buy_it_menu != NULL)
				{
					buy_it_menu->removeFromParent();
				}
				buy_it_menu = Menu::create(item, NULL);
				buy_it_menu->alignItemsVerticallyWithPadding(50);
				item->setPosition(Vec2(origin.x + 750, origin.y - 250));
				this->addChild(buy_it_menu, 2);
			});
		def_clothes_menu = Menu::create(item6, NULL);
		def_clothes_menu->alignItemsVerticallyWithPadding(50);
		//���ò���װ����ť
		item6->setPosition(Vec2(origin.x - 100, origin.y + 70));
		this->addChild(def_clothes_menu, 2);

		//���װ����ħ����
		auto item7 = MenuItemImage::create("shop/def_ap_cloak.png", "shop/def_ap_cloak.png", [&](Ref* sender)
			{
				auto item = MenuItemImage::create("shop/buy_it.png", "shop/buy_it_selected.png", [&](Ref* sender)
					{
						if (my_hero->hero.money > 220 && my_hero->hero.equipments.size() < 6)
						{
							my_hero->hero.money -= 220;
							money_label->setString(std::to_string(my_hero->hero.money));
							money_label_shop->setString(std::to_string(my_hero->hero.money));
							my_hero->hero.ap_def += 60;
							my_hero->hero.equipments.push_back(def_ap_cloak_menu);
							int i = my_hero->hero.equipments.size();
							auto visibleSize = Director::getInstance()->getVisibleSize();
							Vec2 origin = Director::getInstance()->getVisibleOrigin();
							my_hero->hero.equipmentsImages.push_back(Sprite::create("shop/def_ap_cloak1.png"));
							my_hero->hero.equipmentsImages[i * 2 - 2]->setPosition(Vec2(origin.x + vecx[i - 1], origin.y + vecy[0]));
							this->addChild(my_hero->hero.equipmentsImages[i * 2 - 2], 2);
							my_hero->hero.equipmentsImages.push_back(Sprite::create("shop/def_ap_cloak2.png"));
							my_hero->hero.equipmentsImages[i * 2 - 1]->setPosition(Vec2(origin.x + 1338, origin.y + 610));
							this->addChild(my_hero->hero.equipmentsImages[i * 2 - 1], 2);

						}
					});
				if (equipments_introduction != NULL)
				{
					equipments_introduction->removeFromParent();
				}
				equipments_introduction = Sprite::create("shop/def_ap_cloak3.png");
				equipments_introduction->setPosition(Vec2(1730, 630));
				this->addChild(equipments_introduction, 2);
				if (buy_it_menu != NULL)
				{
					buy_it_menu->removeFromParent();
				}
				buy_it_menu = Menu::create(item, NULL);
				buy_it_menu->alignItemsVerticallyWithPadding(50);
				item->setPosition(Vec2(origin.x + 750, origin.y - 250));
				this->addChild(buy_it_menu, 2);
			});
		def_ap_cloak_menu = Menu::create(item7, NULL);
		def_ap_cloak_menu->alignItemsVerticallyWithPadding(50);
		//���ÿ�ħ����װ����ť
		item7->setPosition(Vec2(origin.x + 380, origin.y + 70));
		this->addChild(def_ap_cloak_menu, 2);

		//���װ����ˮ��
		auto item8 = MenuItemImage::create("shop/green_diamond.png", "shop/green_diamond.png", [&](Ref* sender)
			{
				auto item = MenuItemImage::create("shop/buy_it.png", "shop/buy_it_selected.png", [&](Ref* sender)
					{
						if (my_hero->hero.money > 140 && my_hero->hero.equipments.size() < 6)
						{
							my_hero->hero.money -= 140;
							money_label->setString(std::to_string(my_hero->hero.money));
							money_label_shop->setString(std::to_string(my_hero->hero.money));
							my_hero->hero.rehp += 6;
							my_hero->hero.equipments.push_back(green_diamond_menu);
							int i = my_hero->hero.equipments.size();
							auto visibleSize = Director::getInstance()->getVisibleSize();
							Vec2 origin = Director::getInstance()->getVisibleOrigin();
							my_hero->hero.equipmentsImages.push_back(Sprite::create("shop/green_diamond1.png"));
							my_hero->hero.equipmentsImages[i * 2 - 2]->setPosition(Vec2(origin.x + vecx[i - 1], origin.y + vecy[0]));
							this->addChild(my_hero->hero.equipmentsImages[i * 2 - 2], 2);
							my_hero->hero.equipmentsImages.push_back(Sprite::create("shop/green_diamond2.png"));
							my_hero->hero.equipmentsImages[i * 2 - 1]->setPosition(Vec2(origin.x + 438, origin.y + 430));
							this->addChild(my_hero->hero.equipmentsImages[i * 2 - 1], 2);

						}
					});
				if (equipments_introduction != NULL)
				{
					equipments_introduction->removeFromParent();
				}
				equipments_introduction = Sprite::create("shop/green_diamond3.png");
				equipments_introduction->setPosition(Vec2(1730, 630));
				this->addChild(equipments_introduction, 2);
				if (buy_it_menu != NULL)
				{
					buy_it_menu->removeFromParent();
				}
				buy_it_menu = Menu::create(item, NULL);
				buy_it_menu->alignItemsVerticallyWithPadding(50);
				item->setPosition(Vec2(origin.x + 750, origin.y - 250));
				this->addChild(buy_it_menu, 2);

			});
		green_diamond_menu = Menu::create(item8, NULL);
		green_diamond_menu->alignItemsVerticallyWithPadding(50);
		//������ˮ��װ����ť
		item8->setPosition(Vec2(origin.x - 520, origin.y - 110));
		this->addChild(green_diamond_menu, 2);

		//���װ����Ь   
		auto item9 = MenuItemImage::create("shop/basic_shoes.png", "shop/basic_shoes.png", [&](Ref* sender)
			{
				auto item = MenuItemImage::create("shop/buy_it.png", "shop/buy_it_selected.png", [&](Ref* sender)
					{
						if (my_hero->hero.money > 250 && my_hero->hero.equipments.size() < 6 && my_hero->hero.move_speed == 500)//�˴�����ֻ����һ˫Ь,500Ϊ��ʼ�ٶȣ��ǵ��Ժ����
						{
							++basic_shoes_num;
							my_hero->hero.money -= 250;
							money_label->setString(std::to_string(my_hero->hero.money));
							money_label_shop->setString(std::to_string(my_hero->hero.money));
							my_hero->hero.move_speed += 30;
							my_hero->hero.equipments.push_back(basic_shoes_menu);
							int i = my_hero->hero.equipments.size();
							auto visibleSize = Director::getInstance()->getVisibleSize();
							Vec2 origin = Director::getInstance()->getVisibleOrigin();
							my_hero->hero.equipmentsImages.push_back(Sprite::create("shop/basic_shoes1.png"));
							my_hero->hero.equipmentsImages[i * 2 - 2]->setPosition(Vec2(origin.x + vecx[i - 1], origin.y + vecy[0]));
							this->addChild(my_hero->hero.equipmentsImages[i * 2 - 2], 2);
							my_hero->hero.equipmentsImages.push_back(Sprite::create("shop/basic_shoes2.png"));
							my_hero->hero.equipmentsImages[i * 2 - 1]->setPosition(Vec2(origin.x + 858, origin.y + 430));
							this->addChild(my_hero->hero.equipmentsImages[i * 2 - 1], 2);
							if (def_shoes_after_basic_menu != NULL)   //�������˸���Ь�ʹ���һ�����˲�Ь��ķ���Ь�˵���������ÿ�ΰ�ǰһ������Ϊ���ɼ�
							{
								def_shoes_after_basic_menu->setVisible(false);
							}
							//���װ�����   
							auto item12 = MenuItemImage::create("shop/def_shoes_after_basic.png", "shop/def_shoes_after_basic.png", [&](Ref* sender)
								{
									auto visibleSize = Director::getInstance()->getVisibleSize();
									Vec2 origin = Director::getInstance()->getVisibleOrigin();
									auto item = MenuItemImage::create("shop/buy_it.png", "shop/buy_it_selected.png", [&](Ref* sender)
										{
											if (my_hero->hero.money > 460)
											{
												my_hero->hero.money -= 460;
												money_label->setString(std::to_string(my_hero->hero.money));
												money_label_shop->setString(std::to_string(my_hero->hero.money));
												my_hero->hero.move_speed += 30;
												my_hero->hero.defect = my_hero->hero.defect * 1.17 + 102;
												auto visibleSize = Director::getInstance()->getVisibleSize();
												Vec2 origin = Director::getInstance()->getVisibleOrigin();
												int size = my_hero->hero.equipments.size();
												int j = 0;
												for (j = 0; j < size; j++)
												{
													if (my_hero->hero.equipments[j] == basic_shoes_menu)
													{
														my_hero->hero.equipments[j] = def_shoes_after_basic_menu;
														basic_shoes_num--;
														break;
													}
												}
												my_hero->hero.equipmentsImages[j * 2]->setVisible(false);
												my_hero->hero.equipmentsImages[j * 2] = (Sprite::create("shop/def_shoes1.png"));
												my_hero->hero.equipmentsImages[j * 2]->setPosition(Vec2(origin.x + vecx[j], origin.y + vecy[0]));
												this->addChild(my_hero->hero.equipmentsImages[j * 2], 2);
												my_hero->hero.equipmentsImages[j * 2 + 1]->setVisible(false);
												my_hero->hero.equipmentsImages[j * 2 + 1] = (Sprite::create("shop/def_shoes_after_basic.png"));
												my_hero->hero.equipmentsImages[j * 2 + 1]->setPosition(Vec2(origin.x + 898, origin.y + 250));
												this->addChild(my_hero->hero.equipmentsImages[j * 2 + 1], 2);
												if (basic_shoes_num == 0)
												{
													if (equipments_introduction != NULL)
													{
														equipments_introduction->removeFromParent();
														equipments_introduction = NULL;
														buy_it_menu->removeFromParent();
														buy_it_menu = NULL;
													}
													if (def_shoes_menu != NULL)            //�˴������Ѵ��̵�ʱ�����ķ���Ь�˵���Ϊ���ɼ�����Ȼָ��ָ����֮���ִ������·���Ь�˵���ԭ�˵������������
													{
														def_shoes_menu->setVisible(false);
													}
													//���װ������Ь
													auto item12 = MenuItemImage::create("shop/def_shoes2.png", "shop/def_shoes2.png", [&](Ref* sender)
														{
															auto visibleSize = Director::getInstance()->getVisibleSize();
															Vec2 origin = Director::getInstance()->getVisibleOrigin();
															auto item = MenuItemImage::create("shop/buy_it.png", "shop/buy_it_selected.png", [&](Ref* sender)
																{
																	if (my_hero->hero.money > 710 && my_hero->hero.equipments.size() < 6 && my_hero->hero.move_speed == 500)//����ֻ��Ϊһ˫Ь
																	{
																		my_hero->hero.money -= 710;
																		money_label->setString(std::to_string(my_hero->hero.money));
																		money_label_shop->setString(std::to_string(my_hero->hero.money));
																		my_hero->hero.move_speed += 120;
																		my_hero->hero.equipments.push_back(def_shoes_menu);
																		int i = my_hero->hero.equipments.size();
																		auto visibleSize = Director::getInstance()->getVisibleSize();
																		Vec2 origin = Director::getInstance()->getVisibleOrigin();
																		my_hero->hero.equipmentsImages.push_back(Sprite::create("shop/def_shoes1.png"));
																		my_hero->hero.equipmentsImages[i * 2 - 2]->setPosition(Vec2(origin.x + vecx[i - 1], origin.y + vecy[0]));
																		this->addChild(my_hero->hero.equipmentsImages[i * 2 - 2], 2);
																		my_hero->hero.equipmentsImages.push_back(Sprite::create("shop/def_shoes2.png"));
																		my_hero->hero.equipmentsImages[i * 2 - 1]->setPosition(Vec2(origin.x + 898, origin.y + 250));
																		this->addChild(my_hero->hero.equipmentsImages[i * 2 - 1], 2);

																	}
																});
															if (equipments_introduction != NULL)
															{
																equipments_introduction->removeFromParent();
															}
															equipments_introduction = Sprite::create("shop/def_shoes3.png");
															equipments_introduction->setPosition(Vec2(1730, 630));
															this->addChild(equipments_introduction, 2);
															if (buy_it_menu != NULL)
															{
																buy_it_menu->removeFromParent();
															}
															buy_it_menu = Menu::create(item, NULL);
															buy_it_menu->alignItemsVerticallyWithPadding(50);
															item->setPosition(Vec2(origin.x + 750, origin.y - 250));
															this->addChild(buy_it_menu, 2);

														});
													def_shoes_menu = Menu::create(item12, NULL);
													def_shoes_menu->alignItemsVerticallyWithPadding(50);
													//���÷���Ьװ����ť
													item12->setPosition(Vec2(origin.x - 60, origin.y - 290));
													this->addChild(def_shoes_menu, 2);
												}

											}
										});
									if (equipments_introduction != NULL)
									{
										equipments_introduction->removeFromParent();
									}
									equipments_introduction = Sprite::create("shop/def_shoes3.png");
									equipments_introduction->setPosition(Vec2(1730, 630));
									this->addChild(equipments_introduction, 2);
									if (buy_it_menu != NULL)
									{
										buy_it_menu->removeFromParent();
									}
									buy_it_menu = Menu::create(item, NULL);
									buy_it_menu->alignItemsVerticallyWithPadding(50);
									item->setPosition(Vec2(origin.x + 750, origin.y - 250));
									this->addChild(buy_it_menu, 2);

								});
							def_shoes_after_basic_menu = Menu::create(item12, NULL);
							def_shoes_after_basic_menu->alignItemsVerticallyWithPadding(50);
							//���÷���Ьװ����ť
							item12->setPosition(Vec2(origin.x - 60, origin.y - 290));
							this->addChild(def_shoes_after_basic_menu, 2);

						}
					});
				if (equipments_introduction != NULL)
				{
					equipments_introduction->removeFromParent();
				}
				equipments_introduction = Sprite::create("shop/basic_shoes3.png");
				equipments_introduction->setPosition(Vec2(1730, 630));
				this->addChild(equipments_introduction, 2);
				if (buy_it_menu != NULL)
				{
					buy_it_menu->removeFromParent();
				}
				buy_it_menu = Menu::create(item, NULL);
				buy_it_menu->alignItemsVerticallyWithPadding(50);
				item->setPosition(Vec2(origin.x + 750, origin.y - 250));
				this->addChild(buy_it_menu, 2);

			});
		basic_shoes_menu = Menu::create(item9, NULL);
		basic_shoes_menu->alignItemsVerticallyWithPadding(50);
		//���ò�Ьװ����ť
		item9->setPosition(Vec2(origin.x - 100, origin.y - 110));
		this->addChild(basic_shoes_menu, 2);

		//���װ����������   
		auto item10 = MenuItemImage::create("shop/belt_strength.png", "shop/belt_strength.png", [&](Ref* sender)
			{
				auto item = MenuItemImage::create("shop/buy_it.png", "shop/buy_it_selected.png", [&](Ref* sender)
					{
						if (my_hero->hero.money > 900 && my_hero->hero.equipments.size() < 6)
						{
							my_hero->hero.money -= 900;
							money_label->setString(std::to_string(my_hero->hero.money));
							money_label_shop->setString(std::to_string(my_hero->hero.money));
							my_hero->hero.FullHP += 1000;
							my_hero->hero.HP += 1000;
							my_hero->hero.equipments.push_back(belt_strength_menu);
							int i = my_hero->hero.equipments.size();
							auto visibleSize = Director::getInstance()->getVisibleSize();
							Vec2 origin = Director::getInstance()->getVisibleOrigin();
							my_hero->hero.equipmentsImages.push_back(Sprite::create("shop/belt_strength1.png"));
							my_hero->hero.equipmentsImages[i * 2 - 2]->setPosition(Vec2(origin.x + vecx[i - 1], origin.y + vecy[0]));
							this->addChild(my_hero->hero.equipmentsImages[i * 2 - 2], 2);
							my_hero->hero.equipmentsImages.push_back(Sprite::create("shop/belt_strength2.png"));
							my_hero->hero.equipmentsImages[i * 2 - 1]->setPosition(Vec2(origin.x + 1358, origin.y + 430));
							this->addChild(my_hero->hero.equipmentsImages[i * 2 - 1], 2);

						}
					});
				if (equipments_introduction != NULL)
				{
					equipments_introduction->removeFromParent();
				}
				equipments_introduction = Sprite::create("shop/belt_strength3.png");
				equipments_introduction->setPosition(Vec2(1730, 630));
				this->addChild(equipments_introduction, 2);
				if (buy_it_menu != NULL)
				{
					buy_it_menu->removeFromParent();
				}
				buy_it_menu = Menu::create(item, NULL);
				buy_it_menu->alignItemsVerticallyWithPadding(50);
				item->setPosition(Vec2(origin.x + 750, origin.y - 250));
				this->addChild(buy_it_menu, 2);

			});
		belt_strength_menu = Menu::create(item10, NULL);
		belt_strength_menu->alignItemsVerticallyWithPadding(50);
		//������������װ����ť
		item10->setPosition(Vec2(origin.x + 400, origin.y - 110));
		this->addChild(belt_strength_menu, 2);

		//���װ����� 
		auto item11 = MenuItemImage::create("shop/big_stick.png", "shop/big_stick.png", [&](Ref* sender)
			{
				auto item = MenuItemImage::create("shop/buy_it.png", "shop/buy_it_selected.png", [&](Ref* sender)
					{
						if (my_hero->hero.money > 820 && my_hero->hero.equipments.size() < 6)
						{
							my_hero->hero.money -= 820;
							money_label->setString(std::to_string(my_hero->hero.money));
							money_label_shop->setString(std::to_string(my_hero->hero.money));
							my_hero->hero.ap += 120;
							my_hero->hero.equipments.push_back(big_stick_menu);
							int i = my_hero->hero.equipments.size();
							auto visibleSize = Director::getInstance()->getVisibleSize();
							Vec2 origin = Director::getInstance()->getVisibleOrigin();
							my_hero->hero.equipmentsImages.push_back(Sprite::create("shop/big_stick1.png"));
							my_hero->hero.equipmentsImages[i * 2 - 2]->setPosition(Vec2(origin.x + vecx[i - 1], origin.y + vecy[0]));
							this->addChild(my_hero->hero.equipmentsImages[i * 2 - 2], 2);
							my_hero->hero.equipmentsImages.push_back(Sprite::create("shop/big_stick2.png"));
							my_hero->hero.equipmentsImages[i * 2 - 1]->setPosition(Vec2(origin.x + 438, origin.y + 250));
							this->addChild(my_hero->hero.equipmentsImages[i * 2 - 1], 2);

						}
					});
				if (equipments_introduction != NULL)
				{
					equipments_introduction->removeFromParent();
				}
				equipments_introduction = Sprite::create("shop/big_stick3.png");
				equipments_introduction->setPosition(Vec2(1730, 630));
				this->addChild(equipments_introduction, 2);
				if (buy_it_menu != NULL)
				{
					buy_it_menu->removeFromParent();
				}
				buy_it_menu = Menu::create(item, NULL);
				buy_it_menu->alignItemsVerticallyWithPadding(50);
				item->setPosition(Vec2(origin.x + 750, origin.y - 250));
				this->addChild(buy_it_menu, 2);
			});
		big_stick_menu = Menu::create(item11, NULL);
		big_stick_menu->alignItemsVerticallyWithPadding(50);
		//���ô��װ����ť
		item11->setPosition(Vec2(origin.x - 520, origin.y - 290));
		this->addChild(big_stick_menu, 2);

		//���װ������Ь
		auto item12 = MenuItemImage::create("shop/def_shoes.png", "shop/def_shoes.png", [&](Ref* sender)
			{
				auto visibleSize = Director::getInstance()->getVisibleSize();
				Vec2 origin = Director::getInstance()->getVisibleOrigin();
				auto item = MenuItemImage::create("shop/buy_it.png", "shop/buy_it_selected.png", [&](Ref* sender)
					{
						if (my_hero->hero.money > 710 && my_hero->hero.equipments.size() < 6 && my_hero->hero.move_speed == 500)
						{
							my_hero->hero.money -= 710;
							money_label->setString(std::to_string(my_hero->hero.money));
							money_label_shop->setString(std::to_string(my_hero->hero.money));
							my_hero->hero.move_speed += 120;
							my_hero->hero.equipments.push_back(def_shoes_menu);
							int i = my_hero->hero.equipments.size();
							auto visibleSize = Director::getInstance()->getVisibleSize();
							Vec2 origin = Director::getInstance()->getVisibleOrigin();
							my_hero->hero.equipmentsImages.push_back(Sprite::create("shop/def_shoes1.png"));
							my_hero->hero.equipmentsImages[i * 2 - 2]->setPosition(Vec2(origin.x + vecx[i - 1], origin.y + vecy[0]));
							this->addChild(my_hero->hero.equipmentsImages[i * 2 - 2], 2);
							my_hero->hero.equipmentsImages.push_back(Sprite::create("shop/def_shoes2.png"));
							my_hero->hero.equipmentsImages[i * 2 - 1]->setPosition(Vec2(origin.x + 898, origin.y + 250));
							this->addChild(my_hero->hero.equipmentsImages[i * 2 - 1], 2);

						}
					});
				if (equipments_introduction != NULL)
				{
					equipments_introduction->removeFromParent();
				}
				equipments_introduction = Sprite::create("shop/def_shoes3.png");
				equipments_introduction->setPosition(Vec2(1730, 630));
				this->addChild(equipments_introduction, 2);
				if (buy_it_menu != NULL)
				{
					buy_it_menu->removeFromParent();
				}
				buy_it_menu = Menu::create(item, NULL);
				buy_it_menu->alignItemsVerticallyWithPadding(50);
				item->setPosition(Vec2(origin.x + 750, origin.y - 250));
				this->addChild(buy_it_menu, 2);

			});
		def_shoes_menu = Menu::create(item12, NULL);
		def_shoes_menu->alignItemsVerticallyWithPadding(50);
		//���÷���Ьװ����ť
		item12->setPosition(Vec2(origin.x - 60, origin.y - 290));
		this->addChild(def_shoes_menu, 2);

		//���װ��������
		auto item13 = MenuItemImage::create("shop/def_armour.png", "shop/def_armour.png", [&](Ref* sender)
			{
				auto visibleSize = Director::getInstance()->getVisibleSize();
				Vec2 origin = Director::getInstance()->getVisibleOrigin();
				auto item = MenuItemImage::create("shop/buy_it.png", "shop/buy_it_selected.png", [&](Ref* sender)
					{
						if (my_hero->hero.money > 730 && my_hero->hero.equipments.size() < 6)
						{
							my_hero->hero.money -= 730;
							money_label->setString(std::to_string(my_hero->hero.money));
							money_label_shop->setString(std::to_string(my_hero->hero.money));
							my_hero->hero.defect += 210;
							my_hero->hero.armour_effect = true;
							my_hero->hero.equipments.push_back(def_armour_menu);
							int i = my_hero->hero.equipments.size();
							auto visibleSize = Director::getInstance()->getVisibleSize();
							Vec2 origin = Director::getInstance()->getVisibleOrigin();
							my_hero->hero.equipmentsImages.push_back(Sprite::create("shop/def_armour1.png"));
							my_hero->hero.equipmentsImages[i * 2 - 2]->setPosition(Vec2(origin.x + vecx[i - 1], origin.y + vecy[0]));
							this->addChild(my_hero->hero.equipmentsImages[i * 2 - 2], 2);
							my_hero->hero.equipmentsImages.push_back(Sprite::create("shop/def_armour2.png"));
							my_hero->hero.equipmentsImages[i * 2 - 1]->setPosition(Vec2(origin.x + 1358, origin.y + 250));
							this->addChild(my_hero->hero.equipmentsImages[i * 2 - 1], 2);

						}
					});
				if (equipments_introduction != NULL)
				{
					equipments_introduction->removeFromParent();
				}
				equipments_introduction = Sprite::create("shop/def_armour3.png");
				equipments_introduction->setPosition(Vec2(1730, 630));
				this->addChild(equipments_introduction, 2);
				if (buy_it_menu != NULL)
				{
					buy_it_menu->removeFromParent();
				}
				buy_it_menu = Menu::create(item, NULL);
				buy_it_menu->alignItemsVerticallyWithPadding(50);
				item->setPosition(Vec2(origin.x + 750, origin.y - 250));
				this->addChild(buy_it_menu, 2);

			});
		def_armour_menu = Menu::create(item13, NULL);
		def_armour_menu->alignItemsVerticallyWithPadding(50);
		//���÷�����װ����ť
		item13->setPosition(Vec2(origin.x + 400, origin.y - 290));
		this->addChild(def_armour_menu, 2);

	}
	else
	{
		shop_layer->setVisible(true);
		shop_close_menu->setVisible(true);
		money_label_shop->setVisible(true);
		red_diamond_menu->setVisible(true);
		blue_diamond_menu->setVisible(true);
		mp_book_menu->setVisible(true);
		iron_sword_menu->setVisible(true);
		def_clothes_menu->setVisible(true);
		def_ap_cloak_menu->setVisible(true);
		green_diamond_menu->setVisible(true);
		basic_shoes_menu->setVisible(true);
		belt_strength_menu->setVisible(true);
		second_page_menu->setVisible(true);
		if (belt_strength_after_red_menu != NULL)
		{
			belt_strength_after_red_menu->setVisible(true);
		}
		big_stick_menu->setVisible(true);
		if (big_stick_after_book_menu != NULL)
		{
			big_stick_after_book_menu->setVisible(true);
		}
		def_shoes_menu->setVisible(true);
		if (def_shoes_after_basic_menu != NULL)
		{
			def_shoes_after_basic_menu->setVisible(true);
		}
		def_armour_menu->setVisible(true);
		if (def_armour_after_clothes_menu != NULL)
		{
			def_armour_after_clothes_menu->setVisible(true);
		}
		for (int i = 0; i < my_hero->hero.equipments.size(); ++i)
		{
			my_hero->hero.equipmentsImages[2 * i]->setVisible(true);
			if (my_hero->hero.equipments[i] != big_sword_menu)
			{
				my_hero->hero.equipmentsImages[2 * i + 1]->setVisible(true);
			}
		}
	}

}

//�ر��̵����
void MainScene::CloseShop(Ref* pSender)
{
	if (buy_it_menu != NULL)
	{
		buy_it_menu->removeFromParent();
		equipments_introduction->removeFromParent();
		buy_it_menu = NULL;
		equipments_introduction = NULL;
	}
	shop_layer->setVisible(false); //����ɾ��ͼ�㼴ͼ�����Ԫ�أ������ڶ��δ�ʱ���ǳ�ʼ״̬��
	shop_close_menu->setVisible(false);
	money_label_shop->setVisible(false);
	second_page_menu->setVisible(false);
	red_diamond_menu->setVisible(false);
	blue_diamond_menu->setVisible(false);
	mp_book_menu->setVisible(false);
	iron_sword_menu->setVisible(false);
	def_clothes_menu->setVisible(false);
	def_ap_cloak_menu->setVisible(false);
	green_diamond_menu->setVisible(false);
	basic_shoes_menu->setVisible(false);
	belt_strength_menu->setVisible(false);
	if (belt_strength_after_red_menu != NULL)
	{
		belt_strength_after_red_menu->setVisible(false);
	}
	big_stick_menu->setVisible(false);
	if (big_stick_after_book_menu != NULL)
	{
		big_stick_after_book_menu->setVisible(false);
	}
	def_shoes_menu->setVisible(false);
	if (def_shoes_after_basic_menu != NULL)
	{
		def_shoes_after_basic_menu->setVisible(false);
	}
	def_armour_menu->setVisible(false);
	if (def_armour_after_clothes_menu != NULL)
	{
		def_armour_after_clothes_menu->setVisible(false);
	}
	for (int i = 0; i < my_hero->hero.equipments.size(); ++i)
	{
		my_hero->hero.equipmentsImages[2 * i]->setVisible(false);
		my_hero->hero.equipmentsImages[2 * i + 1]->setVisible(false);
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	//��ӽ����̵���Ϸ��ť  
	auto item1 = MenuItemImage::create("shop/shop.png", "shop/shopselected.png", CC_CALLBACK_1(MainScene::OpenShop, this));
	shop_open_menu = Menu::create(item1, NULL);
	shop_open_menu->alignItemsVerticallyWithPadding(50);
	//��ť��������Ļ���м�
	item1->setPosition(Vec2(origin.x - 850, origin.y));
	this->addChild(shop_open_menu, 1);
}

//�ڶ�ҳ�̵�ص�����
void MainScene::second_page(Ref* pSender)
{
	//��ʹ��һҳȫ���ɼ�
	if (buy_it_menu != NULL)
	{
		buy_it_menu->removeFromParent();
		equipments_introduction->removeFromParent();
		buy_it_menu = NULL;
		equipments_introduction = NULL;
	}
	shop_close_menu->setVisible(false);
	red_diamond_menu->setVisible(false);
	blue_diamond_menu->setVisible(false);
	mp_book_menu->setVisible(false);
	iron_sword_menu->setVisible(false);
	def_clothes_menu->setVisible(false);
	def_ap_cloak_menu->setVisible(false);
	green_diamond_menu->setVisible(false);
	basic_shoes_menu->setVisible(false);
	belt_strength_menu->setVisible(false);
	if (belt_strength_after_red_menu != NULL)
	{
		belt_strength_after_red_menu->setVisible(false);
	}
	big_stick_menu->setVisible(false);
	if (big_stick_after_book_menu != NULL)
	{
		big_stick_after_book_menu->setVisible(false);
	}
	def_shoes_menu->setVisible(false);
	if (def_shoes_after_basic_menu != NULL)
	{
		def_shoes_after_basic_menu->setVisible(false);
	}
	def_armour_menu->setVisible(false);
	if (def_armour_after_clothes_menu != NULL)
	{
		def_armour_after_clothes_menu->setVisible(false);
	}
	for (int i = 0; i < my_hero->hero.equipments.size(); ++i)
	{
		my_hero->hero.equipmentsImages[2 * i + 1]->setVisible(false);
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	if (shop_second_layer == NULL)
	{
		//��������ڶ�ҳ�̵�        
		shop_second_layer = Layer::create();
		this->addChild(shop_second_layer, 2);
		shop_second_sprite = Sprite::create("shop/shopopened_second1.png");
		shop_second_layer->addChild(shop_second_sprite, 2);
		shop_second_layer->setAnchorPoint(Vec2(0, 0));
		shop_second_sprite->setAnchorPoint(Vec2(0, 0));
		shop_second_sprite->setPosition(Vec2(0, 207));
		second_page_menu->setVisible(false);



		//��ӹر��̵���Ϸ��ť  
		auto item1 = MenuItemImage::create("shop/close.png", "shop/closeselected.png", CC_CALLBACK_1(MainScene::CloseShop_1, this));
		shop_close_menu_1 = Menu::create(item1, NULL);
		shop_close_menu_1->alignItemsVerticallyWithPadding(50);
		//���ùر��̵갴ť
		item1->setPosition(Vec2(origin.x + 850, origin.y + 425));
		this->addChild(shop_close_menu_1, 2);

		//����л�����һҳ�̵�ҳ����ť
		auto item_1 = MenuItemImage::create("shop/first_page.png", "shop/first_pageselected.png", CC_CALLBACK_1(MainScene::first_page, this));
		first_page_menu = Menu::create(item_1, NULL);
		first_page_menu->alignItemsVerticallyWithPadding(50);
		//���õ�һҳ�̵갴ť
		item_1->setPosition(Vec2(origin.x - 850, origin.y + 260));
		this->addChild(first_page_menu, 2);
	}
	else
	{
		shop_second_layer->setVisible(true);
		first_page_menu->setVisible(true);
		shop_close_menu_1->setVisible(true);

	}

	if (iron_sword_num != 0)//���ô󽣣��ѹ���������
	{
		if (big_sword_menu != NULL)
		{
			big_sword_menu->setVisible(false);
		}
		//���װ����
		auto item2 = MenuItemImage::create("shop/big_sword_after_iron.png", "shop/big_sword_after_iron.png", [&](Ref* sender)
			{
				auto item = MenuItemImage::create("shop/buy_it.png", "shop/buy_it_selected.png", [&](Ref* sender)
					{
						if (my_hero->hero.money > 660)
						{
							++big_sword_num;
							my_hero->hero.money -= 660;
							money_label->setString(std::to_string(my_hero->hero.money));
							money_label_shop->setString(std::to_string(my_hero->hero.money));
							my_hero->hero.attack += 60;
							auto visibleSize = Director::getInstance()->getVisibleSize();
							Vec2 origin = Director::getInstance()->getVisibleOrigin();
							int size = my_hero->hero.equipments.size();
							int j = 0;
							for (j = 0; j < size; j++)
							{
								if (my_hero->hero.equipments[j] == iron_sword_menu)
								{
									my_hero->hero.equipments[j] = big_sword_menu;
									iron_sword_num--;
									break;
								}
							}
							my_hero->hero.equipmentsImages[j * 2]->setVisible(false);
							my_hero->hero.equipmentsImages[j * 2] = (Sprite::create("shop/big_sword1.png"));
							my_hero->hero.equipmentsImages[j * 2]->setPosition(Vec2(origin.x + vecx[j], origin.y + vecy[0]));
							this->addChild(my_hero->hero.equipmentsImages[j * 2], 2);
							my_hero->hero.equipmentsImages[j * 2 + 1] = (Sprite::create("shop/big_sword2.png"));
							my_hero->hero.equipmentsImages[j * 2 + 1]->setPosition(Vec2(origin.x + 423, origin.y + 790));
							this->addChild(my_hero->hero.equipmentsImages[j * 2 + 1], 2);
							if (iron_sword_num == 0)
							{
								if (equipments_introduction != NULL)
								{
									equipments_introduction->removeFromParent();
									equipments_introduction = NULL;
									buy_it_menu->removeFromParent();
									buy_it_menu = NULL;
								}
								if (big_sword_menu != NULL)            //�˴������Ѵ��̵�ʱ�����Ĵ󽣲˵���Ϊ���ɼ�����Ȼָ��ָ����֮���ִ������´󽣲˵���ԭ�˵������������
								{
									big_sword_menu->setVisible(false);
								}
								//���װ����
								auto item2 = MenuItemImage::create("shop/big_sword2.png", "shop/big_sword2.png", [&](Ref* sender)
									{
										auto visibleSize = Director::getInstance()->getVisibleSize();
										Vec2 origin = Director::getInstance()->getVisibleOrigin();
										auto item = MenuItemImage::create("shop/buy_it.png", "shop/buy_it_selected.png", [&](Ref* sender)
											{
												if (my_hero->hero.money > 910 && my_hero->hero.equipments.size() < 6)
												{
													++big_sword_num;
													my_hero->hero.money -= 910;
													money_label->setString(std::to_string(my_hero->hero.money));
													money_label_shop->setString(std::to_string(my_hero->hero.money));
													my_hero->hero.attack += 80;
													my_hero->hero.equipments.push_back(big_sword_menu);
													int i = my_hero->hero.equipments.size();
													auto visibleSize = Director::getInstance()->getVisibleSize();
													Vec2 origin = Director::getInstance()->getVisibleOrigin();
													my_hero->hero.equipmentsImages.push_back(Sprite::create("shop/big_sword1.png"));
													my_hero->hero.equipmentsImages[i * 2 - 2]->setPosition(Vec2(origin.x + vecx[i - 1], origin.y + vecy[0]));
													this->addChild(my_hero->hero.equipmentsImages[i * 2 - 2], 2);
													my_hero->hero.equipmentsImages.push_back(Sprite::create("shop/big_sword2.png"));
													my_hero->hero.equipmentsImages[i * 2 - 1]->setPosition(Vec2(origin.x + 423, origin.y + 790));
													this->addChild(my_hero->hero.equipmentsImages[i * 2 - 1], 2);

												}
											});
										if (equipments_introduction != NULL)
										{
											equipments_introduction->removeFromParent();
										}
										equipments_introduction = Sprite::create("shop/big_sword3.png");
										equipments_introduction->setPosition(Vec2(1730, 630));
										this->addChild(equipments_introduction, 2);
										if (buy_it_menu != NULL)
										{
											buy_it_menu->removeFromParent();
										}
										buy_it_menu = Menu::create(item, NULL);
										buy_it_menu->alignItemsVerticallyWithPadding(50);
										item->setPosition(Vec2(origin.x + 750, origin.y - 250));
										this->addChild(buy_it_menu, 2);
									});
								big_sword_menu = Menu::create(item2, NULL);
								big_sword_menu->alignItemsVerticallyWithPadding(50);
								//���ô�װ����ť
								item2->setPosition(Vec2(origin.x - 500, origin.y + 250));
								this->addChild(big_sword_menu, 2);
							}
						}
					});
				if (equipments_introduction != NULL)
				{
					equipments_introduction->removeFromParent();
				}
				equipments_introduction = Sprite::create("shop/big_sword3.png");
				equipments_introduction->setPosition(Vec2(1730, 630));
				this->addChild(equipments_introduction, 2);
				if (buy_it_menu != NULL)
				{
					buy_it_menu->removeFromParent();
				}
				buy_it_menu = Menu::create(item, NULL);
				buy_it_menu->alignItemsVerticallyWithPadding(50);
				item->setPosition(Vec2(origin.x + 750, origin.y - 250));
				this->addChild(buy_it_menu, 2);
			});
		big_sword_menu = Menu::create(item2, NULL);
		big_sword_menu->alignItemsVerticallyWithPadding(50);
		//���ô�װ����ť
		item2->setPosition(Vec2(origin.x - 500, origin.y + 250));
		this->addChild(big_sword_menu, 2);
	}
	else	//���ô󽣣�δ����������
	{
		if (big_sword_num == 0)
			//���װ����
		{
			auto item2 = MenuItemImage::create("shop/big_sword.png", "shop/big_sword.png", [&](Ref* sender)
				{
					auto item = MenuItemImage::create("shop/buy_it.png", "shop/buy_it_selected.png", [&](Ref* sender)
						{
							if (my_hero->hero.money > 910 && my_hero->hero.equipments.size() < 6)
							{
								++big_sword_num;
								my_hero->hero.money -= 910;
								money_label->setString(std::to_string(my_hero->hero.money));
								money_label_shop->setString(std::to_string(my_hero->hero.money));
								my_hero->hero.attack += 80;
								my_hero->hero.equipments.push_back(big_sword_menu);
								int i = my_hero->hero.equipments.size();
								auto visibleSize = Director::getInstance()->getVisibleSize();
								Vec2 origin = Director::getInstance()->getVisibleOrigin();
								my_hero->hero.equipmentsImages.push_back(Sprite::create("shop/big_sword1.png"));
								my_hero->hero.equipmentsImages[i * 2 - 2]->setPosition(Vec2(origin.x + vecx[i - 1], origin.y + vecy[0]));
								this->addChild(my_hero->hero.equipmentsImages[i * 2 - 2], 2);
								my_hero->hero.equipmentsImages.push_back(Sprite::create("shop/big_sword2.png"));
								my_hero->hero.equipmentsImages[i * 2 - 1]->setPosition(Vec2(origin.x + 423, origin.y + 790));
								this->addChild(my_hero->hero.equipmentsImages[i * 2 - 1], 2);

							}
						});
					if (equipments_introduction != NULL)
					{
						equipments_introduction->removeFromParent();
					}
					equipments_introduction = Sprite::create("shop/big_sword3.png");
					equipments_introduction->setPosition(Vec2(1730, 630));
					this->addChild(equipments_introduction, 2);
					if (buy_it_menu != NULL)
					{
						buy_it_menu->removeFromParent();
					}
					buy_it_menu = Menu::create(item, NULL);
					buy_it_menu->alignItemsVerticallyWithPadding(50);
					item->setPosition(Vec2(origin.x + 750, origin.y - 250));
					this->addChild(buy_it_menu, 2);
				});
			big_sword_menu = Menu::create(item2, NULL);
			big_sword_menu->alignItemsVerticallyWithPadding(50);
			//���ô�װ����ť
			item2->setPosition(Vec2(origin.x - 500, origin.y + 250));
			this->addChild(big_sword_menu, 2);


		}
		else
		{
			auto item2 = MenuItemImage::create("shop/big_sword2.png", "shop/big_sword2.png", [&](Ref* sender)
				{
					auto item = MenuItemImage::create("shop/buy_it.png", "shop/buy_it_selected.png", [&](Ref* sender)
						{
							if (my_hero->hero.money > 910 && my_hero->hero.equipments.size() < 6)
							{
								++big_sword_num;
								my_hero->hero.money -= 910;
								money_label->setString(std::to_string(my_hero->hero.money));
								money_label_shop->setString(std::to_string(my_hero->hero.money));
								my_hero->hero.attack += 80;
								my_hero->hero.equipments.push_back(big_sword_menu);
								int i = my_hero->hero.equipments.size();
								auto visibleSize = Director::getInstance()->getVisibleSize();
								Vec2 origin = Director::getInstance()->getVisibleOrigin();
								my_hero->hero.equipmentsImages.push_back(Sprite::create("shop/big_sword1.png"));
								my_hero->hero.equipmentsImages[i * 2 - 2]->setPosition(Vec2(origin.x + vecx[i - 1], origin.y + vecy[0]));
								this->addChild(my_hero->hero.equipmentsImages[i * 2 - 2], 2);
								my_hero->hero.equipmentsImages.push_back(Sprite::create("shop/big_sword2.png"));
								my_hero->hero.equipmentsImages[i * 2 - 1]->setPosition(Vec2(origin.x + 423, origin.y + 790));
								this->addChild(my_hero->hero.equipmentsImages[i * 2 - 1], 2);

							}
						});
					if (equipments_introduction != NULL)
					{
						equipments_introduction->removeFromParent();
					}
					equipments_introduction = Sprite::create("shop/big_sword3.png");
					equipments_introduction->setPosition(Vec2(1730, 630));
					this->addChild(equipments_introduction, 2);
					if (buy_it_menu != NULL)
					{
						buy_it_menu->removeFromParent();
					}
					buy_it_menu = Menu::create(item, NULL);
					buy_it_menu->alignItemsVerticallyWithPadding(50);
					item->setPosition(Vec2(origin.x + 750, origin.y - 250));
					this->addChild(buy_it_menu, 2);
				});
			big_sword_menu = Menu::create(item2, NULL);
			big_sword_menu->alignItemsVerticallyWithPadding(50);
			//���ô�װ����ť
			item2->setPosition(Vec2(origin.x - 500, origin.y + 250));
			this->addChild(big_sword_menu, 2);
		}
	}
}

//��һҳ�̵�ص�����
void MainScene::first_page(Ref* pSender)
{
	if (buy_it_menu != NULL)
	{
		buy_it_menu->removeFromParent();
		equipments_introduction->removeFromParent();
		buy_it_menu = NULL;
		equipments_introduction = NULL;
	}
	shop_second_layer->setVisible(false); //����ɾ��ͼ�㼴ͼ�����Ԫ�أ������ڶ��δ�ʱ���ǳ�ʼ״̬��
	shop_layer->setVisible(false);
	first_page_menu->setVisible(false);
	second_page_menu->setVisible(false);
	shop_close_menu_1->setVisible(false);
	big_sword_menu->setVisible(false);
	for (int i = 0; i < my_hero->hero.equipments.size(); ++i)
	{
		my_hero->hero.equipmentsImages[2 * i]->setVisible(false);
		my_hero->hero.equipmentsImages[2 * i + 1]->setVisible(false);
	}
	shop_layer->setVisible(true);
	shop_close_menu->setVisible(true);
	red_diamond_menu->setVisible(true);
	blue_diamond_menu->setVisible(true);
	mp_book_menu->setVisible(true);
	iron_sword_menu->setVisible(true);
	def_clothes_menu->setVisible(true);
	def_ap_cloak_menu->setVisible(true);
	green_diamond_menu->setVisible(true);
	basic_shoes_menu->setVisible(true);
	belt_strength_menu->setVisible(true);
	second_page_menu->setVisible(true);
	if (belt_strength_after_red_menu != NULL)
	{
		belt_strength_after_red_menu->setVisible(true);
	}
	big_stick_menu->setVisible(true);
	if (big_stick_after_book_menu != NULL)
	{
		big_stick_after_book_menu->setVisible(true);
	}
	def_shoes_menu->setVisible(true);
	if (def_shoes_after_basic_menu != NULL)
	{
		def_shoes_after_basic_menu->setVisible(true);
	}
	def_armour_menu->setVisible(true);
	if (def_armour_after_clothes_menu != NULL)
	{
		def_armour_after_clothes_menu->setVisible(true);
	}
	for (int i = 0; i < my_hero->hero.equipments.size(); ++i)
	{
		my_hero->hero.equipmentsImages[2 * i]->setVisible(true);
		if (my_hero->hero.equipments[i] != big_sword_menu)
		{
			my_hero->hero.equipmentsImages[2 * i + 1]->setVisible(true);
		}
	}
}

//�ӵڶ�ҳ�ر��̵����    
void MainScene::CloseShop_1(Ref* pSender)
{
	if (buy_it_menu != NULL)
	{
		buy_it_menu->removeFromParent();
		equipments_introduction->removeFromParent();
		buy_it_menu = NULL;
		equipments_introduction = NULL;
	}
	shop_second_layer->setVisible(false); //����ɾ��ͼ�㼴ͼ�����Ԫ�أ������ڶ��δ�ʱ���ǳ�ʼ״̬��
	shop_layer->setVisible(false);
	money_label_shop->setVisible(false);
	first_page_menu->setVisible(false);
	second_page_menu->setVisible(false);
	shop_close_menu_1->setVisible(false);
	big_sword_menu->setVisible(false);
	for (int i = 0; i < my_hero->hero.equipments.size(); ++i)
	{
		my_hero->hero.equipmentsImages[2 * i]->setVisible(false);
		my_hero->hero.equipmentsImages[2 * i + 1]->setVisible(false);
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	//��ӽ����̵���Ϸ��ť  
	auto item1 = MenuItemImage::create("shop/shop.png", "shop/shopselected.png", CC_CALLBACK_1(MainScene::OpenShop, this));
	shop_open_menu = Menu::create(item1, NULL);
	shop_open_menu->alignItemsVerticallyWithPadding(50);
	//��ť��������Ļ���м�
	item1->setPosition(Vec2(origin.x - 850, origin.y));
	this->addChild(shop_open_menu, 1);
}

//��ʾ�ȼ�
void MainScene::level(float) 
{
	std::string level_text;
	int level = my_hero->hero.level;
	//�˴���������Ҫ�ľ���
	int level_up_exp = my_hero->hero.level_add_experience[level];
	//����ֵ��ʱ����
	if (my_hero->hero.hero_exp >= level_up_exp) 
	{
		my_hero->hero.hero_exp -= level_up_exp;
		my_hero->hero.level++;
		my_hero->hero.HP += my_hero->hero.level_add_HP[level];
		my_hero->hero.attack += my_hero->hero.level_add_attack[level];
	}
	level_text += "LEVEL: ";
	level_text += std::to_string(level);
	level_label->setString(level_text);
}

//��ʾ����
void MainScene::experience(float) 
{
	std::string exp_text;

	//����з�Ӣ��+80�㾭�飬+100���
	if (ai_hero->hero.survival == true)
		MainScene::hero_survive = 1;
	if (ai_hero->hero.survival == false && MainScene::hero_survive == 1) 
	{
		my_hero->hero.hero_exp += 80;
		my_hero->hero.money += 100;
		MainScene::hero_survive = 0;
	}
	
	//����з���������ˮ��+100�㾭��,+120���
	if (towers[2]->tower.survival == true)
		MainScene::tower_survive_1 = 1;
	if (towers[3]->tower.survival == true)
		MainScene::tower_survive_2 = 1;
	if (towers[2]->tower.survival == false && MainScene::tower_survive_1 == 1) 
	{
		my_hero->hero.hero_exp += 100;
		my_hero->hero.money += 120;
		MainScene::tower_survive_1 = 0;
	}
	if (towers[3]->tower.survival == false && MainScene::tower_survive_2 == 1) 
	{
		my_hero->hero.hero_exp += 100;
		my_hero->hero.money += 120;
		MainScene::tower_survive_2 = 0;
	}

	exp_text += "EXP: ";
	exp_text += std::to_string(my_hero->hero.hero_exp);
	exp_text += " / ";
	exp_text += std::to_string(my_hero->hero.level_add_experience[my_hero->hero.level]);
	experience_label->setString(exp_text);
}

//��ʾ���
void MainScene::money(float)
{
	money_label->setString(std::to_string(my_hero->hero.money));
}

//����ս������
void MainScene::myhero_be_killed(float)
{
	//����ɱ��
	if (my_hero->hero.survival == true)
		MainScene::myhero_survive = 1;
	if (my_hero->hero.survival == false && MainScene::myhero_survive == 1) 
	{
		++be_killed_result;
		MainScene::myhero_survive = 0;
	}

	//��ɱ��
	if (ai_hero->hero.survival == true)
		MainScene::aihero_survive = 1;
	if (ai_hero->hero.survival == false && MainScene::aihero_survive == 1)
	{
		++killed_hero_result;
		MainScene::aihero_survive = 0;
	}

	//�ݻٵз���������
	if (towers[2]->tower.survival == false && towers[3]->tower.survival == false)
	{
		destroyed_tower_result = 2;
	}
	else if (towers[2]->tower.survival == true && towers[3]->tower.survival == true)
	{
		destroyed_tower_result = 0;
	}
	else
	{
		destroyed_tower_result = 1;
	}
}
//����ս������
bool  MainScene::create_CombatResult()
{

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	//��Ӳ鿴ս����Ϸ��ť  
	auto item1 = MenuItemImage::create("combat_result/check_result.png", "combat_result/check_result_selected.png", CC_CALLBACK_1(MainScene::CheckResult, this));
	open_combat_result = Menu::create(item1, NULL);
	//open_combat_result->alignItemsVerticallyWithPadding(50);
	//��ť��������Ļ���Ͻ�
	item1->setPosition(Vec2(origin.x + 700, origin.y + 200));
	this->addChild(open_combat_result, 1);
	return true;
}
//��ս������
void MainScene::CheckResult(Ref* pSender)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	result_layer = Layer::create();
	this->addChild(result_layer, 2);
	result = Sprite::create("combat_result/result.png");
	result_layer->addChild(result);
	result_layer->setAnchorPoint(Vec2(0, 0));
	result->setAnchorPoint(Vec2(-0.36, -0.36));
	open_combat_result->removeFromParent();

	//��ӹرհ�ť  
	auto item1 = MenuItemImage::create("combat_result/close.png", "combat_result/close_selected.png", CC_CALLBACK_1(MainScene::CloseResult, this));
	close_combat_result = Menu::create(item1, NULL);
	item1->setPosition(Vec2(origin.x + 468, origin.y + 260));
	this->addChild(close_combat_result, 2);

	//չʾӢ������
	//����idչʾͷ��
	hero_image = Layer::create();
	result_layer->addChild(hero_image, 1);
	if (id == 0)
		avatar_image = Sprite::create("characters/daji/daji.png");
	else if (id == 1)
		avatar_image = Sprite::create("characters/yase/yase.png");
	else if (id == 2)
		avatar_image = Sprite::create("characters/houyi/houyi.png");
	hero_image->addChild(avatar_image, 1);
	hero_image->setPosition(Vec2(726, 700));

	int hero_level = my_hero->hero.level;
	level_result_label = Label::create(std::to_string(my_hero->hero.level), "Arial", 50);
	level_result_label->setColor(Color3B(255, 255, 255));
	result_layer->addChild(level_result_label, 1);
	level_result_label->setPosition(Vec2(726, 564));

	attack_label = Label::create(std::to_string(my_hero->hero.attack), "Arial", 50);
	attack_label->setColor(Color3B(255, 255, 255));
	result_layer->addChild(attack_label, 1);
	attack_label->setPosition(Vec2(726, 445));

	std::string HP_text = std::to_string(int(my_hero->hero.HP));//Ϊ�˱���HP���ֶ�λС��
	HP_label = Label::create(HP_text, "Arial", 50);
	HP_label->setColor(Color3B(255, 255, 255));
	result_layer->addChild(HP_label, 1);
	HP_label->setPosition(Vec2(740, 327));

	//չʾӢ��ɱ����
	killed_hero_label = Label::create(std::to_string(killed_hero_result), "Arial", 50);
	killed_hero_label->setColor(Color3B(255, 255, 255));
	result_layer->addChild(killed_hero_label, 1);
	killed_hero_label->setPosition(Vec2(1260, 688));


	killed_soldier_label = Label::create(std::to_string(killed_soldier_result), "Arial", 50);
	killed_soldier_label->setColor(Color3B(255, 255, 255));
	result_layer->addChild(killed_soldier_label, 1);
	killed_soldier_label->setPosition(Vec2(1260, 570));


	destroyed_tower_label = Label::create(std::to_string(destroyed_tower_result), "Arial", 50);
	destroyed_tower_label->setColor(Color3B(255, 255, 255));
	result_layer->addChild(destroyed_tower_label, 1);
	destroyed_tower_label->setPosition(Vec2(1260, 448));

	be_killed_label = Label::create(std::to_string(be_killed_result), "Arial", 50);
	be_killed_label->setColor(Color3B(255, 255, 255));
	result_layer->addChild(be_killed_label, 1);
	be_killed_label->setPosition(Vec2(1260, 334));

	result_layer->setVisible(true);
	close_combat_result->setVisible(true);
	level_result_label->setVisible(true);
	attack_label->setVisible(true);
	HP_label->setVisible(true);
	killed_hero_label->setVisible(true);
	killed_soldier_label->setVisible(true);
	destroyed_tower_label->setVisible(true);
	be_killed_label->setVisible(true);
}
//�ر�ս������
void MainScene::CloseResult(Ref* pSender)
{
	result_layer->setVisible(false);
	close_combat_result->setVisible(false);
	result_layer->setVisible(false);
	close_combat_result->setVisible(false);
	level_result_label->setVisible(false);
	attack_label->setVisible(false);
	HP_label->setVisible(false);
	killed_hero_label->setVisible(false);
	killed_soldier_label->setVisible(false);
	destroyed_tower_label->setVisible(false);
	be_killed_label->setVisible(false);

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//��Ӳ鿴ս����Ϸ��ť  
	auto item1 = MenuItemImage::create("combat_result/check_result.png", "combat_result/check_result_selected.png", CC_CALLBACK_1(MainScene::CheckResult, this));
	open_combat_result = Menu::create(item1, NULL);
	open_combat_result->alignItemsVerticallyWithPadding(50);
	//��ť��������Ļ���Ͻ�
	item1->setPosition(Vec2(origin.x + 700, origin.y + 200));
	this->addChild(open_combat_result, 1);
}
