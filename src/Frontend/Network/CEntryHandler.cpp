#include "CEntryHandler.h"

#include "Visos/Queues/Message.h"

// FUNCTION: LEMBALL 0x00453250
void CEntryHandler::Reset()
{
	m_activationState = 0;
	m_pressed = 0;
	m_hoverState = 0;
}

// FUNCTION: LEMBALL 0x00453260
void CEntryHandler::OnButtonDown(const CVSPoint& p_point, int p_flags)
{
	if (p_flags == MOUSE_BUTTON_INDEX_LEFT || p_flags == MOUSE_BUTTON_INDEX_LEFT_DOUBLE_CLICK) {
		m_pressed = 1;
	}
}

// FUNCTION: LEMBALL 0x00455da0
CEntryHandler::CEntryHandler()
{
	Reset();
}

// FUNCTION: LEMBALL 0x00455dc0
void CEntryHandler::OnEnter()
{
	m_hoverState = 1;
}

// FUNCTION: LEMBALL 0x00455dd0
void CEntryHandler::OnExit()
{
	m_hoverState = 0;
}
