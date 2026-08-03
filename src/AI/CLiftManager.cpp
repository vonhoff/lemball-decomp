#include "AI/CLiftManager.h"

#include "AI/CLift.h"
#include "Platform/Windows/Mixed/Engine/CORE/COMMON.H"
#include "Platform/Windows/Mixed/Engine/MEDIA/EFFSTRM.H"

extern LiftManagerVtableLayout g_LINKSCF_LiftChunkManagerVtable;
extern void* g_pActiveNetworkRuntimeWindow;
extern int g_cbEffTransportMaxPacketBytes;
extern unsigned short g_nNextLiftObjectId;

typedef void(LEMBALL_FASTCALL* LiftRestartProc)(void* pObject);

// FUNCTION: LEMBALL 0x00425680
CLiftManager::CLiftManager(CAI* pAI, int nCapacity)
{
	*(void**) this = g_GAME_EffStreamConstructionVtable;
	*(int*) ((char*) this + 4) = 0x12;
	((GameEffStream*) this)->ResetStateFields();
	*(int*) ((char*) this + 0x2c) = 7;
	*(void**) this = g_LEVEL_EffChunkStreamBaseVtable;
	if (g_pActiveNetworkRuntimeWindow != 0) {
		*(int*) ((char*) this + 0x24) = 1;
		*(int*) ((char*) this + 0x18) += g_cbEffTransportMaxPacketBytes;
	}
	*(void**) this = &g_LINKSCF_LiftChunkManagerVtable;
	m_pAI30 = pAI;
	m_cCapacity38 = nCapacity;
	m_pObjects3C = 0;
}

// FUNCTION: LEMBALL 0x004256e0
void CLiftManager::Restart(void)
{
	char* pObject;
	int cbOffset;
	int i;

	g_nNextLiftObjectId = 0;
	cbOffset = 0;
	if (m_pObjects3C != 0) {
		i = 0;
		if (m_cCapacity38 > 0) {
			do {
				pObject = (char*) m_pObjects3C + cbOffset;
				++i;
				cbOffset += 0x190;
				((LiftRestartProc) (*(void***) pObject)[65])(pObject);
			} while (i < m_cCapacity38);
		}
	}
}

// FUNCTION: LEMBALL 0x00425720
void CLiftManager::Initialise(int nCapacity)
{
	char* pObject;
	int cbOffset;
	int i;

	m_cObjects34 = 0;
	if (nCapacity == 0) {
		m_pObjects3C = 0;
		return;
	}
	m_cCapacity38 = nCapacity;
	if (m_pObjects3C == 0) {
		m_pObjects3C = new CLift[nCapacity];
		cbOffset = 0;
		i = 0;
		if (m_cCapacity38 > 0) {
			do {
				*(CLiftManager**) ((char*) m_pObjects3C + cbOffset + 0x60) = this;
				pObject = (char*) m_pObjects3C + cbOffset;
				cbOffset += 0x190;
				++i;
				((LiftRestartProc) (*(void***) pObject)[65])(pObject);
			} while (i < m_cCapacity38);
		}
	}
}
