#include "AI/CGameObject.h"
#include "Visos/Generic/Memory.h"

extern int g_nLevelFrameClockTick;

// MACINTOSH: LevelChunkObjectBase::~LevelChunkObjectBase()
// FUNCTION: LEMBALL 0x00415160
void CGameObject::DestroyLevelChunkObjectBase(void)
{
	CGameObjectCommandQueue* pQueue;

	m_pVtable00 = (void*) 0x493c40;
	ReSetId();
	((int*) 0x4a6510)[m_nRegistryIndex6A] = 0;
	pQueue = m_pCommandQueue70;
	if (pQueue != 0) {
		FreeVSMemBlock(pQueue->m_pEntries);
		FreeVSMemBlock(pQueue);
	}
	m_nRegistryIndex6A = 0xffff;
}

// FUNCTION: LEMBALL 0x004150d0
void CGameObject::Restart(void)
{
	char* pObjectBytes;
	unsigned short* pPendingEntryCount;
	int nObjectType;

	m_WorldPosition9C.x = m_InitialPosition40.x;
	m_WorldPosition9C.z = m_InitialPosition40.z;
	pObjectBytes = (char*) this;
	*(int*) (pObjectBytes + 0xe4) = 0;
	m_WorldPosition9C.y = m_InitialPosition40.y;
	*(int*) (pObjectBytes + 0xe8) = 0;
	*(int*) (pObjectBytes + 0xec) = 0;
	Initialise();
	m_WorldPosition9C.z = 0;
	m_nRestartValue120 = 0xffff;
	((int*) m_abReservedD4)[0xf] = 0xffff;
	m_WorldPosition9C.y = 0;
	m_WorldPosition9C.x = 0;
	pPendingEntryCount = (unsigned short*) m_pCommandQueue70;
	if (pPendingEntryCount != 0) {
		*pPendingEntryCount = 0;
	}
	nObjectType = m_nEntityType64;
	switch (nObjectType) {
	case 1:
		*(unsigned short*) (pObjectBytes + 4) = 0x200;
		break;
	case 2:
		*(unsigned short*) (pObjectBytes + 4) = 0x100;
		break;
	}
}

// FUNCTION: LEMBALL 0x004151b0
void CGameObject::Initialise(void)
{
	char* pObjectBytes = (char*) this;
	m_nActionPhaseBC = 0;
	*(int*) (pObjectBytes + 0x2c) = 0;
	m_nStateB8 = 0;
	m_nPendingState114 = 0;
	m_nHeadingOctantB4 = 0;
	*(int*) (pObjectBytes + 0x38) = 0;
	*(unsigned short*) (pObjectBytes + 0xc4) = 0;
	*(int*) (pObjectBytes + 0x3c) = 0;
	*(int*) (pObjectBytes + 0xf0) = 0;
	*(int*) (pObjectBytes + 0xc0) = 0;
	m_nRuntimeState8C = 0;
	m_nPendingInstruction08 = 0;
	m_pReachabilityOwner0C = 0;
	*(int*) (pObjectBytes + 0x104) = 0;
	*(int*) (pObjectBytes + 0x108) = 0;
	*(int*) (pObjectBytes + 0x10c) = 0;
	*(int*) (pObjectBytes + 0x30) = 0;
	*(int*) (pObjectBytes + 0x34) = 0;
	*(int*) (pObjectBytes + 0x4c) = 0;
	*(int*) (pObjectBytes + 0x50) = 0;
	*(unsigned short*) (pObjectBytes + 0x6e) = 0;
	*(int*) (pObjectBytes + 0x54) = 0;
	*(int*) (pObjectBytes + 0x58) = 0;
	m_fOnMover11C = 0;
	m_fHasCommands74 = 0;
	m_nNextUpdateTickCC = g_nLevelFrameClockTick;
	*(int*) (pObjectBytes + 0x98) = 0;
	*(int*) (pObjectBytes + 0x10) = 0;
}
