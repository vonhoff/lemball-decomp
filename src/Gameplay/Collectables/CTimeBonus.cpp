#include "CTimeBonus.h"

#include "Gameplay/Simulation/AIScoreConstants.h"
#include "Gameplay/Simulation/CAI.h"
#include "Application/SoundEffects.h"

enum {
	TIME_BONUS_ADDED_SECONDS = 30
};

// FUNCTION: LEMBALL 0x00422c70
void CTimeBonus::SetSFX()
{
	SetSndEffect(SFX_TIMBONUS);
}

// FUNCTION: LEMBALL 0x00422c80
int CTimeBonus::Collected()
{
	g_pAI->Score(AI_SCORE_TIME_BONUS_PICKUP_POINTS);
	g_pAI->AddTime(TIME_BONUS_ADDED_SECONDS);
	return 1;
}
