#ifndef LEMBALL_VISOS_GRAPHICS_CPVSURFACE_H
#define LEMBALL_VISOS_GRAPHICS_CPVSURFACE_H

#include "../Foundation/CVsRect.h" // complete type
#include "../Foundation/CVsSize.h" // complete type

class CZrle;
class CResZrle;
class CBitmap;
class CResBitmap;
class CBigBitmap;
class CSolidRect;
class CPoint;
class CLine;
class CCircle;
class CFilledCircle;
class CClipRect;
class CScreenScroll;
class CResPalette;
class CSurface;
struct CVsPoint;

// SIZE 0x44
// VTABLE: LEMBALL 0x00499508
class CPvSurface {
public:
	CPvSurface();
	virtual void Blit(CZrle* p_primitive, CResZrle* p_zrle) = 0;       // vtable+0x24
	virtual void Blit(CBitmap* p_primitive, CResBitmap* p_bitmap) = 0; // vtable+0x20
	virtual void Blit(CBigBitmap* p_arg0, CResBitmap* p_arg1) = 0;     // vtable+0x1c
	virtual void Blit(CSolidRect* p_rect) = 0;                         // vtable+0x18
	virtual void Blit(CPoint* p_point) = 0;                            // vtable+0x14
	virtual void Blit(CLine* p_line) = 0;                              // vtable+0x10
	virtual void Blit(CCircle* p_circle) = 0;                          // vtable+0x0c
	virtual void Blit(CFilledCircle* p_circle) = 0;                    // vtable+0x08
	virtual void Blit(CClipRect* p_arg0) = 0;                          // vtable+0x04
	virtual void Blit(CScreenScroll* p_scroll) = 0;                    // vtable+0x00
	virtual void Resize(const CVsSize& p_size);                        // vtable+0x28
	virtual void Move(const CVsPoint& p_position) = 0;                 // vtable+0x2c
	virtual void AttachPalette(CResPalette* p_palette) = 0;            // vtable+0x30
	virtual void Flush() = 0;                                          // vtable+0x34
	virtual void* GetCurrDb() = 0;                                     // vtable+0x38
	virtual bool HasBackBuff();                                        // vtable+0x3c
	virtual bool HasZBuff();                                           // vtable+0x40
	virtual void SetWorldWidth(int p_width);                           // vtable+0x44
	virtual int GetWorldWidth();                                       // vtable+0x48
	void SetDontUpdateRect(const CVsRect& p_rect);
	~CPvSurface();

	friend class CSurface;
	friend class CPvScrollableSurface;
	friend class CGWnd;
	friend class CPvGWnd;
	friend class CPvBackBuffSurface;
	friend class CPvZBuffSurface;
	friend class CPvButton;
	friend class CGraphicButton;
	friend class CGunButtons;
	friend class CDepressedButton;
	friend class CSpriteWindow;
	friend class C2D;
	friend class CTrackWindow;
	friend class CHiliteWindow;
	friend class CPasswordHiliteWindow;
	friend class CHiliteController;
	friend class CPasswordDrawer;
	friend class CHiliteButtons;
	friend class CGunController;
	friend class CPanelButton;

private:
	CVsRect m_dontUpdateRect;     // 0x04
	CVsRect m_rect0c;             // 0x0c
	CVsRect m_windowRect;         // 0x14
	short m_relOriginX;           // 0x1c
	short m_relOriginY;           // 0x1e
	CSurface* m_parentSurface;    // 0x20
	CVsRect m_clipRect;           // 0x24
	short m_zoom;                 // 0x2c
	short m_pad2e;                // 0x2e
	unsigned int m_flag70;        // 0x30
	unsigned int m_flag74;        // 0x34
	unsigned int m_flag78;        // 0x38
	unsigned int m_updateEnabled; // 0x3c
	int m_reserved40;             // 0x40
};

#endif
