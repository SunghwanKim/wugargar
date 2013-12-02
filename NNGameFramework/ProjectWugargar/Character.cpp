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
	m_Sight = 0;
	//sight�� ö���ϰ� ĳ���Ͱ� �� ��ġ�� ���̱� ���� ������ ���� ����
	//m_sight = 100.0f + rand() % 50;
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

	m_Sprite->SetPosition(0.0f, 0.0f);	
	AddChild(m_Sprite, 1);

	/*
	m_pShowHP = NNLabel::Create(L"HP", L"���� ���", 10.f);
	m_pShowHP->SetPosition(m_Sprite->GetPositionX(), m_Sprite->GetPositionY()+10.f);
	AddChild(m_pShowHP, 20);
	*/
	m_pShowHP = NNSpriteAtlas::Create(L"wugargar/HPbar.png");
	m_pShowHP->SetCutSize(0,0,50.f,5.f);
	m_pShowHP->SetPosition(m_Sprite->GetPositionX(), m_Sprite->GetPositionY()+10.f);
	AddChild(m_pShowHP, 20);

}

// �����ֺ����� ĳ���� ���� ����
void CCharacter::SetRandomPositionAroundBase()
{
	NNPoint baseLocation;

	switch (m_Identity)
	{
	case Zombie:
		baseLocation = CPlayScene::GetInstance()->GetMapCreator()->GetZombieBase()->GetPosition();
		baseLocation.SetX(baseLocation.GetX()+TILE_SIZE_X);
		baseLocation.SetY(baseLocation.GetY()+TILE_SIZE_Y);
		break;
	case Police:
		baseLocation = CPlayScene::GetInstance()->GetMapCreator()->GetPoliceBase()->GetPosition();
		baseLocation.SetX(baseLocation.GetX()-TILE_SIZE_X);
		break;
	default:
		break;
	}

	int random_location_x = rand() % TILE_SIZE_X;
	int random_location_y = rand() % TILE_SIZE_Y;

	SetPosition((baseLocation.GetX()+random_location_x),(baseLocation.GetY()+random_location_y));
}


void CCharacter::Render()
{
	NNObject::Render();
}

void CCharacter::Update( float dTime )
{
	clock_t currentTime = clock();

	UpdateHPBar();	
	DetermineAttackTarget();

	//���� ����ִ� ���¶�� �̵�/������ �Ұ�
	if(m_Freeze) {
		CheckMeltingTime(currentTime);
	} else {
		//AttackTarget�� �����ϰ� Attack�� �����ϸ�(�����Ÿ� üũ)
		//�����ϰ� �׷��� ������ Attack Target���� ����
		if(TargetInRange()) {
			if( CheckAttackSpeed(currentTime) ) { 
				Attack(currentTime);				
			}
		} else {
			GoToAttackTarget(dTime);
		}
	}
}

