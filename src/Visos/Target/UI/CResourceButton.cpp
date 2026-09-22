#include "CResourceButton.h"

// FUNCTION: LEMBALL 0x00468ec0
CResourceButton::~CResourceButton()
{
	delete static_cast<CPVButton*>(m_resourceInterface);
}

// FUNCTION: LEMBALL 0x00468f80
void CResourceButton::OnPaint(const CVsRect& p_rect)
{
	CFramedButton::OnPaint(p_rect);
}

// FUNCTION: LEMBALL 0x00469990
void CResourceButton::OnDestroy()
{
	static_cast<CPVButton*>(m_resourceInterface)->OnDestroy();
}
