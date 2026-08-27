#ifndef LEMBALL_VISOS_ANIMATION_ANIM_H
#define LEMBALL_VISOS_ANIMATION_ANIM_H

#include "../../Common.h"
#include "../Graphics/Zrle.h" // complete type

// SIZE 0x20
// VTABLE: LEMBALL 0x00497730
class Anim : public Zrle {
public:
	Anim();
	virtual void Draw(Gdi* p_gdi);   // vtable+0x04
	virtual void Render(Gdi* p_gdi); // vtable+0x08
	virtual ~Anim();                 // vtable+0x00

	friend class AnimsManager;
	friend class GraphicButton;

private:
	unsigned int m_animIndex; // 0x18
	ResAnim* m_animResource;  // 0x1c
};

// SYNTHETIC: LEMBALL 0x004452e0
// Anim::`scalar deleting destructor'


#endif

