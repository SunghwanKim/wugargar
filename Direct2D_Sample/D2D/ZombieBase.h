#pragma once

#include "nnobject.h"
#include "NNSprite.h"

class CZombieBase :	public NNObject
{
public:
	CZombieBase(void);
	virtual ~CZombieBase(void);

public :
	NNSprite		*m_ZombieBase;

	void Render();
	void Update( float dTime );

	NNCREATE_FUNC(CZombieBase);

private:
	int m_baseHP;
	int m_baseDefensivePower;

public:
	int GetBaseHP() {return m_baseHP;};
	int GetBaseDefensivePower(){return m_baseDefensivePower;};
};

