#include "DrawingMark.h"

#include "Gdi.h"
#include "VsGdi.h"

// 68K 0x10101a3c __dt__12CDrawingMarkFv
DrawingMark::~DrawingMark()
{
}

// 68K 0x101019c0 Draw__12CDrawingMarkFP4CGDI
// FUNCTION: LEMBALL 0x00439860
void DrawingMark::Draw(Gdi* p_gdi)
{
	p_gdi->AddToList(this);
}

// 68K 0x101019f4 Render__12CDrawingMarkFP4CGDI
// FUNCTION: LEMBALL 0x00439870
void DrawingMark::Render(Gdi* p_gdi)
{
	p_gdi->m_renderTarget->Blit((Point*) this);
}

