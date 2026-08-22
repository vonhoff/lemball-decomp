#ifndef LEMBALL_SCAFFOLD_VISOS_PREVIEWDRAWER_PRIMS_H
#define LEMBALL_SCAFFOLD_VISOS_PREVIEWDRAWER_PRIMS_H

#include "../Common.h"

// SIZE 0x24
struct PreviewDrawer_Prims {
public:
	PreviewDrawer_Prims();
	~PreviewDrawer_Prims();

	undefined m_primitive[0x24]; // 0x00
};

#endif
