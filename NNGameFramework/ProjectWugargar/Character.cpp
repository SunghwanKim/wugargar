#include "Character.h"
#include "PlayScene.h"
#include <math.h>


CCharacter::CCharacter(void)
{	
	m_SplashAttack = false;
	m_SuicideBomber = false;
	m_FreezingAttack = false;

	m_AttackTarget = nullptr;
	m_LastAttackTime = 0;
	m_Freeze = false;
	m_BeginFreezingTIme = 0;
	m_TotalFreezingTime = 0;
	m_Sight = MAP_SIZE_Y * TILE_SIZE_Y / 2;	
}

CCharacter::~CCharacter(void)
{
}


void CCharacter::InitSprite( std::wstring imagePath )
{
	m_Sprite = NNSprite::Create(imagePath);
	
	// �θ� ����� ��ġ�� ������ �ޱ� ������ 
	// �θ��� ĳ���� ����� ��ġ�� �����ϰ� 
	// �ڽ��� sprite�� (0, 0)���� �ʱ�ȭ�Ѵ�.
	// �̰Ͷ����� �ѽð� ���� ��� �Ф�
	// 12/3 �߰� : ĳ������ ������ �̹����� �߾����� ��� ���� ��������
	// �̹��� ����� 2�� �����ذ��� ��

	m_Sprite->SetPosition(-m_Sprite->GetImageWidth()/2, -m_Sprite->GetImageHeight()/2);		
	AddChild(m_Sprite, 1);

	if(GetIdentity() == Zombie)
	{
		m_Animation = NNAnimation::Create(1,imagePath.c_str());
		m_Animation->SetPosition(-((m_Animation->GetSpriteList()[0])->GetSize().GetWidth()/2), -((m_Animation->GetSpriteList()[0])->GetSize().GetHeight()/2));
		//AddChild(m_Animation,1);
	}
	/*
	m_pShowHP = NNLabel::Create(L"HP", L"���� ���", 10.f);
	m_pShowHP->SetPosition(m_Sprite->GetPositionX(), m_Sprite->GetPositionY()+10.f);
	AddChild(m_pShowHP, 20);
	*/
	m_pShowHP = NNSpriteAtlas::Create(L"wugargar/HPbar.png");
	m_pShowHP->SetCutSize(0,0,50.f,5.f);
	if( GetIdentity() == Zombie)
		m_pShowHP->SetPosition(m_Animation->GetPositionX(), m_Animation->GetPositionY()+10.f);
	else
		m_pShowHP->SetPosition(m_Sprite->GetPositionX(), m_Sprite->GetPositionY()+10.f);
	AddChild(m_pShowHP, 20);
}

// �����ֺ����� ĳ���� ���� ����
void CCharacter::SetRandomPositionAroundBase()
{
	float randomPositonY = static_cast<float>(rand() % ((MAP_SIZE_Y-2) * TILE_SIZE_Y) + FIRST_Y_COORDINATE_OF_MAPTILE + TILE_SIZE_Y);
	switch (m_Identity)
	{
	case Zombie:
		SetPosition(GAME_SCREEN_MIN_SIZE_X + 30, randomPositonY);
		break;
	case Police:
		SetPosition(GAME_SCREEN_MAX_SIZE_X - 30, randomPositonY);
		break;
	default:
		break;
	}

	// ���ϴ� ���� �ֺ����� �����ϱ� ���� ����
//	NNPoint baseLocation;
// 	switch (m_Identity)
// 	{
// 	case Zombie:
// 		baseLocation = CPlayScene::GetInstance()->GetMapCreator()->GetZombieBase()->GetPosition();
// 		baseLocation.SetX(baseLocation.GetX()+TILE_SIZE_X);
// 		baseLocation.SetY(baseLocation.GetY()+TILE_SIZE_Y);
// 		break;
// 	case Police:
// 		baseLocation = CPlayScene::GetInstance()->GetMapCreator()->GetPoliceBase()->GetPosition();
// 		baseLocation.SetX(baseLocation.GetX()-TILE_SIZE_X);
// 		break;
// 	default:
// 		break;
// 	}
// 
// 	int random_location_x = rand() % TILE_SIZE_X;
// 	int random_location_y = rand() % TILE_SIZE_Y;
// 
// 	SetPosition((baseLocation.GetX()+random_location_x),(baseLocation.GetY()+random_location_y));
}


