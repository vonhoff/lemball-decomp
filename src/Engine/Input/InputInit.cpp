#include "CMasterInput.h"
#include "Engine/Queues/CTimedQueue.h"
#include "Platform/Windows/Input/CTranslator.h"
#include "Engine/Startup/VsInit.h"
#include "Platform/Windows/CPlatformServices.h"

#include <new.h>
#include <stddef.h>

namespace
{
enum {
	MASTER_INPUT_QUEUE_CAPACITY = 10,
	INPUT_TRANSLATOR_QUEUE_PRIORITY = -0x32,
};
}

// GLOBAL: LEMBALL 0x004a0fb4
char g_szMasterInputQueue[20] = "Master Input Queue.";

// FUNCTION: LEMBALL 0x00459130
bool _INP_Init()
{
	void* storage;

	storage = operator new(sizeof(CTimedQueue));
	if (storage == NULL) {
		g_pMasterInputQueue = NULL;
	}
	else {
		g_pMasterInputQueue = new (storage) CTimedQueue(MASTER_INPUT_QUEUE_CAPACITY, g_szMasterInputQueue);
	}

	storage = operator new(sizeof(CMasterInput));
	if (storage == NULL) {
		g_pMasterInput = NULL;
	}
	else {
		g_pMasterInput = new (storage) CMasterInput(g_pMasterInputQueue);
	}

	storage = operator new(sizeof(CTranslator));
	if (storage == NULL) {
		g_pInputTranslator = NULL;
	}
	else {
		g_pInputTranslator = new (storage) CTranslator();
	}

	g_pMasterInputQueue->Attach(g_pInputTranslator, INPUT_TRANSLATOR_QUEUE_PRIORITY);
	return InitInput();
}

// FUNCTION: LEMBALL 0x004591f0
bool _INP_Quit()
{
	int result;

	result = QuitInput();
	g_pMasterInputQueue->Detach(g_pInputTranslator, INPUT_TRANSLATOR_QUEUE_PRIORITY);
	delete g_pInputTranslator;
	delete g_pMasterInput;
	delete g_pMasterInputQueue;
	return result;
}
