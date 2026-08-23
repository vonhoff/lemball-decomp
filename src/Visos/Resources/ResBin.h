#ifndef LEMBALL_VISOS_RESOURCES_RESBIN_H
#define LEMBALL_VISOS_RESOURCES_RESBIN_H

#include "../../Common.h"
#include "ResBase.h" // complete type

// SIZE 0x48
// VTABLE: LEMBALL 0x00498e60
class ResBin : public ResBase {
public:
	inline ResBin() {}
	static ResBin* Load(unsigned int p_resourceId);
	virtual void SetType(); // vtable+0x34
	virtual ~ResBin();      // vtable+0x00
};

#endif
