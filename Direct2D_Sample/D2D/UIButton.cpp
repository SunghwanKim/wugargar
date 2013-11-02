#include "UIButton.h"
#include "NNInputSystem.h"


CUIButton::CUIButton(void)
{
	m_pPressedImage = NNSprite::Create(L"wugargar/UIbuttons/button_pressed_poorZombie.png");
	m_pNormalImage = NNSprite::Create(L"wugargar/UIbuttons/button_normal_poorZombie.png");
	m_pPressedImage->SetPosition(m_Position.GetX(), m_Position.GetY());
	m_pNormalImage->SetPosition(m_Position.GetX(), m_Position.GetY());

	m_pPressedImage->SetVisible(true);
	m_pNormalImage->SetVisible(true);

	AddChild( m_pPressedImage, 10);
	AddChild( m_pNormalImage, 10);
}


CUIButton::~CUIButton(void)
{
}

void CUIButton::Render()
{
	NNObject::Render();
}

void CUIButton::Update( float dTime )
{

}

// ���� ���콺 Ŀ���� ��ġ�� UIbutton ���� �ִ����� üũ��
// parameter : void
// return  : button ���� ���� �� 1, button��dp ���� �� 0�� ��ȯ
bool CUIButton::CheckButtonArea( void )
{
	NNPoint cursorPosition = NNInputSystem::GetInstance()->GetMousePosition();
	bool isInXCoordRange = (m_Position.GetX() < cursorPosition.GetX()) && ( ( m_Position.GetX() + m_pNormalImage->GetImageWidth() ) > cursorPosition.GetX() );
	bool isInYCoordRange = (m_Position.GetY() < cursorPosition.GetY()) && ( ( m_Position.GetY() + m_pNormalImage->GetImageHeight() ) > cursorPosition.GetY() );
	

	m_pPressedImage->SetVisible(false);
	m_pNormalImage->SetVisible(false);

	// //visible�� false�� �ǵ� �̹����� ��� ������ ������ ����
	// 1. ������ framework�� �����ε� ���� object�� �ڽĵ鿡�� �������� zIndex���� 
	// �ڽ��� �ڽ��� ���� zIndex���� �켱�ϱ� ������ �ڽ��� �ڽ��� ���� zIndex�� ���õ�
	// 2. SetVisible���� �� false������ �̻��ϰ� �� �Դ� �� ����, 
	if ( isInXCoordRange && isInYCoordRange ) {
//		m_pNormalImage->SetVisible(false);

// 		if (NNInputSystem::GetInstance()->GetKeyState( VK_LBUTTON ) == KEY_NOTPRESSED ) {
// 			m_pNormalImage->SetVisible(true);
// 			m_pPressedImage->SetVisible(false);
// 		}

		return true;
	}
	
	return false;
}
