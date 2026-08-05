#include "Platform/Windows/Mixed/Engine/CORE/VSINIT.H"
#include "AI/LevelModeTripletArrayView.h"

;

// FUNCTION: LEMBALL 0x004132c0
void LevelModeTripletArrayView::CopyFixedPointTriplet(int* pOutput, int nIndex)
{
	pOutput[0] = m_anX90[nIndex] << 12;
	pOutput[1] = m_anYA0[nIndex] << 12;
	pOutput[2] = m_anZB0[nIndex] << 12;
}
