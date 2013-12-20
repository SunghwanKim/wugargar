
#include "NNResourceManager.h"

#include "PoliceCreator.h"
#include "PlayScene.h"
#include "PrintConsole.h"
#include "Police.h"
#include "Player.h"

CPoliceCreator::CPoliceCreator(void)
{
	begin_time = clock();
	tableTopIndex = 0;
	LoadScriptFromFile();
	
	//printf_s("CreatePolice ������\n");
	//printf_s("beginetime : %d\n", begin_time);
}


CPoliceCreator::~CPoliceCreator(void)
{
	SafeArrayDelete<PoliceTable>(m_aCreateEnemyTable);
	SafeArrayDelete<float>(m_aPoliceScriptCallTime);
}

/*
�Լ��� : ReturnTableByFile
��ȯ�� : ������ ���Ͽ��� CreateEnemyTable�� ���Ŀ� ���� �����͸� �����ϰ� ��ȯ
**���� �ӽ÷� ���� ���� �־���. ���� ���� I/O�� ������ ����
*/
void CPoliceCreator::LoadScriptFromFile()
{
	NNXML *create_police_xml;

	//create_police_xml = NNResourceManager::GetInstance()->LoadXMLFromFIle("XML/Stage/PoliceCreatorScript.txt");
	create_police_xml = NNResourceManager::GetInstance()->LoadXMLFromFIle("XML/Stage/StageInfo.txt");
	int current_stage = CPlayer::GetInstance()->GetPlayingStage();
	std::string Xpath = "/StageInfo/Stage" + std::to_string(current_stage);
	m_NumStageInfo = std::stoi(create_police_xml->XPathToString(Xpath + "/StageInfoNum/text()").c_str());
	
	m_aCreateEnemyTable = new PoliceTable[m_NumStageInfo];
	m_aPoliceScriptCallTime = new float[m_NumStageInfo];
	ZeroMemory(m_aPoliceScriptCallTime,m_NumStageInfo);
	Xpath.append("/StageInfo");
	
	for (int idx=0; idx<m_NumStageInfo; ++idx) {
		m_aCreateEnemyTable[idx].time = std::stoi(create_police_xml->XPathToString(Xpath + std::to_string(idx+1) + "/Time/text()").c_str());
		m_aCreateEnemyTable[idx].enemy_type = (PoliceType)std::stoi(create_police_xml->XPathToString(Xpath + std::to_string(idx+1) + "/PoliceType/text()").c_str());
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
	
	if((tableTopIndex) >= m_NumStageInfo)
		return NONE_POLICE;

	current_time = clock();
	
	gap_time = (int)(current_time - begin_time);

	if((gap_time) >= m_aCreateEnemyTable[tableTopIndex].time){
		//++tableTopIndex;
		return m_aCreateEnemyTable[tableTopIndex++].enemy_type;
	}

	return NONE_POLICE;
}

// ���� ������ �������� �ִٸ� �������� ����, �ƴ϶�� null ����
CPolice* CPoliceCreator::CreatePoliceOnTime( float stageElapsedTime )
{
	CPolice* returnPolice = nullptr;
	for ( int i=0 ; i<m_NumStageInfo ; ++i ) {
		if (stageElapsedTime - m_aPoliceScriptCallTime[i] > m_aCreateEnemyTable[i].time ) {
			// �ش� script phase�� police���� �ð��� ���
			m_aPoliceScriptCallTime[i] = stageElapsedTime;

			// ���ο� police ��ü�� �����ϰ� �� �����͸� ��ȯ
			returnPolice = CPolice::Create();
			returnPolice->initStatus(
				CCharacterConfig::GetInstance()->GetCharacterInfo(POLICE),
				m_aCreateEnemyTable[i].enemy_type);
			return returnPolice;
		}
	}
	// ���ٸ� null�� ��ȯ�ϰ� �ɰ���
	return returnPolice;
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

