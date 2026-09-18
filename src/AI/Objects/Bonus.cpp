#include "Bonus.h"

#include "../Navigation/Ai.h"
#include "Views/Sound/SoundEffects.h"

// 68K 0x10605f8a Collected__6CBonusFv
// FUNCTION: LEMBALL 0x00422c40
int Bonus::Collected()
{
	g_pAI->Score(1000);
	return 1;
}

// 68K 0x10605fbc SetSFX__6CBonusFv
// FUNCTION: LEMBALL 0x00422c60
void Bonus::SetSfx()
{
	SetSndEffect(SFX_YIPPEE);
}
