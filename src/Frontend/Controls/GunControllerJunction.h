#ifndef LEMBALL_FRONTEND_CONTROLS_GUNCONTROLLERJUNCTION_H
#define LEMBALL_FRONTEND_CONTROLS_GUNCONTROLLERJUNCTION_H

#include "../../Common.h"

// SIZE 0x20
struct GunControllerJunction {
	int m_leftX;                 // 0x00
	int m_y;                     // 0x04
	int m_rightX;                // 0x08
	int m_direction;             // 0x0c
	unsigned int m_leftMessage;  // 0x10
	unsigned int m_rightMessage; // 0x14
	void* m_leftBinding;         // 0x18
	void* m_rightBinding;        // 0x1c
};

#endif
