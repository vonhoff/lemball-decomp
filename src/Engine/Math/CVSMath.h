#ifndef LEMBALL_VISOS_FOUNDATION_CVSMATH_H
#define LEMBALL_VISOS_FOUNDATION_CVSMATH_H

class CVSMath {
public:
	unsigned int SqRoot(unsigned int p_value);
};

inline int VsAbs(int p_val)
{
	int t[2];
	int* p;
	if (p_val >= 0) {
		t[0] = p_val;
		p = &t[0];
	}
	else {
		t[1] = -p_val;
		p = &t[1];
	}
	return *p;
}

unsigned int __stdcall CalculatePowerOfTwo(unsigned int p_exponent);
unsigned int __stdcall ExtractBitField(unsigned int p_value, unsigned int p_shift, unsigned int p_width);
int WithinRect(int p_x, int p_y, int p_minX, int p_minY, int p_maxX, int p_maxY);
int sgn(int p_value);

#endif
