#include "PanelButton.h"

#include "../../Visos/Graphics/Cursor.h"

// 68K 0x10b0c854 __ct__12CPanelButtonFP13CPanelLemmingRC7CVSRectP7CPVGWnd
// STUB: LEMBALL 0x00442390
PanelButton::PanelButton(PanelLemming* p_arg0, const VsRect& p_arg1, PvGWnd* p_arg2) : DepressedButton(p_arg1, p_arg2)
{
}

// 68K 0x10b0cb74 __dt__12CPanelButtonFv
// STUB: LEMBALL 0x004425e0
PanelButton::~PanelButton()
{
}

// 68K 0x10b0cc70 OnInside__12CPanelButtonFRC8CVSPoint
// FUNCTION: LEMBALL 0x00442670
void PanelButton::OnInside(const VsPoint& p_point)
{
	CursorChangeType((eCursorDisplayType) 1, m_pressedInside);
}

// 68K 0x10b0ccb6 DrawButton__12CPanelButtonFv
// STUB: LEMBALL 0x00442690
void PanelButton::DrawButton()
{
}

// 68K 0x10b0d146 OnPaint__12CPanelButtonFRC7CVSRect
// STUB: LEMBALL 0x004429b0
void PanelButton::OnPaint(const VsRect& p_rect)
{
}

// 68K 0x10b0d262 OnReleased__12CPanelButtonF12BUTTON_FLAGS
// FUNCTION: LEMBALL 0x00442aa0
void PanelButton::OnReleased(int p_flags)
{
	m_pressedInside = 0;
	CursorChangeType((eCursorDisplayType) 1, 0);
}

// 68K 0x10b0d2b2 OnExternalButtonUp__12CPanelButtonFRC8CVSPoint12BUTTON_FLAGS
// FUNCTION: LEMBALL 0x00442ac0
void PanelButton::OnExternalButtonUp(const VsPoint& p_point, int p_flags)
{
	if (m_pressedInside != 0) {
		m_pressedInside = 0;
		CursorChangeType((eCursorDisplayType) 1, 0);
	}
}

// 68K 0x10b0d320 OnPressed__12CPanelButtonF12BUTTON_FLAGS
// STUB: LEMBALL 0x00442ae0
void PanelButton::OnPressed(int p_flags)
{
}

// 68K 0x1011cde6 OnEnterButton__12CPanelButtonFv
// FUNCTION: LEMBALL 0x00443930
void PanelButton::OnEnterButton()
{
}

// 68K 0x1011ce10 OnExitButton__12CPanelButtonFv
// FUNCTION: LEMBALL 0x00443940
void PanelButton::OnExitButton()
{
}
