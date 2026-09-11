#include "PadToButton.h"

#include "../../Visos/Foundation/BaseQueue.h"
#include "../../Visos/Graphics/PvButton.h"
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
// FUNCTION: LEMBALL 0x0043a2e0
int PadToButton::ProcessMsg(Message* p_message)
{
	int result = 0;
	int index = 0;
	unsigned short type = p_message->type;
	if (type >= 3 && type <= 4 && m_entryCount > 0) {
		unsigned int* padCode = &m_entries->m_padCode;
		while (*padCode != (unsigned int) p_message->code) {
			padCode += 2;
			index++;
			if (index >= m_entryCount) {
				return result;
			}
		}
		PvButton* button = m_entries[index].m_button;
		if (type == 4) {
			VsPoint point;
			button->OnButtonDown(point, 0);
		}
		else {
			VsPoint point;
			button->OnButtonUp(point, 0);
		}
		result = 1;
	}
	return result;
}

// 68K 0x10b0f9e2 __dt__12CPadToButtonFv
PadToButton::~PadToButton()
{
}
