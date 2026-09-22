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
	unsigned short type = p_message->type;

	translated.time = p_message->time;
	switch ((int) type) {
	case 1:
	case 2:
		index = 0;
		entry = g_dwInputTranslationPairs;
		do {
			if (entry->m_platformCode == (unsigned int) p_message->code) {
				translated.type = 3;
				if (type != 1) {
					translated.type = 4;
				}
				translated.code = (int) g_dwInputTranslationPairs[index].m_inputCode;
				if (translated.code == 0x49) {
					keyState = GetKeyState(0xa0);
					if (keyState < 0) {
						translated.code = 0x4a;
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
