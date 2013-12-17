#pragma once
#include "NNScene.h"
#include "NNSprite.h"
#include "NNAnimation.h"
#include "GameConfig.h"
#include "UIButton.h"
#include "Player.h"

class CUpgradeScene :
	public NNScene
{
public:
	CUpgradeScene(void);
	~CUpgradeScene(void);

public:
	void Render();
	void Update( float dTime );
	void InitUpgradeUI();
	void ShowGlobalMoney();
	
	NNCREATE_FUNC(CUpgradeScene);

protected:
	NNAnimation *m_CoinAnimation;
	NNAnimation *m_ZombieAnimation[NUMBER_OF_ZOMBIE_TYPES-1];
	NNSprite *m_Frame[NUMBER_OF_ZOMBIE_TYPES - 1];
	NNSprite* m_pBackground;

	CUIButton* m_UpgradeButton[7]; //upgrade : 7
	NNLabel* m_ShowGlobalMoney;
};

