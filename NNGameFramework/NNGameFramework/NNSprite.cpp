
#include "NNSprite.h"
#include "NNResourceManager.h"
#include "NNApplication.h"

//////////////////////////////////////////////////////////////////////////
/*					NNSprite											*/
//////////////////////////////////////////////////////////////////////////
NNSprite* NNSprite::Create( std::wstring path )
{
	static RendererStatus rendererStatus = NNApplication::GetInstance()->GetRendererStatus();

	NNSprite* pInstance = nullptr;
	/*switch ( rendererStatus )
	{
	case D2D:
	*/	pInstance = new NND2DSprite( path );
	/*	break;
	default:
		break;
	}*/
	
	return pInstance;
} 

/*
11.27 ����ȣ
�浹 ���� �Լ�. ���� ������ �� ��������Ʈ�� ��� ��������Ʈ ���̿�
���� �浹�� �����ϵ��� ����. �浹�ϸ� true, �ƴϸ� false��ȯ
*/
bool NNSprite::IsCrash( NNSprite *crash_check_sprite )
{
	
	if(((this->GetCenterX() + this->GetImageWidth()/2) > (crash_check_sprite->GetCenterX() - crash_check_sprite->GetImageWidth()/2)) &&
		(((this->GetCenterX() + this->GetImageWidth()/2) <= (crash_check_sprite->GetCenterX() + crash_check_sprite->GetImageWidth()/2)) &&
		((this->GetCenterY() <= (crash_check_sprite->GetCenterY() + crash_check_sprite->GetImageHeight()/2)) &&
		((this->GetCenterX() > (crash_check_sprite->GetCenterY() - crash_check_sprite->GetImageHeight()/2)) ))))
			return true;

	return false;

		
}



//////////////////////////////////////////////////////////////////////////
/*					NND2DSprite											*/
//////////////////////////////////////////////////////////////////////////
NND2DSprite::NND2DSprite()
	: m_pD2DRenderer(nullptr), m_pD2DTexture(nullptr)
{

}
NND2DSprite::NND2DSprite( std::wstring path )
{
	m_pD2DRenderer = static_cast<NND2DRenderer*>(NNApplication::GetInstance()->GetRenderer());
	m_pD2DTexture = static_cast<NND2DTexture*>(NNResourceManager::GetInstance()->LoadTextureFromFile( path ));

	m_ImageWidth = m_pD2DTexture->GetD2DBitmap()->GetSize().width;
	m_ImageHeight = m_pD2DTexture->GetD2DBitmap()->GetSize().height;
}
NND2DSprite::~NND2DSprite()
{
	Destroy();
}
void NND2DSprite::Destroy()
{
	m_pD2DRenderer = nullptr;
	m_pD2DTexture = nullptr;
}

void NND2DSprite::Render()
{
	if ( m_Visible == false ) return;

	NNObject::Render();

	m_pD2DRenderer->GetHwndRenderTarget()->SetTransform( m_Matrix );
	m_pD2DRenderer->GetHwndRenderTarget()->DrawBitmap( 
		m_pD2DTexture->GetD2DBitmap(), D2D1::RectF(0.f,0.f,m_ImageWidth,m_ImageHeight),
		m_Opacity );
}

