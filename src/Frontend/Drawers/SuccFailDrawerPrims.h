#ifndef LEMBALL_FRONTEND_DRAWERS_SUCCFAILDRAWERPRIMS_H
#define LEMBALL_FRONTEND_DRAWERS_SUCCFAILDRAWERPRIMS_H

#include "../../Common.h"

// SIZE 0x48
struct SuccFailDrawerPrims {
public:
	SuccFailDrawerPrims();
	~SuccFailDrawerPrims();

	undefined m_primary[0x24];   // 0x00
	undefined m_secondary[0x24]; // 0x24
};

#endif
