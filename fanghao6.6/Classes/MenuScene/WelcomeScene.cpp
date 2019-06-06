#include"WelcomeScene.h"
#include"ChooseMode.h"

USING_NS_CC;

Scene* WelcomeScene::createScene()
{
	auto scene = Scene::create();
	auto layer = WelcomeScene::create();
	scene->addChild(layer);
	return scene;
}

bool  WelcomeScene::init()
{
	if (!Layer::init()) 
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//���뱳��
	auto sprite = Sprite::create("WelcomeScene/background.jpg");
	if (sprite == nullptr)
	{
		return false;
	}
	else
	{
		//ͼƬ�ֱ���̫�ߣ����������������ʵ�
		sprite->setScale(0.7);
		sprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
		this->addChild(sprite, 0);
	}

	//��ӿ�ʼ��Ϸ��ť  
	auto start = MenuItemImage::create("WelcomeScene/start_button.png", "WelcomeScene/start_button_press.png", CC_CALLBACK_1(WelcomeScene::WelcomeToMode, this));
	auto menu = Menu::create(start, NULL);
	//��ť��������Ļ���·�
	start->setPosition(Vec2(origin.x, origin.y - 300));
	this->addChild(menu, 1);

	return true;
}

//ת��ѡ��ģʽ����  
void WelcomeScene::WelcomeToMode(Ref* pSender)
{
	Director::getInstance()->replaceScene(ChooseMode::createScene());
}
