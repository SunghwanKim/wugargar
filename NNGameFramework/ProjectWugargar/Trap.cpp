#include "Trap.h"


CTrap::CTrap(void)
{
	InitSprite(L"wugargar/trap.png");
	InitStatus();
}


CTrap::~CTrap(void)
{
}

void CTrap::Render()
{
	NNObject::Render();
}

void CTrap::Update( float dTime )
{
	CMapObstacle::Update(dTime);
}

void CTrap::Boom( CZombie* boom_target )
{
	CMapObstacle::Boom(boom_target);
	boom_target->SetAttackSpeed(boom_target->GetAttackSpeed()-1);
	boom_target->SetSpeed(boom_target->GetMovingSpeed()/2);
	//Trap�� �ɸ� ����� ���� �ӵ��� �̵� �ӵ��� ��������.
}

void CTrap::InitStatus()
{
	m_boom_range = 0;
	m_obstacle_damage = 50;

}