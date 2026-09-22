#ifndef LEMBALL_FRONTEND_DRAWERS_CPREVIEWDRAWERPRIMS_H
#define LEMBALL_FRONTEND_DRAWERS_CPREVIEWDRAWERPRIMS_H

#include "../../Visos/Graphics/CBitmapRes.h" // complete type

// SIZE 0x24
struct CPreviewDrawerPrims {
public:
	CPreviewDrawerPrims();
	~CPreviewDrawerPrims();

	CBitmapRes m_bitmap; // 0x00
};

#endif
