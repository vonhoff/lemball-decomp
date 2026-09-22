#include "TargetResourceButton.h"

// FUNCTION: LEMBALL 0x00468ec0
TargetResourceButton::~TargetResourceButton()
{
	delete static_cast<CPvButton*>(m_resourceInterface);
}

// FUNCTION: LEMBALL 0x00468f80
void TargetResourceButton::OnPaint(const CVsRect& p_rect)
{
	TargetFramedButton::OnPaint(p_rect);
}

// FUNCTION: LEMBALL 0x00469990
void TargetResourceButton::OnDestroy()
{
	static_cast<CPvButton*>(m_resourceInterface)->OnDestroy();
}
