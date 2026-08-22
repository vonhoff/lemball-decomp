#ifndef LEMBALL_FRONTEND_DRAWERS_PREVIEWDRAWERPRIMS_H
#define LEMBALL_FRONTEND_DRAWERS_PREVIEWDRAWERPRIMS_H

#include "../../Common.h"

// SIZE 0x24
struct PreviewDrawerPrims {
public:
	PreviewDrawerPrims();
	~PreviewDrawerPrims();

	undefined m_primitive[0x24]; // 0x00
};

#endif
