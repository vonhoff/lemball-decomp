#include "Crate.h"
#include "../../Control/Game/Game.h"
#include "../../Control/Game/GameTime.h"
#include "../Navigation/Ai.h"

// 68K 0x1011ab0e Usage__6CCrateFv
// FUNCTION: LEMBALL 0x0041c530
int Crate::Usage()
{
	return 2;
}

// 68K 0x1011aa7e __dt__6CCrateFv
Crate::~Crate()
{
}

// 68K 0x1061918c Restart__6CCrateFv
// FUNCTION: LEMBALL 0x0041cca0
void Crate::Restart()
{
	BaseGlobalObject::Restart();
	m_pendingAction = (eAction) 24;
}

// 68K 0x106191bc TriggerContents__6CCrateFv
// STUB: LEMBALL 0x0041ccc0
void Crate::TriggerContents()
{
}

// 68K 0x10619230 Process__6CCrateFv
// STUB: LEMBALL 0x0041cd20
bool Crate::Process()
{
	return 0;
}

// 68K 0x10619384 Activate__6CCrateFP11CGameObject
// FUNCTION: LEMBALL 0x0041ce50
bool Crate::Activate(GameObject* p_object)
{
	if (m_action == 24) {
		m_unk0xd0 = 16;
		m_unk0xd4 = 30;
		RequestAction((eAction) 25);
		return 1;
	}
	return 0;
}

// 68K 0x106193e0 DoActivate__6CCrateFv
// FUNCTION: LEMBALL 0x0041ce90
void Crate::DoActivate()
{
	m_stateTimer = g_dwSimulationTimestamp;
	m_unk0xd0 += g_dwGameTick;
	m_unk0xd4 += g_dwGameTick;
	SetSndEffect((eSoundEffect) 20);
	int time;
	if (m_contentsType <= 23) {
		if (m_contentsType >= 21) {
			time = 50;
		} else if (m_contentsType == 4) {
			time = 100;
		}
	} else if (m_contentsType == 0xffff) {
		time = 25;
	}
	g_pAI->AddTime(time);
}

// 68K 0x1061947a ActivatePosition__6CCrateFv
// FUNCTION: LEMBALL 0x0041cf10
AiCoord Crate::ActivatePosition()
{
	int x = m_position.m_xFixed;
	int y = m_position.m_yFixed;
	int z = m_position.m_zFixed;
	if ((m_contentsType >= 21 && m_contentsType <= 23) || m_contentsType == 0xffff) {
		x -= 0x8000;
		return AiCoord(x, y, z);
	}
	x -= 0x30000;
	y -= 0x8000;
	return AiCoord(x, y, z);
}

