#ifndef LEMBALL_VISOS_RESOURCES_CRESEFFECT_H
#define LEMBALL_VISOS_RESOURCES_CRESEFFECT_H

#include "CResBase.h" // complete type

// SIZE 0x48
// VTABLE: LEMBALL 0x00498d60
class CResEffect : public CResBase {
public:
	inline CResEffect(unsigned int p_resourceId) { DoLoad(p_resourceId); }
	static CResEffect* Load(unsigned int p_resourceId);
	virtual void SetType(); // vtable+0x34
};

// SYNTHETIC: LEMBALL 0x0045ebb0
// CResEffect::`scalar deleting destructor'

#endif
