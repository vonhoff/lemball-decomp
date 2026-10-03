#include "Visos/Animation/CStatManager.h"
#include "Visos/Foundation/CVSOStream.h"
#include "Visos/Foundation/VsInit.h"

#include <new.h>
#include <stddef.h>

// FUNCTION: LEMBALL 0x0045aa80
bool _STAT_Init()
{
	void* storage;

	storage = operator new(0x14);
	if (storage != NULL) {
		storage = new (storage) CStatManager(0x20);
	}
	else {
		storage = NULL;
	}
	g_pStatManager = (CStatManager*) storage;

	return g_pStatManager != NULL;
}

// FUNCTION: LEMBALL 0x0045aab0
bool _STAT_Quit()
{
	CStatManager* manager = g_pStatManager;
	manager->StreamOut(*g_pSysOutput);
	delete g_pStatManager;
	return true;
}
