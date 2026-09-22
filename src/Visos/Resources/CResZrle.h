#ifndef LEMBALL_VISOS_RESOURCES_CRESZRLE_H
#define LEMBALL_VISOS_RESOURCES_CRESZRLE_H

#include "CResBitmap.h" // for ResRaster
#include "Visos/Resources/ResRaster.h"

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
class CResZrle : public ResRaster {
public:
	inline CResZrle(unsigned int p_resourceId) : m_height(0), m_width(0), m_originY(0), m_originX(0)
	{
		DoLoad(p_resourceId);
	}
	CResZrle();
	static CResZrle* Load(unsigned int p_resourceId);
	virtual void SetHeader(); // vtable+0x08
	virtual void SetType();   // vtable+0x34

	friend class CText;
	friend class CResAnim;
	friend class CResFont;
	friend class CAnimsManager;
	friend class CPanel;
	friend class CSurface;
	friend class CPauseWindow;
	friend class CGraphicButton;

private:
	short m_width;   // 0x4c
	short m_height;  // 0x4e
	short m_originX; // 0x50
	short m_originY; // 0x52
};

// SYNTHETIC: LEMBALL 0x0045e850
// CResZrle::`vector deleting destructor'

#endif
