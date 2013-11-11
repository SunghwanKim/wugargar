#pragma once
#include "PlayScene.h"

class CPlayer
{

private:
	static CPlayer* m_pInstance;

	CPlayer(void);
	virtual ~CPlayer(void);

public:
	static CPlayer* GetInstance();
	static void ReleaseInstance();

public:

	bool InitPlayer();

	int GetGlobalMoney(){return m_GlobalMoney;}
	int GetLocalMoney(){return m_LocalMoney;}
	int GetClearedStage() {return m_ClearedStage;}
	int GetZombieLevel(ZombieType character) { return m_CharacterLevel[character]; }

	void SetGlobalMoney(int globalMoney) {m_GlobalMoney = globalMoney; }
	void SetLocalMoney(int localMoney) {m_GlobalMoney = localMoney; }
	void SetClearedStage(int ClearedStage) {m_ClearedStage = ClearedStage; }
	void SetZombieLevel(ZombieType character, int level) { m_CharacterLevel[character] = level; }
	
	
private:
	int m_LocalMoney;

	// ���ϴ� save�� �� ������ �����͵�
	int m_CharacterLevel[NUMBER_OF_ZOMBIE_TYPES];
	int m_GlobalMoney;	
	int m_ClearedStage;		//201, 301 ������ ����� �������� ǥ��

};

