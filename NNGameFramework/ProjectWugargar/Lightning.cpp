#include "Lightning.h"
#include "GameConfig.h"
#include "NNInputSystem.h"
#include "PlayScene.h"
#include "MapCreator.h"
#include "NNSprite.h"
#include "Character.h"
#include "Base.h"
#include "MapObstacle.h"

#define LIGHTNING_SPEED 130
#define FALLING_FIGHTNING_SPEED 230
#define LIGHTNINIG_DAMAGE 100
#define PAY_LIGHTNING 100

CLightning::CLightning(void)
{
	InitSprite(L"wugargar/pika.png");
	m_is_fall_lightning = false;
	m_lightning_damage = LIGHTNINIG_DAMAGE;
	m_pay_lightning = PAY_LIGHTNING;
}


CLightning::~CLightning(void)
{
}

void CLightning::Render()
{
	NNObject::Render();
}

void CLightning::Update( float dTime )
{
	
	//������ ������ ������ �������� �̵�. ���� ������ �̵��ϸ� ������ ������ �ٽ� ����.
	if(m_lightning_sprite->GetPositionX() 
		< GAME_SCREEN_MAX_SIZE_X - 20 && !m_is_fall_lightning)
		m_lightning_sprite->SetPosition
		(m_lightning_sprite->GetPosition() 
		+ NNPoint(LIGHTNING_SPEED, 0.0f) * dTime);
	
	else if(!m_is_fall_lightning)
		m_lightning_sprite->SetPosition(0.0f, POSITION_OF_LIGHTNING);
	
	
	//Space�ٰ� ���ȴ����� üũ. �������� Flag�� True�� �ٲ��ش�.
	if(NNInputSystem::GetInstance()->GetKeyState(VK_SPACE) == KEY_DOWN && !m_fall_lightning_sprite)
	{
		printf_s("put");
		CPlayer::GetInstance()->SetLocalMoney
			(CPlayer::GetInstance()->GetLocalMoney() - m_pay_lightning);
		m_is_fall_lightning = true;
		
	}

	//���� '�������� �ִ� ����'��� ������ ��ġ�� ���������� �Ʒ��� ���������� ����
	if(m_is_fall_lightning)
	{
		m_lightning_sprite->SetPosition
			(m_lightning_sprite->GetPosition() + NNPoint(0.0f, LIGHTNING_SPEED) *dTime);
		//Zombie�� Police�� ���鼭 �浹 üũ. �浹�ϸ� flag�� false�� �ٲٰ�, ������ �ְ� ���� ��ġ�� �ٽ� 0,0���� ����
		for (const auto& child : CPlayScene::GetInstance()->GetZombieList())
		{
			//List�� Base�� ����. Base�� ������ ���� �������� ������ �ȵǹǷ� �����ϵ��� ����
			if(child != CPlayScene::GetInstance()->
				GetMapCreator()->GetZombieBase())
				if(IsCrash(child))
				{
					child->SetHP(child->GetHP() - m_lightning_damage);
					m_is_fall_lightning = false;
						
					m_lightning_sprite->
						SetPosition(0.0f, POSITION_OF_LIGHTNING);
					break;
				}	
		}
		
		for (const auto& child : CPlayScene::GetInstance()->GetPoliceList())
		{
			if(child != CPlayScene::GetInstance()->
				GetMapCreator()->GetPoliceBase())
				if(IsCrash(child))
				{
					child->SetHP(child->GetHP() - m_lightning_damage);
					m_is_fall_lightning = false;

					m_lightning_sprite->
						SetPosition(0.0f, POSITION_OF_LIGHTNING);
					break;
				}
		}


		//������ �ƹ����Ե� ���� �ʰ� �Ʒ��� ������ �������� �Ҹ� ó��.
		if(m_lightning_sprite->GetPositionY() >= FIRST_Y_COORDINATE_OF_UIBUTTON)
		{
			m_is_fall_lightning = false;

			m_lightning_sprite->SetPosition(0.0f, POSITION_OF_LIGHTNING);
		}

	}
}

void CLightning::InitSprite( std::wstring imagePath )
{
	m_lightning_sprite = NNSprite::Create(imagePath);
	m_lightning_sprite->SetPosition(0.0f, POSITION_OF_LIGHTNING);	
	AddChild(m_lightning_sprite, 1);

}

/*
�浹 üũ �Լ�. ����� ���Ƿ� �� ������ �Ÿ��� 10������ ��������� �����ϵ��� �Ǿ�����.
�� �� ������������ ���� ������ �ʿ��� ��.
*/
bool CLightning::IsCrash( CCharacter *crash_check_character)
{

	float distance_attacktarget;
	distance_attacktarget = m_lightning_sprite->
		GetPosition().GetDistance(crash_check_character->GetPosition());

	if(distance_attacktarget <= 50){
		printf_s("LightningAttack!\n");
		return true;
		}

	return false;

}
