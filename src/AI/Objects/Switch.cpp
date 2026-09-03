#include "Switch.h"

#include "../../Control/Game/Game.h"
#include "../../Control/Game/GameTime.h"
#include "../Navigation/Ai.h"

// GLOBAL: LEMBALL 0x0049e1b8
word g_wNextSwitchIndex;

// 68K 0x10619660 __ct__7CSwitchFR7AICOORD9swMessageiii
// FUNCTION: LEMBALL 0x0041d040
Switch::Switch(AiCoord& p_arg0, swMessage p_arg1, int p_arg2, int p_arg3, int p_arg4)
	: BaseGlobalObject(p_arg0, 0x14)
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
// STUB: LEMBALL 0x0041d130
void Switch::Throw()
{
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
// STUB: LEMBALL 0x0041d350
void Switch::AddEntry(int p_message, unsigned short p_objectId)
{
}

// 68K 0x10619afa ConvertVer0ToVer1__7CSwitchFv
// STUB: LEMBALL 0x0041d390
void Switch::ConvertVer0ToVer1()
{
}

// 68K 0x10619bce Load__7CSwitchFRPUc
// STUB: LEMBALL 0x0041d430
void Switch::Load(unsigned char*& p_data)
{
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
