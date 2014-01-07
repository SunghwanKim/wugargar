#pragma once
#include "NNScene.h"
#include "NNSprite.h"
#include "NNAnimation.h"
#include "GameConfig.h"
#include "UIButton.h"
#include "Player.h"
#include "NNInputSystem.h"

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
	void OperateUpgradeButton();
	void UpgradeCostSet();

	NNCREATE_FUNC(CUpgradeScene);

protected:
	NNAnimation *m_CoinAnimation;
	//[NUMBER_OF_ZOMBIE_TYPES - 1] ���׷��̵� ��� ������ �����ϸ� ���δ� �ٲ������ �Ф� �׳� define �� �ϳ� �� ���ִ°� ������ �����
	NNAnimation *m_ZombieAnimation[NUMBER_OF_ZOMBIE_TYPES-1];
	NNSprite *m_Frame[NUMBER_OF_ZOMBIE_TYPES - 1];
	NNSprite* m_pBackground;
	NNAnimation* m_pLoading;

	CUIButton* m_UpgradeButton[NUMBER_OF_ZOMBIE_TYPES - 1]; 
	NNLabel* m_ShowGlobalMoney;
	NNLabel* m_ShowUpgradeLevel[NUMBER_OF_ZOMBIE_TYPES - 1];
	int		m_UpgradeCost[NUMBER_OF_ZOMBIE_TYPES-1];
	NNLabel* m_ShowUpgradeCost[NUMBER_OF_ZOMBIE_TYPES - 1];
	CUIButton* m_pNextStageButton;
	CUIButton* m_pPreStageButton;
};

