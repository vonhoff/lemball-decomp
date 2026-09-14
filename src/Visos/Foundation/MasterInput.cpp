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

// 68K 0x1020137c __dt__12CMasterInputFv
// FUNCTION: LEMBALL 0x00472090
MasterInput::~MasterInput()
{
	void* item = m_firstItem;
	for (unsigned int i = 0; i < m_itemCount; i++) {
		// The original list link is at +0x04; the remaining item layout is unknown.
		void* next = ((void**) item)[1];
		operator delete(item);
		item = next;
	}
}

// 68K 0x102013e8 StreamOut__12CMasterInputFR10CVSOStream
// FUNCTION: LEMBALL 0x00472210
VsOStream& MasterInput::StreamOut(VsOStream& p_arg0)
{
	return p_arg0;
}

// GLOBAL: LEMBALL 0x004a279c
MasterInput* g_pMasterInput = 0;
