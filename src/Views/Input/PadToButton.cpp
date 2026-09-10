#include "PadToButton.h"

#include "../../Visos/Foundation/BaseQueue.h"
#include "PadToButtonEntry.h"

// 68K 0x10b0f952 __ct__12CPadToButtonFi
// FUNCTION: LEMBALL 0x0043a250
PadToButton::PadToButton(int p_arg0)
{
	m_entries = new PadToButtonEntry[p_arg0];
	m_entryCapacity = p_arg0;
	m_entryCount = 0;
	for (int i = 0; i < p_arg0; i++) {
		m_entries[i].m_button = 0;
	}
	g_pMasterInputQueue->Attach(this, -25);
}

// 68K 0x10b0fa5c ProcessMsg__12CPadToButtonFP10tagMESSAGE
// STUB: LEMBALL 0x0043a2e0
int PadToButton::ProcessMsg(Message* p_message)
{
	return 0;
}

// 68K 0x10b0f9e2 __dt__12CPadToButtonFv
PadToButton::~PadToButton()
{
}