void CCharacter::Render()
{
	NNObject::Render();
}

void CCharacter::Update( float dTime )
{
	NNObject::Update(dTime);

	clock_t currentTime = clock();

	UpdateHPBar();	
	DetermineAttackTarget();

	//���� ����ִ� ���¶�� �̵�/������ �Ұ�
	if(m_Freeze) {
		CheckMeltingTime(currentTime);
	} else {
		//AttackTarget�� �����ϰ� Attack�� �����ϸ�(�����Ÿ� üũ)
		//�����ϰ� �׷��� ������ �̵�, �þ߿� ������ ������ ����
		if(TargetInRange()) {
			if( CheckAttackTiming(currentTime) ) { 
				AttackEnemy(currentTime);				
			}
		} else if (TargetInSight() ) {			
			GoToAttackTarget(dTime);
		} else {
			GoForward(dTime);
		}
	}
}

void CCharacter::UpdateHPBar( void )
{	
	float healthPointPercentage = m_HealthPoint/m_FullHP*100;
	if( healthPointPercentage >= 70 ) {// HP ���¿� ���� �Ķ���, �����, ���������� ǥ��
		m_pShowHP->SetCutSize(0,0,healthPointPercentage/2,5.f);
		m_pShowHP->SetPosition(m_Sprite->GetPositionX(), m_Sprite->GetPositionY());
	} else if( healthPointPercentage< 70 && healthPointPercentage >= 30){
		m_pShowHP->SetCutSize(0,21,healthPointPercentage/2,26.f);
		m_pShowHP->SetPosition(m_Sprite->GetPositionX(), m_Sprite->GetPositionY());
	} else {
		m_pShowHP->SetCutSize(0,35,healthPointPercentage/2,40.f);
		m_pShowHP->SetPosition(m_Sprite->GetPositionX(), m_Sprite->GetPositionY());
	}
}


/*
�Լ��� : DetermineAttackTarget
���� : �� ��ü�� attack_target�� �����ϴ� �Լ��� ����ϸ� �ȴ�.
���Ŀ��� GoAttackTarget���� �Լ��� ����ϸ� �� ��
�ֿ� ���� : �̱������� ������ Scene�� �ִ� ��� Police�迭�� ���鼭 NNPoint�� �Լ��� �̿���
�Ÿ��� �����ϰ� ���� ����� �Ÿ��� �����Ͽ� �ش��ϴ� idx�� ���� Ÿ���� ��ȯ�Ѵ�.

���� ������ : 2013/11/03
���� ������ : 2013/11/14

ISSUE : 1�� ����. switch���� �ݺ��Ǵ� For���� �ϳ��� ���� ����� ã�ƾ� ��.
���� ���.

11/14 : Enemy�� �������� ���� ��(NULL) attackTarget�� ����� Base�� �����ϵ��� ����
*/
void  CCharacter::DetermineAttackTarget()
{
	float closestTargetDistance = 1000000.0f;
	float nextTargetDistance;

	switch(this->GetIdentity())
	{
	case Zombie:
		for(const auto& enemy : CPlayScene::GetInstance()->GetPoliceList())
		{
			nextTargetDistance = this->GetPosition().GetDistance(enemy->GetPosition());
			if(closestTargetDistance > nextTargetDistance)
			{
				closestTargetDistance = nextTargetDistance;
				m_AttackTarget = enemy;
			}
		}
		break;

	case Police:
		for(const auto& enemy : CPlayScene::GetInstance()->GetZombieList())
		{
			nextTargetDistance= this->GetPosition().GetDistance(enemy->GetPosition());
			if(closestTargetDistance > nextTargetDistance)
			{
			 	closestTargetDistance = nextTargetDistance;
			 	m_AttackTarget = enemy;
			}
		}
 		break;
	default:
		break;
	}
}


void CCharacter::AttackEnemy( clock_t currentTime )
{
	//splash�Ӽ��� true�� ���͸� splash����
	if(m_SplashAttack) {
		SplashAttack(m_AttackTarget->GetPosition(), currentTime);
	} else { // �ƴϸ� normal attack
		NormalAttack(m_AttackTarget, currentTime);
	}	

	// ���� ���� ĳ������ ��� hp�� 0����..
	if (m_SuicideBomber) {
		m_HealthPoint = 0.0f;
	}

	// set the last attack time
	m_LastAttackTime = currentTime;
}

