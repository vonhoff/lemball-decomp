#include "CPadToButton.h"

#include "../../Visos/Foundation/CBaseQueue.h"
#include "../../Visos/Graphics/CPVButton.h"
#include "PadToButtonEntry.h"
#include "Visos/Foundation/CVSPoint.h"
#include "Visos/Foundation/Message.h"

#include <stddef.h>

// FUNCTION: LEMBALL 0x0043a250
CPadToButton::CPadToButton(int p_entryCapacity)
{
	m_entries = new PadToButtonEntry[p_entryCapacity];
	m_entryCapacity = p_entryCapacity;
	m_entryCount = 0;
	for (int i = 0; i < p_entryCapacity; i++) {
		m_entries[i].m_button = NULL;
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
	unsigned short type = p_message->m_type;
	switch ((int) type) {
	case 3:
	case 4: {
		int count = m_entryCount;
		if (count > 0) {
			PadToButtonEntry* entries = m_entries;
			unsigned int messageCode = (unsigned int) p_message->m_code;
			unsigned int* padCode = &entries->m_padCode;
			while (*padCode != messageCode) {
				padCode += 2;
				index++;
				if (index >= count) {
					return result;
				}
			}
			CPVButton* button = entries[index].m_button;
			if (type == 4) {
				CVSPoint point(0, 0);
				button->OnButtonDown(point, 0);
			}
			else {
				CVSPoint point(0, 0);
				button->OnButtonUp(point, 0);
			}
			result = 1;
		}
	} break;
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
