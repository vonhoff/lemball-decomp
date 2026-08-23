#include "Bitmap.h"

#include "Gdi.h"
#include "VsGdi.h"

// 68K 0x1011c0c4 __ct__7CBitmapFv
// FUNCTION: LEMBALL 0x00439580
Bitmap::Bitmap()
	: m_y(0), m_x(0), m_height(0), m_width(0), m_sourceY(0), m_sourceX(0)
{
}

// 68K 0x10100a30 __dt__7CBitmapFv
// SYNTHETIC: LEMBALL 0x004396e0
// Bitmap::`scalar deleting destructor'
Bitmap::~Bitmap()
{
}

// 68K 0x10101598 Draw__7CBitmapFP4CGDI
// FUNCTION: LEMBALL 0x004398a0
void Bitmap::Draw(Gdi* p_gdi)
{
	p_gdi->AddToList(this);
}

// 68K 0x101015f6 Render__7CBitmapFP4CGDI
// FUNCTION: LEMBALL 0x004398b0
void Bitmap::Render(Gdi* p_gdi)
{
	p_gdi->m_renderTarget->Blit(this);
}
