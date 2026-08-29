#include "ResEffect.h"

#include "MogRes.h"

// 68K 0x102047a8 Load__10CResEFFECTFUl
// FUNCTION: LEMBALL 0x0045e380
ResEffect* ResEffect::Load(unsigned int p_resourceId)
{
	ResEffect* res = (ResEffect*) g_pActiveMogRes->Find(p_resourceId);
	if (!res) {
		res = new ResEffect(p_resourceId);
		return (ResEffect*) res->CheckError();
	}
	if (res->m_chunkType != 0x45464620) {
		res->UnLoad();
		return 0;
	}
	return res;
}

// 68K 0x1010102e SetType__10CResEFFECTFv
// FUNCTION: LEMBALL 0x0045eba0
void ResEffect::SetType()
{
	m_chunkType = 0x45464620;
}

// 68K 0x1010105c __dt__10CResEFFECTFv
ResEffect::~ResEffect()
{
}

