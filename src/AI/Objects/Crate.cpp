#include "Crate.h"

#include "../../Control/Game/Game.h"
#include "../../Control/Game/GameTime.h"
#include "../Navigation/Ai.h"

// FUNCTION: LEMBALL 0x0041c470
Crate::Crate(const AiCoord& p_position, GlobalGameObject* p_contents, unsigned short p_contentsId)
	: BaseGlobalObject(p_position, 0x11)
{
	m_contentsId = p_contentsId;
	m_contents = p_contents;
	if (p_contents == 0) {
		m_contentsType = (eObjectType) 0xffff;
	}
	else {
		m_contentsType = p_contents->m_objectType;
	}
}

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

#include "../../Map/Base/Map.h"
#include "../Managers/ObjectManager.h"

// 68K 0x106191bc TriggerContents__6CCrateFv
// FUNCTION: LEMBALL 0x0041ccc0
void Crate::TriggerContents()
{
	if (m_contentsType != 0xffff) {
		GlobalGameObject* contents = (GlobalGameObject*) m_contents;
		contents->m_position.m_xFixed = m_position.m_xFixed;
		contents->m_position.m_yFixed = m_position.m_yFixed;
		contents->m_position.m_zFixed = m_position.m_zFixed;
		g_pGenericGroupObjectManager->AddObject(0xffff, contents, 0);
		m_contentsType = 0xffff;
	}
}

// 68K 0x10619230 Process__6CCrateFv
// FUNCTION: LEMBALL 0x0041cd20
bool Crate::Process()
{
	int y = m_position.m_yFixed >> 12;
	int x = m_position.m_xFixed >> 12;
	int blockX = x >> 4;
	int blockY = y >> 4;
	if (x >= 0 && y >= 0 && blockX < g_pMap->m_ground.m_width && g_pMap->m_ground.m_height > blockY) {
		int cellX = x & 0xf;
		int cellY = y & 0xf;
		m_position.m_zFixed = g_pMap->m_ground.m_ground[blockY * g_pMap->m_ground.m_width + blockX].GetZ(cellX, cellY)
			<< 12;
	}
	else {
		m_position.m_zFixed = 0;
	}
	if (m_isRemoteObject == 0) {
		if (m_action == 25) {
			if (m_unk0xd0 < g_dwGameTick) {
				TriggerContents();
				SetSndEffect((eSoundEffect) 10);
				Action((eAction) 26);
			}
		}
		else if (m_action == 26 && m_unk0xd4 < g_dwGameTick) {
			Action((eAction) 24);
			m_heading = 0;
		}
		return 1;
	}
	if (m_pendingAction == m_action) {
		return 1;
	}
	if (m_action == 25) {
		SetSndEffect((eSoundEffect) 20);
	}
	else if (m_action == 26) {
		TriggerContents();
		SetSndEffect((eSoundEffect) 10);
	}
	m_pendingAction = m_action;
	return 1;
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
		}
		else if (m_contentsType == 4) {
			time = 100;
		}
	}
	else if (m_contentsType == 0xffff) {
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
	if (m_contentsType < 0x15) {
		goto default_position;
	}
	if (m_contentsType <= 0x17 || m_contentsType == 0xffff) {
		goto contents_position;
	}

default_position:
	return AiCoord(x - 0x30000, y - 0x8000, z);

contents_position:
	return AiCoord(x - 0x8000, y, z);
}
