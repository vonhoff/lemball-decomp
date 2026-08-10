#include "AI/CMoverManager.h"

#include "AI/CGameObject.h"
#include "AI/CMover.h"
#include "Visos/Generic/Memory.h"

// FUNCTION: LEMBALL 0x0042F350
void CMoverManager::Remove(CMover* pObject)
{
	int i;
	int j;
	CMover* pSource;
	CMover* pTarget;
	int* pSourceWords;
	int* pTargetWords;

	i = 0;
	if (m_cObjects34 > 0) {
		while (pObject != m_pObjects38 + i) {
			i++;
			if (i >= m_cObjects34) {
				return;
			}
		}
		((CGameObject*) (m_pObjects38 + i))->SetId(0xffff);
		i++;
		while (i < m_cObjects34) {
			pSource = m_pObjects38 + i;
			pTarget = pSource - 1;
			pTarget->CopyPrefix(*pSource);
			pSourceWords = (int*) ((char*) pSource + 0x124);
			pTargetWords = (int*) ((char*) pTarget + 0x124);
			j = 0;
			while (j < 21) {
				pTargetWords[j] = pSourceWords[j];
				j++;
			}
			pSourceWords = (int*) ((char*) pSource + 0x178);
			pTargetWords = (int*) ((char*) pTarget + 0x178);
			j = 0;
			while (j < 10) {
				pTargetWords[j] = pSourceWords[j];
				j++;
			}
			i++;
		}
		m_cObjects34--;
	}
}

// FUNCTION: LEMBALL 0x0042f220
void CMoverManager::Initialise(int nCapacity)
{
	m_nCapacity30 = nCapacity;
	m_cObjects34 = 0;
	if (nCapacity == 0) {
		m_pObjects38 = 0;
		return;
	}

	if (m_pObjects38 == 0) {
		int* pAllocation = (int*) AllocateVSMemBlock(nCapacity * sizeof(CMover) + sizeof(int));
		if (pAllocation != 0) {
			CMover* pObjects = (CMover*) (pAllocation + 1);
			CMover* pObject = pObjects;
			*pAllocation = nCapacity;
			while (--nCapacity >= 0) {
				((void(__fastcall*)(void*)) 0x40256d)(pObject);
				++pObject;
			}
			m_pObjects38 = pObjects;
		}
		else {
			m_pObjects38 = 0;
		}

		int nObjectOffset = 0;
		int i = 0;
		if (m_nCapacity30 > 0) {
			do {
				CMover* pObject = (CMover*) ((char*) m_pObjects38 + nObjectOffset);
				++i;
				nObjectOffset += sizeof(CMover);
				((void(__fastcall*)(void*)) (*(void***) pObject)[0x104 / sizeof(void*)])(pObject);
				*(CMoverManager**) ((char*) m_pObjects38 + nObjectOffset - 0x140) = this;
			} while (i < m_nCapacity30);
		}
	}
}

// FUNCTION: LEMBALL 0x0042f5e0
void CMoverManager::Add(unsigned short nSlotId, unsigned short nFlags, void* pParam3, int nParam4, void* pParam5)
{
	if (m_nCapacity30 > 0) {
		((void(__fastcall*)(void*, unsigned short, unsigned short, void*, int, void*)) 0x40128a)((void*) (m_pObjects38 + m_cObjects34), nSlotId, nFlags, pParam3, nParam4, pParam5);
		m_cObjects34++;
	}
}

// FUNCTION: LEMBALL 0x0042f540
int CMoverManager::GetViewData(CViewData* pViewData)
{
	return ((int(__fastcall*)(void*, CViewData*)) 0x42e590)(this, pViewData);
}

// FUNCTION: LEMBALL 0x0042f680
void CMoverManager::LoadLevel(unsigned short* pLevelData, int nLen, unsigned char nFormat)
{
	unsigned short nCount = *pLevelData;
	((void(__fastcall*) (void*, int)) 0x401811)(this, nCount);
	unsigned int i = 1;
	unsigned short uSlot, uFlags, routeSel, routeCount;
	unsigned int flagOverride;
	while (i < nCount) {
		uSlot = (unsigned int) ((int(__fastcall*) ()) 0x40214e)();
		uFlags = pLevelData[i++];
		flagOverride = (pLevelData[i] & 0x30) != 0;
		routeSel = pLevelData[i++];
		routeCount = pLevelData[i++];
		((void(__fastcall*) (void*, int, int, int, int, int)) 0x401f55)(this, uSlot, uFlags, flagOverride, routeSel, routeCount);
	}
}
