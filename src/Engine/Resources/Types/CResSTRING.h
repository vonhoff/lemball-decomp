#ifndef LEMBALL_VISOS_RESOURCES_CRESSTRING_H
#define LEMBALL_VISOS_RESOURCES_CRESSTRING_H

#include "CResBase.h"

// SIZE 0x48
// VTABLE: LEMBALL 0x00498c48
class CResSTRING : public CResBase {
public:
	inline CResSTRING(unsigned long p_resourceId) { DoLoad(p_resourceId); }
	CResSTRING();
	static CResSTRING* Load(unsigned long p_resourceId);
	virtual void SetType(); // vtable+0x34
};

// SYNTHETIC: LEMBALL 0x0045eac0
// CResSTRING::`vector deleting destructor'

// SYNTHETIC: LEMBALL 0x0045eb30
// CResSTRING::~CResSTRING

#endif
