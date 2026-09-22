#ifndef LEMBALL_VISOS_ANIMATION_CANIM_H
#define LEMBALL_VISOS_ANIMATION_CANIM_H

#include "../Graphics/CZrle.h" // complete type

class CResAnim;

// SIZE 0x20
// VTABLE: LEMBALL 0x00497730
class CAnim : public CZrle {
public:
	CAnim();
	virtual void Draw(CGdi* p_gdi);   // vtable+0x04
	virtual void Render(CGdi* p_gdi); // vtable+0x08
	// FUNCTION: LEMBALL 0x004439e0
	virtual ~CAnim() {}

	friend class CAnimsManager;
	friend class CGraphicButton;
	friend class CPauseWindow;

public:
	unsigned int m_animIndex; // 0x18
	CResAnim* m_animResource; // 0x1c
};

// SYNTHETIC: LEMBALL 0x004439b0
// CAnim::`scalar deleting destructor'

// SYNTHETIC: LEMBALL 0x004452e0
// CAnim::`vector deleting destructor'

#endif
