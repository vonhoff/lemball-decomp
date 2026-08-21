#include "Visos/Generic/CVSPoint.h"

// FUNCTION: LEMBALL 0x0046ba50
int CVSPoint::Equals(const CVSPoint* pOther)
{
	if (pOther->m_nX == m_nX && pOther->m_nY == m_nY) {
		return 1;
	}
	return 0;
}
