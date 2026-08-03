#include "AI/CMineManager.h"

#include "Platform/Windows/Mixed/Engine/CORE/COMMON.H"
#include "Platform/Windows/Mixed/Engine/MEDIA/EFFSTRM.H"

extern MineManagerVtableLayout g_LINKSCF_MineChunkManagerVtable;
extern void* g_pActiveNetworkRuntimeWindow;
extern int g_cbEffTransportMaxPacketBytes;

// FUNCTION: LEMBALL 0x00424020
CMineManager::CMineManager(CAI* pAI, int nCapacity)
{
	*(void**) this = g_GAME_EffStreamConstructionVtable;
	*(int*) ((char*) this + 4) = 0x0d;
	((GameEffStream*) this)->ResetStateFields();
	*(int*) ((char*) this + 0x2c) = 2;
	*(void**) this = g_LEVEL_EffChunkStreamBaseVtable;
	if (g_pActiveNetworkRuntimeWindow != 0) {
		*(int*) ((char*) this + 0x24) = 1;
		*(int*) ((char*) this + 0x18) += g_cbEffTransportMaxPacketBytes;
	}
	*(void**) this = &g_LINKSCF_MineChunkManagerVtable;
	m_pAI30 = pAI;
	m_cCapacity40 = nCapacity;
	m_pObjects34 = 0;
	m_pPositions38 = 0;
}

typedef void(LEMBALL_FASTCALL* MineRestartProc)(void* pObject);

// FUNCTION: LEMBALL 0x00424080
void CMineManager::Restart(void)
{
	int cbOffset;
	int i;
	char* pObject;

	cbOffset = 0;
	if (m_pObjects34 != 0) {
		i = 0;
		if (m_cCapacity40 > 0) {
			do {
				pObject = (char*) m_pObjects34 + cbOffset;
				++i;
				cbOffset += 0x150;
				((MineRestartProc) (*(void***) pObject)[65])(pObject);
			} while (i < m_cCapacity40);
		}
	}
}
