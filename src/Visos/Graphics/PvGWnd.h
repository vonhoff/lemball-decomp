#ifndef LEMBALL_VISOS_GRAPHICS_PVGWND_H
#define LEMBALL_VISOS_GRAPHICS_PVGWND_H

#include "../../Common.h"
#include "Bitmap.h"         // complete type
#include "CopyToBackBuff.h" // complete type
#include "Line.h"           // complete type
#include "Wnd.h"            // complete type

struct TargetWinGDrawCodecState;

// SIZE 0x90
// VTABLE: LEMBALL 0x00499260
class PvGWnd : public Wnd {
public:
	virtual void Create(const VsRect& p_rect,
						PvWnd* p_parent,
						char* p_title,
						unsigned long p_paletteId) = 0;        // vtable+0xa4
	virtual void OnPaint(const VsRect& p_rect) = 0;            // vtable+0xa8
	virtual void AttachPalette(unsigned long p_paletteId) = 0; // vtable+0xac
	virtual void Render() = 0;                                 // vtable+0xb0
	virtual void Flush() = 0;                                  // vtable+0xb4
	virtual unsigned int GetStyle();                           // vtable+0x64
	virtual void OnVisibilityChange();                         // vtable+0x80
	virtual void _OnZoom(int p_oldZoom);                       // vtable+0x2c
	virtual void SetDontUpdateRect(const VsRect& p_rect);      // vtable+0x84
	virtual void SetInnerWindow(const VsRect& p_rect);         // vtable+0x08
	virtual void OnCreate() = 0;                               // vtable+0x18
	virtual void BaseOnDestroy() = 0;                          // vtable+0x1c
	virtual void _OnMove() = 0;                                // vtable+0x28
	virtual void _OnSize() = 0;                                // vtable+0x20
	void Clear(int p_color);
	~PvGWnd() {}

	friend class GWnd;
	friend class Main2DDisplay;
	friend class NetworkOptionsDrawer;
	friend class GraphicButton;
	friend class AnimWnd;
	friend class PvButton;
	friend class SpriteWindow;
	friend class TrackWindow;
	friend class HiliteWindow;
	friend class PasswordHiliteWindow;
	friend class HiliteController;
	friend class PasswordDrawer;
	friend class GunController;
	friend class GunButtons;
	friend class HiliteButtons;
	friend class DepressedButton;
	friend class FrontendResourceLoader;
	friend class CdLoadAnim;
	friend class BaseCursor;
	friend TargetWinGDrawCodecState* __stdcall TargetWinGDrawCodec_Open(void* p_openInfo);
	friend int __stdcall TargetWinGDrawCodec_Draw(struct TargetWinGDrawCodecState* p_state,
												  void* p_request,
												  long p_param2);

private:
	Gdi* m_gdi;                       // 0x4c
	unsigned int m_gdiFlags;          // 0x50
	unsigned int m_paletteResourceId; // 0x54
	Line m_renderState0[1];           // 0x58
	Bitmap m_renderState1[1];         // 0x68
	CopyToBackBuff m_renderState2[1]; // 0x78
	unsigned short m_refreshWidth;    // 0x88
	unsigned short m_refreshHeight;   // 0x8a
	unsigned short m_refreshX;        // 0x8c
	unsigned short m_refreshY;        // 0x8e
};

#endif
