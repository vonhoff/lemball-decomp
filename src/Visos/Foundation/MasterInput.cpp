#include "MasterInput.h"

// 68K 0x10201326 __ct__12CMasterInputFP10CBaseQueue
// FUNCTION: LEMBALL 0x00472070
MasterInput::MasterInput(BaseQueue* p_arg0)
{
	m_itemCount = 0;
	m_firstItem = 0;
	m_queue = p_arg0;
	m_state = 0;
}

// 68K 0x102013e8 StreamOut__12CMasterInputFR10CVSOStream
// FUNCTION: LEMBALL 0x00472210
VsOStream& MasterInput::StreamOut(VsOStream& p_arg0)
{
	return p_arg0;
}

// 68K 0x1020137c __dt__12CMasterInputFv
MasterInput::~MasterInput()
{
}

// GLOBAL: LEMBALL 0x004a279c
MasterInput* g_pMasterInput = 0;
