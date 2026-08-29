#include "Ammo.h"

#include "../../Control/Game/Game.h"
#include "../../Control/Game/GameTime.h"
#include "../Navigation/Ai.h"

// 68K 0x1011a9fe Usage__5CAmmoFv
// FUNCTION: LEMBALL 0x0041c430
int Ammo::Usage()
{
	return 2;
}

// 68K 0x1011aa1a __dt__5CAmmoFv
Ammo::~Ammo()
{
}

// 68K 0x10618ede Restart__5CAmmoFv
// FUNCTION: LEMBALL 0x0041ca90
void Ammo::Restart()
{
	BaseGlobalObject::Restart();
	m_ammo = 0;
}

// 68K 0x10618f0a Process__5CAmmoFv
// STUB: LEMBALL 0x0041cab0
bool Ammo::Process()
{
	return 0;
}

// 68K 0x10619030 Activate__5CAmmoFP11CGameObject
// STUB: LEMBALL 0x0041cbe0
bool Ammo::Activate(GameObject* p_object)
{
	return 0;
}

// 68K 0x106190a4 DoActivate__5CAmmoFv
// FUNCTION: LEMBALL 0x0041cc30
void Ammo::DoActivate()
{
	m_stateTimer = g_dwSimulationTimestamp;
	m_unk0xd4 += g_dwGameTick;
	SetSndEffect((eSoundEffect) 0x11);
	m_activator->PickUpAmmo(25);
	g_pAI->Score(50);
}

// 68K 0x10619118 ActivatePosition__5CAmmoFv
// STUB: LEMBALL 0x0041cc70
AiCoord Ammo::ActivatePosition()
{
	return *(AiCoord*) 0;
}

