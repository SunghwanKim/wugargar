#pragma once

#define GAME_SCREEN_MIN_SIZE_X	0
#define GAME_SCREEN_MAX_SIZE_X	1024
#define GAME_SCREEN_MIN_SIZE_Y	0
#define GAME_SCREEN_MAX_SIZE_Y	768

// �Ʒ� ���� �� #define������ playScene�� ȭ����� ���� �����̴�.
// x, yŸ�� ��ĭ�� ũ�⸦ 50���� �����ߴ�.
// background(�ϴ�)�� ������ Ÿ���� ���۵Ǵ� ������ First y coordinate���� �����Ѵ�.
// max size x�� Ÿ���� ������ �ǹ��Ѵ�. ���� ����� 1024�̹Ƿ� 24�� ���� 1ĭ���� �� ������ �ش�.
// ���� �������δ� 6 * 50 = 300�� ũ�⸦ ������. 520 pixel~ 730������ UI�� ����Ѵ�.
#define FIRST_Y_COORDINATE_OF_BACKGROUND	0
#define FIRST_Y_COORDINATE_OF_MAPTILE		270
#define MAP_SIZE_X	21
#define MAP_SIZE_Y	6
#define TILE_SIZE_X	50
#define TILE_SIZE_Y	50
#define SIZE_OF_UI_BACKGROUND_X				450
#define SIZE_OF_HUMAN_FARM					GAME_SCREEN_MAX_SIZE_X - SIZE_OF_UI_BACKGROUND_X //574
#define FIRST_X_COORDINATE_OF_UIBUTTON		SIZE_OF_HUMAN_FARM
#define FIRST_Y_COORDINATE_OF_UIBUTTON		FIRST_Y_COORDINATE_OF_MAPTILE + TILE_SIZE_Y * MAP_SIZE_Y // 520
#define SECOND_Y_COORDINATE_OF_UIBUTTON		FIRST_Y_COORDINATE_OF_UIBUTTON + GAP_BETWEEN_UIBUTTONS_Y
#define GAP_BETWEEN_UIBUTTONS	10
#define GAP_BETWEEN_UIBUTTONS_Y 10
#define SIZE_OF_UIBUTTON		100
#define NUM_OF_UIBUTTON_IN_ROW 4
#define NUM_OF_UIBUTTON 8

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
	NUMBER_OF_ZOMBIE_TYPES,
	BABY_HUMAN
}; 
