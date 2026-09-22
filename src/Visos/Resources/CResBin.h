#ifndef LEMBALL_VISOS_RESOURCES_CRESBIN_H
#define LEMBALL_VISOS_RESOURCES_CRESBIN_H

#include "CResBase.h" // complete type

// SIZE 0x48
// VTABLE: LEMBALL 0x00498e60
class CResBin : public CResBase {
public:
	inline CResBin(unsigned int p_resourceId) { DoLoad(p_resourceId); }
	static CResBin* Load(unsigned int p_resourceId);
	virtual void SetType(); // vtable+0x34
};

// SYNTHETIC: LEMBALL 0x0045ec70
// CResBin::`scalar deleting destructor'

#endif
