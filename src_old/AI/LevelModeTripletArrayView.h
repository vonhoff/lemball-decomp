#ifndef LEMBALL_LEVELMODETRIPLETARRAYVIEW_H
#define LEMBALL_LEVELMODETRIPLETARRAYVIEW_H

// Level-mode triplet array view. Consolidated from duplicate per-TU definitions
// (LVTRIPLET.CPP, LVMODESLOT.CPP) with identical field layout; method union:
// CopyFixedPointTriplet is defined in LVTRIPLET.CPP, SetTripletEntry in LVMODESLOT.CPP.

struct LevelModeTripletArrayView {
	unsigned char m_abReserved00[0x90];
	int m_anX90[4];
	int m_anYA0[4];
	int m_anZB0[4];

	void CopyFixedPointTriplet(int* pOutput, int nIndex);
	void SetTripletEntry(int nX, int nY, int nZ, int nIndex);
};

#endif
