#ifndef LEMBALL_VISOS_RESOURCES_RESPATCH_H
#define LEMBALL_VISOS_RESOURCES_RESPATCH_H

#include "../../Common.h"
#include "ResBase.h" // complete type

#define kChunkPtch 0x50544348

// SIZE 0x48
// VTABLE: LEMBALL 0x00498da0
class ResPatch : public ResBase {
public:
	inline ResPatch(unsigned int p_resourceId) { DoLoad(p_resourceId); }
	static ResPatch* Load(unsigned int p_resourceId);
	virtual void SetType(); // vtable+0x34
	virtual ~ResPatch();    // vtable+0x00
};

#endif
