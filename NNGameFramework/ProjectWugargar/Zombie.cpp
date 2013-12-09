#include "Zombie.h"
#include "PlayScene.h"
#include "NNResourceManager.h"
#include "NNAudioSystem.h"


CZombie::CZombie(void)
{
	PlayCreationSound();
//	enemyList = &(CPlayScene::GetInstance()->GetPoliceList());
}


CZombie::~CZombie(void)
{
}

void CZombie::Render()
{

}

void CZombie::Update( float dTime )
{

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


