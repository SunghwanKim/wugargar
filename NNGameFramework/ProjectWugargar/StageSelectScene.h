#pragma once
#include "nnscene.h"


class CUIButton;
class NNSound;
class NNSprite;
class NNLabel;


#define  STAGE_NUM 4

//Stage�� ������ �� �ְ� ���ִ� Scene
class CStageSelectScene :
	public NNScene
{
public:
	CStageSelectScene(void);
	~CStageSelectScene(void);

	virtual void Render();
	virtual void Update( float dTime );
	NNCREATE_FUNC(CStageSelectScene);


private:
	void InitMenuButtons( void );
	void InitMapSprite();

	void ShowMapDetail(int stageNum);
	void SettingButton(int stage_idx);

	
private:
	NNSprite		*m_mapSprite;
	CUIButton		*m_stageFlag[STAGE_NUM];
	bool			IsStageClear[STAGE_NUM];
	NNSprite		*m_stageIllust[STAGE_NUM];
	NNSprite		*m_lockStageIllust;

	NNSprite *m_difficultyStar;
	NNSprite *m_stageIllustrate;
	

	NNLabel *m_pShowDifficulty;
	NNLabel *m_pShowCleared;
	NNLabel *m_pShowUnacceptable;

	NNSound *m_pBackgroundSound;

	CUIButton *m_pPlayButton;
	CUIButton *m_pExitButton;

	bool m_onImage;

};

