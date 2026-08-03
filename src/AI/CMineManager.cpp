#include "AI/CMineManager.h"

#include "AI/CMine.h"
#include "Platform/Windows/Mixed/Engine/CORE/COMMON.H"
#include "Platform/Windows/Mixed/Engine/MEDIA/EFFSTRM.H"
#include "Visos/Generic/Memory.h"

struct VsNetEffStreamCommon {
	virtual ~VsNetEffStreamCommon(void);
};

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

struct MineManagerDeletableChild {
	virtual void Delete(unsigned char fDelete);
};

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

// FUNCTION: LEMBALL 0x004240b0
void CMineManager::Initialise(int nCapacity)
{
	char* pObject;
	int cbOffset;
	int i;

	m_cCapacity40 = nCapacity;
	m_cObjects3C = 0;
	if (nCapacity == 0) {
		m_pObjects34 = 0;
		return;
	}
	if (m_pObjects34 == 0) {
		m_pObjects34 = new CMine[nCapacity];
		cbOffset = 0;
		i = 0;
		if (m_cCapacity40 > 0) {
			do {
				pObject = (char*) m_pObjects34 + cbOffset;
				*(int*) (pObject + 0x14c) = i;
				++i;
				*(CMineManager**) (pObject + 0x60) = this;
				cbOffset += 0x150;
				((MineRestartProc) (*(void***) pObject)[65])(pObject);
			} while (i < m_cCapacity40);
		}
		m_pPositions38 = new MinePosition[m_cCapacity40];
	}
}

// FUNCTION: LEMBALL 0x00424170
CMineManager::~CMineManager(void)
{
	*(void**) this = &g_LINKSCF_MineChunkManagerVtable;
	if (m_pObjects34 != 0) {
		((MineManagerDeletableChild*) m_pObjects34)->Delete(3);
		FreeVSMemBlock(m_pPositions38);
	}
	((VsNetEffStreamCommon*) this)->VsNetEffStreamCommon::~VsNetEffStreamCommon();
}

// FUNCTION: LEMBALL 0x00424560
void CMineManager::Triggered(CMine* pMine)
{
	Trigger(*(int*) ((char*) pMine + 0x14c), *(int*) ((char*) pMine + 0x148));
}

// FUNCTION: LEMBALL 0x00424580
void CMineManager::Trigger(int nIndex, int nDelay)
{
	MinePosition* pOrigin;
	MinePosition* pPosition;
	CMine* pMine;
	int cbObject;
	int cbPosition;
	int dx;
	int dy;
	int dz;
	int i;

	pOrigin = (MinePosition*) ((char*) m_pPositions38 + nIndex * 6);
	cbObject = 0;
	i = 0;
	if (m_cObjects3C > 0) {
		cbPosition = 0;
		do {
			if (nIndex != i) {
				pMine = (CMine*) ((char*) m_pObjects34 + cbObject);
				if (*(int*) ((char*) pMine + 0xb8) == 0x18) {
					pPosition = (MinePosition*) ((char*) m_pPositions38 + cbPosition);
					dz = pPosition->z - pOrigin->z;
					dy = pPosition->y - pOrigin->y;
					dx = pPosition->x - pOrigin->x;
					if (dz * dz + dy * dy + dx * dx < 0x801) {
						pMine->Trigger(nDelay + 6);
					}
				}
			}
			cbObject += 0x150;
			++i;
			cbPosition += 6;
		} while (i < m_cObjects3C);
	}
}
