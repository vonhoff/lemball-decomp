#ifndef LEMBALL_VISOS_RESOURCES_CRESRASTER_H
#define LEMBALL_VISOS_RESOURCES_CRESRASTER_H

#include "CResBase.h"
#include "Engine/Math/CVSPoint.h"

// SIZE 0x4c
// VTABLE: LEMBALL 0x00498ab0
class CResRaster : public CResBase {
public:
	inline CResRaster() {}

	friend class CBaseFrontendDrawer;
	friend class CMainOptions1Drawer;
	friend class CMainOptions2Drawer;
	friend class CSuccFailDrawer;
	friend class CSurface;
	friend class CAnimsManager;
	friend class CCDLoadAnim;
	friend class CAboutScreen;

protected:
	CVSPoint m_rasterPoint; // 0x48
};

// SYNTHETIC: LEMBALL 0x0045e820
// CResRaster::`scalar deleting destructor'

#endif
