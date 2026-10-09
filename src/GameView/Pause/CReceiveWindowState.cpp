#include "CReceiveWindowState.h"

// FUNCTION: LEMBALL 0x00439430
void CReceiveWindowState::SetOptionSelection(eOptionSelections p_selection)
{
	m_optionSelection = p_selection;
}

// FUNCTION: LEMBALL 0x00439440
bool CReceiveWindowState::GetPauser()
{
	return false;
}
