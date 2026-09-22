#ifndef LEMBALL_VISOS_RESOURCES_CRESSTRING_H
#define LEMBALL_VISOS_RESOURCES_CRESSTRING_H

#include "CResBase.h" // complete type

// SIZE 0x48
// VTABLE: LEMBALL 0x00498c48
class CResString : public CResBase {
public:
	inline CResString(unsigned int p_resourceId) { DoLoad(p_resourceId); }
	CResString();
	static CResString* Load(unsigned int p_resourceId);
	virtual void SetType(); // vtable+0x34
};

// SYNTHETIC: LEMBALL 0x0045eac0
// CResString::`vector deleting destructor'

#endif
