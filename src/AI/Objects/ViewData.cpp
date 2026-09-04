#include "ViewData.h"

// 68K 0x1011cb4a __ct__9CViewDataFv
// FUNCTION: LEMBALL 0x00439340
ViewData::ViewData()
{
	m_objectId = 0xffff;
	m_auxiliaryPosition.m_xFixed = 0xaa55aa55;
	m_auxiliaryPosition.m_yFixed = 0xaa55aa55;
	m_auxiliaryPosition.m_zFixed = 0xaa55aa55;
	m_transientFlags = 0;
	m_unk0x30 = 0;
	m_action = 0;
	m_stateTimer = 0;
}
