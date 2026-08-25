#include "BaseRemap.h"

#include "../Resources/ResBase.h"
#include "../Resources/ResPalette.h"

#include <new.h>

BaseRemap::BaseRemap()
{
}

// 68K 0x10200920 __ct__10CBaseRemapFUlPUc13ePaletteTypes
// FUNCTION: LEMBALL 0x0046aa80
BaseRemap::BaseRemap(unsigned long p_arg0, unsigned char* p_arg1, ePaletteTypes p_arg2)
{
	m_paletteResource = ResPalette::Load(p_arg0);
	if (p_arg2 == 1) {
		CalculateGreyScale();
		return;
	}
	if (p_arg2 != 2) {
		m_remap = p_arg1;
		return;
	}
	MapRemap(p_arg1);
}

// 68K 0x1020099e __dt__10CBaseRemapFv
// FUNCTION: LEMBALL 0x0046aad0
BaseRemap::~BaseRemap()
{
	if (m_remap != 0) {
		operator delete(m_remap);
	}
	if (m_paletteResource != 0) {
		((ResBase*) m_paletteResource)->UnLoad();
	}
}

// 68K 0x102009fc MapRemap__10CBaseRemapFPCUc
// STUB: LEMBALL 0x0046aaf0
void BaseRemap::MapRemap(const unsigned char* p_mapping)
{
}

// 68K 0x10200aae CalculateGreyScale__10CBaseRemapFv
// STUB: LEMBALL 0x0046ab70
void BaseRemap::CalculateGreyScale()
{
}
