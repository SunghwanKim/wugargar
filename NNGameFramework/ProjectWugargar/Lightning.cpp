#include "Lightning.h"

#include "NNInputSystem.h"
#include "NNSound.h"
#include "NNSprite.h"
#include "NNLabel.h"
#include "NNSprite.h"

#include "GameConfig.h"

#include "PlayScene.h"
#include "MapCreator.h"
#include "Character.h"
#include "Base.h"
#include "MapObstacle.h"
#include "Player.h"


#define LIGHTNING_SPEED 130
#define FALLING_FIGHTNING_SPEED 230
#define LIGHTNINIG_DAMAGE 100
#define PAY_LIGHTNING 100
#define LIGHTNING_COOL_TIME 5

CLightning::CLightning(void)
{
	InitSprite(L"wugargar/pika.png");
	m_is_fall_lightning = false;
	m_lightning_damage = LIGHTNINIG_DAMAGE;
	m_pay_lightning = PAY_LIGHTNING;
	m_startTime = clock() / CLOCKS_PER_SEC;
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
	if(m_lightning_sprite->GetVisible());

	else if(m_cloudSprite->GetPositionX() 
		< GAME_SCREEN_MAX_SIZE_X - 20 && !m_is_fall_lightning)
		m_cloudSprite->SetPosition
		(m_cloudSprite->GetPosition() 
		+ NNPoint(LIGHTNING_SPEED, 0.0f) * dTime);
	
	else if(!m_is_fall_lightning)
		m_cloudSprite->SetPosition(0.0f, POSITION_OF_LIGHTNING);
	
	


	m_currentTime = clock();
	
	//Space�ٰ� ���ȴ����� üũ. �������� Flag�� True�� �ٲ��ش�.
	if(NNInputSystem::GetInstance()->GetKeyState(VK_SPACE) == KEY_DOWN && !m_fall_lightning_sprite && (m_currentTime-m_startTime > LIGHTNING_COOL_TIME))
	{
		printf_s("put");
		m_is_fall_lightning = true;
		m_startTime = m_currentTime;
		
	}

	//���� '�������� �ִ� ����'��� ������ ��ġ�� ���������� �Ʒ��� ���������� ����
	if(m_is_fall_lightning)
	{
		//m_lightning_sprite->SetPosition
		//	(m_lightning_sprite->GetPosition() + NNPoint(0.0f, LIGHTNING_SPEED) *dTime);
		//Zombie�� Police�� ���鼭 �浹 üũ. �浹�ϸ� flag�� false�� �ٲٰ�, ������ �ְ� ���� ��ġ�� �ٽ� 0,0���� ����
		m_lightning_sprite->SetPosition(m_cloudSprite->GetPosition());
		m_lightning_sprite->SetVisible(true);
		

		//Police�� �µ��� ����
		//for (const auto& child : *CPlayScene::GetInstance()->GetZombieList())
		//{
		//	//List�� Base�� ����. Base�� ������ ���� �������� ������ �ȵǹǷ� �����ϵ��� ����
		//	if(child != CPlayScene::GetInstance()->
		//		GetMapCreator()->GetZombieBase())
		//		if(IsCrash(child))
		//		{
		//			child->SetHP(child->GetHP() - m_lightning_damage);
		//			m_is_fall_lightning = false;
		//				
		//			m_lightning_sprite->
		//				SetPosition(0.0f, POSITION_OF_LIGHTNING);
		//			break;
		//		}	
		//}
		
		for (const auto& child : *CPlayScene::GetInstance()->GetPoliceList())
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
		m_is_fall_lightning = false;

		
		////������ �ƹ����Ե� ���� �ʰ� �Ʒ��� ������ �������� �Ҹ� ó��.
		//if(m_lightning_sprite->GetPositionY() >= FIRST_Y_COORDINATE_OF_UIBUTTON)
		//{
		//	m_is_fall_lightning = false;

		//	m_lightning_sprite->SetPosition(0.0f, POSITION_OF_LIGHTNING);
		//}

	}

	//������ ���� 2�ʰ� ����
	if(m_lightning_sprite->GetVisible() && m_currentTime - m_startTime > 2000){
		m_lightning_sprite->SetVisible(false);
	}
}

void CLightning::InitSprite( std::wstring imagePath )
{
	m_lightning_sprite = NNSprite::Create(L"wugargar/Lightning.png");
	m_lightning_sprite->SetVisible(false);
	m_lightning_sprite->SetPosition(0.0f, POSITION_OF_LIGHTNING);	
	AddChild(m_lightning_sprite, 1);

	m_cloudSprite = NNSprite::Create(L"wugargar/Cloud.png");
	m_cloudSprite->SetPosition(0.0f, POSITION_OF_LIGHTNING);
	AddChild(m_cloudSprite, 1);


}

/*
�浹 üũ �Լ�. ����� ���Ƿ� �� ������ �Ÿ��� 10������ ��������� �����ϵ��� �Ǿ�����.
�� �� ������������ ���� ������ �ʿ��� ��.
*/
bool CLightning::IsCrash( CCharacter *crash_check_character)
{

	float distane = m_cloudSprite->GetPositionX() - crash_check_character->GetPositionX();

	if(distane <=50 && distane >= -50)
		return true;

	return false;
/*
	float distance_attacktarget;
	distance_attacktarget = m_lightning_sprite->
		GetPosition().GetDistance(crash_check_character->GetPosition());

	if(distance_attacktarget <= 50){
		printf_s("LightningAttack!\n");
		return true;
		}

	return false;
*/
}
