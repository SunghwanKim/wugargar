#pragma once

#include "NNScene.h"

#include "NNPoint.h"
#include "GameConfig.h"

class NNLabel;
class NNSprite;
class NNSound;
class NNXML;
class CPlayer;
class CHumanFarm;
class CBackground;
class CXMLWriter;
class CPoliceCreator;
class CZombie;
class CPolice;
class CDeadPolice;
class CLightning;
class MapObstaclManager;
class CUIButton;
class CMapCreator;
class CCharacter;
class CHumanInFarm;
class NNAnimation;

typedef std::list<CCharacter*> LIST_CHARACTER;

class CPlayScene : public NNScene
{
private :
	static CPlayScene* m_pInstance;
	CPlayScene(void);
	virtual ~CPlayScene(void);

public:
	static CPlayScene* GetInstance();
	static void ReleaseInstance();

	void Render();
	void Update( float dTime );
	LIST_CHARACTER *GetZombieList() { return &m_llistZombie; }
	LIST_CHARACTER *GetPoliceList() { return &m_llistPolice; }
	LIST_CHARACTER *GetDeadPoliceList() { return &m_llistDeadPolice;}
	CMapCreator* GetMapCreator(void) { return m_pMapCreator;}

private:
	void MakeZombie(ZombieType type);
	void MakeZombie(ZombieType type, NNPoint* position);
	void MakeZombieButtonOperate(float dTime);
	void DeadCharacterCollector();
	void MakePoliceFromScript();
	void MakePoliceFromScriptWithTimeInterval(float stageElapsedTime);
	void CollectDeadPoliceByClick();
	bool CheckGameOver();
	void ShowResult( std::wstring result );	
	void SaveGame(void);

	std::wstring string2wstring(std::string str);

	void loadCharacterInfo();	

	void _initBackground( void );
	void _initUI( void );
	void _initMap( void );

	void initLoading();

private :
	CPlayer		*m_pPlayer;
	CBackground *m_pBackground;
	CMapCreator *m_pMapCreator;
	NNSprite	*m_pUIBackground;

	//result ���� ����
	NNSprite	*m_pResultImage;
	NNLabel		*m_pResultLabel;
	wchar_t		m_aResultBuffer[255];
	CUIButton	*m_pResultOKButton;
	CXMLWriter	*m_SaveManager;

	CUIButton	*m_pUIButtons[NUM_OF_UIBUTTON];
	NNLabel		*m_pShowMouseStatus;
	NNLabel		*m_pShowMeatPoint;
	NNPoint		m_CursorPos;
	CPoliceCreator	*m_pPoliceCreator;
	CHumanFarm		*m_pHumanFarm;
	CLightning		*m_pLightning;
	MapObstaclManager *m_pMapObstacleManager;
	NNSound		*m_backgraound_sound;

	NNXML		*m_PoliceXML;
	NNXML		*m_ZombieXML;

	LIST_CHARACTER		m_llistZombie;
	LIST_CHARACTER		m_llistPolice;
	LIST_CHARACTER 		m_llistDeadPolice;

	wchar_t		temp[256]; // ȭ�� ���콺 Ŀ�� ǥ�ø� ���� �ӽú���
	wchar_t		localmoney[256]; //meat Point ǥ�ø� ���� �ӽú���
	float		m_LocalMoneyTimeChecker; // local money������ ���� 1�ʴ��� ī����
	float		m_StageElapsedTime;// ������ ���۵� �ĺ��� ����� �ð� ������ ����
	int			m_EarningIntervalUpgradeCost; // �� ȹ�� �ӵ��� ��½�Ű�µ� ��� ���

	NNAnimation *m_pLoadingAnimation;
	NNSprite *tmp;

public: // update�Լ��� �������ؼ� �׽�Ʈ �Լ� ������ �� �����ϴ� // Good job~
	void Test_ShowMousePosition();
	void Test_ShowFPS();
	void Test_ShowLocalMoney();
};
