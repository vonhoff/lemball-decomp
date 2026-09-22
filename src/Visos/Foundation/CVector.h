#ifndef LEMBALL_VISOS_FOUNDATION_CVECTOR_H
#define LEMBALL_VISOS_FOUNDATION_CVECTOR_H

// SIZE 0x08
class CVector {
public:
	CVector();
	inline CVector(const int& p_x, const int& p_y) : m_xFixed(p_x), m_yFixed(p_y) {}
	// FUNCTION: LEMBALL 0x00417b30
	inline CVector(const CVector& p_other) : m_xFixed(p_other.m_xFixed), m_yFixed(p_other.m_yFixed) {}
	CVector* SetIntegers(int p_x, int p_y);
	CVector& operator=(const CVector& p_other);

	int m_xFixed; // 0x00
	int m_yFixed; // 0x04
};

enum FormationVectorComponent {
	kFormationVectorX,
	kFormationVectorY,
};

typedef int FormationVectorTemplate[2];

CVector operator*(const CVector& p_vector, int p_scale);
CVector operator+(const CVector& p_left, const CVector& p_right);

extern FormationVectorTemplate g_aFormationTemplates[24];
#endif
