#ifndef LEMBALL_VISOS_RESOURCES_RESBOOL_H
#define LEMBALL_VISOS_RESOURCES_RESBOOL_H

#include "../../Common.h"
#include "ResBase.h" // complete type

// SIZE 0x4c
// VTABLE: LEMBALL 0x00498b88
class ResBool : public ResBase {
public:
	virtual void OnLoad();  // vtable+0x2c
	virtual void SetType(); // vtable+0x34

private:
	unsigned int m_value; // 0x48
};

#endif
