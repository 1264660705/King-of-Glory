#include"MainScene.h"
#include"SimpleAudioEngine.h"
//�ģ�#include"CheckResult.h"
#include<vector>
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

	//����
	int ai_id = 5;
	ai_hero = HeroSprite::create(ai_id);
	map_layer->addChild(ai_hero);
	ai_hero->setPosition(Vec2(6000, 700));

	//���㴥���¼������� �����ҷ�Ӣ��
	auto m = EventListenerTouchOneByOne::create();
	//[=]��ֵ���ݲ���ǰ�����ĵ����б���
	m->onTouchBegan = 
		[=](Touch * touch, Event * e)
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

		//����жϣ����Ŀ���ƶ�λ���е��������ƶ�
		//�ģ�bool MyHero_CanMove = 1;
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
					auto hero_attack_effect = AttackEffect::create(id);
					map_layer->addChild(hero_attack_effect);
					hero_attack_effect->setScale(0.5);
					hero_attack_effect->setPosition(my_hero->getPosition() - map_layer_position);
					auto call = CallFuncN::create([=](Node * t)
						{
							dropBlood(i, my_hero->hero.attack);
						});
					//����Ч��
					//��������(Sequence) ��һ�ַ�װ��������Ķ��󣬵��������ִ��ʱ����װ�Ķ�����˳��ִ��
					//�ȷ��У��ٵ�Ѫ��������ʧ
					hero_attack_effect->runAction(
						Sequence::create(MoveTo::create((my_hero_position - map_layer_position).distance(i->getPosition()) / 200, i->getPosition()),
							call, FadeOut::create(0.1), 0));
					return false;
				}
				/*�ģ�
				if (i->boundingBox().containsPoint(touch_position) && i->getPosition().distance(my_hero_position - map_layer_position)> my_hero->hero.attack_distance)
					MyHero_CanMove = 0;
					*/
			}

			//�����з�Ӣ��
			if (ai_hero->boundingBox().containsPoint(touch_position) && ai_hero->getPosition().distance(my_hero_position - map_layer_position) <= my_hero->hero.attack_distance && ai_hero->hero.survival == true)
			{
				auto hero_attack_effect = AttackEffect::create(id);
				map_layer->addChild(hero_attack_effect);
				hero_attack_effect->setScale(0.5);
				hero_attack_effect->setPosition(my_hero->getPosition() - map_layer_position);
				auto call = CallFuncN::create([=](Node * t)
					{
						dropBlood(ai_hero, my_hero->hero.attack);
					});
				hero_attack_effect->runAction(
					Sequence::create(MoveTo::create((my_hero_position - map_layer_position).distance(ai_hero->getPosition()) / 200, ai_hero->getPosition()),
						call, FadeOut::create(0.1), 0));
				return false;
			}
			/*�ģ�
			if(ai_hero->boundingBox().containsPoint(touch_position) && ai_hero->getPosition().distance(my_hero_position - map_layer_position)> my_hero->hero.attack_distance)
				MyHero_CanMove = 0;
				*/
			//������
			for (int i = 2; i <= 3; ++i)
			{
				if (towers[i]->boundingBox().containsPoint(touch_position) && towers[i]->getPosition().distance(my_hero_position - map_layer_position) <= my_hero->hero.attack_distance && towers[i]->tower.survival == true)
				{
					auto hero_attack_effect = AttackEffect::create(id);
					map_layer->addChild(hero_attack_effect);
					hero_attack_effect->setScale(0.5);
					hero_attack_effect->setPosition(my_hero->getPosition() - map_layer_position);
					auto call = CallFuncN::create([=](Node * t)
						{
							dropBlood(towers[i], my_hero->hero.attack);
						});
					hero_attack_effect->runAction(
						Sequence::create(MoveTo::create((my_hero_position - map_layer_position).distance(towers[i]->getPosition()) / 200, towers[i]->getPosition()),
							call, FadeOut::create(0.1), 0));
					return false;
				}
				/*�ģ�
				if (towers[i]->boundingBox().containsPoint(touch_position) && towers[i]->getPosition().distance(my_hero_position - map_layer_position) > my_hero->hero.attack_distance)
				MyHero_CanMove = 0;
				*/
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
		int speed = 800;

		/*�ģ�
		if (MyHero_CanMove) 
		{
		my_hero->runAction(MoveBy::create(touch_position_change.distance(e->getCurrentTarget()->getPosition()) / speed, Vec2(0, touch_position_change.y - my_hero_position_change.y)));
		map_layer->runAction(MoveBy::create(touch_position_change.distance(e->getCurrentTarget()->getPosition()) / speed, Vec2((my_hero_position_change.x - touch_position_change.x), 0)));
		}
		*/
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
															   
	//ս�������󷵻�ս������
	//�ģ�schedule(schedule_selector(MainScene::onPushSceneCheckResult), 5.0f);
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
				blue_flag = false;
				if ((*blue_iter)->soldier.attack_speed == 100)//��ΪΪÿ��ˢ�£����Ծ�b_num�͸�һ��
				{
					auto tower_attack_effect = AttackEffect::create(3);//С��������Ч
					map_layer->addChild(tower_attack_effect);
					tower_attack_effect->setPosition((*blue_iter)->getPosition());
					auto call = CallFuncN::create([=](Node* t)
						{
							dropBlood(*red_iter, (*blue_iter)->soldier.attack);
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
							dropBlood(towers[3], (*blue_iter)->soldier.attack);
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
							dropBlood(towers[2], (*blue_iter)->soldier.attack);
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
			(*blue_iter)->runAction(MoveBy::create(0.02, Vec2(1.5, 0)));
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
				red_flag = false;
				if ((*red_iter)->soldier.attack_speed == 100)//��ΪΪÿ��ˢ�£����Ծ�b_num�͸�һ��
				{
					auto tower_attack_effect = AttackEffect::create(3);//С��������Ч
					map_layer->addChild(tower_attack_effect);
					tower_attack_effect->setPosition((*red_iter)->getPosition());
					auto call = CallFuncN::create([=](Node* t)
						{
							dropBlood(*blue_iter, (*red_iter)->soldier.attack);
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
						dropBlood(towers[1], (*red_iter)->soldier.attack);
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
						dropBlood(towers[0], (*red_iter)->soldier.attack);
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
			(*red_iter)->runAction(MoveBy::create(0.02, Vec2(-1.5, 0)));
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
		my_hero->hero.HP = 500;
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
		ai_hero->hero.HP = 500;
		ai_hero->blood->setScaleX(0.5);
		ai_hero->setPosition(Vec2(6000, 700));
		ai_hero->hero.resurrection_time = 10;
	}
	if (ai_hero->hero.survival == false)
	{
		--ai_hero->hero.resurrection_time;
	}
}

void MainScene::tower_attack(float)
{
	bool flag0 = true;
	if (towers[0]->tower.survival == true)
	{
		for (auto iter = red_soldier_vec.begin(); iter != red_soldier_vec.end() && flag0 == true; ++iter)
		{
			if ((*iter)->getPosition().distance(towers[0]->getPosition()) < towers[0]->tower.attack_distance && (*iter)->soldier.survival == true)
			{
				flag0 = false;
				auto tower_attack_effect = AttackEffect::create(4);//������������Ч
				map_layer->addChild(tower_attack_effect);
				tower_attack_effect->setScale(0.5);
				tower_attack_effect->setPosition(towers[0]->getPosition());
				auto call = CallFuncN::create([=](Node* t)
					{
						dropBlood((*iter), towers[0]->tower.attack);
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
				flag1 = false;
				auto tower_attack_effect = AttackEffect::create(4);//������������Ч
				map_layer->addChild(tower_attack_effect);
				tower_attack_effect->setScale(0.5);
				tower_attack_effect->setPosition(towers[1]->getPosition());
				auto call = CallFuncN::create([=](Node* t)
					{
						dropBlood((*iter), towers[1]->tower.attack);
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
				flag2 = false;
				auto tower_attack_effect = AttackEffect::create(4);//������������Ч
				map_layer->addChild(tower_attack_effect);
				tower_attack_effect->setScale(0.5);
				tower_attack_effect->setPosition(towers[2]->getPosition());
				auto call = CallFuncN::create([=](Node* t)
					{
						dropBlood((*iter), towers[2]->tower.attack);
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
				flag3 = false;
				auto tower_attack_effect = AttackEffect::create(4);//������������Ч
				map_layer->addChild(tower_attack_effect);
				tower_attack_effect->setScale(0.5);
				tower_attack_effect->setPosition(towers[3]->getPosition());
				auto call = CallFuncN::create([=](Node* t)
					{
						dropBlood((*iter), towers[3]->tower.attack);
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

void MainScene::gameOver(float)
{
	if (towers[0]->tower.survival == false)
	{
		fail->setVisible(true);
	}
	if (towers[3]->tower.survival == false)
	{
		win->setVisible(true);
	}
}

void MainScene::ai_hero_attack(float)
{
	//����һ��ֻ����һ��
	bool flag = true;
	//����������ɫ�� ��ai��Ҫ�Ǵ���
	for (auto iter = blue_soldier_vec.begin(); iter != blue_soldier_vec.end()&& ai_hero->hero.survival == true; ++iter)
	{
		if (ai_hero->getPosition().distance((*iter)->getPosition()) < ai_hero->hero.attack_distance && (*iter)->soldier.survival == true)
		{
			//Ĭ���Ǻ���
			flag = false;
			auto hero_attack_effect = AttackEffect::create(2);
			map_layer->addChild(hero_attack_effect);
			hero_attack_effect->setScale(0.5);
			hero_attack_effect->setPosition(ai_hero->getPosition());
			auto call = CallFuncN::create([=](Node* t)
				{
					dropBlood((*iter), ai_hero->hero.attack);
				});
			//����Ч��
			hero_attack_effect->runAction(
				Sequence::create(MoveTo::create(1.2f, (*iter)->getPosition()),
					call, FadeOut::create(0.1), 0));
			break;
		}
	}

	//����Ӣ��
	if (ai_hero->getPosition().distance(my_hero->getPosition() -map_layer->getPosition()) < ai_hero->hero.attack_distance 
		&& ai_hero->hero.survival == true && my_hero->hero.survival == true && flag==true)
	{
		//Ĭ���Ǻ���
		flag = false;
		auto hero_attack_effect = AttackEffect::create(2);
		map_layer->addChild(hero_attack_effect);
		hero_attack_effect->setScale(0.5);
		hero_attack_effect->setPosition(ai_hero->getPosition());
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
		if (ai_hero->getPosition().distance(towers[i]->getPosition()) <ai_hero->hero.attack_distance && towers[i]->tower.survival == true && flag == true)
		{
			flag = false;
			//Ĭ���Ǻ���
			auto hero_attack_effect = AttackEffect::create(2);
			map_layer->addChild(hero_attack_effect);
			hero_attack_effect->setScale(0.5);
			hero_attack_effect->setPosition(ai_hero->getPosition());
			auto call = CallFuncN::create([=](Node* t)
				{
					dropBlood(towers[i], ai_hero->hero.attack);
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

void MainScene::time(float)
{
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
	time_label->setString(time);//��ʾʱ��

	//ÿһ���ӳ�һ����
	if (time_second == 1)//��ս
	{
		auto blue_soldier = SoldierSprite::create(0);
		map_layer->addChild(blue_soldier);
		blue_soldier_vec.push_back(blue_soldier);
		blue_soldier->setPosition(Vec2(1600, 650));
		//blue_soldier->setPosition(Vec2(2600, 650));

		auto red_soldier = SoldierSprite::create(2);
		map_layer->addChild(red_soldier);
		red_soldier_vec.push_back(red_soldier);
		red_soldier->setPosition(Vec2(4900, 650));
	}
	if (time_second == 3)//�ڳ�
	{
		auto blue_soldier = SoldierSprite::create(1);
		map_layer->addChild(blue_soldier);
		blue_soldier_vec.push_back(blue_soldier);
		blue_soldier->setPosition(Vec2(1600, 650));

		auto red_soldier = SoldierSprite::create(3);
		map_layer->addChild(red_soldier);
		red_soldier_vec.push_back(red_soldier);
		red_soldier->setPosition(Vec2(4700, 650));
	}
	
}

//Ӣ�۵�Ѫ
void MainScene::dropBlood(HeroSprite* hero, int attack)
{
	hero->hero.HP -= attack;
	//�����ţ���Ѫ����Ӧ�仯
	if (hero->hero.HP > 0)
	{
		hero->blood->setScaleX((hero->hero.HP / 500) * 0.5);
	}
	//���ˣ�Ѫ����ʧ��ͼƬ��ʧ���߼�Ϊ����
	else
	{
		hero->blood->setScaleX(0);
		hero->setVisible(false);
		hero->hero.survival = false;
	}
}

//����Ѫ
void MainScene::dropBlood(TowerSprite* tower, int attack)
{
	tower->tower.HP -= attack;
	//�����ţ���Ѫ����Ӧ�仯
	if (tower->tower.HP > 0)
	{
		tower->blood->setScaleX((tower->tower.HP / 800) * 0.5);
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
void MainScene::dropBlood(SoldierSprite* soldier, int attack)
{
	soldier->soldier.HP -= attack;
	//�����ţ���Ѫ����Ӧ�仯
	if (soldier->soldier.HP > 0)
	{
		soldier->blood->setScaleX((soldier->soldier.HP/ 300) * 0.5);
	}
	//���ˣ�Ѫ����ʧ��ͼƬ��ʧ���߼�Ϊ����
	else
	{
		soldier->blood->setScaleX(0);
		soldier->setVisible(false);
		soldier->soldier.survival = false;
	}
}

//�Ƿ��ܹ��˶�
void MainScene::sport(float)
{
	
	/*ɾ�����������Խ�������ߵ���ͼ�Ϸ�ʱ�²�����bug
	//�����߽�
	if (!map->boundingBox().containsPoint(my_hero->getPosition()))
	{
		my_hero->stopAllActions();
	}*/
	//�ҷ�Ӣ������
	if (my_hero->hero.survival == false)
	{
		my_hero->stopAllActions();
	}
	ai_hero->hero.can_move = true;
	for (auto iter=blue_soldier_vec.begin();iter!= blue_soldier_vec.end();++iter)
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
		ai_hero->setPosition(ai_hero->getPosition() + Vec2(-7, 0));
	}
}

//ת���鿴ս������
/*�ģ�
void MainScene::onPushSceneCheckResult(float dt)
{
	if (towers[0]->tower.survival == false || towers[3]->tower.survival == false)
	{
		Director::getInstance()->replaceScene(CheckResult::createScene());
	}
}
*/


