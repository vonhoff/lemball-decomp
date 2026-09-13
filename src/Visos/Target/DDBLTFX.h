#ifndef LEMBALL_VISOS_TARGET_DDBLTFX_H
#define LEMBALL_VISOS_TARGET_DDBLTFX_H

#include "../../Common.h"

// DirectDraw 1 blit-effects ABI from the Windows SDK ddraw.h.
struct DDBLTFX {
	unsigned long dwSize;
	undefined m_unused04[0x4c];
	unsigned long dwFillColor;
	undefined m_unused54[0x10];
};

#endif
