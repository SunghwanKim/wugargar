#pragma once
#include "character.h"

//Create�� ��, �ش� Ÿ�Կ� �´� PoliceInfo�� �����ǵ���
struct PoliceInfo{
	float HealthPoint;
	float HPRatioPer100;
	float MovingSpeed;
	float AttackRange;
	float SplashRange;
	int ATtackPower;
	int DefensivePower;
	int AttackSpeed;
	bool IsSplash;
	std::string SpritePath;
};

class CPolice : public CCharacter
{
public:
	CPolice(void);
	virtual ~CPolice(void);

public:
	void Render();
	void Update( float dTime );
	NNCREATE_FUNC(CPolice);

public:	
	bool IsInfected() {return m_infected;};
	void SetInfectedStatus(bool infected) {m_infected = infected;};


	PoliceInfo policeInfo[5];

protected:
	bool m_infected;
};
