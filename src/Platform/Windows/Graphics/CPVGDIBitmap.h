#ifndef LEMBALL_VISOS_GRAPHICS_CPVGDIBITMAP_H
#define LEMBALL_VISOS_GRAPHICS_CPVGDIBITMAP_H

#include "Engine/Math/CVSRect.h"
#include "Engine/Math/CVSSize.h"

struct CVSPoint;

// SIZE 0x40
// VTABLE: LEMBALL 0x0049a470
class CPVGDIBitmap {
public:
	CPVGDIBitmap();
	CVSSize SetSize(const CVSSize& p_size, int p_pitch);
	virtual void SetLinePtrs(); // vtable+0x00
	void CreateLinePtrs();
	void Free();
	void GetRects(const CVSRect& p_rect, CVSRect*& p_rect0, CVSRect*& p_rect1);
	void Initialise();
	void ResetLinePtrs();
	void ResetScroll();
	void Scroll(const CVSRect& p_rect, const CVSPoint& p_destination);
	void DrawCircleSymmetricPoints(int p_centreX, int p_centreY, int p_xOffset, int p_yOffset, int p_colour);
	void SetBitsBase(unsigned char* p_bits, int p_stride);
	~CPVGDIBitmap();

	friend class CSurface;
	friend class CBaseFrontendDrawer;
	friend class CPVBackBuffSurface;
	friend class CPVZBuffSurface;
	friend class CGraphicsDriver;
	friend class CPlanarDibDriver;
	friend struct CGraphicsState;
	friend class CGWnd;

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
	CVSSize m_size;              // 0x2c
	CVSRect m_rect0;             // 0x30
	CVSRect m_rect1;             // 0x38
};

#endif
