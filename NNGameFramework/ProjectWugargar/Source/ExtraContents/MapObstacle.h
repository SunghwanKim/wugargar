#pragma once
#include "nnobject.h"
#include "NNSprite.h"
#include "Zombie.h"
#include "Character.h"
class CMapObstacle :
	public NNObject
{
public:
	CMapObstacle(void);
	virtual ~CMapObstacle(void);

public:
	void Render();
	void Update( float dTime );
	NNCREATE_FUNC(CMapObstacle);
	virtual void Boom(CCharacter* boom_target);
	void InitStatus(); // virtual �� �ϸ� ������.
	void InitSprite(std::wstring imagePath);
	NNPoint RandomMapPosition();
	bool CheckClickArea();
	NNSprite* GetSprite(){return m_pObstacle_sprite;};
	bool IsCrash( CCharacter *crash_check_character);


	bool is_boom;

protected:
	NNSprite*	m_pObstacle_sprite;
	int			m_obstacle_damage;
	float		m_boom_range;

};

