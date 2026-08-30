#ifndef LEMBALL_VISOS_GRAPHICS_VSGDI_H
#define LEMBALL_VISOS_GRAPHICS_VSGDI_H

#include "../../Common.h"
#include "../Foundation/VsRect.h" // complete type
#include "PvBackBuffSurface.h"    // complete type
#include "PvGdiBitmap.h"          // complete type
#include "PvSurface.h"            // complete type
#include "PvZBuffSurface.h"       // complete type

#pragma warning(disable : 4250)

struct SurfaceListNode {
	class Surface* m_surface;
	SurfaceListNode* m_next;
	SurfaceListNode* m_prev;
};

// SIZE 0x5a0
// PvGdiBitmap at 0. Shared PvSurface virtual base.
class Surface : public PvGdiBitmap, public PvZBuffSurface, public PvBackBuffSurface {
public:
	friend class PvButton;
	friend class GraphicButton;
	friend class GunButtons;
	friend class HiliteController;
	friend class PasswordDrawer;
	Surface(GrafPort* p_arg0);
	Surface(const VsRect& p_arg0, class Surface* p_arg1);
	bool ClipRect(VsRect& p_rect, VsRect* p_clipped);
	int ClipCircle(int p_centerX, int p_centerY, int p_radius);
	int LineClip(int& p_x1, int& p_y1, int& p_x2, int& p_y2);
	virtual void Resize(const VsSize& p_size);          // vtable+0x28
	virtual void SetLinePtrs();                         // vtable+0x00
	virtual void AddToChangeList(const VsRect& p_rect); // vtable+0x04
	virtual ChangeList* GetChangeList();                // vtable+0x08
	virtual void* GetCurrDb();                          // vtable+0x38
	void AttachPalette(ResPalette* p_palette);
	bool BeginRender();
	void EndRender();
	virtual void Blit(Zrle* p_primitive, ResZrle* p_zrle);
	virtual void Blit(Bitmap* p_primitive, ResBitmap* p_bitmap);
	virtual void Blit(BigBitmap* p_arg0, ResBitmap* p_arg1);
	virtual void Blit(Line* p_line);
	virtual void Blit(FilledCircle* p_circle);
	virtual void Blit(class ClipRect* p_arg0);
	virtual void Blit(Circle* p_circle);
	virtual void Blit(ScreenScroll* p_scroll);
	virtual void Blit(SolidRect* p_rect);
	virtual void Blit(Point* p_point);
	virtual void Blit(CopyToBackBuff* p_arg0);
	virtual void Blit(CopyColourToBackBuff* p_arg0);
	virtual void Blit(ZBuffClear* p_arg0);
	virtual void Blit(ZBuffScroll* p_arg0);
	void BlitRect(VsRect p_rect, int p_colour);
	void BlitZrle(int p_x, int p_y, ResZrle* p_zrle, unsigned int p_flags, Remap* p_remap, unsigned short p_depth);
	void BlitZrleClip(const VsRect& p_rect, const VsRect& p_clip, ResZrle* p_zrle, unsigned char p_reverse);
	void BlitZrleClipQzBuff(const VsRect& p_rect, const VsRect& p_clip, ResZrle* p_zrle, unsigned short p_depth);
	void BlitZrleClipQzBuffRemap(const VsRect& p_rect,
								 const VsRect& p_clip,
								 ResZrle* p_zrle,
								 unsigned short p_depth,
								 unsigned char* p_remap);
	void BlitZrleClipR(const VsRect& p_rect, const VsRect& p_clip, ResZrle* p_zrle, unsigned char p_reverse);
	void BlitZrleClipRemap(const VsRect& p_rect,
						   const VsRect& p_clip,
						   ResZrle* p_zrle,
						   unsigned char p_reverse,
						   unsigned char* p_remap);
	void BlitZrleClipRemapR(const VsRect& p_rect,
							const VsRect& p_clip,
							ResZrle* p_zrle,
							unsigned char p_reverse,
							unsigned char* p_remap);
	void BlitZrleClipZBuff(const VsRect& p_rect, const VsRect& p_clip, ResZrle* p_zrle, unsigned short p_depth);
	void BlitZrleClipZBuffRemap(const VsRect& p_rect,
								const VsRect& p_clip,
								ResZrle* p_zrle,
								unsigned short p_depth,
								unsigned char* p_remap);
	void BlitZrleNoClip(const VsRect& p_rect, ResZrle* p_zrle, unsigned char p_reverse);
	void BlitZrleNoClipQzBuff(const VsRect& p_rect, ResZrle* p_zrle, unsigned short p_depth);
	void BlitZrleNoClipQzBuffRemap(const VsRect& p_rect,
								   ResZrle* p_zrle,
								   unsigned short p_depth,
								   unsigned char* p_remap);
	void BlitZrleNoClipR(const VsRect& p_rect, ResZrle* p_zrle, unsigned char p_reverse);
	void BlitZrleNoClipRemap(const VsRect& p_rect, ResZrle* p_zrle, unsigned char p_reverse, unsigned char* p_remap);
	void BlitZrleNoClipRemapR(const VsRect& p_rect, ResZrle* p_zrle, unsigned char p_reverse, unsigned char* p_remap);
	void BlitZrleNoClipZBuff(const VsRect& p_rect, ResZrle* p_zrle, unsigned short p_depth);
	void BlitZrleNoClipZBuffRemap(const VsRect& p_rect,
								  ResZrle* p_zrle,
								  unsigned short p_depth,
								  unsigned char* p_remap);
	virtual void CopyBackBuffToScreen(const VsRect& p_arg0);
	void DrawClippedFilledCircle(int p_centerX, int p_centerY, int p_radius, int p_colour);
	void FilledCircleClipPoints(int p_centerX, int p_centerY, int p_xOffset, int p_yOffset, int p_colour);
	void Flush();
	void Move(const VsPoint& p_position);
	void MoveRel(const VsPoint& p_delta);
	void NewBitmap(const VsRect& p_rect);
	void ResetScroll();
	void SetDefaultCtable();
	void SetWindowPtr(void* p_platformPort);
	void ToScreen(class Surface* p_destinationSurface);
	void CopyDIBBits(void* p_header, unsigned char* p_bits);
	~Surface();

	friend class BaseFrontendDrawer;
	friend class GWnd;
	friend class PvBackBuffSurface;

private:
	undefined4 m_unk0xe8;           // 0xe8
	short m_presentX;               // 0xec
	short m_presentY;               // 0xee
	void* m_platformBitmap;         // 0xf0
	undefined4 m_unk0xf4;           // 0xf4
	void* m_drawingPort;            // 0xf8
	undefined m_bitmapInfo[0x28];   // 0xfc
	undefined m_colourTable[0x400]; // 0x124
	undefined4 m_unk0x524;          // 0x524
	SurfaceListNode* m_unk0x528;    // 0x528
	SurfaceListNode* m_unk0x52c;    // 0x52c
	unsigned int m_unk0x530;        // 0x530
	undefined m_lock[0x18];         // 0x534
	undefined4 m_unk0x54c;          // 0x54c
	ChangeList* m_changeList;       // 0x550
	void* m_currDb;                 // 0x554
	undefined4 m_unk0x558;          // 0x558
};

#endif
