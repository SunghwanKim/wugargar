#pragma once
#include "GameConfig.h"
#include <string>
#include "Character.h"
#include "NNXML.h"



struct CharacterInfo{
	float FullHP;	
	float MovingSpeed;
	float AttackRange;
	float SplashRange;
	int AttackPower;
	int DefensivePower;
	int AttackSpeed;
	bool IsSplash;
	bool IsIceAttack;
	std::string SpritePath;
	std::string TypeName;
	CharacterIdentity identity;
	PoliceType policeType;


	//������ 12/16 ����� ���� ���̴� ������
	int WalkAniImageNum;
	std::string *WalkAniImagePath;
	int DeadAniImageNum;
	std::string *DeadAniImagePath;
	int CreationCost;
	ZombieType zombieType;
	clock_t FreezingAttackDuration;

};

class CCharacterConfig
{
public:
	CCharacterConfig(void);
	virtual ~CCharacterConfig(void);

	static CCharacterConfig* m_pInstance;	
	static CCharacterConfig* GetInstance();
	static void ReleaseInstance();
	
	void InitPoliceInfo(NNXML *PoliceInfoXml);
	void InitZombieInfo(NNXML *ZombieInfoXml);

	void initialize_Xpath_Police(std::string *xPath, int idx);
	void initialize_Xpath_Zombie(std::string *xPath, int idx);

	CharacterInfo *GetPoliceInfo(){return policeInfo;};
	CharacterInfo *GetZombieInfo(){return zombieInfo;};



private:
	int num_of_police_type;
	int num_of_zombie_type;

	CharacterInfo *policeInfo;
	CharacterInfo *zombieInfo;


};

