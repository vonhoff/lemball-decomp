#ifndef LEMBALL_VISOS_RESOURCES_RESTUNE_H
#define LEMBALL_VISOS_RESOURCES_RESTUNE_H

#include "CResBase.h" // complete type

#define kChunkTune 0x54554e45

// SIZE 0x48
// VTABLE: LEMBALL 0x00498de0
class ResTune : public CResBase {
public:
	inline ResTune(unsigned int p_resourceId) { DoLoad(p_resourceId); }
	static ResTune* Load(unsigned int p_resourceId);
	virtual void SetType(); // vtable+0x34
};

// SYNTHETIC: LEMBALL 0x0045ec10
// ResTune::`scalar deleting destructor'

#endif
