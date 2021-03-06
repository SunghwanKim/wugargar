#include "HumanInFarm.h"
#include "GameConfig.h"
#include "nnAnimation.h"
#include "NNMacroSet.h"
#include "NNSpriteAtlas.h"
#include "NNInputSystem.h"
#include "PlayScene.h"
#include "Player.h"


CHumanInFarm::CHumanInFarm(void)
{
	CreateBaby();
	DeadImagePath[0] = L"wugargar/deadhuman/0.png";
	DeadImagePath[1] = L"wugargar/deadhuman/1.png";
	DeadImagePath[2] = L"wugargar/deadhuman/2.png";
	DeadImagePath[3] = L"wugargar/deadhuman/3.png";
	DeadImagePath[4] = L"wugargar/deadhuman/4.png";
	DeadImagePath[5] = L"wugargar/deadhuman/5.png";
}


CHumanInFarm::~CHumanInFarm(void)
{
}

void CHumanInFarm::Render()
{
	NNObject::Render();
}

void CHumanInFarm::Update( float dTime )
{
	NNObject::Update(dTime);
	life_time = clock()/CLOCKS_PER_SEC - birth_time;
	Grow();
	if( m_AgeState == GROWN_UP || m_AgeState == DEAD ){
		CollectMeatPointFromGrownUp();
	}
}

void CHumanInFarm::CreateBaby()
{
	m_pSmallBaby = NNAnimation::Create(11,L"wugargar/smallbaby/0.png",
		L"wugargar/smallbaby/1.png",
		L"wugargar/smallbaby/2.png",
		L"wugargar/smallbaby/3.png",
		L"wugargar/smallbaby/4.png",
		L"wugargar/smallbaby/5.png",
		L"wugargar/smallbaby/6.png",
		L"wugargar/smallbaby/7.png",
		L"wugargar/smallbaby/8.png",
		L"wugargar/smallbaby/9.png",
		L"wugargar/smallbaby/10.png");
	SetRandomPositionInFarm();
	AddChild(m_pSmallBaby,100);
	life_time = 0;
	birth_time = clock()/CLOCKS_PER_SEC;
	m_AgeState = SMALL_BABY;
}

void CHumanInFarm::SetRandomPositionInFarm()
{
	int random_location_x = rand() % (SIZE_OF_HUMAN_FARM_X - 100);
	int random_location_y = rand() % (SIZE_OF_HUMAN_FARM_Y - 50);

	m_pSmallBaby->SetPosition( NNPoint(static_cast<float> (random_location_x + 20.0f), static_cast<float>(FIRST_Y_COORDINATE_OF_UIBUTTON + random_location_y)) );
}

void CHumanInFarm::Grow()
{
	if(life_time >= 10 && life_time < 20 && m_AgeState == SMALL_BABY)//10�ʿ� �ѹ� �ڶ�.
	{
		m_AgeState = MIDDLE_BABY;
		NNPoint NowPosition = m_pSmallBaby->GetPosition();
		RemoveChild(m_pSmallBaby,1);
		m_pMiddleBaby = NNAnimation::Create(4,L"wugargar/middlebaby/0.png",
			L"wugargar/middlebaby/1.png",
			L"wugargar/middlebaby/2.png",
			L"wugargar/middlebaby/3.png");
		m_pMiddleBaby->SetPosition(NowPosition);
		AddChild(m_pMiddleBaby,100);
	}
	else if (life_time >= 20 && m_AgeState == MIDDLE_BABY)//20�ʿ� ��� ��
	{
		m_AgeState = GROWN_UP;
		NNPoint NowPosition = m_pMiddleBaby->GetPosition();
		RemoveChild(m_pMiddleBaby,true);
		m_pGrownUp = NNAnimation::Create(6,
			L"wugargar/grown-up/1.png",
			L"wugargar/grown-up/2.png",
			L"wugargar/grown-up/3.png",
			L"wugargar/grown-up/4.png",
			L"wugargar/grown-up/3.png",
			L"wugargar/grown-up/2.png");
		m_pGrownUp->SetPosition(NowPosition);
		AddChild(m_pGrownUp,100);
	}
}

void CHumanInFarm::CollectMeatPointFromGrownUp()
{
	if( NNInputSystem::GetInstance()->GetKeyState(VK_LBUTTON) == KEY_UP ) {
		NNPoint cursorPosition = NNInputSystem::GetInstance()->GetMousePosition();
		bool isInXCoordRange = (m_pGrownUp->GetPositionX() < cursorPosition.GetX()) && ( ( m_pGrownUp->GetPositionX() + 50 ) > cursorPosition.GetX() );
		bool isInYCoordRange = (m_pGrownUp->GetPositionY() < cursorPosition.GetY()) && ( ( m_pGrownUp->GetPositionY() + 90 ) > cursorPosition.GetY() );
		
		if(isInXCoordRange && isInYCoordRange){
			if(m_AgeState == DEAD && m_pDead){
				printf_s("�ŵд�\n");
				RemoveChild(m_pDead,true);
				RemoveChild(m_pGrownUp,true);
				CPlayer* m_pPlayer = CPlayer::GetInstance();
				m_pPlayer->SetMeatPoint(m_pPlayer->GetMeatPoint() + 30);
				return;
			}
			else if(m_AgeState == GROWN_UP && m_pGrownUp){
				printf_s("�Ͷ߸���\n");
				int rannum = rand()%6;
				m_AgeState = DEAD;
				NNPoint NowPosition = m_pGrownUp->GetPosition();
				//RemoveChild(m_pGrownUp,true);
				m_pGrownUp->SetVisible(false);
				m_pDead = NNSprite::Create(DeadImagePath[rannum]);
				m_pDead->SetPosition(NowPosition);
				AddChild(m_pDead,100);
				return;
			}
		}
	}
}
