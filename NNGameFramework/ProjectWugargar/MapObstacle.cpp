#include "MapObstacle.h"
#include "GameConfig.h"
#include "PlayScene.h"
#include "NNInputSystem.h"
#include "NNPoint.h"

#define BATTALE_LINE 100

/*
MapObstacle�� ȭ�鿡 �����ϰ� �ѷ����� ����, �� �ǹ�
*/

CMapObstacle::CMapObstacle(void)
{
	is_boom = false;
}


CMapObstacle::~CMapObstacle(void)
{
}

void CMapObstacle::Render()
{
	NNObject::Render();
}

void CMapObstacle::Update( float dTime )
{

	//Zombie����Ʈ�� ���鼭 ���� Obstacle�� ��Ҵ����� üũ
	for (const auto& child : CPlayScene::GetInstance()->GetZombieList())
	{
		if(child != CPlayScene::GetInstance()->GetMapCreator()->GetZombieBase())
		if(IsCrash(child))
		{
			printf_s("Boom!\n");
			Boom(child);
			break;
		}


	}
	
}

//���� obstacle�� ������� HP�� ��� ���Ļ��¸� true�� (�� �Լ��� �����ε���)
void CMapObstacle::Boom( CCharacter* boom_target )
{
	boom_target->SetHP(boom_target->GetHP() - m_obstacle_damage);
	is_boom = true;
}

void CMapObstacle::InitSprite(std::wstring imagePath )
{
	m_pObstacle_sprite = NNSprite::Create(imagePath);

	m_pObstacle_sprite->SetPosition(RandomMapPosition());
	AddChild(m_pObstacle_sprite, 0);

}


//�� �� �����ϰ� �ѷ����� �κ�. �����ϸ� ������ ���� �̷����� ����(BattleLine)�� �ѷ������� ������ ����
NNPoint CMapObstacle::RandomMapPosition()
{
	return (NNPoint(static_cast<float>(rand() % (MAP_SIZE_X-1) * TILE_SIZE_X), static_cast<float>(FIRST_Y_COORDINATE_OF_MAPTILE + rand()%(MAP_SIZE_Y-1)*TILE_SIZE_Y)));
}

//Area���ΰ� Ŭ���Ǿ����� üũ. UIBUTTIONŬ���� �Լ� ������
bool CMapObstacle::CheckClickArea()
{
	NNPoint cursorPosition = NNInputSystem::GetInstance()->GetMousePosition();

	bool isInXCoordRange = (m_pObstacle_sprite->GetPositionX() < cursorPosition.GetX()) && ( ( m_pObstacle_sprite->GetPositionX() + m_pObstacle_sprite->GetImageWidth() ) > cursorPosition.GetX() );
	bool isInYCoordRange = (m_pObstacle_sprite->GetPositionY() < cursorPosition.GetY()) && ( ( m_pObstacle_sprite->GetPositionY() + m_pObstacle_sprite->GetImageHeight() ) > cursorPosition.GetY() );

	if ( !(isInXCoordRange && isInYCoordRange))
	{
		return false;
	}
	else
	{
		if (NNInputSystem::GetInstance()->GetKeyState( VK_LBUTTON ) == KEY_UP )
			return true;
	}
	
	return false;

}

//�浹 üũ �Լ�. ����� ���Ƿ� �� ������ �Ÿ��� 10������ ��������� �����ϵ��� �Ǿ�����.
//�� �� ������������ ���� ������ �ʿ��� ��.
bool CMapObstacle::IsCrash( CCharacter *crash_check_character)
{
	float distance_attacktarget;
	distance_attacktarget = m_pObstacle_sprite->GetPosition().GetDistance(crash_check_character->GetPosition());

	if(distance_attacktarget <= 10/*this->GetSprite()->GetImageWidth()*/)
		return true;
	
	return false;

	/*if(((thi->GetCenterX() + thi->GetImageWidth()/2) > (crash_check_sprite->GetCenterX() - crash_check_sprite->GetImageWidth()/2)) &&
		(((thi->GetCenterX() + thi->GetImageWidth()/2) <= (crash_check_sprite->GetCenterX() + crash_check_sprite->GetImageWidth()/2)) &&
		((thi->GetCenterY() <= (crash_check_sprite->GetCenterY() + crash_check_sprite->GetImageHeight()/2)) &&
		((thi->GetCenterX() > (crash_check_sprite->GetCenterY() - crash_check_sprite->GetImageHeight()/2)) ))))
		return true;

	return false;*/
}
