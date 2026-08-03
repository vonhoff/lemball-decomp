#include "AI/CLaserManager.h"

#include "AI/CLaser.h"
#include "Platform/Windows/Mixed/Engine/CORE/COMMON.H"
#include "Platform/Windows/Mixed/Engine/MEDIA/EFFSTRM.H"

struct LaserManagerVtableLayout;
extern LaserManagerVtableLayout g_LINKSCF_LasrChunkManagerVtable;
extern void* g_pActiveNetworkRuntimeWindow;
extern int g_cbEffTransportMaxPacketBytes;

typedef char LaserManagerSizeMustMatchLayout[sizeof(CLaserManager) == 0x40 ? 1 : -1];
typedef char LaserSizeMustMatchArrayStride[sizeof(CLaser) == 0x148 ? 1 : -1];

typedef void(LEMBALL_FASTCALL* LaserRestartProc)(void* pObject);

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
	*(void**) this = &g_LINKSCF_LasrChunkManagerVtable;
	m_pAI3C = pAI;
	m_cCapacity30 = nCapacity;
	m_pObjects38 = 0;
}

// FUNCTION: LEMBALL 0x00429380
void CLaserManager::Restart(void)
{
	char* pManagerBytes;
	void* pChunkObject;
	int i;
	int nOffset;

	pManagerBytes = (char*) this;
	if (*(char**) (pManagerBytes + 0x38) != 0) {
		i = 0;
		if (*(int*) (pManagerBytes + 0x30) > 0) {
			nOffset = 0;
			do {
				pChunkObject = *(char**) (pManagerBytes + 0x38) + nOffset;
				++i;
				nOffset += 0x148;
				((LaserRestartProc) (*(void***) pChunkObject)[65])(pChunkObject);
			} while (*(int*) (pManagerBytes + 0x30) > i);
		}
	}
}

// FUNCTION: LEMBALL 0x004293b0
void CLaserManager::Initialise(int nCapacity)
{
	char* pObject;
	int i;
	int cbOffset;

	m_cObjects34 = 0;
	if (nCapacity == 0) {
		m_pObjects38 = 0;
		return;
	}
	if (m_pObjects38 == 0) {
		m_pObjects38 = new CLaser[m_cCapacity30];
		cbOffset = 0;
		i = 0;
		if (m_cCapacity30 > 0) {
			do {
				*(CLaserManager**) ((char*) m_pObjects38 + cbOffset + 0x60) = this;
				pObject = (char*) m_pObjects38 + cbOffset;
				cbOffset += 0x148;
				++i;
				((LaserRestartProc) (*(void***) pObject)[65])(pObject);
			} while (i < m_cCapacity30);
		}
	}
}
