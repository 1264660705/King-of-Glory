#ifndef  _SHOP_MENU_H
#define  _SHOP_MENU_H

#include "cocos2d.h"

class ShopMenu :public cocos2d::Layer
{
public:

    static bool init();

	//���̵����
	void OpenShop(Ref* pSender);
};

#endif