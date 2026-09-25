#ifndef LEMBALL_VISOS_GRAPHICS_CSURFACE_H
#define LEMBALL_VISOS_GRAPHICS_CSURFACE_H

#include "../Foundation/CVsRect.h"
#include "CPVBackBuffSurface.h"
#include "CPVScrollableSurface.h"
#include "CPVSurface.h"
#include "CPVZBuffSurface.h"

class GrafPort;
class CChangeList;
class CResZRLE;
class CRemap;
struct CVsPoint;

#pragma warning(disable : 4250)

struct SurfaceListNode {
	class CSurface* m_surface;
	SurfaceListNode* m_next;
	SurfaceListNode* m_prev;
};

// SIZE 0x5a0
// VTABLE: LEMBALL 0x00499df0 CPVSurface
// VTABLE: LEMBALL 0x00499e40 CPVBackBuffSurface
// VTABLE: LEMBALL 0x00499e50 CPVZBuffSurface
// VTABLE: LEMBALL 0x00499e58 CPVGDIBitmap
class CSurface : public CPVScrollableSurface, public CPVZBuffSurface, public CPVBackBuffSurface {
public:
	friend class CPVButton;
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
	virtual void* GetCurrDB();                           // vtable+0x38
	virtual void AttachPalette(CResPALETTE* p_palette);  // vtable+0x30
	bool BeginRender();
	void EndRender();
	virtual void Blit(CZRLE* p_primitive, CResZRLE* p_zrle);
	virtual void Blit(CBitmap* p_primitive, CResBITMAP* p_bitmap);
	virtual void Blit(CBigBitmap* p_primitive, CResBITMAP* p_bitmap);
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
	void BlitZRLE(int p_x, int p_y, CResZRLE* p_zrle, unsigned int p_flags, CRemap* p_remap, unsigned short p_depth);
	void BlitZRLEClip(const CVsRect& p_rect, const CVsRect& p_clip, CResZRLE* p_zrle, unsigned int p_reverse);
	void BlitZRLEClipQZBuff(const CVsRect& p_rect, const CVsRect& p_clip, CResZRLE* p_zrle, unsigned short p_depth);
	void BlitZRLEClipQZBuffRemap(const CVsRect& p_rect,
								 const CVsRect& p_clip,
								 CResZRLE* p_zrle,
								 unsigned short p_depth,
								 unsigned char* p_remap);
	void BlitZRLEClipR(const CVsRect& p_rect, const CVsRect& p_clip, CResZRLE* p_zrle, unsigned int p_reverse);
	void BlitZRLEClipRemap(const CVsRect& p_rect,
						   const CVsRect& p_clip,
						   CResZRLE* p_zrle,
						   unsigned int p_reverse,
						   unsigned char* p_remap);
	void BlitZRLEClipRemapR(const CVsRect& p_rect,
							const CVsRect& p_clip,
							CResZRLE* p_zrle,
							unsigned int p_reverse,
							unsigned char* p_remap);
	void BlitZRLEClipZBuff(const CVsRect& p_rect, const CVsRect& p_clip, CResZRLE* p_zrle, unsigned short p_depth);
	void BlitZRLEClipZBuffRemap(const CVsRect& p_rect,
								const CVsRect& p_clip,
								CResZRLE* p_zrle,
								unsigned short p_depth,
								unsigned char* p_remap);
	void BlitZRLENoClip(const CVsRect& p_rect, CResZRLE* p_zrle, unsigned int p_reverse);
	void BlitZRLENoClipQZBuff(const CVsRect& p_rect, CResZRLE* p_zrle, unsigned short p_depth);
	void BlitZRLENoClipQZBuffRemap(const CVsRect& p_rect,
								   CResZRLE* p_zrle,
								   unsigned short p_depth,
								   unsigned char* p_remap);
	void BlitZRLENoClipR(const CVsRect& p_rect, CResZRLE* p_zrle, unsigned int p_reverse);
	void BlitZRLENoClipRemap(const CVsRect& p_rect, CResZRLE* p_zrle, unsigned int p_reverse, unsigned char* p_remap);
	void BlitZRLENoClipRemapR(const CVsRect& p_rect, CResZRLE* p_zrle, unsigned int p_reverse, unsigned char* p_remap);
	void BlitZRLENoClipZBuff(const CVsRect& p_rect, CResZRLE* p_zrle, unsigned short p_depth);
	void BlitZRLENoClipZBuffRemap(const CVsRect& p_rect,
								  CResZRLE* p_zrle,
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
	friend class CPVBackBuffSurface;

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
// CSurface::`vbtable'{for `CPVBackBuffSurface'}

// GLOBAL: LEMBALL 0x00499de0
// CSurface::`vbtable'{for `CPVZBuffSurface'}

// GLOBAL: LEMBALL 0x00499de8
// CSurface::`vbtable'{for `CPVScrollableSurface'}

#endif
