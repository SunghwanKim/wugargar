#include "Zombie.h"
#include "Wugargar.h"


CZombie::CZombie(void)
{
}


CZombie::~CZombie(void)
{
}

void CZombie::Render()
{

}

void CZombie::Update( float dTime )
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

CCharacter * CZombie::GetCloseEnemy()
{
	//Old Logic (���� ����)
	/*
	m_pParent->

	auto& iter = boolist.begin();
	
	for ( m_ParentListIterator = m_pParent->GetChildList().begin() ; m_ParentListIterator != m_pParent->GetChildList().end() ; m_ParentListIterator++ ) 
	{
		CPolice* police;
		*m_ParentListIterator = police;
		if(typeid(m_ParentListIterator) == typeid(CPolice*))
		{
			next_distance = m_Position.GetDistanceTwoPoint(zombie_location_point, m_ParentListIterator->GetPosition());
			
		}
	}*/

	float return_distnace = 1000000.0f;
	float next_distance;
	CPolice *tmp_closer_target = NULL;
	NNPoint* tmppoint = nullptr;

	for(const auto& child : CWugargar::GetInstance()->GetPoliceList())
	{
		// �� ������ this.GetPosition()->�� �Ǹ� �� ����. tmppoint�� �ʿ����
	//	next_distance = tmppoint->GetDistanceTwoPoint(this->GetPositionX(), this->GetPositionY(), child->GetPositionX(), child->GetPositionY());
// 		if(return_distnace > next_distance)
// 		{
// 			return_distnace = next_distance;
// 			tmp_closer_target = child;
// 		}

	}

	return tmp_closer_target;
	
}