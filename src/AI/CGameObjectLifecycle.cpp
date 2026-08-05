#include "AI/CGameObject.h"

extern int g_nLevelFrameClockTick;

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
	*(unsigned short*) (pObjectBytes + 0xbc) = 0;
	*(int*) (pObjectBytes + 0x2c) = 0;
	*(int*) (pObjectBytes + 0xb8) = 0;
	*(int*) (pObjectBytes + 0x114) = 0;
	*(unsigned short*) (pObjectBytes + 0xb4) = 0;
	*(int*) (pObjectBytes + 0x38) = 0;
	*(unsigned short*) (pObjectBytes + 0xc4) = 0;
	*(int*) (pObjectBytes + 0x3c) = 0;
	*(int*) (pObjectBytes + 0xf0) = 0;
	*(int*) (pObjectBytes + 0xc0) = 0;
	*(int*) (pObjectBytes + 0x8c) = 0;
	*(int*) (pObjectBytes + 0x08) = 0;
	*(int*) (pObjectBytes + 0x0c) = 0;
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
	*(int*) (pObjectBytes + 0x11c) = 0;
	*(int*) (pObjectBytes + 0x74) = 0;
	*(int*) (pObjectBytes + 0xcc) = g_nLevelFrameClockTick;
	*(int*) (pObjectBytes + 0x98) = 0;
	*(int*) (pObjectBytes + 0x10) = 0;
}
