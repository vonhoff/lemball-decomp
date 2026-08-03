#include "AI/CMaze.h"

#include "Visos/Generic/Memory.h"

// FUNCTION: LEMBALL 0x00423090
CMaze::CMaze(CMap* pMap)
{
	m_pMap00 = pMap;
	m_ppRows04 = 0;
	m_nFrontier100C = 0;
	m_nReserved08 = 0;
	m_cColumns1010 = 0;
	m_cRows1014 = 0;
}

// FUNCTION: LEMBALL 0x004230c0
CMaze::~CMaze(void)
{
	int i;

	if (m_ppRows04 != 0) {
		for (i = 0; i < m_cRows1014; ++i) {
			FreeVSMemBlock(m_ppRows04[i]);
		}
		FreeVSMemBlock(m_ppRows04);
	}
}
