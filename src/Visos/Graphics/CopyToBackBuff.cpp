#include "CopyToBackBuff.h"

#include "Gdi.h"
#include "VsGdi.h"

// 68K 0x1010a668 __ct__15CCopyToBackBuffFv
// FUNCTION: LEMBALL 0x00439550
CopyToBackBuff::CopyToBackBuff()
	: m_field0a(0), m_field08(0), m_field0e(0), m_field0c(0)
{
}

// 68K 0x10100c0e __dt__15CCopyToBackBuffFv
// SYNTHETIC: LEMBALL 0x004396b0
// CopyToBackBuff::`scalar deleting destructor'
CopyToBackBuff::~CopyToBackBuff()
{
}

// 68K 0x10101efe Draw__15CCopyToBackBuffFP4CGDI
// FUNCTION: LEMBALL 0x00439900
void CopyToBackBuff::Draw(Gdi* p_gdi)
{
	p_gdi->AddToList(this);
}

// 68K 0x10101f36 Render__15CCopyToBackBuffFP4CGDI
// FUNCTION: LEMBALL 0x00439910
void CopyToBackBuff::Render(Gdi* p_gdi)
{
	p_gdi->m_renderTarget->Blit(this);
}
