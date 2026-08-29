#ifndef LEMBALL_FRONTEND_DRAWERS_PREVIEWDRAWERPRIMS_H
#define LEMBALL_FRONTEND_DRAWERS_PREVIEWDRAWERPRIMS_H

#include "../../Common.h"
#include "../../Visos/Graphics/BitmapRes.h" // complete type

// SIZE 0x24
struct PreviewDrawerPrims {
public:
	PreviewDrawerPrims();
	~PreviewDrawerPrims();

	BitmapRes m_bitmap; // 0x00
};

#endif
