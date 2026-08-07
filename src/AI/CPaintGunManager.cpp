#include "AI/CPaintGunManager.h"
#include "AI/CPaintGun.h"
#include "Visos/Generic/Memory.h"

// Mac blueprint: CPaintGunManager::Add(ushort, int, int, int, int)

// FUNCTION: LEMBALL 0x0042c590
void CPaintGunManager::Add(unsigned short nSlot, int nWorldX, int nWorldY, int nWorldZ, int nSubtype)
{
	CPaintGun* pObject;
	int nLocal[3];

	if (m_nObjectCount34 < m_nCapacity30) {
		nLocal[0] = nWorldX << 12;
		nLocal[1] = nWorldY << 12;
		nLocal[2] = nWorldZ << 12;
		pObject = (CPaintGun*) (m_pObjects38 + m_nObjectCount34 * 0x144);
		pObject->Set(nSlot, nLocal, 0);
		pObject->m_nSubtype140 = nSubtype;
		m_nObjectCount34 = m_nObjectCount34 + 1;
	}
}
