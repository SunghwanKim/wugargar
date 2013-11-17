#pragma once

#include "NNScene.h"
#include "NNLabel.h"
#include "NNPoint.h"
#include "Background.h"
#include "UIButton.h"
#include "MapCreator.h"
#include "CreatePolice.h"
#include "Police.h"
#include "Zombie.h"
#include "GameConfig.h"
// 
// enum GameResult{
// 	NOT_END,
// 	LOSE,
// 	WIN
// };

class CPlayScene : public NNScene
{
private :
	static CPlayScene* m_pInstance;
	CPlayScene(void);
	virtual ~CPlayScene(void);

public:
	void Render();
	void Update( float dTime );
	std::list<CCharacter*> GetZombieList() { return m_llistZombie; }
	std::list<CCharacter*> GetPoliceList() { return m_llistPolice; }
	CMapCreator* GetMapCreator(void) { return m_pMapCreator;}
	

	static CPlayScene* GetInstance();
	static void ReleaseInstance();
	void MakeZombie(ZombieType type);
	void MakeCharacterWalk(float dTime);
	void MakeZombieButtonOperate(float dTime);
	void DeadCharacterCollector();
	void MakePoliceFromScript();
	void IncreaseLocalMoney(float dTime);
	bool CheckGameOver();



private :
	void _initBackground( void );
	void _initUI( void );
	void _initMap( void );

private :
	CBackground *m_pBackground;
	CMapCreator *m_pMapCreator;
	NNSprite	*m_pUIBackground;
	CUIButton	*m_pUIMakeZombieButton[NUMBER_OF_ZOMBIE_TYPES];
	NNLabel		*m_pShowMouseStatus;
	NNPoint		m_CursorPos;
	CCreatePolice *m_pCreatePolice;

	std::list<CCharacter*>		m_llistZombie;
	std::list<CCharacter*>		m_llistPolice;

	wchar_t		temp[256]; // ȭ�� ���콺 Ŀ�� ǥ�ø� ���� �ӽú���

public: // update�Լ��� �������ؼ� �׽�Ʈ �Լ� ������ �� �����ϴ� // Good job~
	void Test_ShowMousePosition();
	void Test_ShowFPS();
};
