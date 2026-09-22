#include "CTimeBonus.h"

#include "../Navigation/CAI.h"
#include "Views/Sound/SoundEffects.h"

// FUNCTION: LEMBALL 0x00422c70
void CTimeBonus::SetSFX()
{
	SetSndEffect(SFX_TIMBONUS);
}

// FUNCTION: LEMBALL 0x00422c80
int CTimeBonus::Collected()
{
	g_pAI->Score(0xfa);
	g_pAI->AddTime(0x1e);
	return 1;
}
