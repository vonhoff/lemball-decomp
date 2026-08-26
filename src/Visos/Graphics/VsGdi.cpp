#include "VsGdi.h"

// 68K 0x1010830c __ct__8CSurfaceFRC7CVSRectP8CSurface
// STUB: LEMBALL 0x0046c050
Surface::Surface(const VsRect& p_arg0, class Surface* p_arg1)
{
}

// 68K 0x10109048 __ct__8CSurfaceFP8GrafPort
// STUB: LEMBALL 0x0046c5d0
Surface::Surface(GrafPort* p_arg0)
{
}

// 68K 0x10109200 __dt__8CSurfaceFv
// STUB: LEMBALL 0x0046c710
Surface::~Surface()
{
}

// 68K 0x10108bf2 ResetScroll__8CSurfaceFv
// STUB: LEMBALL 0x0046c990
void Surface::ResetScroll()
{
}

// 68K 0x10108c8a SetLinePtrs__8CSurfaceFv
// STUB: LEMBALL 0x0046c9f0
void Surface::SetLinePtrs()
{
}

// 68K 0x10109466 AddToChangeList__8CSurfaceFRC7CVSRect
// STUB: LEMBALL 0x0046cb20
void Surface::AddToChangeList(const VsRect& p_rect)
{
}

// 68K 0x1010956c GetChangeList__8CSurfaceFv
// STUB: LEMBALL 0x0046cbd0
ChangeList* Surface::GetChangeList()
{
	return 0;
}

// 68K 0x1010959a Blit__8CSurfaceFP9CClipRect
// STUB: LEMBALL 0x0046cbe0
void Surface::Blit(class ClipRect* p_arg0)
{
}

// 68K 0x10109e1e ToScreen__8CSurfaceFP8CSurface
// STUB: LEMBALL 0x0046cda0
void Surface::ToScreen(class Surface* p_destinationSurface)
{
}

// 68K 0x10108fa4 AttachPalette__8CSurfaceFP11CResPALETTE
// STUB: LEMBALL 0x0046d040
void Surface::AttachPalette(ResPalette* p_palette)
{
}

// 68K 0x10108642 NewBitmap__8CSurfaceFRC7CVSRect
// ASSERT: _VSRELassert("AllocatedBitmap", "VSGDI.CPP", 736)
// STUB: LEMBALL 0x0046d090
void Surface::NewBitmap(const VsRect& p_rect)
{
}

// 68K 0x10109fc0 Resize__8CSurfaceFRC7CVSSize
// STUB: LEMBALL 0x0046d420
void Surface::Resize(const VsSize& p_size)
{
}

// 68K 0x1010a0fe MoveRel__8CSurfaceFRC8CVSPoint
// STUB: LEMBALL 0x0046d560
void Surface::MoveRel(const VsPoint& p_delta)
{
}

// 68K 0x1010a166 Move__8CSurfaceFRC8CVSPoint
// STUB: LEMBALL 0x0046d5b0
void Surface::Move(const VsPoint& p_position)
{
}

// 68K 0x1010858c SetWindowPtr__8CSurfaceFP8GrafPort
// STUB: LEMBALL 0x0046d7e0
void Surface::SetWindowPtr(void* p_platformPort)
{
}

// 68K 0x10108e58 SetDefaultCtable__8CSurfaceFv
// STUB: LEMBALL 0x0046d930
void Surface::SetDefaultCtable()
{
}

// STUB: LEMBALL 0x0046d9f0
bool Surface::BeginRender()
{
	return 0;
}

// STUB: LEMBALL 0x0046daa0
void Surface::EndRender()
{
}

// 68K 0x1010171a Blit__8CSurfaceFP10CBigBitmapP10CResBITMAP
// STUB: LEMBALL 0x0046dbc0
void Surface::Blit(BigBitmap* p_arg0, ResBitmap* p_arg1)
{
}

// 68K 0x10105df8 Flush__8CSurfaceFv
// STUB: LEMBALL 0x0046dc50
void Surface::Flush()
{
}

// 68K 0x10105dce GetCurrDB__8CSurfaceFv
// STUB: LEMBALL 0x0046dc80
void* Surface::GetCurrDb()
{
	return 0;
}

// 68K 0x10111b02 Blit__8CSurfaceFP13CScreenScroll
// STUB: LEMBALL 0x00474c20
void Surface::Blit(ScreenScroll* p_scroll)
{
}

// 68K 0x10111c76 Blit__8CSurfaceFP11CZBuffClear
// STUB: LEMBALL 0x00474d40
void Surface::Blit(ZBuffClear* p_arg0)
{
}

// 68K 0x10111d0c Blit__8CSurfaceFP12CZBuffScroll
// STUB: LEMBALL 0x00474dc0
void Surface::Blit(ZBuffScroll* p_arg0)
{
}

// 68K 0x10105b48 Blit__8CSurfaceFP15CCopyToBackBuff
// STUB: LEMBALL 0x00474dd0
void Surface::Blit(CopyToBackBuff* p_arg0)
{
}

