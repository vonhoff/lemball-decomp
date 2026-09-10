#ifndef LEMBALL_VISOS_RESOURCES_RESRASTER_H
#define LEMBALL_VISOS_RESOURCES_RESRASTER_H

#include "../../Common.h"
#include "ResBase.h" // complete type

// SIZE 0x4c
// VTABLE: LEMBALL 0x00498ab0
class ResRaster : public ResBase {
public:
	inline ResRaster()
	{
		m_y = 0;
		m_x = 0;
	}

	friend class BaseFrontendDrawer;
	friend class MainOptions1Drawer;
	friend class MainOptions2Drawer;
	friend class SuccFailDrawer;
	friend class Surface;
	friend class AnimsManager;
	friend class CdLoadAnim;
	friend class TargetAboutScreen;

protected:
	short m_x; // 0x48
	short m_y; // 0x4a
};

// SYNTHETIC: LEMBALL 0x0045e820
// ResRaster::`scalar deleting destructor'

#endif
