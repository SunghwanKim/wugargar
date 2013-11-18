#include "Character.h"
#include "PlayScene.h"


CCharacter::CCharacter(void)
{
}



CCharacter::~CCharacter(void)
{
}

void CCharacter::initStatus( void )
{

}

/*
�Լ��� : DetermineAttackTarget
���� : �� ��ü�� attack_target�� �����ϴ� �Լ��� ����ϸ� �ȴ�.
���Ŀ��� GoAttackTarget���� �Լ��� ����ϸ� �� ��
�ֿ� ���� : �̱������� ������ Scene�� �ִ� ��� Police�迭�� ���鼭 NNPoint�� �Լ��� �̿���
�Ÿ��� �����ϰ� ���� ����� �Ÿ��� �����Ͽ� �ش��ϴ� idx�� ���� Ÿ���� ��ȯ�Ѵ�.

���� ������ : 2013/11/03
���� ������ : 2013/11/14

ISSUE : 1�� ����. switch���� �ݺ��Ǵ� For���� �ϳ��� ���� ����� ã�ƾ� ��.
���� ���.

11/14 : Enemy�� �������� ���� ��(NULL) attackTarget�� ����� Base�� �����ϵ��� ����
*/
void  CCharacter::DetermineAttackTarget()
{
	float return_distnace = 1000000.0f;
	float next_distance;
	CZombie *tmp_closer_target_zombie = NULL;
	CPolice *tmp_closer_target_police = NULL;
	CCharacter *return_target = NULL;



	switch(this->GetIdentity())
	{
	case Zombie:
		for(const auto& child : CPlayScene::GetInstance()->GetPoliceList())
		{
			next_distance = this->GetPosition().GetDistance(child->GetPosition());
			 	if(return_distnace > next_distance)
			 	{
			 		return_distnace = next_distance;
			 		m_AttackTarget = child;
			 	}
		}
		if(m_AttackTarget == NULL)
			m_AttackTarget = CPlayScene::GetInstance()->GetMapCreator()->GetPoliceBase();
		break;

	case Police:
		for(const auto& child : CPlayScene::GetInstance()->GetZombieList())
		{
			next_distance= this->GetPosition().GetDistance(child->GetPosition());
			 	if(return_distnace > next_distance)
			 	{
			 		return_distnace = next_distance;
			 		m_AttackTarget = child;
			 	}
		}
		if(m_AttackTarget == NULL)
			m_AttackTarget = CPlayScene::GetInstance()->GetMapCreator()->GetZombieBase();
		break;
	default:
		break;
	}
}

void CCharacter::InitSprite( std::wstring imagePath )
{
	m_Sprite = NNSprite::Create(imagePath);

	// �θ� ����� ��ġ�� ������ �ޱ� ������ 
	// �θ��� ĳ���� ����� ��ġ�� �����ϰ� 
	// �ڽ��� sprite�� (0, 0)���� �ʱ�ȭ�Ѵ�.
	// �̰Ͷ����� �ѽð� ���� ��� �Ф�

	m_Sprite->SetPosition(0.0f, 0.0f);	
	AddChild(m_Sprite, 1);
}



void CCharacter::SetRandomPositionAroundBase()
{
	NNPoint baseLocation;
	
	switch (m_Identity)
	{
	case Zombie:
		baseLocation = CPlayScene::GetInstance()->GetMapCreator()->GetZombieBase()->GetPosition();
		baseLocation.SetX(baseLocation.GetX()+TILE_SIZE_X);
		baseLocation.SetY(baseLocation.GetY()+TILE_SIZE_Y);
		break;
	case Police:
		baseLocation = CPlayScene::GetInstance()->GetMapCreator()->GetPoliceBase()->GetPosition();
		baseLocation.SetX(baseLocation.GetX()-TILE_SIZE_X);
		break;
	default:
		break;
	}
	
	int random_location_x = rand() % TILE_SIZE_X;
	int random_location_y = rand() % TILE_SIZE_Y;

	SetPosition((baseLocation.GetX()+random_location_x),(baseLocation.GetY()+random_location_y));
}


void CCharacter::Render()
{
	NNObject::Render();
}

void CCharacter::Update( float dTime )
{
	//AttackTarget�� �����ϰ� Attack�� �����ϸ�(�����Ÿ� üũ)
	//�����ϰ� �׷��� ������ Attack Target���� ����
	DetermineAttackTarget();

	if(IsAttack())
		Attack();
	else
		GoToAttackTarget(dTime);
}

void CCharacter::Attack()
{
	CCharacter* target = this->m_AttackTarget;

	if(this->m_AttackTarget){
		int damage = this->GetAttackPower() - target->GetDefensivePower();
		target->SetHP(target->GetHP()-damage) ;
	}
}


/*
����ȣ. 11/14
Attack_target���� �Ÿ��� ����Ͽ� ���� �������� �����ϵ��� ��.
Issue : ������ �����ϴ� MakeCharacterWalk�� ������� �������
*/
void CCharacter::GoToAttackTarget(float dTime)
{
	float gap_x = m_AttackTarget->GetPositionX() - m_Position.GetX();
	float gap_y = m_AttackTarget->GetPositionY() - m_Position.GetY();
	float t_x = (gap_x) / (gap_x+gap_y);
	float t_y = (gap_y) / (gap_x+gap_y);
	
	switch (m_Identity)
	{
	case Zombie:
		this->SetPosition(this->m_Position - NNPoint( -(m_MovingSpeed*t_x),-( m_MovingSpeed*t_y) )*dTime);
		break;
	case Police:
		this->SetPosition(this->m_Position - NNPoint( (m_MovingSpeed*t_x),( m_MovingSpeed*t_y) )*dTime);
		break;
	default:
		break;
	}
	
	
}


/*
����ȣ. 11/14
���� ��Ȳ���� AttackTarget�� ���� �����Ÿ� �ȿ� ���ִ��� üũ.
������ �����ϸ� True, �������� ������(�־) False
*/
bool CCharacter::IsAttack()
{
	float distance_attacktarget;

	distance_attacktarget = this->GetPosition().GetDistance(m_AttackTarget->GetPosition());

	if(distance_attacktarget <= m_AttackRange)
		return true;
	else
		return false;

}
