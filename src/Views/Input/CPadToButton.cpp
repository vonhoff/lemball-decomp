#include "CPadToButton.h"

#include "../../Visos/Foundation/CBaseQueue.h"
#include "../../Visos/Graphics/CPVButton.h"
#include "PadToButtonEntry.h"
#include "Visos/Foundation/CVsPoint.h"
#include "Visos/Foundation/Message.h"

// FUNCTION: LEMBALL 0x0043a250
CPadToButton::CPadToButton(int p_arg0)
{
	m_entries = new PadToButtonEntry[p_arg0];
	m_entryCapacity = p_arg0;
	m_entryCount = 0;
	for (int i = 0; i < p_arg0; i++) {
		m_entries[i].m_button = 0;
	}
	g_pMasterInputQueue->Attach(this, -25);
}

// FUNCTION: LEMBALL 0x0043a2b0
CPadToButton::~CPadToButton()
{
	g_pMasterInputQueue->Detach(this, -25);
	delete[] m_entries;
}

// FUNCTION: LEMBALL 0x0043a2e0
int CPadToButton::ProcessMsg(Message* p_message)
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
		CPVButton* button = m_entries[index].m_button;
		if (type == 4) {
			CVsPoint point;
			button->OnButtonDown(point, 0);
		}
		else {
			CVsPoint point;
			button->OnButtonUp(point, 0);
		}
		result = 1;
	}
	return result;
}

// FUNCTION: LEMBALL 0x0043a380
void CPadToButton::AddBinding(CPVButton* p_button, unsigned int p_padCode)
{
	m_entries[m_entryCount].m_button = p_button;
	m_entries[m_entryCount].m_padCode = p_padCode;
	m_entryCount++;
}
