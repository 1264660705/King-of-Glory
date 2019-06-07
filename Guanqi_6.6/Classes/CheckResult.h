#ifndef __CHECK_RESULT_H__
#define __CHECK_RESULT_H__

#include "cocos2d.h"
#include"SimpleAudioEngine.h"

using namespace CocosDenshion;

class CheckResult :public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	SimpleAudioEngine* Sound = SimpleAudioEngine::getInstance();

	virtual bool init();
	cocos2d::Label* experience_label;//��ʾ����
	cocos2d::Label* level_label;//��ʾ�ȼ�
	cocos2d::Label* money_label;//��ʾ���
	cocos2d::Label* killed_hero_label;//��ʾ��ɱӢ����
	cocos2d::Label* killed_soldier_label;//��ʾ��ɱС����
	cocos2d::Label* destroyed_tower_label;//��ʾ�ݻ�����
	cocos2d::Label* be_killed_label;//��ʾ����ɱ��


	//ת�����˵�  
	void ReturnToWelcome(Ref* pSender);

	CREATE_FUNC(CheckResult);
};
extern int money_result;       //ӵ�еĽ��
extern int level_result;//�ȼ�
extern int experience_result;//����
extern int killed_hero_result;      //��ɱӢ����
extern int killed_soldier_result;//��ɱС����
extern int destroyed_tower_result;//�ݻ�����
extern int be_killed_result;   //������

#endif