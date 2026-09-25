#ifndef LEMBALL_VISOS_GRAPHICS_CZRLE_H
#define LEMBALL_VISOS_GRAPHICS_CZRLE_H

#include "CPrimitive.h"

class CResBase;
class CRemap;

// SIZE 0x18
// VTABLE: LEMBALL 0x00499628
class CZRLE : public CPrimitive {
public:
	inline CZRLE() : m_y(0), m_x(0) {}
	virtual void Draw(CGDI* p_gdi);   // vtable+0x04
	virtual void Render(CGDI* p_gdi); // vtable+0x08
	virtual ~CZRLE() {}

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
// CZRLE::CZRLE

// SYNTHETIC: LEMBALL 0x00467b40
// CZRLE::`vector deleting destructor'

// SYNTHETIC: LEMBALL 0x00467ba0
// CZRLE::~CZRLE

#endif
