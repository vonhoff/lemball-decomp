#include "CBonus.h"

#include "../Navigation/CAI.h"
#include "Views/Sound/SoundEffects.h"

// FUNCTION: LEMBALL 0x00422c40
int CBonus::Collected()
{
	g_pAI->Score(1000);
	return 1;
}

// FUNCTION: LEMBALL 0x00422c60
void CBonus::SetSFX()
{
	SetSndEffect(SFX_YIPPEE);
}
