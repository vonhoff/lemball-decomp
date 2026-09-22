#ifndef LEMBALL_VISOS_GRAPHICS_CPVGDIBITMAP_H
#define LEMBALL_VISOS_GRAPHICS_CPVGDIBITMAP_H

#include "../Foundation/CVsPoint.h" // complete type
#include "../Foundation/CVsRect.h"  // complete type
#include "../Foundation/CVsSize.h"  // complete type

// SIZE 0x40
// VTABLE: LEMBALL 0x0049a470
class CPvGdiBitmap {
public:
	CPvGdiBitmap();
	CVsSize SetSize(const CVsSize& p_size, int p_pitch);
	virtual void SetLinePtrs(); // vtable+0x00
	void CreateLinePtrs();
	void Free();
	void GetRects(const CVsRect& p_rect, CVsRect*& p_rect0, CVsRect*& p_rect1);
	void Initialise();
	void ResetLinePtrs();
	void ResetScroll();
	void Scroll(const CVsRect* p_rect, const CVsPoint* p_destination);
	void DrawCircleSymmetricPoints(int p_centerX, int p_centerY, int p_xOffset, int p_yOffset, unsigned char p_color);
	void SetBitsBase(unsigned char* p_bits, int p_stride);
	~CPvGdiBitmap();

	friend class CSurface;
	friend class CBaseFrontendDrawer;
	friend class CPvBackBuffSurface;
	friend class CPvZBuffSurface;
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
	short m_width;               // 0x2c
	short m_height;              // 0x2e
	CVsRect m_rect0;             // 0x30
	CVsRect m_rect1;             // 0x38
};

#endif
