#include "DepressedButton.h"

#include "../Foundation/ChangeList.h"
#include "Gdi.h"
#include "VsGdi.h"

// 68K 0x101175de OnReleased__16CDepressedButtonF12BUTTON_FLAGS
// STUB: LEMBALL 0x0043a620
void DepressedButton::OnReleased(int p_flags)
{
}

// 68K 0x1011763e OnPressed__16CDepressedButtonF12BUTTON_FLAGS
// STUB: LEMBALL 0x0043a660
void DepressedButton::OnPressed(int p_flags)
{
}

// 68K 0x1011769c OnEnterButton__16CDepressedButtonFv
// STUB: LEMBALL 0x0043a6a0
void DepressedButton::OnEnterButton()
{
}

// 68K 0x101176f0 OnExitButton__16CDepressedButtonFv
// STUB: LEMBALL 0x0043a6e0
void DepressedButton::OnExitButton()
{
}

// 68K 0x10210394 _DrawButton__16CDepressedButtonFv
// STUB: LEMBALL 0x00468300
void DepressedButton::_DrawButton()
{
	if (m_depressed == m_lastDrawnDepressed) {
		if (m_gdi->m_renderTarget->PvSurface::HasBackBuff() == 0) {
			CheckForceDraw();
			return;
		}
	}
	m_gdi->m_renderTarget->m_flag78 = 1;
	m_lastDrawnDepressed = m_depressed;
	CheckForceDraw();
}

// 68K 0x10210416 OnPaint__16CDepressedButtonFRC7CVSRect
// STUB: LEMBALL 0x00468360
void DepressedButton::OnPaint(const VsRect& p_rect)
{
	int clipOk;
	ChangeList* changeList;

	clipOk = m_gdi->m_renderTarget->PvSurface::HasBackBuff();
	if ((clipOk != 0 && (m_pressed != 0 || m_depressed != m_lastDrawnDepressed)) ||
		(m_gdi->m_primitiveCount == 0 &&
		 (m_autoDraw != 0 || m_forceDrawCount != 0 || m_pressed != m_lastDrawnPressed))) {
		if (GetSizeStatus() != 0) {
			_DrawButton();
			DrawButton();
		}
		changeList = m_gdi->m_renderTarget->GetChangeList();
		m_gdi->AddToList(m_primitive);
		changeList->Reset();
		m_drawCompleted = 1;
	}
}
