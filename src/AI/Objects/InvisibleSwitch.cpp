#include "InvisibleSwitch.h"

#include "../../Map/Base/Map.h"
#include "../Navigation/Ai.h"

// 68K 0x106129ea __ct__16CInvisibleSwitchFv
// FUNCTION: LEMBALL 0x00409ca0
InvisibleSwitch::InvisibleSwitch() : GlobalGameObject(0x36, 0, 0)
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
// FUNCTION: LEMBALL 0x00409d70
void InvisibleSwitch::Set(const Coord3d& p_cornerA, const Coord3d& p_cornerB)
{
	m_objectCount = 0;
	m_minCorner = p_cornerA;
	m_maxCorner = p_cornerB;
	short temporary;
	if (m_maxCorner.m_x < m_minCorner.m_x) {
		temporary = m_minCorner.m_x;
		m_minCorner.m_x = m_maxCorner.m_x;
		m_maxCorner.m_x = temporary;
	}
	if (m_maxCorner.m_y < m_minCorner.m_y) {
		temporary = m_minCorner.m_y;
		m_minCorner.m_y = m_maxCorner.m_y;
		m_maxCorner.m_y = temporary;
	}
	m_repeatable = 0;
	m_position.m_xFixed = ((int) m_minCorner.m_x) << 12;
	m_triggered = 0;
	m_position.m_yFixed = ((int) m_minCorner.m_y) << 12;
	m_position.m_zFixed = ((int) m_minCorner.m_z) << 12;
	for (int y = m_minCorner.m_y; y <= m_maxCorner.m_y; y += 0x10) {
		for (int x = m_minCorner.m_x; x <= m_maxCorner.m_x; x += 0x10) {
			int blockX = x / 0x10;
			if (blockX >= 0) {
				int blockY = y / 0x10;
				if (blockY >= 0 && blockX < g_pMap->m_ground.m_width && blockY < g_pMap->m_ground.m_height) {
					Ground* ground = g_pMap->m_ground.m_ground + g_pMap->m_ground.m_width * blockY + blockX;
					((unsigned char*) &ground->m_collision)[1] |= 0x80;
				}
			}
		}
	}
	m_targetCount = 0;
}

// 68K 0x10612c94 VerifyObjects__16CInvisibleSwitchFv
// STUB: LEMBALL 0x00409ec0
void InvisibleSwitch::VerifyObjects()
{
}

// 68K 0x10612d88 AddObject__16CInvisibleSwitchFP11CGameObject
// FUNCTION: LEMBALL 0x00409f70
void InvisibleSwitch::AddObject(GameObject* p_object)
{
	if (m_objectCount < 24) {
		m_objects[m_objectCount] = p_object;
		m_objectCount++;
		p_object->m_unk0x120 = GetId();
	}
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
		InvisibleSwitchTarget* target;
		int i = 0;
		if (i < m_targetCount) {
			target = m_targets;
			do {
				g_pAI->SwitchMessage((swMessage) target->m_message, target->m_objectId, 0, 0);
				target++;
				i++;
			} while (i < m_targetCount);
		}
		SetSndEffect((eSoundEffect) 0x15);
		Action((eAction) 0x18);
	}
	return 1;
}

// 68K 0x10613016 Load__16CInvisibleSwitchFRPUc
// FUNCTION: LEMBALL 0x0040a110
void InvisibleSwitch::Load(unsigned char*& p_data)
{
	unsigned short id = *(unsigned short*) p_data;
	p_data += 2;
	SetId(id);

	Coord3d cornerA;
	cornerA.m_x = *(short*) p_data;
	p_data += 2;
	cornerA.m_y = *(short*) p_data;
	p_data += 2;
	cornerA.m_z = *(short*) p_data;
	p_data += 2;

	Coord3d cornerB;
	cornerB.m_x = *(short*) p_data;
	p_data += 2;
	cornerB.m_y = *(short*) p_data;
	p_data += 2;
	cornerB.m_z = *(short*) p_data;
	p_data += 2;

	Set(cornerA, cornerB);

	if (g_pAI->m_levelVersion >= 9) {
		unsigned int repeatable = *(unsigned short*) p_data;
		p_data += 2;
		m_repeatable = repeatable;
	}
	else {
		m_repeatable = 0;
	}

	m_targetCount = *(unsigned short*) p_data;
	p_data += 2;
	InvisibleSwitchTarget* target;
	int index = 0;
	if (m_targetCount > 0) {
		target = m_targets;
		do {
			target->m_message = *(unsigned short*) p_data;
			p_data += 2;
			target->m_objectId = *(unsigned short*) p_data;
			p_data += 2;
			target++;
			index++;
		} while (index < m_targetCount);
	}
}

// 68K 0x10612ac0 __dt__16CInvisibleSwitchFv
InvisibleSwitch::~InvisibleSwitch()
{
}
