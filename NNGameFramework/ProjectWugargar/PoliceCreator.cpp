#include "PoliceCreator.h"
#include "Police.h"
#include "PlayScene.h"
#include "PrintConsole.h"
#include "NNResourceManager.h"
#include "NNXML.h"

CPoliceCreator::CPoliceCreator(void)
{
	begin_time = clock();
	tableTopIndex = 0;
	ReturnTableByFile();
	
	//printf_s("CreatePolice ������\n");
	//printf_s("beginetime : %d\n", begin_time);
}


CPoliceCreator::~CPoliceCreator(void)
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
void CPoliceCreator::ReturnTableByFile()
{
	NNXML *create_police_xml;

	create_police_xml = NNResourceManager::GetInstance()->LoadXMLFromFIle("XML/Stage/StageInfo.txt");
	int current_stage = CPlayer::GetInstance()->GetPlayingStage();
	std::string Xpath = "/StageInfo/Stage" + std::to_string(current_stage);
	num_stage_info = std::stoi(create_police_xml->XPathToString(Xpath + "/StageInfoNum/text()").c_str());
	
	create_enemy_table = new CreateEnemyTable[num_stage_info];
	Xpath.append("/StageInfo");
	
	for (int idx=0; idx<num_stage_info; ++idx) {
		create_enemy_table[idx].time = std::stoi(create_police_xml->XPathToString(Xpath + std::to_string(idx+1) + "/Time/text()").c_str());
		create_enemy_table[idx].enemy_type = (PoliceType)std::stoi(create_police_xml->XPathToString(Xpath + std::to_string(idx+1) + "/PoliceType/text()").c_str());
	}
}


/*
�Լ��� : CreateEnemy
��ȯ�� : void
������ CreateEnemyTable�� ������� Police�� ����
�ֿ� ���� : gap_time(���� �ð��� ���α׷� ���� �ð��� �A ������ ��(sec)�� ȯ���� ��)�� ����
			gap_time�� ������ ���̺��� ���� �ð����� ū ��� ������ ������ŭ ������ Ÿ���� ���� ����
			�����Ŀ� index�� �÷��־� ������ ������ �κ����� �̵��ϰ� ��.
*/
PoliceType CPoliceCreator::ReturnCreateEnemyInfo()
{	
	
	if((tableTopIndex) >= num_stage_info)
		return NONE_POLICE;

	current_time = clock();
	
	gap_time = (int)(current_time - begin_time);

	if((gap_time) >= create_enemy_table[tableTopIndex].time){
		//++tableTopIndex;
		return create_enemy_table[tableTopIndex++].enemy_type;
	}

	return NONE_POLICE;
}

//void CCreatePolice::SetCreateInfoByXML( NNXML *StageXML )
//{
//	int num_info = std::stoi(StageXML->XPathToString("/stage/StageInfoNum/text()").c_str());
//	create_enemy_table = new CreateEnemyTable[num_info];
//	std::string tmp_path = "/stage/StageInfo";
//
//
//	for(int idx=0; idx<num_info; ++idx)
//	{
//		tmp_path.append(std::to_string(idx));
//		std::string tmp_time_path = tmp_path;
//		std::string tmp_type_path = tmp_path;
//		tmp_time_path.append("/Time/text()");
//		tmp_type_path.append("/PoliceType/text()");
//
//		create_enemy_table[idx].time = std::stoi(StageXML->XPathToString(tmp_time_path.c_str()));
//		create_enemy_table[idx].enemy_type = (PoliceType)std::stoi(StageXML->XPathToString(tmp_type_path.c_str()));
//	}
//
//
//
//}	

