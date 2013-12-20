
#include <math.h>

#include "NNResourceManager.h"
#include "NNAudioSystem.h"
#include "NNSpriteAtlas.h"
#include "NNAnimation.h"
#include "NNSound.h"
#include "NNSprite.h"

#include "Player.h"
#include "Character.h"
#include "PlayScene.h"

CCharacter::CCharacter(void)
{	
	m_SplashAttack = false;
	m_SuicideBomber = false;
	m_FreezingAttack = false;
	m_IsBleeding = false;
	m_bleedingTime = 0.0f;

	m_AttackTarget = nullptr;
	m_LastAttackTime = 0;
	m_Freeze = false;
	m_BeginFreezingTIme = 0;
	m_TotalFreezingTime = 0;
	m_Sight = MAP_SIZE_Y * TILE_SIZE_Y / 2;	

	InitHPBar();
	InitHitEffect(L"wugargar/effect/blood.png");
}

CCharacter::~CCharacter(void)
{
	PlayDeadSound();
}


void CCharacter::InitHPBar( void )
{
	m_pShowHP = NNSpriteAtlas::Create(L"wugargar/HPbar.png");
	m_pShowHP->SetCutSize(0,0,50.f,5.f);
	m_pShowHP->SetPosition(-20, -50); //��������Ʈ�� �̹��� ũ�⿡ �°� ��ġ�� ���߿� ���
	AddChild(m_pShowHP, 20);
}


void CCharacter::InitSprite( std::wstring imagePath )
{
	
	// �θ� ����� ��ġ�� ������ �ޱ� ������ 
	// �θ��� ĳ���� ����� ��ġ�� �����ϰ� 
	// �ڽ��� sprite�� (0, 0)���� �ʱ�ȭ�Ѵ�.
	// �̰Ͷ����� �ѽð� ���� ��� �Ф�
	// 12/3 �߰� : ĳ������ ������ �̹����� �߾����� ��� ���� ��������
	// �̹��� ����� 2�� �����ذ��� ��

 	m_Sprite = NNSprite::Create(imagePath);
 	m_Sprite->SetPosition(-(m_Sprite->GetImageWidth()/2), -(m_Sprite->GetImageHeight()/2));
	AddChild(m_Sprite,1);
}

void CCharacter::initStatus( CharacterInfo *characterInfo, int characterType )
{
	m_FullHP = characterInfo[characterType].FullHP;
	m_HealthPoint = m_FullHP;
	m_MovingSpeed = characterInfo[characterType].MovingSpeed;
	m_AttackPower = characterInfo[characterType].AttackPower;
	m_DefensivePower = characterInfo[characterType].DefensivePower;
	m_AttackRange = characterInfo[characterType].AttackRange;
	m_SplashAttackRange = characterInfo[characterType].SplashRange;
	m_AttackSpeed = characterInfo[characterType].AttackSpeed;
	m_SplashAttack = characterInfo[characterType].IsSplash;
	m_Identity = characterInfo[characterType].identity;
	//	m_spritePath = CCharacter::string2wstring(policeInfo[police_type_idx].SpritePath.c_str());
	m_typeName = characterInfo[characterType].TypeName;

	for (int type=0 ; type<NUMBER_OF_CHARACTER_STATUS ; ++type)
	{
		m_Animation[type] = NNAnimation::Create();
		int animationLength = characterInfo[characterType].numberOfAnimationImage[type];
		for (int sprite=0; sprite<animationLength ; ++sprite)
		{
			m_Animation[type]->AddSpriteNode(const_cast<wchar_t*>
				( string2wstring(characterInfo[characterType].animationPath[type][sprite]).c_str() ) );
		}
		m_Animation[type]->SetFrameTimeInSection(CHARACTER_ANIMATION_PLAY_TIME/animationLength,0,animationLength-1);
		m_Animation[type]->SetLoop(false);
		m_Animation[type]->SetVisible(false);
		m_Animation[type]->SetPosition(-((m_Animation[type]->GetSpriteList()[0])->GetSprite()->GetImageWidth()/2),
			-((m_Animation[type]->GetSpriteList()[0])->GetSprite()->GetImageHeight()/2));
		AddChild(m_Animation[type]);
	}
	m_Animation[CHARACTER_WALK]->SetVisible(true);
	m_Animation[CHARACTER_WALK]->SetLoop(true);
}


void CCharacter::InitHitEffect( std::wstring imagePath )
{
	m_HitEffect = NNSprite::Create(imagePath);
	m_HitEffect->SetPosition(-(m_HitEffect->GetImageWidth()/2), -(m_HitEffect->GetImageHeight()/2));
	m_HitEffect->SetVisible(false);
	AddChild(m_HitEffect,10);
}


// �����ֺ����� ĳ���� ���� ����
void CCharacter::SetRandomPositionAroundBase()
{
	float randomPositonY = static_cast<float>(rand() % ((MAP_SIZE_Y-3) * TILE_SIZE_Y) + FIRST_Y_COORDINATE_OF_MAPTILE + TILE_SIZE_Y);
	switch (m_Identity)
	{
	case ZOMBIE:
		SetPosition(GAME_SCREEN_MIN_SIZE_X + 30, randomPositonY);
		break;
	case POLICE:
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


	ShowHitEffect(dTime);
	UpdateHPBar();	
	UpdateZindex();
	UpdateAttackTarget();

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
	} else if( healthPointPercentage< 70 && healthPointPercentage >= 30){
		m_pShowHP->SetCutSize(0,21,healthPointPercentage/2,26.f);
	} else {
		m_pShowHP->SetCutSize(0,35,healthPointPercentage/2,40.f);
	}
}

