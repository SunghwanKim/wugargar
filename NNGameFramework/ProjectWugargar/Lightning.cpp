#include "Lightning.h"
#include "GameConfig.h"
#include "NNInputSystem.h"

#define LIGHTNING_SPEED 130

CLightning::CLightning(void)
{
	InitSprite(L"wugargar/pika.png");
	m_is_fall_lightning = false;
}


CLightning::~CLightning(void)
{
}

void CLightning::Render()
{
	NNObject::Render();
}

void CLightning::Update( float dTime )
{
	
	if(m_lightning_sprite->GetPositionX() < GAME_SCREEN_MAX_SIZE_X - 20)
		m_lightning_sprite->SetPosition(m_lightning_sprite->GetPosition() + NNPoint(LIGHTNING_SPEED, 0.0f) * dTime);
	else
		m_lightning_sprite->SetPosition(0.0f, POSITION_OF_LIGHTNING);
	if(NNInputSystem::GetInstance()->GetKeyState(VK_SPACE) == KEY_DOWN && !m_fall_lightning_sprite)
	{
		printf_s("put");
		m_fall_lightning_sprite = NNSprite::Create(L"wugargar/pika.png");
		m_fall_lightning_sprite->SetPosition(m_lightning_sprite->GetPosition());
		AddChild(m_fall_lightning_sprite, 1);
		m_is_fall_lightning = true;
		
	}

	if(m_is_fall_lightning)
	{
		m_fall_lightning_sprite->SetPosition(m_fall_lightning_sprite->GetPosition() + NNPoint(LIGHTNING_SPEED, 0.0f) *dTime);
		
		if(m_fall_lightning_sprite->GetPositionY() >= FIRST_Y_COORDINATE_OF_UIBUTTON)
		{
			RemoveChild(m_fall_lightning_sprite, true);
			m_is_fall_lightning = false;
			printf_s("���� �Ҹ�\n");
		}

	}
}

void CLightning::InitSprite( std::wstring imagePath )
{
	m_lightning_sprite = NNSprite::Create(imagePath);
	m_lightning_sprite->SetPosition(0.0f, POSITION_OF_LIGHTNING);	
	AddChild(m_lightning_sprite, 1);

}

/*
11.27 ����ȣ
�浹 ���� �Լ�. ���� ������ �� ��������Ʈ�� ��� ��������Ʈ ���̿�
���� �浹�� �����ϵ��� ����. �浹�ϸ� true, �ƴϸ� false��ȯ
*/
bool CLightning::IsCrash( NNSprite *crash_check_sprite, NNSprite *thi )
{

	if(((thi->GetCenterX() + thi->GetImageWidth()/2) > (crash_check_sprite->GetCenterX() - crash_check_sprite->GetImageWidth()/2)) &&
		(((thi->GetCenterX() + thi->GetImageWidth()/2) <= (crash_check_sprite->GetCenterX() + crash_check_sprite->GetImageWidth()/2)) &&
		((thi->GetCenterY() <= (crash_check_sprite->GetCenterY() + crash_check_sprite->GetImageHeight()/2)) &&
		((thi->GetCenterX() > (crash_check_sprite->GetCenterY() - crash_check_sprite->GetImageHeight()/2)) ))))
		return true;

	return false;


}
