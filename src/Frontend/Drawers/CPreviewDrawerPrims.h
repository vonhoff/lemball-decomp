#ifndef LEMBALL_FRONTEND_DRAWERS_CPREVIEWDRAWERPRIMS_H
#define LEMBALL_FRONTEND_DRAWERS_CPREVIEWDRAWERPRIMS_H

#include "../../Visos/Graphics/BitmapRes.h" // complete type

// SIZE 0x24
struct CPreviewDrawerPrims {
public:
	CPreviewDrawerPrims();
	~CPreviewDrawerPrims();

	BitmapRes m_bitmap; // 0x00
};

#endif
