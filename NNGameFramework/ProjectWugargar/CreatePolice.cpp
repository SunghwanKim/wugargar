#include "CreatePolice.h"
#include "Police.h"
#include "NormalPolice.h"
#include "PlayScene.h"
#include "PrintConsole.h"

CCreatePolice::CCreatePolice(void)
{
	begin_time = clock();
	table_top_index = 0;
	create_enemy_table = new CreateEnemyTable[8];
	ReturnTableByFile();
	
	//printf_s("CreatePolice ������\n");
	//printf_s("beginetime : %d\n", begin_time);
}


CCreatePolice::~CCreatePolice(void)
{
	SafeArrayDelete<CreateEnemyTable>(create_enemy_table);
}

/*
�Լ��� : ReturnTableByFile
��ȯ�� : ������ ���Ͽ��� CreateEnemyTable�� ���Ŀ� ���� �����͸� �����ϰ� ��ȯ
**���� �ӽ÷� ���� ���� �־���. ���� ���� I/O�� ������ ����
*/
// �����ϴ� �����Ͱ� local variable�� ����Ŵ, local variable�� �Լ��� ������ ������Ƿ� �ᱹ ���� ���� ��ȯ�ϰ� ��.
// ���� �Ϸ�
void CCreatePolice::ReturnTableByFile()
{
	create_enemy_table[0].time = 2;
	create_enemy_table[0].enemy_type = NORMAL_POLICE;
	create_enemy_table[1].time = 3;
	create_enemy_table[1].enemy_type = NORMAL_POLICE;
	create_enemy_table[2].time = 5;
	create_enemy_table[2].enemy_type = NORMAL_POLICE;
	create_enemy_table[3].time = 7;
	create_enemy_table[3].enemy_type = NORMAL_POLICE;
	create_enemy_table[4].time = 10;
	create_enemy_table[4].enemy_type = NORMAL_POLICE;

	create_enemy_table[5].time = 15;
	create_enemy_table[5].enemy_type = NORMAL_POLICE;
	create_enemy_table[6].time = 20;
	create_enemy_table[6].enemy_type = NORMAL_POLICE;
	create_enemy_table[7].time = 25;
	create_enemy_table[7].enemy_type = NORMAL_POLICE;
}


/*
�Լ��� : CreateEnemy
��ȯ�� : void
������ CreateEnemyTable�� ������� Police�� ����
�ֿ� ���� : gap_time(���� �ð��� ���α׷� ���� �ð��� �A ������ ��(sec)�� ȯ���� ��)�� ����
			gap_time�� ������ ���̺��� ���� �ð����� ū ��� ������ ������ŭ ������ Ÿ���� ���� ����
			�����Ŀ� index�� �÷��־� ������ ������ �κ����� �̵��ϰ� ��.
*/
enemyType CCreatePolice::GetCreateEnemyInfo()
{
	
	current_time = clock();
	
	gap_time = (int)(current_time - begin_time)/(CLOCKS_PER_SEC);
	printf_s("CreateEnemy. current : %d, gap : %d\n", current_time, gap_time);

	if((gap_time) >= create_enemy_table[table_top_index].time)
	{
		++table_top_index;
		return create_enemy_table[table_top_index].enemy_type;

	}

	return NOT_TIME;
}

