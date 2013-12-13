#include "PoorZombie.h"

CPoorZombie::CPoorZombie(void)
{
	initStatus();
}


CPoorZombie::~CPoorZombie(void)
{

}


void CPoorZombie::initStatus( void )
{
	m_zombieType = POOR_ZOMBIE;
	m_HealthPoint = 100;
	m_FullHP = m_HealthPoint;
	m_MovingSpeed = 40.0f;
	m_AttackPower = 8;
	m_DefensivePower = 2;
	m_AttackRange = 30.0f;
	m_NumberOfTarget = 1;
	m_AttackSpeed = 2000;
	m_CreateCost = 100;
	m_Identity = Zombie;
	m_SplashAttack = false;

	ApplyZombieLevel();

	WalkAnimationImagePath.push_back(L"wugargar/poor/walk/0.png");
	WalkAnimationImagePath.push_back(L"wugargar/poor/walk/1.png");
	WalkAnimationImagePath.push_back(L"wugargar/poor/walk/2.png");
	WalkAnimationImagePath.push_back(L"wugargar/poor/walk/3.png");
	WalkAnimationImagePath.push_back(L"wugargar/poor/walk/4.png");
	WalkAnimationImagePath.push_back(L"wugargar/poor/walk/5.png");
	WalkAnimationImagePath.push_back(L"wugargar/poor/walk/6.png");
	WalkAnimationImagePath.push_back(L"wugargar/poor/walk/7.png");

	DeadAnimationImagePath.push_back(L"wugargar/poor/dead/0.png");
	DeadAnimationImagePath.push_back(L"wugargar/poor/dead/1.png");
	DeadAnimationImagePath.push_back(L"wugargar/poor/dead/2.png");
	DeadAnimationImagePath.push_back(L"wugargar/poor/dead/3.png");
	DeadAnimationImagePath.push_back(L"wugargar/poor/dead/4.png");
	DeadAnimationImagePath.push_back(L"wugargar/poor/dead/5.png");
	DeadAnimationImagePath.push_back(L"wugargar/poor/dead/6.png");
	DeadAnimationImagePath.push_back(L"wugargar/poor/dead/7.png");

	m_Animation = NNAnimation::Create();
	int i = 0;
	for(auto &iter = WalkAnimationImagePath.begin(); iter != WalkAnimationImagePath.end(); ++iter)
	{
		m_Animation->AddSpriteNode(const_cast<wchar_t*>((*iter).c_str()));
		m_Animation->GetSpriteList()[i++]->SetFrameTime( 0.13f );
	}
	m_Animation->SetPosition(-((m_Animation->GetSpriteList()[0])->GetSprite()->GetImageWidth()/2), -((m_Animation->GetSpriteList()[0])->GetSprite()->GetImageHeight()/2));

	m_DeadAnimation = NNAnimation::Create();
	i=0;
	for(auto &iter = DeadAnimationImagePath.begin(); iter != DeadAnimationImagePath.end(); ++iter)
	{
		m_DeadAnimation->AddSpriteNode(const_cast<wchar_t*>((*iter).c_str()));
		m_DeadAnimation->GetSpriteList()[i++]->SetFrameTime( 0.2f );
	}
	m_DeadAnimation->SetLoop(false);

	m_Animation->SetPosition(-((m_Animation->GetSpriteList()[0])->GetSprite()->GetImageWidth()/2), -((m_Animation->GetSpriteList()[0])->GetSprite()->GetImageHeight()/2));
	AddChild(m_Animation);
	m_DeadAnimation->SetPosition(-((m_Animation->GetSpriteList()[0])->GetSprite()->GetImageWidth()/2), -((m_Animation->GetSpriteList()[0])->GetSprite()->GetImageHeight()/2));
	m_DeadAnimation->SetVisible(false);
	AddChild(m_DeadAnimation);
	
	m_pShowHP->SetCutSize(0,0,50.f,5.f);
	m_pShowHP->SetPosition(m_Animation->GetPositionX(), m_Animation->GetPositionY()); //스프라이트와 이미지 크기에 맞게 배치는 나중에 고려

}


void CPoorZombie::Render()
{
	NNObject::Render();
}

void CPoorZombie::Update( float dTime )
{
	CCharacter::Update(dTime);
}
