#ifndef LEMBALL_AI_BASE_C3DVECTOR_H
#define LEMBALL_AI_BASE_C3DVECTOR_H

class CFixed;
// SIZE 0x0c
class C3DVector {
public:
	C3DVector() {}
	C3DVector(const CFixed& p_x, const CFixed& p_y, const CFixed& p_z);
	C3DVector& operator=(const C3DVector& p_other);

	int m_xFixed; // 0x00
	int m_yFixed; // 0x04
	int m_zFixed; // 0x08
};

#endif
