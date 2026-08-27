#ifndef LEMBALL_VISOS_GRAPHICS_ZRLE_H
#define LEMBALL_VISOS_GRAPHICS_ZRLE_H

#include "../../Common.h"
#include "Primitive.h" // complete type

// SIZE 0x18
// VTABLE: LEMBALL 0x00499628
class Zrle : public Primitive {
public:
	inline Zrle() : m_y(0), m_x(0) {}
	virtual void Draw(Gdi* p_gdi);   // vtable+0x04
	virtual void Render(Gdi* p_gdi); // vtable+0x08
	virtual ~Zrle();                 // vtable+0x00

	friend class Text;
	friend class AnimsManager;
	friend class Anim;
	friend class Surface;
	friend class GraphicButton;

private:
	short m_x;            // 0x04
	short m_y;            // 0x06
	ResBase* m_resource;  // 0x08
	unsigned int m_flags; // 0x0c
	Remap* m_remap;       // 0x10
	unsigned int m_state; // 0x14
};

// 68K 0x10115b92 __ct__5CZRLEFv
// SYNTHETIC: LEMBALL 0x00467ac0
// Zrle::Zrle

// SYNTHETIC: LEMBALL 0x00467b40
// Zrle::`scalar deleting destructor'


#endif

