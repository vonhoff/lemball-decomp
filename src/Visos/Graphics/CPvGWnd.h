#ifndef LEMBALL_VISOS_GRAPHICS_CPVGWND_H
#define LEMBALL_VISOS_GRAPHICS_CPVGWND_H

#include "CBitmap.h"         // complete type
#include "CCopyToBackBuff.h" // complete type
#include "CLine.h"           // complete type
#include "CWnd.h"            // complete type
class CGdi;
class CPvWnd;
class CVsRect;

struct WinGDrawState;

// SIZE 0x90
// VTABLE: LEMBALL 0x00499260
class CPvGWnd : public CWnd {
public:
	CPvGWnd();
	virtual void Create(const CVsRect& p_rect,
						CPvWnd* p_parent,
						char* p_title,
						unsigned long p_paletteId) = 0;        // vtable+0xa4
	virtual void OnPaint(const CVsRect& p_rect) = 0;           // vtable+0xa8
	virtual void AttachPalette(unsigned long p_paletteId) = 0; // vtable+0xac
	virtual void Render() = 0;                                 // vtable+0xb0
	virtual void Flush() = 0;                                  // vtable+0xb4
	virtual unsigned int GetStyle();                           // vtable+0x64
	virtual void OnVisibilityChange();                         // vtable+0x80
	virtual void InternalOnZoom(int p_oldZoom);                // vtable+0x2c
	virtual void SetDontUpdateRect(const CVsRect& p_rect);     // vtable+0x84
	virtual void SetInnerWindow(const CVsRect& p_rect);        // vtable+0x08
	virtual void InternalOnCreate() = 0;                       // vtable+0x18
	virtual void InternalOnDestroy() = 0;                      // vtable+0x1c
	virtual void InternalOnMove() = 0;                         // vtable+0x28
	virtual void InternalOnSize() = 0;                         // vtable+0x20
	void Clear(int p_color);

	friend struct CGraphicsState;
	friend class CGWnd;
	friend class CMain2DDisplay;
	friend class CNetworkOptionsDrawer;
	friend class CGraphicButton;
	friend class CAnimWnd;
	friend class CPvButton;
	friend class CSpriteWindow;
	friend class CTrackWindow;
	friend class CHiliteWindow;
	friend class CPasswordHiliteWindow;
	friend class CHiliteController;
	friend class CPasswordDrawer;
	friend class CGunController;
	friend class CGunButtons;
	friend class CHiliteButtons;
	friend class CDepressedButton;
	friend class CFramedButton;
	friend class CTextButton;
	friend class CPanelButton;
	friend class CFrontendResourceLoader;
	friend class CCdLoadAnim;
	friend class CBaseCursor;
	friend class C2D;
	friend class CPauseWindow;
	friend WinGDrawState* __stdcall WinGDrawOpen(void* p_openInfo);
	friend int __stdcall WinGDrawFrame(struct WinGDrawState* p_state, void* p_request, long p_param2);

private:
	CGdi* m_gdi;                       // 0x4c
	unsigned int m_gdiFlags;           // 0x50
	unsigned int m_paletteResourceId;  // 0x54
	CLine m_clearRectangle[1];         // 0x58
	CBitmap m_backBufferCopy[1];       // 0x68
	CCopyToBackBuff m_zBufferClear[1]; // 0x78
	unsigned short m_refreshWidth;     // 0x88
	unsigned short m_refreshHeight;    // 0x8a
	unsigned short m_refreshX;         // 0x8c
	unsigned short m_refreshY;         // 0x8e
};

#endif