// set Z-index for suitable viewing
void CCharacter::UpdateZindex()
{
	m_Zindex = 20 + static_cast<int>(m_Position.GetY());
}


// ���긮�� �� �����ֱ�
void CCharacter::ShowHitEffect( float dTime )
{
	if ( m_IsBleeding ) {
		m_bleedingTime += dTime;
		if ( m_bleedingTime > 0.3 ) {
			m_bleedingTime = 0;
			m_IsBleeding = false;
			m_HitEffect->SetVisible(false);
		}
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
void  CCharacter::UpdateAttackTarget()
{
	float closestTargetDistance = 1000000.0f;
	float nextTargetDistance;

	switch(this->GetIdentity())
	{
	case ZOMBIE:
		for(const auto& enemy : *(CPlayScene::GetInstance()->GetPoliceList()))
		{
			nextTargetDistance = this->GetPosition().GetDistance(enemy->GetPosition());
			if(closestTargetDistance > nextTargetDistance)
			{
				closestTargetDistance = nextTargetDistance;
				m_AttackTarget = enemy;
			}
		}
		break;

	case POLICE:
		for(const auto& enemy : *CPlayScene::GetInstance()->GetZombieList())
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
	target->GetHitEffect()->SetVisible(true);
	target->SetIsBleeding(true);
}


 /*
 ����ȣ. 11/20
 ���� ���� ó��. ���� ������ PlayScene�� ��� Enemy�� ����Ʈ�� ���鼭
 ������ SplashRange���ο� �ִ� ������ ��� �������� �޵��� ó����
 */
void CCharacter::SplashAttack( NNPoint splashPoint, clock_t currentTime )
{
	
	 switch (m_Identity)
	 {
	 case ZOMBIE:
		 for (const auto& enemy : *CPlayScene::GetInstance()->GetPoliceList()) {		 
			 if(this->m_SplashAttackRange >= splashPoint.GetDistance(enemy->GetPosition())) {
				 NormalAttack(enemy, currentTime);
			 }
		 }
		 break;
	 case POLICE:
		 for (const auto& enemy : *CPlayScene::GetInstance()->GetZombieList()) {
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
	case ZOMBIE:
		m_Position = m_Position + NNPoint( (m_MovingSpeed), 0.0f) * dTime;
		break;
	case POLICE:
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

void CCharacter::PlayDeadSound()
{
	std::string soundPath;
	switch (m_Identity)
	{
	case ZOMBIE:
		soundPath = "sound/ZombieDeadSound.wav";
		break;
	case POLICE:
		return;
	default:
		return;
	}

	m_dead_sound = NNResourceManager::GetInstance()->LoadSoundFromFile(soundPath, false, false);

	NNAudioSystem::GetInstance()->Play(m_dead_sound);
}
// 
// void CCharacter::InitZombieAnimation()
// {
// 	m_WalkAnimation = NNAnimation::Create();
// 	int i = 0;
// 	for(auto &iter = WalkAnimationImagePath.begin(); iter != WalkAnimationImagePath.end(); ++iter)
// 	{
// 		m_WalkAnimation->AddSpriteNode(const_cast<wchar_t*>((*iter).c_str()));
// 		m_WalkAnimation->GetSpriteList()[i++]->SetFrameTime( 0.2f );
// 	}
// 	m_WalkAnimation->SetPosition(-((m_WalkAnimation->GetSpriteList()[0])->GetSprite()->GetImageWidth()/2), -((m_WalkAnimation->GetSpriteList()[0])->GetSprite()->GetImageHeight()/2));
// 
// 	m_DeadAnimation = NNAnimation::Create();
// 	i=0;
// 	for(auto &iter = DeadAnimationImagePath.begin(); iter != DeadAnimationImagePath.end(); ++iter)
// 	{
// 		m_DeadAnimation->AddSpriteNode(const_cast<wchar_t*>((*iter).c_str()));
// 		m_DeadAnimation->GetSpriteList()[i++]->SetFrameTime( 0.1f );
// 	}
// 	m_DeadAnimation->SetLoop(false);
// 
// 	m_WalkAnimation->SetPosition(-((m_WalkAnimation->GetSpriteList()[0])->GetSprite()->GetImageWidth()/2), -((m_WalkAnimation->GetSpriteList()[0])->GetSprite()->GetImageHeight()/2));
// 	AddChild(m_WalkAnimation);
// 
// 	m_DeadAnimation->SetPosition(-((m_WalkAnimation->GetSpriteList()[0])->GetSprite()->GetImageWidth()/2), -((m_WalkAnimation->GetSpriteList()[0])->GetSprite()->GetImageHeight()/2));
// 	m_DeadAnimation->SetVisible(false);
// 	AddChild(m_DeadAnimation);
// 
// 	m_pShowHP->SetCutSize(0,0,50.f,5.f);
// 	m_pShowHP->SetPosition(m_WalkAnimation->GetPositionX(), m_WalkAnimation->GetPositionY()); //��������Ʈ�� �̹��� ũ�⿡ �°� ��ġ�� ���߿� ���
// }
