#include "StageSelectScene.h"
#include "Player.h"
#include "PlayScene.h"
#include "NNInputSystem.h"
#include "NNSceneDirector.h"


CStageSelectScene::CStageSelectScene(void)
{
	InitMapSprite();
}


CStageSelectScene::~CStageSelectScene(void)
{
}

void CStageSelectScene::InitMenuButtons( void )
{

}

void CStageSelectScene::Render()
{
	NNObject::Render();
}

void CStageSelectScene::Update( float dTime )
{
	NNScene::Update(dTime);


	int current_stage =  CPlayer::GetInstance()->GetCurrentStage() / 100;
	if(NNInputSystem::GetInstance()->GetKeyState(VK_LBUTTON))
	{
		for (int idx=0; idx<STAGE_NUM; ++idx)
		{
			//���õ� �������� ��ư�� Ŭ���� �� �� �ִ� �������� �� ��� ���õ�
			if(m_stageFlag[idx]->CheckButtonArea() && (idx+1) <= current_stage)
			{
				//Player�� ���� �������� ���� ��Ȳ �ܿ� PlayScene���� ����Ǵ�
				//���������� ǥ���ϱ� ���� ���� �߰�.
				//�̹� ������ ���������� �����ϰ� �ϱ� ����.
				CPlayer::GetInstance()->SetPlayingStage(idx+1);
				NNSceneDirector::GetInstance()->ChangeScene(CPlayScene::GetInstance());
				break;
			}
		}
	}


}

void CStageSelectScene::InitMapSprite()
{
	//Map Sprite ��ġ
	m_mapSprite = NNSprite::Create(L"wugargar/stageSelect.jpg");
	m_mapSprite->SetPosition(0.0f, 0.0f);
	AddChild( m_mapSprite );

	int current_stage = CPlayer::GetInstance()->GetCurrentStage() / 100;

	//MapStageFlag Sprite��ġ. Ŭ���� �� ���������� ����, �ƴ� ���������� �Ķ��� ������ ǥ�� (�ӽ�)
	for(int idx=0; idx<current_stage; ++idx)
	{
		m_stageFlag[idx] = CUIButton::Create(L"wugargar/stageselect_on.png", L"wugargar/stageselect_on.png");
		AddChild( m_stageFlag[idx] );
	}
	for(int idx=current_stage; idx<STAGE_NUM; ++idx)
	{
		m_stageFlag[idx] = CUIButton::Create(L"wugargar/stageselect_off.png",L"wugargar/stageselect_off.png");
		AddChild( m_stageFlag[idx] );
	}

	m_stageFlag[0]->SetPosition(301,219);
	m_stageFlag[1]->SetPosition(448,183);
	m_stageFlag[2]->SetPosition(465,321);
	m_stageFlag[3]->SetPosition(601,403);

}