// �Ϲ� ����, ���� ���ݷ°� ���� ���� ���� ��ŭ�� �� HP���� ���ش�.
void CCharacter::NormalAttack( CCharacter* target, clock_t currentTime )
{
	// When character has freezing attack
	if (m_FreezingAttack) {
		m_AttackTarget->SetFreeze(true);
		m_AttackTarget->SetBeginFreezingTime(currentTime);
		m_AttackTarget->SetTotalFreezingTime(m_FreezingAttackDuration);
	}

	int damage = m_AttackPower - target->GetDefensivePower();
	float targetHP = target->GetHP(); 
	target->SetHP(targetHP-damage) ;
}


 /*
 ����ȣ. 11/20
 ���� ���� ó��. ���� ������ PlayScene�� ��� Enemy�� ����Ʈ�� ���鼭
 ������ SplashRange���ο� �ִ� ������ ��� �������� �޵��� ó����
 */
void CCharacter::SplashAttack( NNPoint splashPoint, clock_t currentTime )
{
// 	for (const auto& enemy : (*enemyList) {		 
// 		 if(this->m_SplashAttackRange >= splashPoint.GetDistance(enemy->GetPosition())) {
// 			 NormalAttack(enemy);
// 		 }
// 	}
	
	 switch (m_Identity)
	 {
	 case Zombie:
		 for (const auto& enemy : CPlayScene::GetInstance()->GetPoliceList()) {		 
			 if(this->m_SplashAttackRange >= splashPoint.GetDistance(enemy->GetPosition())) {
				 NormalAttack(enemy, currentTime);
			 }
		 }
		 break;
	 case Police:
		 for (const auto& enemy : CPlayScene::GetInstance()->GetZombieList()) {
			 if(this->m_SplashAttackRange >= splashPoint.GetDistance(enemy->GetPosition())) {
				 NormalAttack(enemy, currentTime);
			 }
		 }
		 break;
	 default:
		 break;

	 }
}


/*
����ȣ. 11/14
Attack_target���� �Ÿ��� ����Ͽ� ���� �������� �����ϵ��� ��.
Issue : ������ �����ϴ� MakeCharacterWalk�� ������� �������
*/
void CCharacter::GoToAttackTarget(float dTime)
{
	float gap = m_Position.GetDistance(m_AttackTarget->GetPosition());
	float gap_x = m_AttackTarget->GetPositionX() - m_Position.GetX();
	float gap_y = m_AttackTarget->GetPositionY() - m_Position.GetY();
	float cosd = (gap_x) / gap;
	float sind = (gap_y) / gap;

	m_Position = m_Position + NNPoint( (m_MovingSpeed * cosd),( m_MovingSpeed * sind) )*dTime;	
}





/*
11.21 ����ȣ
��� �� �ð�, ���� �ð��� ����ִ� ���¸� üũ�Ͽ� ���� ���°� 
������ �õ��������� �ص��������� üũ�ϰ� �ݿ�.
*/
void CCharacter::CheckMeltingTime( clock_t currentTime )
{
	if( (currentTime - m_BeginFreezingTIme) >= m_TotalFreezingTime)	{
		m_Freeze = false;		
	}


#ifdef _DEBUG
	if( (currentTime - m_BeginFreezingTIme) >= m_TotalFreezingTime) {
		printf_s("UNICE\n");
	}
	if ( currentTime % 1000 == 0) {
		printf_s("ICETIME : %d, NOWTIME : %d\n", m_BeginFreezingTIme, currentTime);
	}	
#endif

}


void CCharacter::GoForward(float dTime)
{
	switch (m_Identity)
	{
	case Zombie:
		m_Position = m_Position + NNPoint( (m_MovingSpeed), 0.0f) * dTime;
		break;
	case Police:
		m_Position = m_Position - NNPoint( (m_MovingSpeed), 0.0f) * dTime;
		break;
	default:
		break;
	}
	
}

//string�� wstring����
std::wstring CCharacter::string2wstring(std::string str) {

	std::wstring wstr(str.length(),L' ');

	copy(str.begin(),str.end(),wstr.begin());

	return wstr;

}