// 68K 0x10105be0 Blit__8CSurfaceFP21CCopyColourToBackBuff
// STUB: LEMBALL 0x00474e60
void Surface::Blit(CopyColourToBackBuff* p_arg0)
{
}

// 68K 0x10105c6c CopyBackBuffToScreen__8CSurfaceFRC7CVSRect
// STUB: LEMBALL 0x00474ee0
void Surface::CopyBackBuffToScreen(const VsRect& p_arg0)
{
}

// 68K 0x10111d36 Blit__8CSurfaceFP6CPoint
// STUB: LEMBALL 0x00474fd0
void Surface::Blit(Point* p_point)
{
}

// 68K 0x10111e5a Blit__8CSurfaceFP10CSolidRect
// STUB: LEMBALL 0x00475080
void Surface::Blit(SolidRect* p_rect)
{
}

// 68K 0x10111fca Blit__8CSurfaceFP5CLine
// STUB: LEMBALL 0x004750c0
void Surface::Blit(Line* p_line)
{
}

// 68K 0x10112604 Blit__8CSurfaceFP7CCircle
// STUB: LEMBALL 0x00475290
void Surface::Blit(Circle* p_circle)
{
}

// 68K 0x101132f4 Blit__8CSurfaceFP13CFilledCircle
// STUB: LEMBALL 0x00475490
void Surface::Blit(FilledCircle* p_circle)
{
}

// 68K 0x10111ee8 BlitRect__8CSurfaceF7CVSRecti
// STUB: LEMBALL 0x004756e0
void Surface::BlitRect(VsRect p_rect, int p_colour)
{
}

// 68K 0x101121bc LineClip__8CSurfaceFRiRiRiRi
// STUB: LEMBALL 0x004757a0
int Surface::LineClip(int& p_x1, int& p_y1, int& p_x2, int& p_y2)
{
	return 0;
}

// 68K 0x10113178 ClipCircle__8CSurfaceFiii
// STUB: LEMBALL 0x00475bc0
int Surface::ClipCircle(int p_centerX, int p_centerY, int p_radius)
{
	return 0;
}

// 68K 0x1011355a DrawClippedFilledCircle__8CSurfaceFiiii
// STUB: LEMBALL 0x00476190
void Surface::DrawClippedFilledCircle(int p_centerX, int p_centerY, int p_radius, int p_colour)
{
}

// 68K 0x1011387e FilledCircleClipPoints__8CSurfaceFiiiii
// STUB: LEMBALL 0x00476470
void Surface::FilledCircleClipPoints(int p_centerX, int p_centerY, int p_xOffset, int p_yOffset, int p_colour)
{
}

// 68K 0x10113bf4 ClipRect__8CSurfaceFR7CVSRectP7CVSRect
// STUB: LEMBALL 0x00476580
bool Surface::ClipRect(VsRect& p_rect, VsRect* p_clipped)
{
	return 0;
}

// 68K 0x1010519e BlitZRLEClip__8CSurfaceFRC7CVSRectRC7CVSRectP8CResZRLEUc
// STUB: LEMBALL 0x004766f0
void Surface::BlitZrleClip(const VsRect& p_rect, const VsRect& p_clip, ResZrle* p_zrle, unsigned char p_reverse)
{
}

// 68K 0x1010539e BlitZRLEClipZBuff__8CSurfaceFRC7CVSRectRC7CVSRectP8CResZRLEUs
// STUB: LEMBALL 0x00476910
void Surface::BlitZrleClipZBuff(const VsRect& p_rect, const VsRect& p_clip, ResZrle* p_zrle, unsigned short p_depth)
{
}

// 68K 0x10105606 BlitZRLEClipQZBuff__8CSurfaceFRC7CVSRectRC7CVSRectP8CResZRLEUs
// STUB: LEMBALL 0x00476bf0
void Surface::BlitZrleClipQzBuff(const VsRect& p_rect, const VsRect& p_clip, ResZrle* p_zrle, unsigned short p_depth)
{
}

// 68K 0x10113e7a BlitZRLEClipR__8CSurfaceFRC7CVSRectRC7CVSRectP8CResZRLEUc
// STUB: LEMBALL 0x00476ee0
void Surface::BlitZrleClipR(const VsRect& p_rect, const VsRect& p_clip, ResZrle* p_zrle, unsigned char p_reverse)
{
}

// 68K 0x10104cd8 BlitZRLENoClip__8CSurfaceFRC7CVSRectP8CResZRLEUc
// STUB: LEMBALL 0x00477130
void Surface::BlitZrleNoClip(const VsRect& p_rect, ResZrle* p_zrle, unsigned char p_reverse)
{
}

// 68K 0x10104e4c BlitZRLENoClipZBuff__8CSurfaceFRC7CVSRectP8CResZRLEUs
// STUB: LEMBALL 0x00477200
void Surface::BlitZrleNoClipZBuff(const VsRect& p_rect, ResZrle* p_zrle, unsigned short p_depth)
{
}

