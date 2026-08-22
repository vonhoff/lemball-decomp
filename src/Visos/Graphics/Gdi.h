#ifndef LEMBALL_SCAFFOLD_VISOS_GRAPHICS_GDI_H
#define LEMBALL_SCAFFOLD_VISOS_GRAPHICS_GDI_H

#include "../../Common.h"

// SIZE 0x10
class Gdi {
public:
	Gdi(const VsRect& p_arg0, int p_arg1, Surface* p_arg2);
	void AddToList(Primitive* p_primitive);
	void Render();
	~Gdi();

private:
	Primitive** m_primitives; // 0x00
	int m_primitiveCount;     // 0x04
	int m_primitiveCapacity;  // 0x08
	void* m_renderTarget;     // 0x0c
};

#endif
