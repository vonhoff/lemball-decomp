#ifndef LEMBALL_CMOVE3D_H
#define LEMBALL_CMOVE3D_H

struct CPt3 {
	int x;
	int y;
	int z;
};

struct CMove3d {
	int m_nStartTime;
	CPt3 m_Start;
	CPt3 m_Velocity;

	void Set(CPt3& start, CPt3& end, int nStartTime, int nSpeed);
	void Position(CPt3& position, int nTime);
};

typedef char CMove3dSizeCheck[sizeof(CMove3d) == 0x1c ? 1 : -1];

#endif
