#include "CreatePolice.h"
#include "Police.h"
#include "NormalPolice.h"
#include "Wugargar.h"


CCreatePolice::CCreatePolice(void)
{
	begin_time = clock();
	table_top_index = 0;
	create_enemy_table = ReturnTableByFile();
}


CCreatePolice::~CCreatePolice(void)
{

}


void CCreatePolice::Render()
{

}

void CCreatePolice::Update( float dTime )
{
	current_time = clock();
	
	gap_time = (int)(current_time - begin_time)/(CLOCKS_PER_SEC);

	CreateEnemy();
}


/*
�Լ��� : ReturnTableByFile
��ȯ�� : ������ ���Ͽ��� CreateEnemyTable�� ���Ŀ� ���� �����͸� �����ϰ� ��ȯ
**���� �ӽ÷� ���� ���� �־���. ���� ���� I/O�� ������ ����
*/
// �����ϴ� �����Ͱ� local variable�� ����Ŵ, local variable�� �Լ��� ������ ������Ƿ� �ᱹ ���� ���� ��ȯ�ϰ� ��.
CreateEnemyTable* CCreatePolice::ReturnTableByFile()
{
	CreateEnemyTable tmp_enemy_table[5];
	tmp_enemy_table[0].time = 2;
	tmp_enemy_table[0].enemy_type = NORMAL_POLICE;
	tmp_enemy_table[0].num_enemy = 1;
	tmp_enemy_table[1].time = 3;
	tmp_enemy_table[1].enemy_type = NORMAL_POLICE;
	tmp_enemy_table[1].num_enemy = 2;
	tmp_enemy_table[2].time = 5;
	tmp_enemy_table[2].enemy_type = NORMAL_POLICE;
	tmp_enemy_table[2].num_enemy = 3;
	tmp_enemy_table[3].time = 7;
	tmp_enemy_table[3].enemy_type = NORMAL_POLICE;
	tmp_enemy_table[3].num_enemy = 2;
	tmp_enemy_table[4].time = 10;
	tmp_enemy_table[4].enemy_type = NORMAL_POLICE;
	tmp_enemy_table[4].num_enemy = 1;


	return tmp_enemy_table;
}


/*
�Լ��� : CreateEnemy
��ȯ�� : void
������ CreateEnemyTable�� ������� Police�� ����
�ֿ� ���� : gap_time(���� �ð��� ���α׷� ���� �ð��� �A ������ ��(sec)�� ȯ���� ��)�� ����
			gap_time�� ������ ���̺��� ���� �ð����� ū ��� ������ ������ŭ ������ Ÿ���� ���� ����
			�����Ŀ� index�� �÷��־� ������ ������ �κ����� �̵��ϰ� ��.
*/
void CCreatePolice::CreateEnemy()
{
	CPolice *tmp_police;
	if((gap_time) >= create_enemy_table[table_top_index].time)
	{
		for(int idx=0; idx<create_enemy_table[table_top_index].num_enemy; ++idx)
		{
			switch (create_enemy_table[table_top_index].enemy_type)
			{
			case NORMAL_POLICE:
				tmp_police = CNormalPolice::Create();
				AddChild(tmp_police);
				CWugargar::GetInstance()->GetPoliceList().push_back(tmp_police);
				break;
			case GUN_POLICE:
				break;
			case SHILD_POLICE:
				break;
			case HELLICOPTER:
				break;
			default:
				break;
			}
			
		}

		++table_top_index;
	}
}

