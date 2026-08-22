#ifndef LEMBALL_VISOS_GRAPHICS_HOTAREAELEMENT_H
#define LEMBALL_VISOS_GRAPHICS_HOTAREAELEMENT_H

#include "../../Common.h"

// SIZE 0x0c
struct HotAreaElement {
public:
	// No functions mapped yet.

	HotAreaHandler* m_handler;  // 0x00
	HotAreaElement* m_previous; // 0x04
	HotAreaElement* m_next;     // 0x08
};

#endif
