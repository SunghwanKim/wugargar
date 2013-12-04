#pragma once
#include "NNObject.h"
#include "GameConfig.h"
#include "NNXML.h"
/*
Ŭ���� �̸� : CCreatePolice
Ŭ���� ���� : ��(����)�� �����Ǵ� ������ ����� �ش� ���ؿ� �°�
			�������� �����ǵ��� ��

���� ������ : 2013/11/03
���� ������ : 2013/11/04
*/


//��(����)�� Ÿ��. �� Ÿ�Կ� ���缭 �ش��ϴ� ������ ������
//enum enemyType{
//	NOT_TIME,
//	NORMAL_POLICE,
//	GUN_POLICE,
//	SHILD_POLICE,
//	HELLICOPTER
//};

/*
����ü : ���������� ���� ��� ������ ��Ÿ��.
������ �ð�(time)�� ������ ���� Ÿ��(enemy_type)���� ������ ���� ��(num_enemy)�� ����
*/

typedef struct CreateEnemyTable
{
	int time;
	PoliceType enemy_type;
	//int num_enemy;
}CreateEnemyTable;


class CCreatePolice
{
public:
	CCreatePolice(void);
	virtual ~CCreatePolice(void);

public:
	//void Render();
	//void Update( float dTime );

	CreateEnemyTable *create_enemy_table;

	//CPolice *tmp_police;
	clock_t begin_time;
	clock_t current_time;
	int gap_time;
	int table_top_index;
	void				ReturnTableByFile();
	PoliceType				ReturnCreateEnemyInfo();
	void				SetCreateInfoByXML(NNXML *StageXML);


};

