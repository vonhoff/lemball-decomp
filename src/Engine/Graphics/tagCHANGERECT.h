#ifndef LEMBALL_VISOS_FOUNDATION_TAGCHANGERECT_H
#define LEMBALL_VISOS_FOUNDATION_TAGCHANGERECT_H

#include "Engine/Math/CVSRect.h"

// SIZE 0x0c
class tagCHANGERECT : public CVSRect {
public:
	tagCHANGERECT();

	unsigned int m_drawMark; // 0x08
};

#endif
