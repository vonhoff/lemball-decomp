#ifndef LEMBALL_FRONTEND_DRAWERS_CSUCCFAILDRAWERPRIMS_H
#define LEMBALL_FRONTEND_DRAWERS_CSUCCFAILDRAWERPRIMS_H

#include "../../Visos/Graphics/BitmapRes.h" // complete type

// SIZE 0x48
struct CSuccFailDrawerPrims {
public:
	CSuccFailDrawerPrims();
	~CSuccFailDrawerPrims();

	BitmapRes m_primary;   // 0x00
	BitmapRes m_secondary; // 0x24
};

#endif
