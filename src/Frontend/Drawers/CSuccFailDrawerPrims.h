#ifndef LEMBALL_FRONTEND_DRAWERS_CSUCCFAILDRAWERPRIMS_H
#define LEMBALL_FRONTEND_DRAWERS_CSUCCFAILDRAWERPRIMS_H

#include "../../Visos/Graphics/CBitmapRes.h" // complete type

// SIZE 0x48
struct CSuccFailDrawerPrims {
public:
	CSuccFailDrawerPrims();
	~CSuccFailDrawerPrims();

	CBitmapRes m_primary;   // 0x00
	CBitmapRes m_secondary; // 0x24
};

#endif
