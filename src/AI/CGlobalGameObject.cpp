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
