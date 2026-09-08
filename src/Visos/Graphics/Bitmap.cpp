#include "Bitmap.h"

#include "CopyToBackBuff.h"
#include "Gdi.h"
#include "VsGdi.h"

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
	p_gdi->m_renderTarget->Blit((CopyToBackBuff*) this);
}
