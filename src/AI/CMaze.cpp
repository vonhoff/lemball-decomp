#include "AI/CMaze.h"

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
