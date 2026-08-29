#include "Translator.h"

#include "../Target/TargetPlatformServices.h"
#include "BaseQueue.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// 68K 0x10100890 __dt__11CTranslatorFv
Translator::~Translator()
{
}

// 68K 0x10100692 ProcessMsg__11CTranslatorFP10tagMESSAGE
// FUNCTION: LEMBALL 0x00472a60
int Translator::ProcessMsg(Message* p_message)
{
	Message translated;
	int index;
	TargetInputTranslationEntry* entry;
	short keyState;

	translated.time = p_message->time;
	if (p_message->type != 0 && p_message->type < 3) {
		index = 0;
		entry = g_dwInputTranslationPairs;
		while (index < 61) {
			if (entry->m_platformCode == (unsigned int) p_message->code) {
				translated.type = 3;
				if (p_message->type != 1) {
					translated.type = 4;
				}
				translated.code = (int) g_dwInputTranslationPairs[index].m_inputCode;
				if (translated.code == 0x49) {
					keyState = GetKeyState(0xa0);
					if (keyState < 0) {
						translated.code = 0x4a;
					}
				}
				translated.payload = 0;
				translated.source = 0;
				g_pMasterInputQueue->Post(translated);
				return 1;
			}
			entry = entry + 1;
			index = index + 1;
		}
	}
	m_processedCount = m_processedCount + 1;
	return 0;
}

// GLOBAL: LEMBALL 0x004a9364
Translator* g_pInputTranslator;
