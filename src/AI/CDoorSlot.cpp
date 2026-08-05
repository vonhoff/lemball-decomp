#include "Platform/Windows/Mixed/Engine/CORE/VSINIT.H"
#include "AI/CDoorManager.h"

extern unsigned short LEMBALL_FASTCALL GetManagedEntitySlotIdThunk(int nManagedEntityObject);

struct CDoor {
	unsigned char m_abData[0x14c];
};

// FUNCTION: LEMBALL 0x0040e600
unsigned short CDoorManager::Id(int nIndex)
{
	if (nIndex < m_nObjectCount34) {
		return GetManagedEntitySlotIdThunk((int) (unsigned long) &m_pObjects3C[nIndex]);
	}
	return 0xffff;
}
