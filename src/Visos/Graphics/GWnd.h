#ifndef LEMBALL_VISOS_GRAPHICS_GWND_H
#define LEMBALL_VISOS_GRAPHICS_GWND_H

#include "../../Common.h"
#include "Wnd.h" // complete type

struct TargetWinGDrawCodecState;

// SIZE 0x90
// VTABLE: LEMBALL 0x00499260
class GWnd : public Wnd {
public:
	virtual int ProcessOtherMessages(unsigned int p_message, unsigned int p_wParam, unsigned int p_lParam); // vtable+0xa0
	virtual void AttachPalette(unsigned long p_paletteId);                                                // vtable+0xac
	virtual void Create(const VsRect& p_rect, PvWnd* p_parent, char* p_title);                            // vtable+0x04
	virtual void Create(const VsRect& p_rect, PvWnd* p_parent, char* p_title, unsigned long p_paletteId); // vtable+0xa4
	virtual void Flush();                                                                                 // vtable+0xb4
	virtual void Move(const VsPoint& p_point);                                                            // vtable+0x38
	virtual void OnCreate();                                                                              // vtable+0x18
	virtual void OnDestroy();                                                                             // vtable+0x1c
	virtual void OnMove();                                                                                // vtable+0x28
	virtual void OnPaint(const VsRect& p_rect);                                                           // vtable+0xa8
	virtual void OnSize();                                                                                // vtable+0x20
	virtual void Refresh(VsRect* p_rect);                                                                 // vtable+0x70
	virtual void Render();                                                                                // vtable+0xb0
	~GWnd();

	friend class Main2DDisplay;
	friend class PvGWnd;
	friend class NetworkOptionsDrawer;
	friend class GraphicButton;
	friend class AnimWnd;
	friend class PvButton;
	friend class SpriteWindow;
	friend class TrackWindow;
	friend class HiliteWindow;
	friend class PasswordHiliteWindow;
	friend class GunController;
	friend class DepressedButton;
	friend TargetWinGDrawCodecState* TargetWinGDrawCodec_Open(void* p_openInfo);
	friend int TargetWinGDrawCodec_Draw(struct TargetWinGDrawCodecState* p_state, void* p_request);

private:
	Gdi* m_gdi;                       // 0x4c
	unsigned int m_gdiFlags;          // 0x50
	unsigned int m_paletteResourceId; // 0x54
	undefined m_renderState0[0x10];   // 0x58
	undefined m_renderState1[0x10];   // 0x68
	undefined m_renderState2[0x10];   // 0x78
	unsigned short m_refreshWidth;    // 0x88
	unsigned short m_refreshHeight;   // 0x8a
	unsigned short m_refreshX;        // 0x8c
	unsigned short m_refreshY;        // 0x8e
};

#endif
