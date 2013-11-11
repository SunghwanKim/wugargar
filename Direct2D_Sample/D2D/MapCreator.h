#pragma once
#include "nnobject.h"
#include "NNSprite.h"
#include "NNMacroSet.h"
#include "Base.h"

// x, yŸ�� ��ĭ�� ũ�⸦ 50���� �����ߴ�.
// background(�ϴ�)�� ������ Ÿ���� ���۵Ǵ� ������ First y coordinate���� �����Ѵ�.
// max size x�� Ÿ���� ������ �ǹ��Ѵ�. ���� ����� 1024�̹Ƿ� 24�� ���� 1ĭ���� �� ������ �ش�.
// ���� �������δ� 6 * 50 = 300�� ũ�⸦ ������. 520 pixel~ 730������ UI�� ����Ѵ�.

#define FIRST_Y_COORDINATE_OF_BACKGROUND	0
#define FIRST_Y_COORDINATE_OF_MAPTILE		220
#define MAP_SIZE_X	21
#define MAP_SIZE_Y	6
#define TILE_SIZE_X	50
#define TILE_SIZE_Y	50
#define FIRST_Y_COORDINATE_OF_UIBUTTON		FIRST_Y_COORDINATE_OF_MAPTILE + TILE_SIZE_Y * MAP_SIZE_Y // 520
#define GAP_BETWEEN_UIBUTTONS	15
#define SIZE_OF_UIBUTTONS		150

enum MapType{
	TILE = 0x1,
	ZOMBIE_BASE = 0x2,
	POLICE_STATION = 0x4,	
};

inline MapType operator | (MapType a, MapType b)
{return static_cast<MapType>(static_cast<int>(a) | static_cast<int>(b));}

class CMapCreator :	public NNObject
{
public:
	CMapCreator(void);
	virtual ~CMapCreator(void);

public:
	void Render();
	void Update( float dTime );	
	void SetMap( void );

	CBase* GetZombieBase(void) { return m_pZombieBase; }
	CBase* GetPoliceBase(void) { return m_pPoliceBase; }

	NNCREATE_FUNC(CMapCreator);

private: 
	void initMap(bool fromFile);	
	void SetTileSprite(int x_coord, int y_coord, std::wstring* imagePath);
	void GetImagePathFromMapData(int x_coord, int y_coord, std::wstring* imagePath);
	void BuildBase(int x_coord, int y_coord, CharacterIdentity identity);
	
private:
	CBase*		m_pZombieBase;
	CBase*		m_pPoliceBase;
	MapType		m_Map[MAP_SIZE_X][MAP_SIZE_Y];
	NNSprite*	m_pSprites[MAP_SIZE_X][MAP_SIZE_Y];
};

