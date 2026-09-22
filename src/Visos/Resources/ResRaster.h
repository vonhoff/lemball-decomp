#ifndef LEMBALL_VISOS_RESOURCES_RESRASTER_H
#define LEMBALL_VISOS_RESOURCES_RESRASTER_H

#include "CResBase.h" // complete type

// SIZE 0x4c
// VTABLE: LEMBALL 0x00498ab0
class ResRaster : public CResBase {
public:
	inline ResRaster()
	{
		m_y = 0;
		m_x = 0;
	}

	friend class CBaseFrontendDrawer;
	friend class CMainOptions1Drawer;
	friend class CMainOptions2Drawer;
	friend class CSuccFailDrawer;
	friend class CSurface;
	friend class CAnimsManager;
	friend class CCdLoadAnim;
	friend class TargetAboutScreen;

protected:
	short m_x; // 0x48
	short m_y; // 0x4a
};

// SYNTHETIC: LEMBALL 0x0045e820
// ResRaster::`scalar deleting destructor'

#endif
