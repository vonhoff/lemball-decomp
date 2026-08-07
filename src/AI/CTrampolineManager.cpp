#include "AI/CTrampolineManager.h"
#include "AI/CTrampoline.h"
#include "Visos/Generic/Memory.h"

// Mac blueprint: CTrampolineManager::Add(ushort, int, int, int)

// FUNCTION: LEMBALL 0x0042b5a0
void CTrampolineManager::Add(unsigned short nSlot, int nWorldX, int nWorldY, int nWorldZ)
{
	int nLocal[3];

	if (m_nObjectCount34 < m_nCapacity30) {
		nLocal[0] = nWorldX << 12;
		nLocal[1] = nWorldY << 12;
		nLocal[2] = nWorldZ << 12;
		((CTrampoline*) (m_pObjects38 + m_nObjectCount34 * 0x144))->Set(nSlot, nLocal);
		m_nObjectCount34 = m_nObjectCount34 + 1;
	}
}
