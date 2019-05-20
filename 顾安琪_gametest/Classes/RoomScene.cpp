#include"RoomScene.h"
#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "MapScene.h"
USING_NS_CC;
using namespace std;

Scene* RoomScene::createScene()
{
	auto scene = Scene::create();
	auto layer = RoomScene::create();
	scene->addChild(layer);
	return scene;
}

bool  RoomScene::init()
{
	if (!Layer::init()) {
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//���뱳��
	auto sprite = Sprite::create("room.png");
	if (sprite == nullptr)
	{
		return false;
	}
	else
	{
		// position the sprite on the center of the screen
		sprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

		// add the sprite as a child to this layer
		this->addChild(sprite, 0);
	}

	/////////////////////
	//��ӿ�ʼ��Ϸ��ť  
	auto item1 = MenuItemImage::create("startGame.png", "startGame_selected.png", CC_CALLBACK_1(RoomScene::RoomToMap, this));
	auto menu_oper = Menu::create(item1, NULL);
	menu_oper->alignItemsVerticallyWithPadding(50);
	//��ť��������Ļ���м�
	item1->setPosition(Vec2(origin.x, origin.y-300));

	this->addChild(menu_oper, 1);
	//////////////////////

	//�˳�
	auto quit = MenuItemFont::create("quit", CC_CALLBACK_1(RoomScene::backToMainScene, this));
	quit->setName("quit");

	//create menu, it's an autorelease object
	auto menu = Menu::create(quit, nullptr);
	menu->alignItemsVertically();
	this->addChild(menu);
	menu->setPositionY(visibleSize.height / 2 + 400);
	menu->setPositionX(visibleSize.width / 2 + 300);

	return true;

}

//����MapScene�ķ���
void RoomScene::RoomToMap(Ref* pSender) {
	auto scene = Scene::create();
	scene->addChild(MapScene::create());
	Director::getInstance()->replaceScene(TransitionFlipX::create(.5, scene));
}

//����HelloWorld������
void RoomScene::backToMainScene(Ref * pSender) {
	auto scene = Scene::create();
	scene->addChild(HelloWorld::create());
	Director::getInstance()->replaceScene(TransitionFlipX::create(.5, scene));
}
