#ifndef LEMBALL_VISOS_GRAPHICS_CSURFACE_H
#define LEMBALL_VISOS_GRAPHICS_CSURFACE_H

#include "../Foundation/CVsRect.h" // complete type
#include "CPvBackBuffSurface.h"    // complete type
#include "CPvScrollableSurface.h"  // complete type
#include "CPvSurface.h"            // complete type
#include "CPvZBuffSurface.h"       // complete type

class GrafPort;
class CChangeList;
class CResZrle;
class CRemap;
struct CVsPoint;

#pragma warning(disable : 4250)

struct SurfaceListNode {
	class CSurface* m_surface;
	SurfaceListNode* m_next;
	SurfaceListNode* m_prev;
};

// SIZE 0x5a0
// CPvScrollableSurface at 0. Shared CPvSurface virtual base.
// VTABLE: LEMBALL 0x00499df0 CPvSurface
// VTABLE: LEMBALL 0x00499e40 CPvBackBuffSurface
// VTABLE: LEMBALL 0x00499e50 CPvZBuffSurface
// VTABLE: LEMBALL 0x00499e58 CPvGdiBitmap
class CSurface : public CPvScrollableSurface, public CPvZBuffSurface, public CPvBackBuffSurface {
public:
	friend class CPvButton;
	friend class CGraphicButton;
	friend class CGunButtons;
	friend class CHiliteController;
	friend class CPasswordDrawer;
	CSurface(GrafPort* p_port);
	CSurface(const CVsRect& p_rect, class CSurface* p_parentSurface);
	bool ClipRect(CVsRect& p_rect, CVsRect* p_clipped);
	int ClipCircle(int p_centerX, int p_centerY, int p_radius);
	inline unsigned int ClipCode(int p_x, int p_y);
	int LineClip(int& p_x1, int& p_y1, int& p_x2, int& p_y2);
	virtual void Resize(const CVsSize& p_size);          // vtable+0x28
	virtual void SetLinePtrs();                          // vtable+0x00
	virtual void AddToChangeList(const CVsRect* p_rect); // vtable+0x04
	virtual CChangeList* GetChangeList();                // vtable+0x08
	virtual void* GetCurrDb();                           // vtable+0x38
	virtual void AttachPalette(CResPalette* p_palette);  // vtable+0x30
	bool BeginRender();
	void EndRender();
	virtual void Blit(CZrle* p_primitive, CResZrle* p_zrle);
	virtual void Blit(CBitmap* p_primitive, CResBitmap* p_bitmap);
	virtual void Blit(CBigBitmap* p_primitive, CResBitmap* p_bitmap);
	virtual void Blit(CLine* p_line);
	virtual void Blit(CFilledCircle* p_circle);
	virtual void Blit(class CClipRect* p_clipRect);
	virtual void Blit(CCircle* p_circle);
	virtual void Blit(CScreenScroll* p_scroll);
	virtual void Blit(CSolidRect* p_rect);
	virtual void Blit(CPoint* p_point);
	virtual void Blit(CCopyToBackBuff* p_copy);
	virtual void Blit(CCopyColourToBackBuff* p_fill);
	virtual void Blit(CZBuffScroll* p_scroll);
	virtual void Blit(CZBuffClear* p_clear);
	void BlitRect(CVsRect p_rect, int p_colour);
	void BlitZrle(int p_x, int p_y, CResZrle* p_zrle, unsigned int p_flags, CRemap* p_remap, unsigned short p_depth);
	void BlitZrleClip(const CVsRect& p_rect, const CVsRect& p_clip, CResZrle* p_zrle, unsigned int p_reverse);
	void BlitZrleClipQzBuff(const CVsRect& p_rect, const CVsRect& p_clip, CResZrle* p_zrle, unsigned short p_depth);
	void BlitZrleClipQzBuffRemap(const CVsRect& p_rect,
								 const CVsRect& p_clip,
								 CResZrle* p_zrle,
								 unsigned short p_depth,
								 unsigned char* p_remap);
	void BlitZrleClipR(const CVsRect& p_rect, const CVsRect& p_clip, CResZrle* p_zrle, unsigned int p_reverse);
	void BlitZrleClipRemap(const CVsRect& p_rect,
						   const CVsRect& p_clip,
						   CResZrle* p_zrle,
						   unsigned int p_reverse,
						   unsigned char* p_remap);
	void BlitZrleClipRemapR(const CVsRect& p_rect,
							const CVsRect& p_clip,
							CResZrle* p_zrle,
							unsigned int p_reverse,
							unsigned char* p_remap);
	void BlitZrleClipZBuff(const CVsRect& p_rect, const CVsRect& p_clip, CResZrle* p_zrle, unsigned short p_depth);
	void BlitZrleClipZBuffRemap(const CVsRect& p_rect,
								const CVsRect& p_clip,
								CResZrle* p_zrle,
								unsigned short p_depth,
								unsigned char* p_remap);
	void BlitZrleNoClip(const CVsRect& p_rect, CResZrle* p_zrle, unsigned int p_reverse);
	void BlitZrleNoClipQzBuff(const CVsRect& p_rect, CResZrle* p_zrle, unsigned short p_depth);
	void BlitZrleNoClipQzBuffRemap(const CVsRect& p_rect,
								   CResZrle* p_zrle,
								   unsigned short p_depth,
								   unsigned char* p_remap);
	void BlitZrleNoClipR(const CVsRect& p_rect, CResZrle* p_zrle, unsigned int p_reverse);
	void BlitZrleNoClipRemap(const CVsRect& p_rect, CResZrle* p_zrle, unsigned int p_reverse, unsigned char* p_remap);
	void BlitZrleNoClipRemapR(const CVsRect& p_rect, CResZrle* p_zrle, unsigned int p_reverse, unsigned char* p_remap);
	void BlitZrleNoClipZBuff(const CVsRect& p_rect, CResZrle* p_zrle, unsigned short p_depth);
	void BlitZrleNoClipZBuffRemap(const CVsRect& p_rect,
								  CResZrle* p_zrle,
								  unsigned short p_depth,
								  unsigned char* p_remap);
	virtual void CopyBackBuffToScreen(const CVsRect& p_rect);
	void DrawClippedFilledCircle(int p_centerX, int p_centerY, int p_radius, int p_colour);
	void FilledCircleClipPoints(int p_centerX, int p_centerY, int p_xOffset, int p_yOffset, int p_colour);
	void DrawClippedCircleOutline(int p_centerX, int p_centerY, int p_radius, unsigned char p_colour);
	void DrawCircleSpans(int p_centerX, int p_centerY, int p_xOffset, int p_yOffset, unsigned char p_colour);
	int ClipCirclePoint(int p_x, int p_y);
	void DrawClippedCirclePoint(int p_centerX, int p_centerY, int p_xOffset, int p_yOffset, unsigned char p_colour);
	void Flush();
	virtual void Move(const CVsPoint& p_position); // vtable+0x2c
	void MoveRel(const CVsPoint& p_delta);
	void NewBitmap(const CVsRect& p_rect);
	void ResetScroll();
	void SetDefaultCtable();
	void SetWindowPtr(void* p_platformPort);
	void ToScreen(class CSurface* p_destinationSurface);
	void CopyDibBits(void* p_header, unsigned char* p_bits);
	~CSurface();

