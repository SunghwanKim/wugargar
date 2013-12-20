#pragma once
#include "NNObject.h"
#include "NNXML.h"

#include "GameConfig.h"
/*
Ŭ���� �̸� : CCreatePolice
Ŭ���� ���� : ��(����)�� �����Ǵ� ������ ����� �ش� ���ؿ� �°�
			�������� �����ǵ��� ��

���� ������ : 2013/11/03
���� ������ : 2013/11/04
*/


/*
����ü : ���������� ���� ��� ������ ��Ÿ��.
������ �ð�(time)�� ������ ���� Ÿ��(enemy_type)���� ������ ���� ��(num_enemy)�� ����
*/

class CPolice;

struct PoliceTable
{
	int time;
	PoliceType enemy_type;
};


class CPoliceCreator
{
public:
	CPoliceCreator(void);
	virtual ~CPoliceCreator(void);
	
	NNCREATE_FUNC(CPoliceCreator);
	void				LoadScriptFromFile();
	PoliceType			ReturnCreateEnemyInfo();
	CPolice*			CreatePoliceOnTime(float stageElapsedTime);

// 	int GetNumStageInfo() const { return m_NumStageInfo; }
// 	int GetTableTopIndex() const { return tableTopIndex; }
// 	float* GetPoliceScriptCallTime() const { return m_aPoliceScriptCallTime; }
// 	
// 	void SetTableTopIndex(int val) { tableTopIndex = val; }
// 	void SetPoliceScriptCallTime(float time, int index) { m_aPoliceScriptCallTime[index] = time; }
// 	
private:
	PoliceTable *m_aCreateEnemyTable;
	float* m_aPoliceScriptCallTime;
	clock_t begin_time;
	clock_t current_time;
	int gap_time;
	int m_NumStageInfo;
	int tableTopIndex;
};

