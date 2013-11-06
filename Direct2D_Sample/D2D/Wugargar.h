#pragma once

#include "nnscene.h"
#include "Background.h"
#include "PoorZombie.h"
#include "NNLabel.h"
#include "NNPoint.h"
#include "UIButton.h"
#include "MapCreator.h"
#include "CreatePolice.h"


enum ZombieType
{
	POOR_ZOMBIE,
	VOMIT_ZOMBIE,
	SMOG_ZOMBIE,
	ICE_ZOMBIE,
	KAMIKAJE_ZOMBIE,
	MUSCLE_ZOMBIE,
}; // ���� ������ Ÿ���� �˾ƺ��� ���� enum�� - ä��

class CWugargar : public NNScene
{
private :
	static CWugargar* m_pInstance;

private:
	CWugargar(void);
	~CWugargar(void);

public:
	void Render();
	void Update( float dTime );
	std::list<CZombie*> GetZombieList() { return m_llistZombie; }
	std::list<CPolice*> GetPoliceList() { return m_llistPolice; }
	CMapCreator* GetMapCreator(void) { return m_pMapCreator;}

	static CWugargar* GetInstance();
	static void ReleaseInstance();
	void MakeZombie(ZombieType type);
	void MakeCharacterWalk(float dTime);
	void MakeZombieButtonOperate(float dTime);


private :
	void _initBackground( void );
	void _initUI( void );
	void _initMap( void );

private :
	CBackground *m_pBackground;
//	CPoorZombie *m_pPoorZombie; ���ִ��� �𸣰���.. ������ �Ǵ°���? - ä��
	CMapCreator *m_pMapCreator;
	NNSprite	*m_pUIBackground;
	CUIButton	*m_pUIButton1;
	CUIButton	*m_pUIButton2;
	CUIButton	*m_pUIButton3;
	CUIButton	*m_pUIButton4;
	CUIButton	*m_pUIButton5;
	CUIButton	*m_pUIButton6;
	NNLabel		*m_pShowMouseStatus;
	NNPoint		m_CursorPos;
	CCreatePolice *m_pCreatePolice;

	std::list<CZombie*>		m_llistZombie;
	std::list<CPolice*>		m_llistPolice;

	wchar_t		temp[256]; // ȭ�� ���콺 Ŀ�� ǥ�ø� ���� �ӽú���

public: // update�Լ��� �������ؼ� �׽�Ʈ �Լ� ������ �� �����ϴ�
	void Test_ShowMousePosition();
	void Test_ShowFPS();
};
