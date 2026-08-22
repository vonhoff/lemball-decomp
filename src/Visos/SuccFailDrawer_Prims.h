#ifndef LEMBALL_SCAFFOLD_VISOS_SUCCFAILDRAWER_PRIMS_H
#define LEMBALL_SCAFFOLD_VISOS_SUCCFAILDRAWER_PRIMS_H

#include "../Common.h"

// SIZE 0x48
struct SuccFailDrawer_Prims {
public:
	SuccFailDrawer_Prims();
	~SuccFailDrawer_Prims();

	undefined m_primary[0x24];   // 0x00
	undefined m_secondary[0x24]; // 0x24
};

#endif
