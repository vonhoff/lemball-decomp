#ifndef LEMBALL_FRONTEND_CONTROLS_HILITECONTROLLERJUNCTION_H
#define LEMBALL_FRONTEND_CONTROLS_HILITECONTROLLERJUNCTION_H

#include "../../Common.h"

// SIZE 0x10
struct HiliteControllerJunction {
public:
	// No functions mapped yet.

	unsigned int m_present;        // 0x00
	int m_x;                       // 0x04
	int m_y;                       // 0x08
	unsigned int m_controlMessage; // 0x0c
};

#endif
