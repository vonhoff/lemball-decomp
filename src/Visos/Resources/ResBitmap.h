#ifndef LEMBALL_VISOS_RESOURCES_RESBITMAP_H
#define LEMBALL_VISOS_RESOURCES_RESBITMAP_H

#include "../../Common.h"
#include "ResBase.h" // complete type

// SIZE 0x54
// VTABLE: LEMBALL 0x00498d20
class ResBitmap : public ResBase {
public:
	static ResBitmap* Load(unsigned int p_resourceId);
	virtual void SetHeader(); // vtable+0x08
	virtual void SetType();   // vtable+0x34
	virtual ~ResBitmap();     // vtable+0x00

private:
	unsigned short m_width;      // 0x48
	unsigned short m_height;     // 0x4a
	unsigned char m_depth;       // 0x4c
	unsigned char m_flags;       // 0x4d
	undefined m_platformData[6]; // 0x4e
};

#endif
