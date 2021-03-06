
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
	m_pShowHP->SetPosition(-20, -50); //스프라이트와 이미지 크기에 맞게 배치는 나중에 고려
	AddChild(m_pShowHP, 20);
}


void CCharacter::InitSprite( std::wstring imagePath )
{
	
	// 부모 노드의 위치에 영향을 받기 때문에 
	// 부모인 캐릭터 노드의 위치를 설정하고 
	// 자식인 sprite는 (0, 0)으로 초기화한다.
	// 이것때문에 한시간 반을 헤맴 ㅠㅠ
	// 12/3 추가 : 캐릭터의 원점을 이미지의 중앙으로 잡기 위해 원점에서
	// 이미지 사이즈를 2로 나눠준값을 뺌

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
	m_FreezingAttack = characterInfo[characterType].IsIceAttack;
	m_FreezingAttackDuration = characterInfo[characterType].FreezingAttackDuration;
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


// 기지주변에서 캐릭터 랜덤 생성
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

	// 이하는 기지 주변에서 생성하기 위한 내용
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

	//현재 얼어있는 상태라면 이동/공격이 불가
	if(m_Freeze) {
		CheckMeltingTime(currentTime);
	} else {
		//AttackTarget을 설정하고 Attack이 가능하면(사정거리 체크)
		//공격하고 그렇지 않으면 이동, 시야에 없으면 앞으로 전진
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
	if( healthPointPercentage >= 70 ) {// HP 상태에 따라 파란색, 노란색, 빨간색으로 표시
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


// 피흘리는 거 보여주기
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
함수명 : DetermineAttackTarget
사용법 : 이 객체의 attack_target을 설정하는 함수로 사용하면 된다.
이후에는 GoAttackTarget같은 함수를 사용하면 될 듯
주요 로직 : 싱글톤으로 선언한 Scene에 있는 모든 Police배열을 돌면서 NNPoint의 함수를 이용해
거리를 측정하고 가장 가까운 거리로 갱신하여 해당하는 idx를 가진 타겟을 반환한다.

최초 수정일 : 2013/11/03
최종 수정일 : 2013/11/14

ISSUE : 1차 통합. switch문의 반복되는 For문을 하나로 줄일 방법을 찾아야 됨.
수정 요망.

11/14 : Enemy가 존재하지 않을 때(NULL) attackTarget을 상대의 Base로 설정하도록 변경
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
	//splash속성이 true인 몬스터면 splash어택
	if(m_SplashAttack) {
		SplashAttack(m_AttackTarget->GetPosition(), currentTime);
	} else { // 아니면 normal attack
		NormalAttack(m_AttackTarget, currentTime);
	}	

	// 자폭 공격 캐릭터일 경우 hp를 0으로..
	if (m_SuicideBomber) {
		m_HealthPoint = 0.0f;
	}

	// set the last attack time
	m_LastAttackTime = currentTime;
}

// 일반 공격, 나의 공격력과 적의 방어력 차이 만큼을 적 HP에서 빼준다.
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
 정인호. 11/20
 광역 공격 처리. 현재 로직은 PlayScene의 모든 Enemy를 리스트로 돌면서
 지정된 SplashRange내부에 있는 적들은 모두 데미지를 받도록 처리함
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
정인호. 11/14
Attack_target과의 거리를 계산하여 그쪽 방향으로 접근하도록 함.
Issue : 기존에 존재하는 MakeCharacterWalk와 어떤식으로 연계될지
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
11.21 정인호
얼게 된 시간, 현재 시간과 얼어있는 상태를 체크하여 현재 상태가 
아직도 냉동상태인지 해동상태인지 체크하고 반영.
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

//string을 wstring으로
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
// 	m_pShowHP->SetPosition(m_WalkAnimation->GetPositionX(), m_WalkAnimation->GetPositionY()); //스프라이트와 이미지 크기에 맞게 배치는 나중에 고려
// }
