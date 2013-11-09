#include "Character.h"
#include "Wugargar.h"
#include <time.h>
#include <stdlib.h>


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
�Լ��� : GetCloseEnemy
��ȯ�� : �ش� ��ü(����/����)�κ��� ���� ����� ��ü(����/����)�� ��ȯ
���� : �� ��ü�� attack_target�� �����ϴ� �Լ��� ����ϸ� �ȴ�.
���Ŀ��� GoAttackTarget���� �Լ��� ����ϸ� �� ��
�ֿ� ���� : �̱������� ������ Scene�� �ִ� ��� Police�迭�� ���鼭 NNPoint�� �Լ��� �̿���
�Ÿ��� �����ϰ� ���� ����� �Ÿ��� �����Ͽ� �ش��ϴ� idx�� ���� Ÿ���� ��ȯ�Ѵ�.

���� ������ : 2013/11/03
���� ������ : 2013/11/04

ISSUE : 1�� ����. switch���� �ݺ��Ǵ� For���� �ϳ��� ���� ����� ã�ƾ� ��.
���� ���.
*/
void  CCharacter::GetCloseEnemy()
{
	float return_distnace = 1000000.0f;
	float next_distance;
	CZombie *tmp_closer_target_zombie = NULL;
	CPolice *tmp_closer_target_police = NULL;
	CCharacter *return_target = NULL;

	switch(this->GetIdentity())
	{
	case Zombie:
		for(const auto& child : CWugargar::GetInstance()->GetPoliceList())
		{
			next_distance = this->GetPosition().GetDistance(child->GetPosition());
			 	if(return_distnace > next_distance)
			 	{
			 		return_distnace = next_distance;
			 		m_AttackTarget = child;
			 	}
		}
		break;

	case Police:
		for(const auto& child : CWugargar::GetInstance()->GetZombieList())
		{
			next_distance= this->GetPosition().GetDistance(child->GetPosition());
			 	if(return_distnace > next_distance)
			 	{
			 		return_distnace = next_distance;
			 		m_AttackTarget = child;
			 	}
		}
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
		baseLocation = CWugargar::GetInstance()->GetMapCreator()->GetZombieBase()->GetPosition();
		baseLocation.SetX(baseLocation.GetX()+TILE_SIZE_X);
		baseLocation.SetY(baseLocation.GetY()+TILE_SIZE_Y);
		break;
	case Police:
		baseLocation = CWugargar::GetInstance()->GetMapCreator()->GetPoliceBase()->GetPosition();
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

}