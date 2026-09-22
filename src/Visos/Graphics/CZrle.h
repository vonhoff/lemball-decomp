#ifndef LEMBALL_VISOS_GRAPHICS_CZRLE_H
#define LEMBALL_VISOS_GRAPHICS_CZRLE_H

#include "CPrimitive.h" // complete type

class CResBase;
class CRemap;

// SIZE 0x18
// VTABLE: LEMBALL 0x00499628
class CZrle : public CPrimitive {
public:
	inline CZrle() : m_y(0), m_x(0) {}
	virtual void Draw(CGdi* p_gdi);   // vtable+0x04
	virtual void Render(CGdi* p_gdi); // vtable+0x08
	virtual ~CZrle() {}

	friend class CText;
	friend class CAnimsManager;
	friend class CAnim;
	friend class CPauseWindow;
	friend class CSurface;
	friend class CGraphicButton;
	friend class CBaseCursor;

public:
	short m_x;            // 0x04
	short m_y;            // 0x06
	CResBase* m_resource; // 0x08
	unsigned int m_flags; // 0x0c
	CRemap* m_remap;      // 0x10
	unsigned int m_state; // 0x14
};

// SYNTHETIC: LEMBALL 0x00467ac0
// CZrle::CZrle

// SYNTHETIC: LEMBALL 0x00467b40
// CZrle::`vector deleting destructor'

// SYNTHETIC: LEMBALL 0x00467ba0
// CZrle::~CZrle

#endif
