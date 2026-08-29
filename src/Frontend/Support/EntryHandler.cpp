#include "EntryHandler.h"

// 68K 0x108063d8 Reset__13CEntryHandlerFv
// FUNCTION: LEMBALL 0x00453250
void EntryHandler::Reset()
{
	m_activationState = 0;
	m_pressed = 0;
	m_hoverState = 0;
}

// 68K 0x1080640c OnButtonDown__13CEntryHandlerFRC8CVSPoint12BUTTON_FLAGS
// FUNCTION: LEMBALL 0x00453260
void EntryHandler::OnButtonDown(const VsPoint& p_point, int p_flags)
{
	if (p_flags == 0 || p_flags == 3) {
		m_pressed = 1;
	}
}

// 68K 0x1011c3ca __ct__13CEntryHandlerFv
// FUNCTION: LEMBALL 0x00455da0
EntryHandler::EntryHandler()
{
	Reset();
}

// 68K 0x1011c456 OnEnter__13CEntryHandlerFv
// FUNCTION: LEMBALL 0x00455dc0
void EntryHandler::OnEnter()
{
	m_hoverState = 1;
}

// 68K 0x1011c486 OnExit__13CEntryHandlerFv
// FUNCTION: LEMBALL 0x00455dd0
void EntryHandler::OnExit()
{
	m_hoverState = 0;
}

// 68K 0x1011c36e __dt__13CEntryHandlerFv
EntryHandler::~EntryHandler()
{
}

