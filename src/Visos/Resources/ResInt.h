#ifndef LEMBALL_VISOS_RESOURCES_RESINT_H
#define LEMBALL_VISOS_RESOURCES_RESINT_H

#include "../../Common.h"
#include "ResBase.h" // complete type

// SIZE 0x04
struct IntPayload {
	int m_value;
};

// SIZE 0x4c
// VTABLE: LEMBALL 0x00498b48
class ResInt : public ResBase {
public:
	ResInt();
	virtual void OnLoad();  // vtable+0x2c
	virtual void SetType(); // vtable+0x34
	virtual ~ResInt();      // vtable+0x00

	friend class FontTable;

private:
	int m_value; // 0x48
};

// SYNTHETIC: LEMBALL 0x0045e930
// ResInt::`scalar deleting destructor'


#endif

