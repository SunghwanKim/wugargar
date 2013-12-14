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

	//static������ Update�� �������� ���鼭�� '���� ��������' ���������� ��ȣ�� �˰� ��.
	//..�� ���� ����� ������?
	static int tmp_current_stage;

	int current_stage =  CPlayer::GetInstance()->GetCurrentStage() / 100;
	if(NNInputSystem::GetInstance()->GetKeyState(VK_LBUTTON))
	{
		for (int idx=0; idx<STAGE_NUM; ++idx)
		{
			//StageFlag(���׶� ��)�� �ԷµǾ��� ���� ó��
			//Stage�� ������ ����ִ� �͵��� ����.
			if(m_stageFlag[idx]->CheckButtonArea())
			{
				m_stageIllustrate->SetPosition(m_stageFlag[idx]->GetPositionX()+50, m_stageFlag[idx]->GetPositionY()-50);
				m_stageIllustrate->SetVisible(true);
				ShowMapDetail(idx+1);
				m_pPlayButton->SetPosition(m_stageIllustrate->GetPositionX(), m_stageIllustrate->GetPositionY()+30);
				m_pExitButton->SetPosition(m_stageIllustrate->GetPositionX(), m_stageIllustrate->GetPositionY()+50);
				m_pPlayButton->SetVisible(true);
				m_pExitButton->SetVisible(true);
				tmp_current_stage = idx+1;
				break;
			}

			
			//���õ� �������� ��ư�� Ŭ���� �� �� �ִ� �������� �� ��� ���õ�
			if(m_pPlayButton->CheckButtonArea() && tmp_current_stage  <= current_stage)
			{
				//Player�� ���� �������� ���� ��Ȳ �ܿ� PlayScene���� ����Ǵ�
				//���������� ǥ���ϱ� ���� ���� �߰�.
				//�̹� ������ ���������� �����ϰ� �ϱ� ����.
				CPlayer::GetInstance()->SetPlayingStage(idx+1);
				NNSceneDirector::GetInstance()->ChangeScene(CPlayScene::GetInstance());
				break;
			}

			//Exit��ư�� ������ �ٽ� �������� ���� ȭ������ ���ư�
			if(m_pExitButton->CheckButtonArea())
			{
				m_pExitButton->SetVisible(false);
				m_pPlayButton->SetVisible(false);
				m_pShowDifficulty->SetVisible(false);
				m_stageIllustrate->SetVisible(false);
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

	//StageIllustrate Sprite(��ǳ��) �̸� ��ġ. �����ִ� ��� ������ �����ϰ� visible�� true��
	m_stageIllustrate = NNSprite::Create(L"wugargar/StageIllustrate.png");
	AddChild(m_stageIllustrate);
	m_stageIllustrate->SetVisible(false);

	//���������� UI��ư �ΰ� Play, exit�� ��ġ
	m_pPlayButton = CUIButton::Create(L"wugargar/playbutton.png", L"wugargar/playbutton.png");
	AddChild(m_pPlayButton);
	m_pPlayButton->SetVisible(false);
	m_pExitButton = CUIButton::Create(L"wugargar/exitbutton.png", L"wugargar/exitbutton.png");
	AddChild(m_pExitButton);
	m_pExitButton->SetVisible(false);
	
	

}

void CStageSelectScene::ShowMapDetail( int stageNum )
{
	//������ Map�� Detail�� ������..
	//XML���� �����͸� �ҷ����� �ٶ���������, ���� ������
	//�����͸� �ҷ����� �κ��� PlayScene�� �����Ƿ� �ӽ÷� �����ϵ��� ��.

	switch (stageNum)
	{
	case 1:
		m_pShowDifficulty = NNLabel::Create(L"�ڡ١١١�", L"���� ���", 20);
		break;
	case 2:
		m_pShowDifficulty = NNLabel::Create(L"�ڡڡ١١�", L"���� ���", 20);
		break;
	case 3:
		m_pShowDifficulty = NNLabel::Create(L"�ڡڡڡ١�", L"���� ���", 20);
		break;
	case 4:
		m_pShowDifficulty = NNLabel::Create(L"�ڡڡڡڡ�", L"���� ���", 20);
		break;
	default:
		break;
	}

	m_pShowDifficulty->SetPosition(m_stageIllustrate->GetPositionX(), m_stageIllustrate->GetPositionY());
	AddChild(m_pShowDifficulty);
}
