#include "StageSelectScene.h"
#include "Player.h"
#include "PlayScene.h"
#include "NNInputSystem.h"
#include "NNSceneDirector.h"
#include "UpgradeScene.h"
#include "NNResourceManager.h"
#include "NNAudioSystem.h"


CStageSelectScene::CStageSelectScene(void)
{
	InitMapSprite();

	m_pBackgroundSound = NNResourceManager::GetInstance()->LoadSoundFromFile("sound/StageSelectSceneBackground.mp3", true, true);
	NNAudioSystem::GetInstance()->Play(m_pBackgroundSound);
	m_pPlayButton = nullptr;
	m_pExitButton = nullptr;
}


CStageSelectScene::~CStageSelectScene(void)
{
	NNAudioSystem::GetInstance()->Stop( m_pBackgroundSound );
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


	if(NNInputSystem::GetInstance()->GetKeyState(VK_LBUTTON))
	{
	
		for (int idx=0; idx<STAGE_NUM; ++idx)
		{
			//StageFlag(���׶� ��)�� �ԷµǾ��� ���� ó��
			//Stage�� ������ ����ִ� �͵��� ����.
			if(m_stageFlag[idx]->CheckButtonArea())
			{
				if(IsStageClear[idx])
				{
					m_stageIllust[idx]->SetPosition(GAME_SCREEN_MAX_SIZE_X/2, GAME_SCREEN_MIN_SIZE_Y);
					m_stageIllust[idx]->SetVisible(true);

					//Player�� ���� �������� ���� ��Ȳ �ܿ� PlayScene���� ����Ǵ�
					//���������� ǥ���ϱ� ���� ���� �߰�.
					//�̹� ������ ���������� �����ϰ� �ϱ� ����.
					CPlayer::GetInstance()->SetPlayingStage(idx+1);
				}
				else
				{
					m_lockStageIllust->SetPosition(GAME_SCREEN_MAX_SIZE_X/2, GAME_SCREEN_MIN_SIZE_Y);
					m_lockStageIllust->SetVisible(true);
				}
				SettingButton(idx);
				break;
			}
		}

		//���õ� �������� ��ư�� Ŭ���� �� �� �ִ� �������� �� ��� ���õ�
		if(m_pPlayButton && m_pPlayButton->CheckButtonArea())
		{
			NNSceneDirector::GetInstance()->ChangeScene(CUpgradeScene::Create());
			return ;
		}

		//Exit��ư�� ������ �ٽ� �������� ���� ȭ������ ���ư�
		if(m_pExitButton && m_pExitButton->CheckButtonArea())
		{
			for(int idx=0; idx<STAGE_NUM; ++idx)
				m_stageIllust[idx]->SetVisible(false);

			m_lockStageIllust->SetVisible(false);
			// remove�� ����Ʈ������ ���ְ� ��ü�� �ȳ�����(true�� �Ⱥٿ���) label�� ����־���.
			RemoveChild(m_pExitButton, true);
			RemoveChild(m_pPlayButton, true);
			m_pPlayButton = nullptr;
			m_pExitButton = nullptr;
			
		}

	}
	



}

void CStageSelectScene::InitMapSprite()
{
	//Map Sprite ��ġ
	m_mapSprite = NNSprite::Create(L"wugargar/stageSelect.jpg");
	m_mapSprite->SetPosition(0.0f, 0.0f);
	AddChild( m_mapSprite );

	int current_stage = CPlayer::GetInstance()->GetClearedStage() % 100;

	//MapStageFlag Sprite��ġ. Ŭ���� �� ���������� ����, �ƴ� ���������� �Ķ��� ������ ǥ�� (�ӽ�)
	for(int idx=0; idx<current_stage; ++idx)
	{
		m_stageFlag[idx] = CUIButton::Create(L"wugargar/stageselect_on.png", L"wugargar/stageselect_on.png");
		AddChild( m_stageFlag[idx] );
		IsStageClear[idx] = true;
	}
	for(int idx=current_stage; idx<STAGE_NUM; ++idx)
	{
		m_stageFlag[idx] = CUIButton::Create(L"wugargar/stageselect_off.png",L"wugargar/stageselect_off.png");
		AddChild( m_stageFlag[idx] );
		IsStageClear[idx] = false;
	}

	m_stageFlag[0]->SetPosition(301,219);
	m_stageFlag[1]->SetPosition(448,183);
	m_stageFlag[2]->SetPosition(465,321);
	m_stageFlag[3]->SetPosition(601,403);


	m_stageIllust[0] = NNSprite::Create(L"wugargar/StageSelect/Stage1ExplainImage.png");
	m_stageIllust[1] = NNSprite::Create(L"wugargar/StageSelect/Stage2ExplainImage.png");
	m_stageIllust[2] = NNSprite::Create(L"wugargar/StageSelect/Stage3ExplainImage.png");
	m_stageIllust[3] = NNSprite::Create(L"wugargar/StageSelect/Stage4ExplainImage.png");
	m_lockStageIllust = NNSprite::Create(L"wugargar/StageSelect/LockStageImage.png");
	AddChild(m_lockStageIllust);
	m_lockStageIllust->SetVisible(false);
	
	for(int idx=0; idx<STAGE_NUM; ++idx)
	{
		AddChild(m_stageIllust[idx]);
		m_stageIllust[idx]->SetVisible(false);
	}
	

	//StageIllustrate Sprite(��ǳ��) �̸� ��ġ. �����ִ� ��� ������ �����ϰ� visible�� true��
	//m_stageIllustrate = NNSprite::Create(L"wugargar/StageIllustrate.png");
	//AddChild(m_stageIllustrate);
	//m_stageIllustrate->SetVisible(false);

	
	
	

}

//Button�� �ٸ� Sprite ������ �и��� ����~
//Sprite�� Set Visible ������ ���൵ ������ ������
//Button�� ���� �׷��� ó���� ���, ������ ���� ��, �� ��ġ��
//Ŭ���� ��� �÷��̰� ��������.
//���� ���� Set Button�� ������ �׶��׶� Remove���ֱ� ����.
void CStageSelectScene::SettingButton(int stage_idx)
{
	if(IsStageClear[stage_idx]){
		m_pPlayButton = CUIButton::Create(L"wugargar/playbutton.png", L"wugargar/playbutton.png");
		AddChild(m_pPlayButton);
		m_pPlayButton->SetPosition(GAME_SCREEN_MAX_SIZE_X/2, GAME_SCREEN_MIN_SIZE_Y);
	}
	
	m_pExitButton = CUIButton::Create(L"wugargar/exitbutton.png", L"wugargar/exitbutton.png");
	AddChild(m_pExitButton);
	m_pExitButton->SetPosition(GAME_SCREEN_MAX_SIZE_X/2, GAME_SCREEN_MIN_SIZE_Y+20);


}
