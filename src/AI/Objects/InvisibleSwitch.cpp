#include "InvisibleSwitch.h"

#include "../Navigation/Ai.h"

// 68K 0x106129ea __ct__16CInvisibleSwitchFv
// STUB: LEMBALL 0x00409ca0
InvisibleSwitch::InvisibleSwitch()
{
}

// 68K 0x10612a36 Restart__16CInvisibleSwitchFv
// FUNCTION: LEMBALL 0x00409cc0
void InvisibleSwitch::Restart()
{
	GlobalGameObject::Restart();
	Initialise();
}

// 68K 0x10612a72 Initialise__16CInvisibleSwitchFv
// FUNCTION: LEMBALL 0x00409ce0
void InvisibleSwitch::Initialise()
{
	m_action = (eAction) 0x18;
	m_targetCount = 0;
	m_triggered = 0;
	m_objectCount = 0;
	m_repeatable = 0;
	m_activator = 0;
	m_scoreAwarded = 0;
}

// 68K 0x10612b24 Set__16CInvisibleSwitchFRC8tCoord3dRC8tCoord3d
// STUB: LEMBALL 0x00409d70
void InvisibleSwitch::Set(const Coord3d& p_cornerA, const Coord3d& p_cornerB)
{
}

// 68K 0x10612c94 VerifyObjects__16CInvisibleSwitchFv
// STUB: LEMBALL 0x00409ec0
void InvisibleSwitch::VerifyObjects()
{
}

// 68K 0x10612d88 AddObject__16CInvisibleSwitchFP11CGameObject
// STUB: LEMBALL 0x00409f70
void InvisibleSwitch::AddObject(GameObject* p_object)
{
}

// 68K 0x10612dfe StepOn__16CInvisibleSwitchFRC7AICOORDP11CGameObject
// STUB: LEMBALL 0x00409fa0
void InvisibleSwitch::StepOn(const AiCoord& p_position, GameObject* p_object)
{
}

// 68K 0x10612ec2 DoActivate__16CInvisibleSwitchFv
// FUNCTION: LEMBALL 0x0040a020
void InvisibleSwitch::DoActivate()
{
	if (m_scoreAwarded == 0) {
		g_pAI->Score(50);
		m_scoreAwarded = 1;
	}
}

// 68K 0x10612f14 Process__16CInvisibleSwitchFv
// FUNCTION: LEMBALL 0x0040a050
bool InvisibleSwitch::Process()
{
	VerifyObjects();
	if (m_isRemoteObject) {
		if (m_pendingAction != m_action && m_action != (eAction) 0x1a) {
			return 1;
		}
	}

	if (m_action == (eAction) 0x1a) {
		if (m_repeatable == 0) {
			m_triggered = 1;
		}
		if (m_activator != 0) {
			AddObject(m_activator);
			m_activator = 0;
		}
		for (int i = 0; i < m_targetCount; i++) {
			g_pAI->SwitchMessage((swMessage) m_targets[i].m_message, m_targets[i].m_objectId, 0, 0);
		}
		SetSndEffect((eSoundEffect) 0x15);
		Action((eAction) 0x18);
	}
	return 1;
}

// 68K 0x10613016 Load__16CInvisibleSwitchFRPUc
// STUB: LEMBALL 0x0040a110
void InvisibleSwitch::Load(unsigned char*& p_data)
{
}

// 68K 0x10612ac0 __dt__16CInvisibleSwitchFv
InvisibleSwitch::~InvisibleSwitch()
{
}

