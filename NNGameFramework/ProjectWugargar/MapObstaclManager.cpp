#include "MapObstaclManager.h"
#include "Mine.h"
#include "Trap.h"



/*
MapObstacle���� �����ϰ� �����ϴ� Ŭ����
PlayScene�� �� Ŭ������ �ϳ� ��� ������ �����ϸ�
�� Ŭ������ MapObstacle���� LinkedList�� �����ϰ� �����Ѵ�.
REGEN_TIME�� ���� ������ MapObstacle�� ����,
������ MapObstacle�� Ŭ���� �������� �κ��� ����Ѵ�.
*/

MapObstaclManager::MapObstaclManager(void)
{
	m_obstacle_start_time = clock(); //�ð������� ���� ���� ����
}


MapObstaclManager::~MapObstaclManager(void)
{
}



void MapObstaclManager::Update( float dTime )
{
	
	//REGEN_TIME���� Obstacle ����
	if(clock()/CLOCKS_PER_SEC - m_obstacle_start_time/CLOCKS_PER_SEC  > REGEN_TIME)
	{
		CMapObstacle *tmpMapObstacle = nullptr;

		//�����ϰ� ���� �Ǵ� ���� �����Ѵ�.
		if(rand()%2)
			tmpMapObstacle = CMine::Create();
		else
			tmpMapObstacle = CTrap::Create();

		//������ obstacle�� ����Ʈ�� �־�����.
		printf_s("����! %d %d\n", tmpMapObstacle->GetSprite()->GetPositionX(), tmpMapObstacle->GetPositionY());
		m_pList_mapObstacle.push_back(tmpMapObstacle);
		AddChild(tmpMapObstacle,10);
		m_obstacle_start_time = clock();
	}
	
	//Obstacle list�� �Ź� ���鼭 Click���θ� Ȯ���Ѵ�.
	for(auto& iter = m_pList_mapObstacle.begin() ; iter != m_pList_mapObstacle.end() ; iter++ )
	{
		(*iter)->Update(dTime);
		//Obstacle�� Update�Լ��� ������ �������� �ʾ� ���Ƿ� ���� �ڵ�

		//obstacle�� Ŭ���Ǿ��ų� Zombie�� �����Ͽ� ���ĵǾ��� �� �������ش�.
		if((*iter)->CheckClickArea() || (*iter)->is_boom)
		{
			CMapObstacle *tmp_obstacle;
			tmp_obstacle = *iter;
			if(!(*iter)->is_boom)
				printf_s("Obstacle Click Check\n");

			m_pList_mapObstacle.erase(iter);
			RemoveChild(tmp_obstacle, true);
			break; //������ ������ �κ�
		}


	}
}
