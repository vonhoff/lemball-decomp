#ifndef LEMBALL_VISOS_RESOURCES_RESPRES_H
#define LEMBALL_VISOS_RESOURCES_RESPRES_H

#include "../../Common.h"
#include "ResBase.h" // complete type

#define kChunkPres 0x50524553

// SIZE 0x48
// VTABLE: LEMBALL 0x00498e20
class ResPres : public ResBase {
public:
	inline ResPres(unsigned int p_resourceId) { DoLoad(p_resourceId); }
	static ResPres* Load(unsigned int p_resourceId);
	virtual void SetType(); // vtable+0x34
	virtual ~ResPres();     // vtable+0x00
};

// SYNTHETIC: LEMBALL 0x0045ec40
// ResPres::`scalar deleting destructor'

#endif
