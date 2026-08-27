#include "TimeBonus.h"

// 68K 0x10605fec SetSFX__10CTimeBonusFv
// FUNCTION: LEMBALL 0x00422c70
void TimeBonus::SetSfx()
{
	SetSndEffect(0x16);
}

// 68K 0x10606022 Collected__10CTimeBonusFv
// STUB: LEMBALL 0x00422c80
int TimeBonus::Collected()
{
	return 0;
}

// 68K 0x10119a16 __dt__10CTimeBonusFv
TimeBonus::~TimeBonus()
{
}

