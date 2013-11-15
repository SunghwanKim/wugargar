#include "PlayScene.h"
#include "NNPoint.h"
#include "NNInputSystem.h"
#include "NNApplication.h"

#include "PrintConsole.h"

#include "Player.h"
#include "NormalPolice.h"
#include "PoorZombie.h"
#include "VomitZombie.h"
#include "MuscleZombie.h"
#include "KamikazeZombie.h"
#include "SmogZombie.h"
#include "IceZombie.h"
#include "NNSceneDirector.h"
#include "NextStageScene.h"

CPlayScene* CPlayScene::m_pInstance = nullptr;

CPlayScene* CPlayScene::GetInstance()
{
	if ( m_pInstance == nullptr )
	{
		m_pInstance = new CPlayScene();
	}

	return m_pInstance;
}

void CPlayScene::ReleaseInstance()
{
	if ( m_pInstance != nullptr )
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

CPlayScene::CPlayScene(void)
{	
	_initBackground();
	_initMap();
	_initUI();
	

	// temporary
	// FPS�� ǥ���ϰ�, �ܼ�â�� ���
	m_pShowMouseStatus = NNLabel::Create( L"cursor position", L"���� ����", 35.f );
	m_pShowMouseStatus->SetPosition(0.0f, 0.0f);
	AddChild( m_pShowMouseStatus , 20);
	CPrintConsole* print_console = new CPrintConsole;

	m_pCreatePolice = new CCreatePolice;
	CPlayer::GetInstance()->SetPlayerForNewStage();

}

CPlayScene::~CPlayScene(void)
{

}

// init background
void CPlayScene::_initBackground( void )
{
	m_pBackground = CBackground::Create(); 
	AddChild( m_pBackground , 0);
}

// init map

void CPlayScene::_initMap( void )
{	
	m_pMapCreator = CMapCreator::Create();
	AddChild( m_pMapCreator , 1);


}


// init UI
void CPlayScene::_initUI( void )
{
	std::wstring buttonpath_normal[NUMBER_OF_ZOMBIE_TYPES];
	std::wstring buttonpath_pressed[NUMBER_OF_ZOMBIE_TYPES];

	buttonpath_normal[POOR_ZOMBIE] = L"wugargar/UIbuttons/button_normal_poorZombie.png";
	buttonpath_pressed[POOR_ZOMBIE]  = L"wugargar/UIbuttons/button_pressed_poorZombie.png";
	buttonpath_normal[VOMIT_ZOMBIE] = L"wugargar/UIbuttons/button_normal_vomitZombie.png";
	buttonpath_pressed[VOMIT_ZOMBIE] = L"wugargar/UIbuttons/button_pressed_vomitZombie.png";
	buttonpath_normal[MUSCLE_ZOMBIE] = L"wugargar/UIbuttons/button_normal_muscleZombie.png";
	buttonpath_pressed[MUSCLE_ZOMBIE] = L"wugargar/UIbuttons/button_pressed_muscleZombie.png";
	buttonpath_normal[KAMIKAJE_ZOMBIE] = L"wugargar/UIbuttons/button_normal_kamikazeZombie.png";
	buttonpath_pressed[KAMIKAJE_ZOMBIE] = L"wugargar/UIbuttons/button_pressed_kamikazeZombie.png";
	buttonpath_normal[SMOG_ZOMBIE] = L"wugargar/UIbuttons/button_normal_smogZombie.png";
	buttonpath_pressed[SMOG_ZOMBIE] = L"wugargar/UIbuttons/button_pressed_smogZombie.png";
	buttonpath_normal[ICE_ZOMBIE] = L"wugargar/UIbuttons/button_normal_iceZombie.png";
	buttonpath_pressed[ICE_ZOMBIE] = L"wugargar/UIbuttons/button_pressed_iceZombie.png";

	m_pUIBackground = NNSprite::Create(L"wugargar/UIbuttons/UIBackground.jpg");
	m_pUIBackground->SetPosition(0.0f, FIRST_Y_COORDINATE_OF_UIBUTTON);
	AddChild( m_pUIBackground , 19);

	// zombie type�� ����Ͽ� refactoring - ��ȯ
	for (int i=0 ; i<NUMBER_OF_ZOMBIE_TYPES ; ++i) {
		m_pUIMakeZombieButton[i] = CUIButton::Create(buttonpath_normal[i], buttonpath_pressed[i]);
		m_pUIMakeZombieButton[i]->SetPosition(static_cast<float>(GAP_BETWEEN_UIBUTTONS + i*(GAP_BETWEEN_UIBUTTONS+SIZE_OF_UIBUTTON) ), static_cast<float>(FIRST_Y_COORDINATE_OF_UIBUTTON + 30) );
		AddChild( m_pUIMakeZombieButton[i] , 20);
	}
}


void CPlayScene::Render()
{
	NNObject::Render();
}

void CPlayScene::Update( float dTime )
{
	//��������ó��.
	//Win, Lose ������ ���� Scene�̳� Ȥ�� �׿� ���ϴ� ������ �ʿ�
	//������! ��� Update�� ���� ����� ��.
	//(�׷��� ������ ����ν� m_attacktarget�� ����� ���װ� ������ ��)
	switch (CheckGameOver())
	{
	case WIN:
		NNSceneDirector::GetInstance()->ChangeScene(CNextStageScene/*CResultScene*/::Create());
		break;
	case LOSE:
		break;
	default:
		break;
	}

	NNScene::Update(dTime);

	

	//Test_ShowMousePosition(); // ���콺 Ŀ�� ��ġ �ӽ� �׽�Ʈ
	Test_ShowFPS(); //FPS��� �ӽ� �׽�Ʈ

	// ���� ���� ��ư �Է� ó���� �� �Լ��� ������ - ä��
	MakeZombieButtonOperate(dTime);

	// ������ ���� �̵� 
	// ��ư �Է�ó�� �ȿ� ���� �ʿ䰡 ���°� ���� update �Լ��� �� - ��ȯ
	MakeCharacterWalk(dTime);

	CheckDeadCharacter();

	//���� �����س��� ���� ������ �Ѿ���� ���� ó��. �ӽ�.
	if(m_pCreatePolice->table_top_index < 4) {
		MakePoliceFromScript();
	}

	// next stage ȭ������ �̵��ϱ� ���� �ӽ� ����
	// ���� ġ�� �̵���
	if( NNInputSystem::GetInstance()->GetKeyState(VK_RETURN) == KEY_DOWN ) {
		NNSceneDirector::GetInstance()->ChangeScene(CNextStageScene::Create());
		m_pInstance = nullptr;
	}
}

void CPlayScene::MakeZombieButtonOperate(float dTime)
{
	// button1�� ��Ŭ������ �� ���� ����
	// ��� ������ ���� ������ ó���� -ä��

	// �ڵ� refactoring��. zombie type�� intó�� ����ϴµ� �̰� �� �ɸ�. - ��ȯ
	for ( int i=0 ; i<NUMBER_OF_ZOMBIE_TYPES ; ++i ) {
		if( NNInputSystem::GetInstance()->GetKeyState(VK_LBUTTON) ) {	
			if ( m_pUIMakeZombieButton[i]->CheckButtonArea() ) {			
				MakeZombie(static_cast<ZombieType>(i));// ���� ����
			}		
		}
	}
}


void CPlayScene::MakeZombie(ZombieType type)
{
	CZombie *tmpZombieObject = nullptr;
	std::wstring imagePath[NUMBER_OF_ZOMBIE_TYPES];

	imagePath[POOR_ZOMBIE] = L"wugargar/poor_zombie.png";
	imagePath[ICE_ZOMBIE] = L"wugargar/ice_zombie.png";
	imagePath[VOMIT_ZOMBIE] = L"wugargar/vomit_zombie.png";
	imagePath[SMOG_ZOMBIE] = L"wugargar/smog_zombie.png";
	imagePath[MUSCLE_ZOMBIE] = L"wugargar/muscle_zombie.png";
	imagePath[KAMIKAJE_ZOMBIE] = L"wugargar/kamikaze_zombie.png";

	switch(type)
	{
	case POOR_ZOMBIE :
		tmpZombieObject = CPoorZombie::Create();
		break;
	case ICE_ZOMBIE :
		tmpZombieObject = CIceZombie::Create();
		break;
	case VOMIT_ZOMBIE :
		tmpZombieObject = CVomitZombie::Create();
		break;
	case SMOG_ZOMBIE :
		tmpZombieObject = CSmogZombie::Create();
		break;
	case MUSCLE_ZOMBIE :
		tmpZombieObject = CMuscleZombie::Create();
		break;
	case KAMIKAJE_ZOMBIE :
		tmpZombieObject = CKamikazeZombie::Create();
		break;
	default:
		break; // Ŭ������ �Ű������� �Է¹޾� ����ϰ� ����� ������ ������ ��� �����ؼ� �ϴ� �ϵ��ڵ���
	}	

	tmpZombieObject->SetRandomPositionAroundBase();
	tmpZombieObject->InitSprite(imagePath[type]);
	// z_index������ y�� ���� Ŭ���� �տ� ��ġ�Ͽ� �տ� �ִ� ĳ���Ϳ� �߹ؿ� ǥ�õ��� �ʰ� ��.
	// ���߿� �߰��� ������ �ʿ䰡 �־��, z_index�� �ѹ� �� ��Ƽ� #define���� �ִ� ���� ���� �Ű���.
	AddChild( tmpZombieObject , static_cast<int> (10 + tmpZombieObject->GetPositionY() / 10) );

	// insert into zombie list
	m_llistZombie.push_back(tmpZombieObject);
}


//11/11 ���� �۵� Ȯ��
void CPlayScene::MakeCharacterWalk(float dTime)
{
	for ( auto& iter = m_llistPolice.begin() ; iter != m_llistPolice.end() ; iter++ ) {
		(*iter)->SetPosition((*iter)->GetPosition() - NNPoint( ((*iter)->GetMovingSpeed()), 0.0f) * dTime);
	}


	for ( auto& iter = m_llistZombie.begin() ; iter != m_llistZombie.end() ; iter++ ) {
		(*iter)->SetPosition((*iter)->GetPosition() + NNPoint( ((*iter)->GetMovingSpeed()), 0.0f) * dTime);
	}
}


void CPlayScene::MakePoliceFromScript()
{
	enemyType create_enemy_type;
	CPolice *tmpPoliceObject = nullptr;
	create_enemy_type =	m_pCreatePolice->GetCreateEnemyInfo();
	std::wstring imagePath[5];
	imagePath[NORMAL_POLICE] = L"wugargar/normal_police.png";

	switch (create_enemy_type)
	{
	case NORMAL_POLICE:
		tmpPoliceObject = CNormalPolice::Create();
		break;
	case GUN_POLICE:
		break;
	case SHILD_POLICE:
		break;
	case HELLICOPTER:
		break;
	default:
		break;
	}

	if(create_enemy_type != NOT_TIME){
		tmpPoliceObject->SetRandomPositionAroundBase();
		tmpPoliceObject->InitSprite( imagePath[create_enemy_type]);
		AddChild(tmpPoliceObject, 10);
		m_llistPolice.push_back(tmpPoliceObject);

	}
	
}

void CPlayScene::CheckDeadCharacter()
{	
	for ( auto& iter = m_llistPolice.begin() ; iter != m_llistPolice.end() ; iter++ ) {
		if(( (*iter)->GetHP()<= 0 ) || ((*iter)->GetPositionX() < GAME_SCREEN_MIN_SIZE_X)){
			CCharacter *tmpCharacter;
			tmpCharacter = *iter;
			m_llistPolice.erase(iter);
			RemoveChild(tmpCharacter,true);
			break;
		}
	}

	for ( auto& iter = m_llistZombie.begin() ; iter != m_llistZombie.end() ; iter++ ) {
		if(( (*iter)->GetHP()<= 0 ) || ((*iter)->GetPositionX() > GAME_SCREEN_MAX_SIZE_X)){
			CCharacter *tmpCharacter;
			tmpCharacter = *iter;
			m_llistZombie.erase(iter);
			RemoveChild(tmpCharacter,true);
			break;
		}
	}
}

/*
����ȣ. 11/14
���� Base�� ��Ȳ�� üũ�Ͽ� ���� ���� ���θ� üũ.
*/
GameResult CPlayScene::CheckGameOver()
{
	if(m_pMapCreator->GetPoliceBase()->GetHP() <= 0)
		return WIN;
	else if(m_pMapCreator->GetZombieBase()->GetHP() <= 0)
		return LOSE;
	else
		return NOT_END;
}

/////////////////////////////////////////////////////////
///////////////////test �Լ� /////////////////////////////
void CPlayScene::Test_ShowMousePosition()
{
	// ���콺 �������� �������� ���� �ӽ� ����	
	// wsprintf�� float���� ����� �� ���� �ӽù������� �����
//	 	POINT pt;
//	 	GetCursorPos(&pt);
//	 	ScreenToClient( NNApplication::GetInstance()->GetHWND(), &pt);	 
	 
//	 	ZeroMemory(temp, 256);	
//	 	wsprintf(temp, L"windowcoord x : %4d y : %4d", pt.x, pt.y  );
//	 	m_pShowMouseStatus->SetString(temp);
	// ���콺 �����ǿ� �� 
}

void CPlayScene::Test_ShowFPS()
{
	// fps ��¿� �ӽ�
	ZeroMemory(temp, 256);	
	swprintf_s(temp, _countof(temp), L"FPS = %0.3f", NNApplication::GetInstance()->GetFPS() );
	m_pShowMouseStatus->SetString(temp);
	// fps ��¿� ��
}





