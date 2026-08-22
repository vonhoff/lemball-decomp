#ifndef LEMBALL_SCAFFOLD_VISOS_GRAPHICS_PVSURFACE_H
#define LEMBALL_SCAFFOLD_VISOS_GRAPHICS_PVSURFACE_H

#include "../../Common.h"
#include "../Foundation/VsRect.h" // complete type

// SIZE 0x44
// VTABLE: LEMBALL 0x00499508
class PvSurface {
public:
	PvSurface();
	virtual bool HasBackBuff();                // vtable+0x3c
	virtual bool HasZBuff();                   // vtable+0x40
	virtual int GetWorldWidth();               // vtable+0x48
	virtual void Resize(const VsSize& p_size); // vtable+0x28
	virtual void SetWorldWidth(int p_width);   // vtable+0x44
	void SetDontUpdateRect(const VsRect& p_rect);
	~PvSurface();

private:
	VsRect m_dontUpdateRect;         // 0x04
	undefined m_geometryState[0x30]; // 0x0c
	unsigned int m_updateEnabled;    // 0x3c
	void* m_reserved40;              // 0x40
};

#endif
