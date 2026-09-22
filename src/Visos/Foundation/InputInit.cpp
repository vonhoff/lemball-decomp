#include "Visos/Foundation/CMasterInput.h"
#include "Visos/Foundation/CTimedQueue.h"
#include "Visos/Foundation/CTranslator.h"
#include "Visos/Foundation/VsInit.h"
#include "Visos/Target/System/CPlatformServices.h"

#include <new.h>

// GLOBAL: LEMBALL 0x004a0fb4
char g_szMasterInputQueue[20] = "Master Input Queue.";

// FUNCTION: LEMBALL 0x00459130
bool InternalInpInit()
{
	void* storage;

	storage = operator new(0x58);
	if (storage == 0) {
		g_pMasterInputQueue = 0;
	}
	else {
		g_pMasterInputQueue = new (storage) CTimedQueue(10, g_szMasterInputQueue);
	}

	storage = operator new(0x18);
	if (storage == 0) {
		g_pMasterInput = 0;
	}
	else {
		g_pMasterInput = new (storage) CMasterInput(g_pMasterInputQueue);
	}

	storage = operator new(0x10);
	if (storage == 0) {
		g_pInputTranslator = 0;
	}
	else {
		g_pInputTranslator = new (storage) CTranslator();
	}

	g_pMasterInputQueue->Attach(g_pInputTranslator, -0x32);
	return InitInput();
}

// FUNCTION: LEMBALL 0x004591f0
bool InternalInpQuit()
{
	int result;

	result = QuitInput();
	g_pMasterInputQueue->Detach(g_pInputTranslator, -0x32);
	delete g_pInputTranslator;
	delete g_pMasterInput;
	delete g_pMasterInputQueue;
	return result;
}
