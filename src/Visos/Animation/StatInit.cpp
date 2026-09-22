#include "Visos/Animation/CStatManager.h"
#include "Visos/Foundation/CVsOStream.h"
#include "Visos/Foundation/VsInit.h"

#include <new.h>

// FUNCTION: LEMBALL 0x0045aa80
bool InternalStatInit()
{
	void* storage;

	storage = operator new(0x14);
	if (storage != 0) {
		storage = new (storage) CStatManager(0x20);
	}
	else {
		storage = 0;
	}
	g_pStatManager = (CStatManager*) storage;

	return g_pStatManager != 0;
}

// FUNCTION: LEMBALL 0x0045aab0
bool InternalStatQuit()
{
	g_pStatManager->StreamOut(*g_pSysOutput);
	delete g_pStatManager;
	return 1;
}
