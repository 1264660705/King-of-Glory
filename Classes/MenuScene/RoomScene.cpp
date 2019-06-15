#include<string>
#include"RoomScene.h"
#include"../NetWork/Server.h"
#include"../NetWork/Client.h"
#include"OnlineChooseHero.h"

USING_NS_CC;

extern std::string choose_mode;
Server* server;
Client* client1;
Client* client2;


Scene* RoomScene::createScene()
{
	auto scene = Scene::create();
	auto layer = RoomScene::create();
	scene->addChild(layer);
	return scene;
}

bool  RoomScene::init()
{
	server = new Server();
	if (choose_mode == "create")
	{
		client1 = new Client("A");
	}
	else if (choose_mode == "join")
	{
		client2 = new Client("B");
	}

	if (!Layer::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//���뱳��
	auto sprite = Sprite::create("RoomScene/room.png");
	if (sprite == nullptr)
	{
		return false;
	}
	else
	{
		sprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
		this->addChild(sprite, 0);
	}

	//��ӿ�ʼ��Ϸ��ť  
	auto item1 = MenuItemImage::create("RoomScene/startGame.png", "RoomScene/startGame_selected.png", CC_CALLBACK_1(RoomScene::RoomToGame, this));
	auto menu = Menu::create(item1, NULL);
	menu->alignItemsVerticallyWithPadding(50);
	//��ť��������Ļ���м�
	item1->setPosition(Vec2(origin.x, origin.y - 300));
	this->addChild(menu, 1);

	//ȷ�����ӳɹ�
	std::string resurrection("Resurrection time: ");
	if (choose_mode == "create")
	{
		while (true)
		{
			if (server->is_connected())
			{
				break;
			}
		}
	}
	return true;
}

void RoomScene::RoomToGame(Ref* pSender)
{
	if (choose_mode == "create")
	{
		//�������μ���Ϸ
		Director::getInstance()->replaceScene(OnlineChooseHero::createScene(client1));
	}
	else if (choose_mode == "join")
	{
		//�Ժ췽�μ���Ϸ
		Director::getInstance()->replaceScene(OnlineChooseHero::createScene(client2));
	}
}


