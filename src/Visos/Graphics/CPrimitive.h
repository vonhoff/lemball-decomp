#ifndef LEMBALL_VISOS_GRAPHICS_CPRIMITIVE_H
#define LEMBALL_VISOS_GRAPHICS_CPRIMITIVE_H

class CGdi;

// SIZE 0x04
// VTABLE: LEMBALL 0x00496ca8
class CPrimitive {
public:
	inline CPrimitive() {}

	virtual ~CPrimitive() {}              // vtable+0x00
	virtual void Draw(CGdi* p_gdi) = 0;   // vtable+0x04
	virtual void Render(CGdi* p_gdi) = 0; // vtable+0x08
};

// SYNTHETIC: LEMBALL 0x00432350
// CPrimitive::`scalar deleting destructor'

#endif
