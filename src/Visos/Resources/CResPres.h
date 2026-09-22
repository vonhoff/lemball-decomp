#ifndef LEMBALL_VISOS_RESOURCES_CRESPRES_H
#define LEMBALL_VISOS_RESOURCES_CRESPRES_H

#include "CResBase.h" // complete type

#define kChunkPres 0x50524553

// SIZE 0x48
// VTABLE: LEMBALL 0x00498e20
class CResPres : public CResBase {
public:
	inline CResPres(unsigned int p_resourceId) { DoLoad(p_resourceId); }
	static CResPres* Load(unsigned int p_resourceId);
	virtual void SetType(); // vtable+0x34
};

// SYNTHETIC: LEMBALL 0x0045ec40
// CResPres::`scalar deleting destructor'

#endif
