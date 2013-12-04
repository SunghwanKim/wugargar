#pragma once
#include "GameConfig.h"
#include <string>
#include "Character.h"
#include "NNXML.h"



struct PoliceInfo{
	float HealthPoint;
	float HPRatioPer100;
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
};

struct ZombieInfo
{
	float HealthPoint;
	float HPRatioPer100;
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
};

class CCharacterConfig
{
public:
	CCharacterConfig(void);
	virtual ~CCharacterConfig(void);

	static CCharacterConfig* m_pInstance;

	PoliceInfo policeInfo[NUMBER_OF_POLICE_TYPES];
	ZombieInfo zombieInfo[NUMBER_OF_ZOMBIE_TYPES];

	void DeterminePoliceInfo(NNXML *PoliceInfoXml);
	static void ReleaseInstance();
	static CCharacterConfig* GetInstance();
	void initialize_Xpath(std::string *xPath, int idx);
	PoliceInfo *GetPoliceInfo(){return policeInfo;};
	ZombieInfo *GetZombieInfo(){return zombieInfo;};



private:
	int num_of_police_type;
	int num_of_zombie_type;

};

