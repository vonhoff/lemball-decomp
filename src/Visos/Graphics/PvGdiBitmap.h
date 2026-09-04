#ifndef LEMBALL_VISOS_GRAPHICS_PVGDIBITMAP_H
#define LEMBALL_VISOS_GRAPHICS_PVGDIBITMAP_H

#include "../../Common.h"
#include "../Foundation/VsPoint.h" // complete type
#include "../Foundation/VsRect.h"  // complete type
#include "../Foundation/VsSize.h"  // complete type

// SIZE 0x40
// VTABLE: LEMBALL 0x0049a470
class PvGdiBitmap {
public:
	PvGdiBitmap();
	VsSize SetSize(const VsSize& p_size, int p_pitch);
	virtual void SetLinePtrs(); // vtable+0x00
	void CreateLinePtrs();
	void Free();
	void GetRects(const VsRect& p_rect, VsRect*& p_rect0, VsRect*& p_rect1);
	void Initialise();
	void ResetLinePtrs();
	void ResetScroll();
	void Scroll(const VsRect* p_rect, const VsPoint* p_destination);
	void DrawCircleSymmetricPoints(int p_centerX, int p_centerY, int p_xOffset, int p_yOffset, unsigned char p_color);
	void SetBitsBase(unsigned char* p_bits, int p_stride);
	~PvGdiBitmap();

	friend class Surface;
	friend class BaseFrontendDrawer;
	friend class PvBackBuffSurface;
	friend class PvZBuffSurface;
	friend class TargetGraphicsDriver;
	friend class GWnd;

private:
	void** m_lines;              // 0x04
	unsigned char* m_bits;       // 0x08
	unsigned char* m_bitsBase;   // 0x0c
	unsigned int m_directScroll; // 0x10
	unsigned int m_firstLine;    // 0x14
	unsigned int m_xOffset;      // 0x18
	int m_stride;                // 0x1c
	unsigned int m_rowPadding;   // 0x20
	unsigned int m_extraRows;    // 0x24
	unsigned int m_lineCapacity; // 0x28
	short m_width;               // 0x2c
	short m_height;              // 0x2e
	VsRect m_rect0;              // 0x30
	VsRect m_rect1;              // 0x38
};

#endif
