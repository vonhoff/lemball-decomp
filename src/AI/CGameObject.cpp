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
	*(int*) (pObject + 0x40) = (int) 0xaa55aa55;
	*(int*) (pObject + 0x44) = (int) 0xaa55aa55;
	*(int*) (pObject + 0x48) = (int) 0xaa55aa55;
	*(int*) (pObject + 0x4c) = (int) 0xaa55aa55;
	*(int*) (pObject + 0x50) = (int) 0xaa55aa55;
	*(int*) (pObject + 0x54) = (int) 0xaa55aa55;
	*(int*) (pObject + 0x78) = (int) 0xaa55aa55;
	*(int*) (pObject + 0x7c) = (int) 0xaa55aa55;
	*(int*) (pObject + 0x80) = (int) 0xaa55aa55;
	*(int*) (pObject + 0x84) = (int) 0xaa55aa55;
	*(int*) (pObject + 0x9c) = (int) 0xaa55aa55;
	*(int*) (pObject + 0xa0) = (int) 0xaa55aa55;
	*(int*) (pObject + 0xa4) = (int) 0xaa55aa55;
	*(int*) (pObject + 0xa8) = (int) 0xaa55aa55;
	*(int*) (pObject + 0xac) = (int) 0xaa55aa55;
	*(int*) (pObject + 0xb0) = (int) 0xaa55aa55;
	*(int*) (pObject + 0xd8) = (int) 0xaa55aa55;
	*(int*) (pObject + 0xdc) = (int) 0xaa55aa55;
	*(int*) (pObject + 0xe0) = (int) 0xaa55aa55;
	*(int*) (pObject + 0xe4) = (int) 0xaa55aa55;
	*(int*) (pObject + 0xe8) = (int) 0xaa55aa55;
	*(int*) (pObject + 0xec) = (int) 0xaa55aa55;
	*(int*) (pObject + 0xf4) = (int) 0xaa55aa55;
	*(int*) (pObject + 0xf8) = (int) 0xaa55aa55;
	*(int*) (pObject + 0xfc) = (int) 0xaa55aa55;
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
