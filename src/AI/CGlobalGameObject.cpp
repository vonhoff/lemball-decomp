#include "AI/CGlobalGameObject.h"

// FUNCTION: LEMBALL 0x00416d90
int CGlobalGameObject::UsableState(void)
{
	int nState = m_nUsableState134;
	if (nState != 0) {
		m_nUsableState134 = 0;
	}
	return nState;
}

// FUNCTION: LEMBALL 0x00416e70
void CGlobalGameObject::CancelRequest(void)
{
	if (m_fResetPending130 != 0) {
		m_nPendingState114 = 0;
		m_fResetPending130 = 0;
		m_nRuntimeState8C = 0;
	}
}
