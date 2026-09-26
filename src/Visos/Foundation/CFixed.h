#ifndef LEMBALL_VISOS_FOUNDATION_CFIXED_H
#define LEMBALL_VISOS_FOUNDATION_CFIXED_H

// SIZE 0x04
class CFixed {
public:
	CFixed();
	inline CFixed(int p_value) : m_value(p_value) {}
	inline CFixed(const CFixed& p_other) : m_value(p_other.m_value) {}
	static CFixed FromInteger(int p_value) { return CFixed(p_value << 12); }

	int m_value; // 0x00
};

CFixed operator+(const CFixed& p_left, const CFixed& p_right);
#endif
