#include "ReceiveWindowState.h"

// 68K 0x1011b00a SetOptionSelection__19CReceiveWindowStateF17eOptionSelections
// FUNCTION: LEMBALL 0x00439430
void ReceiveWindowState::SetOptionSelection(int p_selection)
{
	m_optionSelection = p_selection;
}

// 68K 0x1011b05c GetPauser__19CReceiveWindowStateFv
// FUNCTION: LEMBALL 0x00439440
bool ReceiveWindowState::GetPauser()
{
	return 0;
}
