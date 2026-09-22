#ifndef LEMBALL_VISOS_GRAPHICS_CHOTAREAELEMENT_H
#define LEMBALL_VISOS_GRAPHICS_CHOTAREAELEMENT_H

// SIZE 0x0c
struct CHotAreaElement {
	CHotAreaHandler* m_handler;  // 0x00
	CHotAreaElement* m_previous; // 0x04
	CHotAreaElement* m_next;     // 0x08
};

#endif
