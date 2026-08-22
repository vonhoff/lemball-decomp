#ifndef LEMBALL_SCAFFOLD_VISOS_RESOURCES_RESZRLE_H
#define LEMBALL_SCAFFOLD_VISOS_RESOURCES_RESZRLE_H

#include "../../Common.h"
#include "ResBase.h" // complete type

// SIZE 0x54
// VTABLE: LEMBALL 0x00498a70
class ResZrle : public ResBase {
public:
	ResZrle();
	static ResZrle* Load(unsigned int p_resourceId);
	virtual void SetHeader(); // vtable+0x08
	virtual void SetType();   // vtable+0x34
	virtual ~ResZrle();       // vtable+0x00

private:
	short m_x;       // 0x48
	short m_y;       // 0x4a
	short m_width;   // 0x4c
	short m_height;  // 0x4e
	short m_originX; // 0x50
	short m_originY; // 0x52
};

#endif
