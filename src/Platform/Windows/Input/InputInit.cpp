#include "CTranslator.h"
#include "Engine/Input/CMasterInput.h"
#include "Engine/Queues/CBaseQueue.h"
#include "Engine/Queues/CTimedQueue.h"
#include "Engine/Startup/VsInit.h"
#include "Platform/Windows/Input/InputTranslation.h"

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
	g_pMasterInputQueue = new CTimedQueue(MASTER_INPUT_QUEUE_CAPACITY, g_szMasterInputQueue);
	g_pMasterInput = new CMasterInput(g_pMasterInputQueue);
	g_pInputTranslator = new CTranslator();

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