void CCharacter::UpdateHPBar( void )
{
	float HP = m_HealthPoint;
	if( HP/m_HPRatioPer100 >= 70 ) {// HP ���¿� ���� �Ķ���, �����, ���������� ǥ��
		m_pShowHP->SetCutSize(0,0,HP/2,5.f);
		m_pShowHP->SetPosition(m_Sprite->GetPositionX(), m_Sprite->GetPositionY());
	} else if( HP/m_HPRatioPer100 < 70 && HP/m_HPRatioPer100 >= 30){
		m_pShowHP->SetCutSize(0,21,HP/2,26.f);
		m_pShowHP->SetPosition(m_Sprite->GetPositionX(), m_Sprite->GetPositionY()-21.f);
	} else {
		m_pShowHP->SetCutSize(0,35,HP/2,40.f);
		m_pShowHP->SetPosition(m_Sprite->GetPositionX(), m_Sprite->GetPositionY()-35.f);
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
	float return_distnace = 1000000.0f;
	float next_distance;
	CZombie *tmp_closer_target_zombie = NULL;
	CPolice *tmp_closer_target_police = NULL;
	CCharacter *return_target = NULL;



	switch(this->GetIdentity())
	{
	case Zombie:
		for(const auto& enemy : CPlayScene::GetInstance()->GetPoliceList())
		{
			next_distance = this->GetPosition().GetDistance(enemy->GetPosition());
			if(return_distnace > next_distance)
			{
				return_distnace = next_distance;
				m_AttackTarget = enemy;
			}
		}
// 		if(m_AttackTarget == NULL)
// 			m_AttackTarget = CPlayScene::GetInstance()->GetMapCreator()->GetPoliceBase();
		break;

	case Police:
		for(const auto& enemy : CPlayScene::GetInstance()->GetZombieList())
		{
			next_distance= this->GetPosition().GetDistance(enemy->GetPosition());
			if(return_distnace > next_distance)
			{
			 	return_distnace = next_distance;
			 	m_AttackTarget = enemy;
			}
		}
// 		if(m_AttackTarget == NULL)
// 			m_AttackTarget = CPlayScene::GetInstance()->GetMapCreator()->GetZombieBase();
 		break;
	default:
		break;
	}
}


void CCharacter::Attack( clock_t currentTime )
{
	// When character has freezing attack
	if (m_FreezingAttack) {
		m_AttackTarget->SetFreeze(true);
		m_AttackTarget->SetBeginFreezingTime(currentTime);
		m_AttackTarget->SetTotalFreezingTime(m_FreezingAttackDuration);
	}

	
	//splash�Ӽ��� true�� ���͸� splash����
	if(m_SplashAttack) {
		SplashAttack(m_AttackTarget->GetPosition());
	} else { // �ƴϸ� normal attack
		NormalAttack(m_AttackTarget);
	}	

	// ���� ���� ĳ������ ��� hp�� 0����..
	if (m_SuicideBomber) {
		m_HealthPoint = 0.0f;
	}

	// set the last attack time
	m_LastAttackTime = currentTime;
}

// �Ϲ� ����, ���� ���ݷ°� ���� ���� ���� ��ŭ�� �� HP���� ���ش�.
void CCharacter::NormalAttack( CCharacter* target )
{
	int damage = m_AttackPower - target->GetDefensivePower();
	float targetHP = target->GetHP(); 
	target->SetHP(targetHP-damage) ;
}


 /*
 ����ȣ. 11/20
 ���� ���� ó��. ���� ������ PlayScene�� ��� Enemy�� ����Ʈ�� ���鼭
 ������ SplashRange���ο� �ִ� ������ ��� �������� �޵��� ó����
 */
void CCharacter::SplashAttack( NNPoint splashPoint )
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
				 NormalAttack(enemy);
			 }
		 }
		 break;
	 case Police:
		 for (const auto& enemy : CPlayScene::GetInstance()->GetZombieList()) {
			 if(this->m_SplashAttackRange >= splashPoint.GetDistance(enemy->GetPosition())) {
				 NormalAttack(enemy);
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
	float gap_x = m_AttackTarget->GetPositionX() - m_Position.GetX();
	float gap_y = m_AttackTarget->GetPositionY() - m_Position.GetY();
	float t_x = (gap_x) / (gap_x+gap_y);
	float t_y = (gap_y) / (gap_x+gap_y);


	float distance_attacktarget;
	distance_attacktarget = this->GetPosition().GetDistance(m_AttackTarget->GetPosition());
	//Character�� �Ȱ�ġ�� �ϱ� ���� ������ ���� �ڵ�
	
	/*if(distance_attacktarget > m_sight)
	{
		MakeCharacterWalk(dTime);
		return ;
	}*/
	
	if(this->GetPositionX() < m_AttackTarget->GetPositionX())
		this->SetPosition(this->m_Position - NNPoint( -(m_MovingSpeed*t_x),-( m_MovingSpeed*t_y) )*dTime);
	else
		this->SetPosition(this->m_Position - NNPoint( (m_MovingSpeed*t_x),( m_MovingSpeed*t_y) )*dTime);

	//switch (m_Identity)
	//{
	//case Zombie:
	//	this->SetPosition(this->m_Position - NNPoint( -(m_MovingSpeed*t_x),-( m_MovingSpeed*t_y) )*dTime);
	//	break;
	//case Police:
	//	this->SetPosition(this->m_Position - NNPoint( (m_MovingSpeed*t_x),( m_MovingSpeed*t_y) )*dTime);
	//	break;
	//default:
	//	break;
	//}
	//

	
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


void CCharacter::MakeCharacterWalk(float dTime)
{
	switch (m_Identity)
	{
	case Zombie:
		this->SetPosition(this->GetPosition() + NNPoint( (this->GetMovingSpeed()), 0.0f) * dTime);
		break;
	case Police:
		this->SetPosition(this->GetPosition() - NNPoint( (this->GetMovingSpeed()), 0.0f) * dTime);
		break;
	default:
		break;
	}

}
