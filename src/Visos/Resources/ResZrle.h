#ifndef LEMBALL_VISOS_RESOURCES_RESZRLE_H
#define LEMBALL_VISOS_RESOURCES_RESZRLE_H

#include "../../Common.h"
#include "ResBitmap.h" // for ResRaster

// SIZE 0x0c
struct ZrleHeader {
	short m_x;
	short m_y;
	short m_width;
	short m_height;
	short m_originX;
	short m_originY;
};

// SIZE 0x54
// VTABLE: LEMBALL 0x00498a70
class ResZrle : public ResRaster {
public:
	inline ResZrle(unsigned int p_resourceId) : m_height(0), m_width(0), m_originY(0), m_originX(0)
	{
		DoLoad(p_resourceId);
	}
	ResZrle();
	static ResZrle* Load(unsigned int p_resourceId);
	virtual void SetHeader(); // vtable+0x08
	virtual void SetType();   // vtable+0x34

	friend class Text;
	friend class ResAnim;
	friend class ResFont;
	friend class AnimsManager;
	friend class Surface;
	friend class GraphicButton;

private:
	short m_width;   // 0x4c
	short m_height;  // 0x4e
	short m_originX; // 0x50
	short m_originY; // 0x52
};

// 68K 0x10115f3c __dt__8CResZRLEFv
// SYNTHETIC: LEMBALL 0x0045e850
// ResZrle::`vector deleting destructor'

#endif
