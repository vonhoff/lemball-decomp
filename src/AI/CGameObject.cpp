#include "Visos/Generic/Memory.h"
#include "AI/CGameObject.h"

extern void* AllocateVSMemBlock(unsigned int nBytes);
extern int g_GAME_ManagedEntityRegistryTable[1000];
extern unsigned short g_GAME_ManagedEntityRegistryCount;

// FUNCTION: LEMBALL 0x00414f30
CGameObject* CGameObject::ConstructCGameObject(int nEntityType, unsigned short nReserved68, unsigned short nCommandCapacity)
{
	char* pObject;
	CGameObjectCommandQueue* pQueue;
	unsigned int i;

	pObject = (char*) this;
	m_bounds14.m_nMaxX = -1;
	m_bounds14.m_nMaxY = -1;
	m_bounds14.m_nMinX = 0;
	m_bounds14.m_nMinY = 0;
	m_bounds14.m_nMinZ = 0;
	m_bounds14.m_nMaxZ = -1;
	m_InitialPosition40.x = (int) 0xaa55aa55;
	m_InitialPosition40.y = (int) 0xaa55aa55;
	m_InitialPosition40.z = (int) 0xaa55aa55;
	m_nJumpVelX4C = (int) 0xaa55aa55;
	m_nJumpVelY50 = (int) 0xaa55aa55;
	m_nJumpVelZ54 = (int) 0xaa55aa55;
	m_nRectMinX78 = (int) 0xaa55aa55;
	m_nRectMinY7C = (int) 0xaa55aa55;
	m_nRectMaxX80 = (int) 0xaa55aa55;
	m_nRectMaxY84 = (int) 0xaa55aa55;
	m_WorldPosition9C.x = (int) 0xaa55aa55;
	m_WorldPosition9C.y = (int) 0xaa55aa55;
	m_WorldPosition9C.z = (int) 0xaa55aa55;
	m_nDestWorldXA8 = (int) 0xaa55aa55;
	m_nDestWorldYAC = (int) 0xaa55aa55;
	m_nDestWorldZB0 = (int) 0xaa55aa55;
	m_nJumpFieldD8 = (int) 0xaa55aa55;
	m_nJumpFieldDC = (int) 0xaa55aa55;
	m_nJumpFieldE0 = (int) 0xaa55aa55;
	m_nJumpFieldE4 = (int) 0xaa55aa55;
	m_nJumpFieldE8 = (int) 0xaa55aa55;
	m_nJumpFieldEC = (int) 0xaa55aa55;
	m_nEntityType64 = nEntityType;
	*(unsigned short*) (pObject + 0x68) = nReserved68;
	m_pVtable00 = (void*) 0x493c40;
	if (nCommandCapacity != 0) {
		pQueue = (CGameObjectCommandQueue*) AllocateVSMemBlock(sizeof(CGameObjectCommandQueue));
		if (pQueue != 0) {
			pQueue->m_cEntries = 0;
			pQueue->m_cCapacity = nCommandCapacity;
			pQueue->m_pEntries = (CGameObjectCommand*)
				AllocateVSMemBlock((unsigned int) nCommandCapacity * sizeof(CGameObjectCommand));
			if (pQueue->m_pEntries != 0) {
				for (i = nCommandCapacity - 1; (int) i >= 0; --i) {
					((void(__fastcall*) (CGameObjectCommand*)) 0x401c08)(&pQueue->m_pEntries[i]);
				}
			}
			m_pCommandQueue70 = pQueue;
		} else {
			m_pCommandQueue70 = 0;
		}
	} else {
		m_pCommandQueue70 = 0;
	}
	m_nSlotId6C = 0xffff;
	if (g_GAME_ManagedEntityRegistryCount != 0) {
		unsigned int iIndex = 0;
		unsigned short nCount = g_GAME_ManagedEntityRegistryCount;
		while (iIndex < nCount) {
			if (g_GAME_ManagedEntityRegistryTable[iIndex] == 0) {
				m_nRegistryIndex6A = (unsigned short) iIndex;
				++g_GAME_ManagedEntityRegistryCount;
				break;
			}
			++iIndex;
		}
	} else {
		m_nRegistryIndex6A = (unsigned short) g_GAME_ManagedEntityRegistryCount;
		++g_GAME_ManagedEntityRegistryCount;
	}
	g_GAME_ManagedEntityRegistryTable[m_nRegistryIndex6A] = (int) this;
	(void) pObject;
	return this;
}
