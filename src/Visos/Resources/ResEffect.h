#ifndef LEMBALL_VISOS_RESOURCES_RESEFFECT_H
#define LEMBALL_VISOS_RESOURCES_RESEFFECT_H

#include "../../Common.h"
#include "ResBase.h" // complete type

// SIZE 0x48
// VTABLE: LEMBALL 0x00498d60
class ResEffect : public ResBase {
public:
	static ResEffect* Load(unsigned int p_resourceId);
	virtual void SetType(); // vtable+0x34
	virtual ~ResEffect();   // vtable+0x00
};

#endif
