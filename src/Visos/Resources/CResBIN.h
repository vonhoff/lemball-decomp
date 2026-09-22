#ifndef LEMBALL_VISOS_RESOURCES_CRESBIN_H
#define LEMBALL_VISOS_RESOURCES_CRESBIN_H

#include "CResBase.h" // complete type

// SIZE 0x48
// VTABLE: LEMBALL 0x00498e60
class CResBIN : public CResBase {
public:
	inline CResBIN(unsigned int p_resourceId) { DoLoad(p_resourceId); }
	static CResBIN* Load(unsigned int p_resourceId);
	virtual void SetType(); // vtable+0x34
};

// SYNTHETIC: LEMBALL 0x0045ec70
// CResBIN::`scalar deleting destructor'

#endif
