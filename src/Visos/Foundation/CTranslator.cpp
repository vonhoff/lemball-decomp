#include "CTranslator.h"

#include "../Target/System/CPlatformServices.h"
#include "CBaseQueue.h"

#define WIN32_LEAN_AND_MEAN
#include "Visos/Foundation/Message.h"
#include "Visos/Target/Input/InputTranslationEntry.h"

#include <windows.h>

// FUNCTION: LEMBALL 0x00472a60
int CTranslator::ProcessMsg(Message* p_message)
{
	Message translated;
	InputTranslationEntry* entry;
	int index;
	short keyState;
	Message* message = p_message;

	translated.m_time = message->m_time;
	unsigned short type = message->m_type;
	switch ((int) type) {
	case MESSAGE_RAW_KEY_UP:
	case MESSAGE_RAW_KEY_DOWN:
		index = 0;
		entry = g_dwInputTranslationPairs;
		do {
			if (entry->m_platformCode == (unsigned int) message->m_code) {
				translated.m_type = MESSAGE_KEY_UP;
				if (type != MESSAGE_RAW_KEY_UP) {
					translated.m_type = MESSAGE_KEY_DOWN;
				}
				translated.m_code = (int) g_dwInputTranslationPairs[index].m_inputCode;
				if (translated.m_code == INPUT_KEY_SHIFT) {
					keyState = GetKeyState(VK_LSHIFT);
					if (keyState < 0) {
						translated.m_code = INPUT_KEY_LEFT_SHIFT;
					}
				}
				g_pMasterInputQueue->Post(translated);
				return 1;
			}
			entry = entry + 1;
			index = index + 1;
		} while (entry < g_dwInputTranslationPairs + 61);
	}
	m_processedCount = m_processedCount + 1;
	return 0;
}

// GLOBAL: LEMBALL 0x004a9364
CTranslator* g_pInputTranslator;
