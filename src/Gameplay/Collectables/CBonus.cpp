#include "CBonus.h"

#include "Application/SoundEffects.h"
#include "Gameplay/Simulation/AIScoreConstants.h"
#include "Gameplay/Simulation/CAI.h"

// FUNCTION: LEMBALL 0x00422c40
int CBonus::Collected()
{
	g_pAI->Score(AI_SCORE_BONUS_PICKUP_POINTS);
	return 1;
}

// FUNCTION: LEMBALL 0x00422c60
void CBonus::SetSFX()
{
	SetSndEffect(SFX_YIPPEE);
}
