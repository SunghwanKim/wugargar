#include "Zombie.h"
#include "PlayScene.h"
#include "NNResourceManager.h"
#include "NNAudioSystem.h"
#include "NNAnimation.h"
#include "Player.h"


CZombie::CZombie(void)
{
	PlayCreationSound();
//	enemyList = &(CPlayScene::GetInstance()->GetPoliceList());
	m_Shadow = NNSprite::Create(L"wugargar/shadow.png");
	m_Shadow->SetPosition(-35,20);
	AddChild(m_Shadow);

	InitAttackEffect();
}


CZombie::~CZombie(void)
{
}

void CZombie::Render()
{
	NNObject::Render();
}

void CZombie::Update( float dTime )
{
	CCharacter::Update(dTime);
}

void CZombie::ApplyZombieLevel()
{
	CPlayer* player = CPlayer::GetInstance();
	int currentZombieLevel = player->GetZombieLevel(m_zombieType);

	m_HealthPoint += m_HealthPoint/10*currentZombieLevel;
	m_AttackPower += m_AttackPower/5 *currentZombieLevel;
	m_DefensivePower += m_DefensivePower/5 + currentZombieLevel;
}


//12.09 ����ȣ
//������ ������ ���带 ����ϴ� �Լ�.
void CZombie::PlayCreationSound()
{
	std::string soundPath;

	//�����ϰ� �� ���� ���� �Ҹ� �� �ϳ��� ����ϰų�
	//����� ���� �ʴ´�.
	switch (rand() % 3)
	{
	case 0:
		soundPath = "sound/ZombieCreateSound1.wav";
		break;
	case 1:
		soundPath = "sound/ZombieCreateSound2.wav";
		break;
	default:
		return;
	}

	m_creation_sound = NNResourceManager::GetInstance()->LoadSoundFromFile(soundPath, false, false);
	
	NNAudioSystem::GetInstance()->Play(m_creation_sound);

}

void CZombie::initStatus( CharacterInfo *characterInfo, int characterType )
{
	CCharacter::initStatus(characterInfo, characterType);

	m_zombieType = static_cast<ZombieType>(characterInfo[characterType].characterType);

	m_CreateCost = characterInfo[characterType].CreationCost;
	m_FreezingAttackDuration = characterInfo[characterType].FreezingAttackDuration;
	m_SuicideBomber = characterInfo[characterType].IsSelfDestruction;

	ApplyZombieLevel();
}

void CZombie::InitAttackEffect()
{
	m_AttackEffect = NNAnimation::Create();
	m_AttackEffect -> AddSpriteNode(L"wugargar/muzzle/Frame0.png");
	m_AttackEffect -> AddSpriteNode(L"wugargar/muzzle/Frame1.png");
	m_AttackEffect -> AddSpriteNode(L"wugargar/muzzle/Frame2.png");
	m_AttackEffect -> AddSpriteNode(L"wugargar/muzzle/Frame3.png");
	m_AttackEffect -> AddSpriteNode(L"wugargar/muzzle/Frame4.png");
	m_AttackEffect -> AddSpriteNode(L"wugargar/muzzle/Frame5.png");
	m_AttackEffect -> AddSpriteNode(L"wugargar/muzzle/Frame6.png");
	m_AttackEffect -> AddSpriteNode(L"wugargar/muzzle/Frame7.png");

	m_AttackEffect -> SetFrameTimeInSection(0.2f,0,7);
	m_AttackEffect -> SetPosition(5,10);

	AddChild(m_AttackEffect);
}