	friend class CBaseFrontendDrawer;
	friend class CGWnd;
	friend class CPvBackBuffSurface;

private:
	short m_presentX;                    // 0xec
	short m_presentY;                    // 0xee
	void* m_platformBitmap;              // 0xf0
	unsigned int m_unk0xf4;              // 0xf4
	void* m_drawingPort;                 // 0xf8
	char m_bitmapInfo[0x28];             // 0xfc
	char m_colourTable[0x400];           // 0x124
	unsigned int m_bitmapPixelCount;     // 0x524
	SurfaceListNode* m_childSurfaceHead; // 0x528
	SurfaceListNode* m_childSurfaceTail; // 0x52c
	unsigned int m_childSurfaceCount;    // 0x530
	char m_lock[0x18];                   // 0x534
	unsigned int m_lockInitialized;      // 0x54c
	CChangeList* m_changeList;           // 0x550
	void* m_currDb;                      // 0x554
};

// GLOBAL: LEMBALL 0x00499dd8
// CSurface::`vbtable'{for `CPvBackBuffSurface'}

// GLOBAL: LEMBALL 0x00499de0
// CSurface::`vbtable'{for `CPvZBuffSurface'}

// GLOBAL: LEMBALL 0x00499de8
// CSurface::`vbtable'{for `CPvScrollableSurface'}

#endif
