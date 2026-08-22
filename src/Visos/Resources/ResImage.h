#ifndef LEMBALL_SCAFFOLD_VISOS_RESOURCES_RESIMAGE_H
#define LEMBALL_SCAFFOLD_VISOS_RESOURCES_RESIMAGE_H

#include "../../Common.h"
#include "ResBase.h" // complete type

// SIZE 0x54
// VTABLE: LEMBALL 0x00498ce0
class ResImage : public ResBase {
public:
	// No functions mapped yet.

private:
	unsigned short m_width;     // 0x48
	unsigned short m_height;    // 0x4a
	unsigned char m_depth;      // 0x4c
	unsigned char m_flags;      // 0x4d
	undefined m_unknown0x4e[2]; // 0x4e
	unsigned int m_imageState;  // 0x50
};

#endif
