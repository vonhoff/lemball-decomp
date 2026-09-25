#ifndef LEMBALL_VISOS_RESOURCES_CRESPATCH_H
#define LEMBALL_VISOS_RESOURCES_CRESPATCH_H

#include "CResBase.h"

#define kChunkPtch 0x50544348

// SIZE 0x48
// VTABLE: LEMBALL 0x00498da0
class CResPatch : public CResBase {
public:
	inline CResPatch(unsigned int p_resourceId) { DoLoad(p_resourceId); }
	static CResPatch* Load(unsigned int p_resourceId);
	virtual void SetType(); // vtable+0x34
};

// SYNTHETIC: LEMBALL 0x0045ebe0
// CResPatch::`scalar deleting destructor'

#endif
