#include "StatInit.h"

#include "CStatManager.h"
#include "Engine/Streams/CVSOStream.h"

#include <stddef.h>

enum {
	STAT_MANAGER_INITIAL_CAPACITY = 32
};

// FUNCTION: LEMBALL 0x0045aa80
bool _STAT_Init()
{
	g_pStatManager = new CStatManager(STAT_MANAGER_INITIAL_CAPACITY);

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
