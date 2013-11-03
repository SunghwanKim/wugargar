#include "Wugargar.h"
#include "NNPoint.h"
#include "NNInputSystem.h"
#include "NNLabel.h"
#include "NNApplication.h"
#include <tchar.h>
#include <time.h>

CWugargar::CWugargar(void)
{
	m_Background = CBackground::Create();
	m_ZombieBase = CZombieBase::Create();
	m_UIButton1 = CUIButton::Create();
	m_UIButton1->SetPosition(20.0f, 500.0f);
//	m_character = CCharacter::Create();
	//m_UIButton1->SetScale(220.0f, 220.0f);

	m_ShowMouseStatus = NNLabel::Create( L"cursor position", L"���� ���", 35.f );
	m_ShowMouseStatus->SetPosition(0.0f, 0.0f);
	
	AddChild( m_Background, 1);
	AddChild( m_ZombieBase, 2);
	AddChild( m_ShowMouseStatus ,10);
	AddChild( m_UIButton1 , 10);
}

CWugargar::~CWugargar(void)
{
}

void CWugargar::Render()
{
	NNObject::Render();
}

void CWugargar::Update( float dTime )
{		
	// ���콺 �������� �������� ���� �ӽ� ����	
	// wsprintf�� float���� ����� �� ���� �ӽù������� �����
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient( NNApplication::GetInstance()->GetHWND(), &pt);

	POINT pt2;
	pt2.x = (int)m_UIButton1->GetPositionX();
  	pt2.y = (int)m_UIButton1->GetPositionY();
	
	ZeroMemory(temp, 256);	
	wsprintf(temp, L"image coord x : %4d y : %4d\nwindowcoord x : %4d y : %4d", pt2.x, pt2.y, pt.x, pt.y  );
	m_ShowMouseStatus->SetString(temp);
	// ���콺 �����ǿ� �� 

	if( NNInputSystem::GetInstance()->GetKeyState(VK_LBUTTON) == KEY_DOWN ) {	
		if ( m_UIButton1->CheckButtonArea() ) {

			// ���� ��ü�� ����Ű�� �����Ͱ� �ϳ��ۿ� ����.
		
			AddChild( CPoorZombie::Create(), 3);
		}		
	//	SetPosition( m_PoorZombie->GetPosition() + NNPoint(-5.0f, 0.0f) * dTime );// ���� ��ü�� �̵����� �ٽø����
	}

}

