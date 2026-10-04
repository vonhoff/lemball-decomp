#include "CDepressedButton.h"

#include "Engine/Graphics/Surfaces/CChangeList.h"
#include "Engine/Graphics/Primitives/CGDI.h"
#include "Engine/Graphics/Surfaces/CSurface.h"
#include "Engine/Queues/Message.h"

// FUNCTION: LEMBALL 0x0043a620 FOLDED
void CDepressedButton::OnReleased(int p_flags)
{
	if (m_pressed != 0 && (p_flags == MOUSE_BUTTON_INDEX_LEFT || p_flags == MOUSE_BUTTON_INDEX_LEFT_DOUBLE_CLICK)) {
		m_enabled = 1;
		return;
	}
	m_enabled = 0;
}

// FUNCTION: LEMBALL 0x0043a660 FOLDED
void CDepressedButton::OnPressed(int p_flags)
{
	if (m_pressed != 0 && (p_flags == MOUSE_BUTTON_INDEX_LEFT || p_flags == MOUSE_BUTTON_INDEX_LEFT_DOUBLE_CLICK)) {
		m_enabled = 1;
		return;
	}
	m_enabled = 0;
}

// FUNCTION: LEMBALL 0x0043a6a0 FOLDED
void CDepressedButton::OnEnterButton()
{
	if (m_pressed != 0 &&
		(m_buttonState[MOUSE_BUTTON_INDEX_LEFT] != 0 || m_buttonState[MOUSE_BUTTON_INDEX_LEFT_DOUBLE_CLICK] != 0)) {
		m_enabled = 1;
		return;
	}
	m_enabled = 0;
}

// FUNCTION: LEMBALL 0x0043a6e0 FOLDED
void CDepressedButton::OnExitButton()
{
	if (m_pressed != 0 &&
		(m_buttonState[MOUSE_BUTTON_INDEX_LEFT] != 0 || m_buttonState[MOUSE_BUTTON_INDEX_LEFT_DOUBLE_CLICK] != 0)) {
		m_enabled = 1;
		return;
	}
	m_enabled = 0;
}

// FUNCTION: LEMBALL 0x00468300 FOLDED
void CDepressedButton::_DrawButton()
{
	if (m_enabled != m_state || m_gdi->m_renderTarget->HasBackBuff() != 0) {
		m_gdi->m_renderTarget->m_flag78 = 1;
		m_state = m_enabled;
	}
	CheckForceDraw();
}

// FUNCTION: LEMBALL 0x00468360 FOLDED
void CDepressedButton::OnPaint(const CVSRect& p_rect)
{
	int clipOk;
	CChangeList* changeList;

	clipOk = m_gdi->m_renderTarget->HasBackBuff();
	if ((clipOk != 0 && (m_pressed != 0 || m_enabled != m_state)) ||
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
