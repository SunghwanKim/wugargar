#pragma once
#include "NNObject.h"

//Ŭ���� ���漱��
class NNAnimation;
class NNSpriteAtlas;
class NNSprite;

enum Age
{
	SMALL_BABY,
	MIDDLE_BABY,
	GROWN_UP,
	DEAD
};

class CHumanInFarm : public NNObject
{
public:
	CHumanInFarm(void);
	virtual ~CHumanInFarm(void);

public :
	NNSpriteAtlas *m_pSmallBabyAtlas;
	NNSpriteAtlas *m_pMiddleBabyAtlas;
	NNSpriteAtlas *m_pGrownUpAtlas;

	NNAnimation *m_pSmallBaby;
	NNAnimation *m_pMiddleBaby;
	NNAnimation *m_pGrownUp;

	NNSprite *m_pDead;

	std::wstring DeadImagePath[6];

	void Render();
	void Update( float dTime );
	NNCREATE_FUNC(CHumanInFarm);

public :
	void CreateBaby();
	void SetRandomPositionInFarm();
	void Grow();
	void CollectMeatPointFromGrownUp();
	void Hungry(); //���� ����

private :
	clock_t life_time;
	clock_t birth_time;
	Age m_AgeState;
};

