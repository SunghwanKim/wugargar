#include "Police.h"
#include "Zombie.h"
#include "Wugargar.h"


CPolice::CPolice(void)
{
}


CPolice::~CPolice(void)
{
}

void CPolice::Render()
{

}

void CPolice::Update( float dTime )
{

}



/*
�Լ��� : GetCloseEnemy
��ȯ�� : �ش� ��ü(����)�κ��� ���� ����� ��ü(����)�� ��ȯ
���� : �� ��ü�� attack_target�� �����ϴ� �Լ��� ����ϸ� �ȴ�.
���Ŀ��� GoAttackTarget���� �Լ��� ����ϸ� �� ��
�ֿ� ���� : �̱������� ������ Scene�� �ִ� ��� Police�迭�� ���鼭 NNPoint�� �Լ��� �̿���
�Ÿ��� �����ϰ� ���� ����� �Ÿ��� �����Ͽ� �ش��ϴ� idx�� ���� Ÿ���� ��ȯ�Ѵ�.

���� ������ : 2013/11/03
���� ������ : 2013/11/03
*/
CCharacter * CPolice::GetCloseEnemy()
{


	float return_distnace = 1000000.0f;
	float next_distance;
	CZombie *tmp_closer_target = NULL;
	NNPoint* tmppoint = nullptr;

	for(const auto& child : CWugargar::GetInstance()->GetZombieList())
	{
	//	next_distance = tmppoint->GetDistanceTwoPoint(this->GetPositionX(), this->GetPositionY(), child->GetPositionX(), child->GetPositionY());
// 		if(return_distnace > next_distance)
// 		{
// 			return_distnace = next_distance;
// 			tmp_closer_target = child;
// 		}

	}

	return tmp_closer_target;

}