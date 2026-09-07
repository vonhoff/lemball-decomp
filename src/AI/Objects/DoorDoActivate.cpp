#include "../../Control/Game/Game.h"
#include "../../Control/Game/GameTime.h"
#include "../Navigation/Ai.h"
#include "Door.h"

// 68K 0x10606be8 DoActivate__5CDoorFv
// FUNCTION: LEMBALL 0x0040dec0
void Door::DoActivate()
{
	m_activationPending = 1;
	m_stateTimer = g_dwSimulationTimestamp;
	m_actionDeadline += g_dwGameTick;
	if (m_action != (eAction) 0x1c) {
		int actionArgument = (unsigned short) m_actionArgument;
		int score;
		switch (actionArgument) {
		case 0x14:
			score = 0x19;
			break;
		case 0x15:
		case 0x16:
		case 0x17:
			score = 0x4b;
			break;
		default:
			score = 0x19;
			break;
		}
		g_pAI->Score(score);
	}
}
