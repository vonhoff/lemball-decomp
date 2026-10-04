#ifndef LEMBALL_VISOS_RESOURCES_CRESEFFECT_H
#define LEMBALL_VISOS_RESOURCES_CRESEFFECT_H

#include "CResBase.h"

// SIZE 0x48
// VTABLE: LEMBALL 0x00498d60
class CResEFFECT : public CResBase {
public:
	inline CResEFFECT(unsigned int p_resourceId) { DoLoad(p_resourceId); }
	static CResEFFECT* Load(unsigned int p_resourceId);
	virtual void SetType(); // vtable+0x34
};

// SYNTHETIC: LEMBALL 0x0045ebb0
// CResEFFECT::`scalar deleting destructor'

#endif
