#include "Visos/Animation/CStatManager.h"
#include "Visos/Foundation/CVSOStream.h"
#include "Visos/Foundation/VsInit.h"

#include <new.h>
#include <stddef.h>

enum {
	STAT_MANAGER_INITIAL_CAPACITY = 32
};

// FUNCTION: LEMBALL 0x0045aa80
bool _STAT_Init()
{
	void* storage;

	storage = operator new(sizeof(CStatManager));
	if (storage != NULL) {
		storage = new (storage) CStatManager(STAT_MANAGER_INITIAL_CAPACITY);
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
