#ifndef LEMBALL_VISOS_GRAPHICS_VSGDI_H
#define LEMBALL_VISOS_GRAPHICS_VSGDI_H

#include "../../Common.h"
#include "../Foundation/VsRect.h" // complete type
#include "PvBackBuffSurface.h"    // complete type
#include "PvGdiBitmap.h"          // complete type
#include "PvSurface.h"            // complete type
#include "PvZBuffSurface.h"       // complete type

// SIZE 0x5a0
class Surface : public virtual PvSurface, public PvGdiBitmap, public PvZBuffSurface, public PvBackBuffSurface {
public:
	ChangeList* GetChangeList();
	Surface(GrafPort* p_arg0);
	Surface(const VsRect& p_arg0, class Surface* p_arg1);
	bool ClipRect(VsRect& p_rect, VsRect* p_clipped);
	int ClipCircle(int p_centerX, int p_centerY, int p_radius);
	int LineClip(int& p_x1, int& p_y1, int& p_x2, int& p_y2);
	virtual void Resize(const VsSize& p_size); // vtable+0x28
	virtual void SetLinePtrs();                // vtable+0x00
	virtual void* GetCurrDb();                 // vtable+0x38
	void AddToChangeList(const VsRect& p_rect);
	void AttachPalette(ResPalette* p_palette);
	void Blit(BigBitmap* p_arg0, ResBitmap* p_arg1);
	virtual void Blit(Bitmap* p_primitive);
	void Blit(Bitmap* p_primitive, ResBitmap* p_bitmap);
	void Blit(Circle* p_circle);
	virtual void Blit(CopyColourToBackBuff* p_arg0);
	virtual void Blit(CopyToBackBuff* p_arg0);
	void Blit(FilledCircle* p_circle);
	void Blit(Line* p_line);
	void Blit(Point* p_point);
	void Blit(ScreenScroll* p_scroll);
	void Blit(SolidRect* p_rect);
	void Blit(ZBuffClear* p_arg0);
	void Blit(ZBuffScroll* p_arg0);
	void Blit(Zrle* p_primitive, ResZrle* p_zrle);
	void Blit(class ClipRect* p_arg0);
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
	void CopyBackBuffToScreen(const VsRect& p_arg0);
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
	~Surface();

private:
	undefined m_surfaceState[0x46c]; // 0xf0
};

#endif
