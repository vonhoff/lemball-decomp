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
// STUB: LEMBALL 0x00453260
unsigned int EntryHandler::OnButtonDown(const VsPoint& p_point, int p_flags)
{
	return 0;
}

// 68K 0x1011c3ca __ct__13CEntryHandlerFv
// STUB: LEMBALL 0x00455da0
EntryHandler::EntryHandler()
{
}

// 68K 0x1011c456 OnEnter__13CEntryHandlerFv
// STUB: LEMBALL 0x00455dc0
void EntryHandler::OnEnter()
{
}

// 68K 0x1011c486 OnExit__13CEntryHandlerFv
// STUB: LEMBALL 0x00455dd0
void EntryHandler::OnExit()
{
}

// 68K 0x1011c36e __dt__13CEntryHandlerFv
// SYNTHETIC: LEMBALL 0x00455e10
// EntryHandler::`scalar deleting destructor'
EntryHandler::~EntryHandler()
{
}
