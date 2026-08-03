#include "AI/CLaserManager.h"

#include "Platform/Windows/Mixed/Engine/CORE/COMMON.H"
#include "Platform/Windows/Mixed/Engine/MEDIA/EFFSTRM.H"

extern void* g_LINKSCF_LasrChunkManagerVtable[10];
extern void* g_pActiveNetworkRuntimeWindow;
extern int g_cbEffTransportMaxPacketBytes;

typedef char LaserManagerSizeMustMatchLayout[sizeof(CLaserManager) == 0x40 ? 1 : -1];

// FUNCTION: LEMBALL 0x00429320
CLaserManager::CLaserManager(CAI* pAI, int nCapacity)
{
	*(void**) this = g_GAME_EffStreamConstructionVtable;
	*(int*) ((char*) this + 4) = 0x15;
	((GameEffStream*) this)->ResetStateFields();
	*(int*) ((char*) this + 0x2c) = 10;
	*(void**) this = g_LEVEL_EffChunkStreamBaseVtable;
	if (g_pActiveNetworkRuntimeWindow != 0) {
		*(int*) ((char*) this + 0x24) = 1;
		*(int*) ((char*) this + 0x18) += g_cbEffTransportMaxPacketBytes;
	}
	*(void**) this = g_LINKSCF_LasrChunkManagerVtable;
	m_pAI3C = pAI;
	m_cCapacity30 = nCapacity;
	m_pObjects38 = 0;
}
