#include "Switch.h"

#include "../../Control/Game/Game.h"
#include "../../Control/Game/GameTime.h"
#include "../Navigation/Ai.h"

// GLOBAL: LEMBALL 0x0049e1b8
word g_wNextSwitchIndex;

// 68K 0x10619660 __ct__7CSwitchFR7AICOORD9swMessageiii
// FUNCTION: LEMBALL 0x0041d040
Switch::Switch(AiCoord& p_arg0, swMessage p_arg1, int p_arg2, int p_arg3, int p_arg4)
	: BaseGlobalObject(p_arg0, OBJECT_SWITCH)
{
	m_position.m_xFixed = p_arg0.m_xFixed;
	m_position.m_yFixed = p_arg0.m_yFixed;
	m_position.m_zFixed = p_arg0.m_zFixed;
	m_legacyType = p_arg1;
	m_legacyFirst = p_arg2;
	m_legacyLast = p_arg3;
	m_legacyAux = p_arg4;
	m_switchId = g_wNextSwitchIndex++;
}

// 68K 0x10619732 Restart__7CSwitchFv
// FUNCTION: LEMBALL 0x0041d100
void Switch::Restart()
{
	BaseGlobalObject::Restart();
	m_entryCount = 0;
	m_scoreAwarded = 0;
	m_actionArgument = 0;
}

// 68K 0x106197d2 Throw__7CSwitchFv
// FUNCTION: LEMBALL 0x0041d130
void Switch::Throw()
{
	SwitchEntry* entry;
	int i = 0;
	if (i < m_entryCount) {
		entry = m_entries;
		do {
			g_pAI->SwitchMessage((swMessage) entry->m_message, entry->m_objectId, 0, 0);
			entry++;
			i++;
		} while (i < m_entryCount);
	}
	SetSndEffect((eSoundEffect) 0x15);
}

// 68K 0x10619852 Process__7CSwitchFv
// STUB: LEMBALL 0x0041d180
bool Switch::Process()
{
	return 0;
}

// 68K 0x10619950 Activate__7CSwitchFP11CGameObject
// FUNCTION: LEMBALL 0x0041d280
bool Switch::Activate(GameObject* p_object)
{
	switch ((unsigned short) m_actionArgument) {
	case 0:
		m_unk0xd4 = 20;
		m_actionArgument = 1;
		RequestAction((eAction) 26);
		return 1;
	case 1:
		m_unk0xd4 = 20;
		m_actionArgument = 0;
		RequestAction((eAction) 26);
		return 1;
	default:
		return 1;
	}
}

// 68K 0x106199ca DoActivate__7CSwitchFv
// FUNCTION: LEMBALL 0x0041d2e0
void Switch::DoActivate()
{
	m_stateTimer = g_dwSimulationTimestamp;
	m_unk0xd4 += g_dwGameTick;
	if (m_scoreAwarded == 0) {
		g_pAI->AddTime(25);
		m_scoreAwarded = 1;
	}
}

// 68K 0x10619a24 ActivatePosition__7CSwitchFv
// FUNCTION: LEMBALL 0x0041d320
AiCoord Switch::ActivatePosition()
{
	int y = m_position.m_yFixed;
	int z = m_position.m_zFixed;
	int x = m_position.m_xFixed - 0x8000;
	return AiCoord(x, y, z);
}

// 68K 0x10619a9c AddEntry__7CSwitchF9swMessageUs
// FUNCTION: LEMBALL 0x0041d350
void Switch::AddEntry(int p_message, unsigned short p_objectId)
{
	if (m_entryCount < 0x20) {
		m_entries[m_entryCount].m_message = p_message;
		m_entries[m_entryCount].m_objectId = p_objectId;
		m_entryCount++;
	}
}

// 68K 0x10619afa ConvertVer0ToVer1__7CSwitchFv
// FUNCTION: LEMBALL 0x0041d390
void Switch::ConvertVer0ToVer1()
{
	switch (m_legacyType) {
	case 1: {
		unsigned int liftId = g_pAI->LiftId(m_legacyFirst);
		AddEntry(1, liftId);
		break;
	}
	case 2: {
		for (int i = m_legacyFirst; i < m_legacyLast; i++) {
			unsigned int liftId = g_pAI->LiftId(i);
			AddEntry(1, liftId);
		}
		break;
	}
	case 3: {
		unsigned int doorId = g_pAI->DoorId(m_legacyFirst);
		if (doorId != 0xffff) {
			AddEntry(3, doorId);
		}
		break;
	}
	}
}

// 68K 0x10619bce Load__7CSwitchFRPUc
// FUNCTION: LEMBALL 0x0041d430
unsigned char* Switch::Load(unsigned char*& p_data)
{
	unsigned short* data = (unsigned short*) p_data;
	unsigned short count = *data;
	p_data += 2;
	if (count != 0) {
		unsigned int remaining = count;
		do {
			unsigned short objectId;
			unsigned short* cursor = (unsigned short*) p_data;
			int message = *cursor++;
			p_data = (unsigned char*) cursor;
			objectId = *cursor++;
			p_data = (unsigned char*) cursor;
			AddEntry(message, objectId);
			remaining--;
		} while (remaining != 0);
	}
	return p_data;
}

// 68K 0x1011abce Usage__7CSwitchFv
// FUNCTION: LEMBALL 0x0041dc40
int Switch::Usage()
{
	return 2;
}

// 68K 0x1011abec GetViewData__7CSwitchFR9CViewData
// STUB: LEMBALL 0x0041dc50
void Switch::GetViewData(ViewData& p_viewData)
{
}

// 68K 0x1061976c __dt__7CSwitchFv
Switch::~Switch()
{
}