// 68K 0x10114162 BlitZRLENoClipZBuffRemap__8CSurfaceFRC7CVSRectP8CResZRLEUsPUc
// STUB: LEMBALL 0x00477310
void Surface::BlitZrleNoClipZBuffRemap(const VsRect& p_rect,
									   ResZrle* p_zrle,
									   unsigned short p_depth,
									   unsigned char* p_remap)
{
}

// 68K 0x10104fa6 BlitZRLENoClipQZBuff__8CSurfaceFRC7CVSRectP8CResZRLEUs
// STUB: LEMBALL 0x00477440
void Surface::BlitZrleNoClipQzBuff(const VsRect& p_rect, ResZrle* p_zrle, unsigned short p_depth)
{
}

// 68K 0x101142ac BlitZRLENoClipQZBuffRemap__8CSurfaceFRC7CVSRectP8CResZRLEUsPUc
// STUB: LEMBALL 0x00477540
void Surface::BlitZrleNoClipQzBuffRemap(const VsRect& p_rect,
										ResZrle* p_zrle,
										unsigned short p_depth,
										unsigned char* p_remap)
{
}

// 68K 0x101143ee BlitZRLENoClipR__8CSurfaceFRC7CVSRectP8CResZRLEUc
// STUB: LEMBALL 0x00477660
void Surface::BlitZrleNoClipR(const VsRect& p_rect, ResZrle* p_zrle, unsigned char p_reverse)
{
}

// 68K 0x101144ee BlitZRLEClipRemap__8CSurfaceFRC7CVSRectRC7CVSRectP8CResZRLEUcPUc
// STUB: LEMBALL 0x00477740
void Surface::BlitZrleClipRemap(const VsRect& p_rect,
								const VsRect& p_clip,
								ResZrle* p_zrle,
								unsigned char p_reverse,
								unsigned char* p_remap)
{
}

// 68K 0x101147c6 BlitZRLEClipZBuffRemap__8CSurfaceFRC7CVSRectRC7CVSRectP8CResZRLEUsPUc
// STUB: LEMBALL 0x004779d0
void Surface::BlitZrleClipZBuffRemap(const VsRect& p_rect,
									 const VsRect& p_clip,
									 ResZrle* p_zrle,
									 unsigned short p_depth,
									 unsigned char* p_remap)
{
}

// 68K 0x10114ab8 BlitZRLEClipQZBuffRemap__8CSurfaceFRC7CVSRectRC7CVSRectP8CResZRLEUsPUc
// STUB: LEMBALL 0x00477c60
void Surface::BlitZrleClipQzBuffRemap(const VsRect& p_rect,
									  const VsRect& p_clip,
									  ResZrle* p_zrle,
									  unsigned short p_depth,
									  unsigned char* p_remap)
{
}

// 68K 0x10114e22 BlitZRLEClipRemapR__8CSurfaceFRC7CVSRectRC7CVSRectP8CResZRLEUcPUc
// STUB: LEMBALL 0x00477f50
void Surface::BlitZrleClipRemapR(const VsRect& p_rect,
								 const VsRect& p_clip,
								 ResZrle* p_zrle,
								 unsigned char p_reverse,
								 unsigned char* p_remap)
{
}

// 68K 0x10115124 BlitZRLENoClipRemap__8CSurfaceFRC7CVSRectP8CResZRLEUcPUc
// STUB: LEMBALL 0x004781e0
void Surface::BlitZrleNoClipRemap(const VsRect& p_rect,
								  ResZrle* p_zrle,
								  unsigned char p_reverse,
								  unsigned char* p_remap)
{
}

// 68K 0x10115230 BlitZRLENoClipRemapR__8CSurfaceFRC7CVSRectP8CResZRLEUcPUc
// STUB: LEMBALL 0x004782d0
void Surface::BlitZrleNoClipRemapR(const VsRect& p_rect,
								   ResZrle* p_zrle,
								   unsigned char p_reverse,
								   unsigned char* p_remap)
{
}

// 68K 0x1011534a Blit__8CSurfaceFP5CZRLEP8CResZRLE
// STUB: LEMBALL 0x004783c0
void Surface::Blit(Zrle* p_primitive, ResZrle* p_zrle)
{
}

// 68K 0x10115868 Blit__8CSurfaceFP7CBitmapP10CResBITMAP
// STUB: LEMBALL 0x004787f0
void Surface::Blit(Bitmap* p_primitive, ResBitmap* p_bitmap)
{
}

// 68K 0x101153f4 BlitZRLE__8CSurfaceFiiP8CResZRLEUlP6CRemapUs
// STUB: LEMBALL 0x00478bb0
void Surface::BlitZrle(int p_x, int p_y, ResZrle* p_zrle, unsigned int p_flags, Remap* p_remap, unsigned short p_depth)
{
}

void Surface::Blit(Bitmap* p_primitive)
{
}
