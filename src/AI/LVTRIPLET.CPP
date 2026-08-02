#include "ENGINE/CORE/VSINIT.H"

struct LevelModeTripletArrayView {
	unsigned char m_abReserved00[0x90];
	int m_anX90[4];
	int m_anYA0[4];
	int m_anZB0[4];

	void CopyFixedPointTriplet(int* pOutput, int nIndex);
};

// FUNCTION: LEMBALL 0x004132c0
void LevelModeTripletArrayView::CopyFixedPointTriplet(int* pOutput, int nIndex)
{
	pOutput[0] = m_anX90[nIndex] << 12;
	pOutput[1] = m_anYA0[nIndex] << 12;
	pOutput[2] = m_anZB0[nIndex] << 12;
}
