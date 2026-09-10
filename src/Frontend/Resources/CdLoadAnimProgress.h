#ifndef LEMBALL_FRONTEND_RESOURCES_CDLOADANIMPROGRESS_H
#define LEMBALL_FRONTEND_RESOURCES_CDLOADANIMPROGRESS_H

#include "../../Common.h"
#include "CdLoadAnimDraw.h" // complete type

// SIZE 0xc4
// VTABLE: LEMBALL 0x00497c8c
class CdLoadAnimProgress {
public:
	virtual void Draw(short p_progress); // vtable+0x00

	CdLoadAnimDraw m_draw; // 0x04 / object+0x74
};

#endif
