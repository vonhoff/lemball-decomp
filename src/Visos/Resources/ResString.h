#ifndef LEMBALL_VISOS_RESOURCES_RESSTRING_H
#define LEMBALL_VISOS_RESOURCES_RESSTRING_H

#include "../../Common.h"
#include "ResBase.h" // complete type

// SIZE 0x48
// VTABLE: LEMBALL 0x00498c48
class ResString : public ResBase {
public:
	inline ResString(unsigned int p_resourceId) { DoLoad(p_resourceId); }
	ResString();
	static ResString* Load(unsigned int p_resourceId);
	virtual void SetType(); // vtable+0x34
	virtual ~ResString();   // vtable+0x00
};

// SYNTHETIC: LEMBALL 0x0045eac0
// ResString::`vector deleting destructor'

#endif
