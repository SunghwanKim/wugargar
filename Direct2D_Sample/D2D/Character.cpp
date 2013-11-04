#include "Character.h"
#include "Wugargar.h"

CCharacter::CCharacter(void)
{
}


CCharacter::~CCharacter(void)
{
}

void CCharacter::Render()
{

}

void CCharacter::Update( float dTime )
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
*/
CCharacter * CCharacter::GetCloseEnemy()
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
			 			tmp_closer_target_police = child;
			 		}

				
		}
		return_target = tmp_closer_target_police;
		break;

	case Police:
		for(const auto& child : CWugargar::GetInstance()->GetZombieList())
		{
				next_distance= this->GetPosition().GetDistance(child->GetPosition());
			 		if(return_distnace > next_distance)
			 		{
			 			return_distnace = next_distance;
			 			tmp_closer_target_zombie = child;
			 		}

		}
		return_target = tmp_closer_target_zombie;
		break;
	default:
		break;
	}

	return return_target;

}
