#ifndef LEMBALL_FRONTEND_DRAWERS_SUCCFAILDRAWERPRIMS_H
#define LEMBALL_FRONTEND_DRAWERS_SUCCFAILDRAWERPRIMS_H

#include "../../Common.h"
#include "../../Visos/Graphics/BitmapRes.h" // complete type

// SIZE 0x48
struct SuccFailDrawerPrims {
public:
	SuccFailDrawerPrims();
	~SuccFailDrawerPrims();

	BitmapRes m_primary;   // 0x00
	BitmapRes m_secondary; // 0x24
};

#endif
