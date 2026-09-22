#ifndef LEMBALL_VISOS_GRAPHICS_CGDI_H
#define LEMBALL_VISOS_GRAPHICS_CGDI_H

class CVsRect;
class CSurface;
class CPrimitive;
struct WinGDrawState;

// SIZE 0x10
class CGdi {
public:
	CGdi(const CVsRect& p_arg0, int p_arg1, CSurface* p_arg2);
	void AddToList(CPrimitive* p_primitive);
	void Render();
	~CGdi();

	friend class CBitmap;
	friend class CBigBitmap;
	friend class CClipRect;
	friend class CCopyColourToBackBuff;
	friend class CCopyToBackBuff;
	friend class CDrawingMark;
	friend class CScreenScroll;
	friend class CLine;
	friend class CPopActive;
	friend class CPushActive;
	friend class CSolidRect;
	friend class CText;
	friend class CZrle;
	friend class CMain2DDisplay;
	friend class C2D;
	friend struct CGraphicsState;
	friend class CGWnd;
	friend class CPvGWnd;
	friend class CBaseFrontendDrawer;
	friend class CAnimsManager;
	friend class CGraphicButton;
	friend class CGunButtons;
	friend class CDepressedButton;
	friend class CFramedButton;
	friend class CTextButton;
	friend class CPvButton;
	friend class CPanelButton;
	friend class CGunController;
	friend class CBitmapRes;
	friend class CBitmapResBase;
	friend class CCdLoadAnim;
	friend class CBaseCursor;
	friend class CAnim;
	friend class CSpriteWindow;
	friend class CTrackWindow;
	friend class CHiliteWindow;
	friend class CAboutScreen;
	friend class CHiliteButtons;
	friend class CPasswordHiliteWindow;
	friend class CHiliteController;
	friend class CPasswordDrawer;
	friend class CPauseWindow;
	friend struct WinGDrawState;
	friend WinGDrawState* __stdcall WinGDrawOpen(void* p_openInfo);

private:
	CPrimitive** m_primitives; // 0x00
	int m_primitiveCount;      // 0x04
	int m_primitiveCapacity;   // 0x08
	CSurface* m_renderTarget;  // 0x0c
};

extern CPrimitive* g_pCurrentPrimitive;
#endif
