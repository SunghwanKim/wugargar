#include "StartScene.h"
#include "NNInputSystem.h"
#include "NNSceneDirector.h"
#include "PlayScene.h"
#include "NNResourceManager.h"
#include "GameConfig.h"
#include "CharacterConfig.h"
#include "NNApplication.h"
#include "NNAudioSystem.h"
#include "StageSelectScene.h"
#include "UpgradeScene.h"
#include "NNSound.h"
#include "NNSprite.h"
#include "NNLabel.h"


CStartScene::CStartScene(void)
{
	InitBackGround();
	InitButtons();

	//Sound�߰�. ������̸� ���� loop�Ǹ� ����ǵ���
	m_startscene_background_sound = NNResourceManager::GetInstance()->
		LoadSoundFromFile("sound/StartscenBackgroundSound.mp3", true, true);

	NNAudioSystem::GetInstance()->Play(m_startscene_background_sound);
}

void CStartScene::InitBackGround( void )
{
	m_pStartSceneBackground = 
		NNSprite::Create( L"wugargar/StartSceneImage.jpg");
	m_pStartSceneBackground->SetPosition(0.0f, 0.0f);
	AddChild( m_pStartSceneBackground );
}


void CStartScene::InitButtons( void )
{
	//���� ��ư���� Label�� �̿��� ������� ����. �� ����
	m_MenuBar[MENU_PLAY] = 
		NNLabel::Create( L"NEW GAME", L"�ü�ü", 25.f );
	m_MenuBar[MENU_PLAY]->SetPosition( 48, 480 );
	AddChild( m_MenuBar[MENU_PLAY] );

	m_MenuBar[MENU_LOAD] =
		NNLabel::Create( L"LOAD GAME", L"�ü�ü", 25.f );
	m_MenuBar[MENU_LOAD]->SetPosition(48, 520 );
	AddChild( m_MenuBar[MENU_LOAD] );

	m_MenuBar[MENU_QUIT] = 
		NNLabel::Create( L"QUIT", L"�ü�ü", 25.f );
	m_MenuBar[MENU_QUIT]->SetPosition(48, 560 );
	AddChild( m_MenuBar[MENU_QUIT] );

	//���� ���� �Ͼ������ ����(�Ⱥ��̴ϱ�)
	for(int idx=0; idx<MENU_BAR_NUM; ++idx)
		m_MenuBar[idx]->SetColor( 230.f, 230.f, 230.f );

	m_KeyOn = 0;			
	// ���� ����Ű�� �ִ� �޴� ��ġ

}

CStartScene::~CStartScene(void)
{
	//�Ҹ��ڿ��� ����� ��� ����
	NNAudioSystem::GetInstance()->Stop( m_startscene_background_sound );

}

void CStartScene::Render()
{
	NNObject::Render();
}

void CStartScene::Update( float dTime )
{
	NNScene::Update(dTime);

	
	//m_KeyOn�� �̿��Ͽ� ���� ����, �Է� ����
	if ( NNInputSystem::GetInstance()->GetKeyState(VK_UP) == KEY_DOWN
		&& m_KeyOn!=MENU_PLAY)
		--m_KeyOn;

	else if ( NNInputSystem::GetInstance()->GetKeyState(VK_DOWN) == KEY_DOWN 
		&& m_KeyOn!=MENU_QUIT)
		++m_KeyOn;

	m_KeyOn = (m_KeyOn + MENU_BAR_NUM) % MENU_BAR_NUM;
	//���� ����Ű�� �ִ� �κ��� ������ ����������
	m_MenuBar[m_KeyOn]->SetColor( 255.f, 50.f, 50.f );


	//���� ��ư ������ ���õ� �κ� �����ϰԵ�
	if ( NNInputSystem::GetInstance()->GetKeyState(VK_RETURN) == KEY_DOWN )
	{
		switch (m_KeyOn)
		{
		case MENU_PLAY:
			NNSceneDirector::GetInstance()->
				ChangeScene(CStageSelectScene::Create());
			return ;
			break;
		case MENU_LOAD:
			load();
			NNSceneDirector::GetInstance()->
				ChangeScene(CStageSelectScene::Create());
			return ;
			break;
		case MENU_QUIT:
			//���� ó�� ����
			PostMessage( NNApplication::GetInstance()->GetHWND(), 
				WM_DESTROY, 0, 0 );
			break;
		default:
			break;
		}


	}

	//���� ���õǾ� �ִ� �κ� �� ���� ������� �缳��
	for(int idx=0; idx<MENU_BAR_NUM; ++idx)
		if(idx!= m_KeyOn)
			m_MenuBar[idx]->SetColor( 230.f, 230.f, 230.f );


}

// load game
// NNXML�� xml������ savefile�� �ҷ��´�.-sh
void CStartScene::load()
{
	// initialize
	
 	CPlayer* pPlayer = CPlayer::GetInstance();
	NNXML *m_TestXML = NNResourceManager::GetInstance()->
		LoadXMLFromFIle("savegame.sav");

	// ����ó��
	if (!(m_TestXML->GetLoadSuccess())) {
		return ;
	}

	// get files from save file
	int globalMoney = std::stoi(m_TestXML->
		XPathToString("/player1/GlobalMoney/text()").c_str());
	pPlayer->SetGlobalMoney( globalMoney ); 

	int totalkill = std::stoi(m_TestXML->
		XPathToString("/player1/TotalKill/text()").c_str());
	pPlayer->SetTotalKill( totalkill ); 

	int totalloss = std::stoi(m_TestXML->
		XPathToString("/player1/TotalLoss/text()").c_str());
	pPlayer->SetTotalLoss( totalloss);

	int currentstage =  std::stoi(m_TestXML->
		XPathToString("/player1/CurrentStage/text()").c_str());
	pPlayer->SetCurrentStage(currentstage); 

	float infectionrate = std::stof(m_TestXML->
		XPathToString("/player1/InfectionRate/text()").c_str());
	pPlayer->SetInfectionRate(infectionrate ); 


	for ( int i=0; i<NUMBER_OF_ZOMBIE_TYPES ; ++i ) {
		std::string xPath = "/player1/UnitLevel/";
		xPath.append("UnitNo");
		xPath.append(std::to_string(i) );
		xPath.append("/text()");
		pPlayer->SetZombieLevel(static_cast<ZombieType>(i), 
			std::stoi(m_TestXML->XPathToString(xPath).c_str() ) );
	}
	
#ifdef _DEBUG
	printf("%d\n", pPlayer->GetGlobalMoney() );
	printf("%d\n", pPlayer->GetTotalKill() );
	printf("%d\n", pPlayer->GetTotalLoss() );
	printf("%d\n", pPlayer->GetClearedStage() );
	printf("%f\n", pPlayer->GetInfectionRate() );
	for ( int i=0 ; i<NUMBER_OF_ZOMBIE_TYPES ; ++i ) {
		printf("%d ", pPlayer->GetZombieLevel
			(static_cast<ZombieType>(i)) );
	}	
#endif

}
