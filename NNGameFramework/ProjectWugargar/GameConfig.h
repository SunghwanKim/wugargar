#pragma once

#define GAME_SCREEN_MIN_SIZE_X	0
#define GAME_SCREEN_MAX_SIZE_X	1024
#define GAME_SCREEN_MIN_SIZE_Y	0
#define GAME_SCREEN_MAX_SIZE_Y	640

// �Ʒ� ���� �� #define������ playScene�� ȭ����� ���� �����̴�.
// x, yŸ�� ��ĭ�� ũ�⸦ 50���� �����ߴ�.
// background(�ϴ�)�� ������ Ÿ���� ���۵Ǵ� ������ First y coordinate���� �����Ѵ�.
// max size x�� Ÿ���� ������ �ǹ��Ѵ�. ���� ����� 1024�̹Ƿ� 24�� ���� 1ĭ���� �� ������ �ش�.
// ���� �������δ� 6 * 50 = 300�� ũ�⸦ ������. 520 pixel~ 730������ UI�� ����Ѵ�.
#define FIRST_Y_COORDINATE_OF_BACKGROUND	0
#define FIRST_Y_COORDINATE_OF_MAPTILE		220
#define MAP_SIZE_X	21
#define MAP_SIZE_Y	5
#define TILE_SIZE_X	50
#define TILE_SIZE_Y	50
#define SIZE_OF_UI_BACKGROUND_X				450
#define SIZE_OF_HUMAN_FARM_X				GAME_SCREEN_MAX_SIZE_X - SIZE_OF_UI_BACKGROUND_X //574
#define SIZE_OF_HUMAN_FARM_Y				230 //UIBUTTON_Y
#define FIRST_X_COORDINATE_OF_UIBUTTON		SIZE_OF_HUMAN_FARM_X
#define FIRST_Y_COORDINATE_OF_UIBUTTON		449 // 520
#define SECOND_Y_COORDINATE_OF_UIBUTTON		FIRST_Y_COORDINATE_OF_UIBUTTON + GAP_BETWEEN_UIBUTTONS_Y
#define GAP_BETWEEN_UIBUTTONS	10
#define GAP_BETWEEN_UIBUTTONS_Y 10
#define SIZE_OF_UIBUTTON		100
#define NUM_OF_UIBUTTON_IN_ROW 4
#define NUM_OF_UIBUTTON 8
#define POSITION_OF_LIGHTNING				FIRST_Y_COORDINATE_OF_BACKGROUND + 10
#define DEAD_POLICE_IMAGE_WIDTH				58
#define DEAD_POLICE_IMAGE_HEIGHT			17

// ���� ������ Ÿ���� �˾ƺ��� ���� enum�� - ä��
// array���� ����ϹǷ� ������ enum�� ����� �Ҵ����� �� ��. - ��ȯ
enum ZombieType // ��ư �����Ҷ��� �� enum���� ����ϱ� ������ ��ư init�� ���� �������� ��¿������ ���� �ƴ� BABY�� �߰���. 
	//��� image_path �迭 ������ ���� NUM_OF_UIBUTTON �� define ���� - ä��
{
	POOR_ZOMBIE,
	VOMIT_ZOMBIE,
	MUSCLE_ZOMBIE,
	KAMIKAJE_ZOMBIE,
	SMOG_ZOMBIE,
	ICE_ZOMBIE,
	HERO_ZOMBIE_SM9,
	BABY_HUMAN,
	NUMBER_OF_ZOMBIE_TYPES,
	NONE_ZOMBIE
}; 

enum PoliceType
{
	
	NORMAL_POLICE,
	MACHINEGUN_POLICE,
	SHIELD_POLICE,
	GRENADE_POLICE,
	SHOTGUN_POLICE,
	FIREBAT_POLICE,
	NUMBER_OF_POLICE_TYPES,
	NONE_POLICE //CreatePolice�� ���� ���, ���ִ� �� �����
};

//
//#define POLICE_TYPE_NUM 6
//#define ZOMBIE_TYPE_NUM 8
