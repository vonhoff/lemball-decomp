#include "CInvisibleSwitch.h"

#include "../../Map/Base/CMap.h"
#include "../Navigation/CAI.h"
#include "AI/Base/AiCoord.h"
#include "AI/Base/CGameObject.h"
#include "AI/Base/CGlobalGameObject.h"
#include "AI/Base/Coord3d.h"
#include "AI/Base/ObjectActions.h"
#include "AI/Base/ObjectTypes.h"
#include "AI/Objects/InvisibleSwitchTarget.h"
#include "AI/Objects/SwitchEntry.h"
#include "Map/Ground/CGround.h"
#include "Map/Ground/CGroundArray.h"
#include "Views/Sound/SoundEffects.h"

// FUNCTION: LEMBALL 0x00409ca0
CInvisibleSwitch::CInvisibleSwitch() : CGlobalGameObject(OBJECT_INVISIBLE_SWITCH, 0, 0)
{
}

// FUNCTION: LEMBALL 0x00409cc0
void CInvisibleSwitch::Restart()
{
	CGlobalGameObject::Restart();
	Initialise();
}

// FUNCTION: LEMBALL 0x00409ce0
void CInvisibleSwitch::Initialise()
{
	m_action = ACTION_0x18;
	m_targetCount = 0;
	m_triggered = 0;
	m_objectCount = 0;
	m_repeatable = 0;
	m_activator = 0;
	m_scoreAwarded = 0;
}

// FUNCTION: LEMBALL 0x00409d10
CInvisibleSwitch::~CInvisibleSwitch()
{
}

// FUNCTION: LEMBALL 0x00409d20
void CInvisibleSwitch::SetPointFromIntegers(short p_x, short p_y, short p_z)
{
	short y = p_y;
	short z = p_z;
	short x = p_x;
	Coord3d first;
	first.m_x = x;
	first.m_y = y;
	first.m_z = z;
	Coord3d second;
	second.m_x = x;
	second.m_y = y;
	second.m_z = z;
	Set(first, second);
}

// FUNCTION: LEMBALL 0x00409d70
void CInvisibleSwitch::Set(const Coord3d& p_cornerA, const Coord3d& p_cornerB)
{
	m_objectCount = 0;
	m_minCorner = p_cornerA;
	m_maxCorner = p_cornerB;
	if (m_minCorner.m_x > m_maxCorner.m_x) {
		short x = m_minCorner.m_x;
		m_minCorner.m_x = m_maxCorner.m_x;
		m_maxCorner.m_x = x;
	}
	short minY = m_minCorner.m_y;
	if (minY > m_maxCorner.m_y) {
		m_minCorner.m_y = m_maxCorner.m_y;
		m_maxCorner.m_y = minY;
	}
	m_repeatable = 0;
	m_position.m_xFixed = ((int) m_minCorner.m_x) << 12;
	m_position.m_yFixed = ((int) m_minCorner.m_y) << 12;
	m_triggered = 0;
	m_position.m_zFixed = ((int) m_minCorner.m_z) << 12;
	for (int y = m_minCorner.m_y; y <= m_maxCorner.m_y; y += 0x10) {
		for (int x = m_minCorner.m_x; x <= m_maxCorner.m_x; x += 0x10) {
			int blockX = x / 0x10;
			if (blockX >= 0) {
				int blockY = y / 0x10;
				if (blockY >= 0) {
					int width = g_pMap->m_ground.m_width;
					if (width <= blockX) {
						continue;
					}
					if (g_pMap->m_ground.m_height <= blockY) {
						continue;
					}
					CGround* ground = g_pMap->m_ground.m_ground + width * blockY + blockX;
					((unsigned char*) &ground->m_collision)[1] |= 0x80;
				}
			}
		}
	}
	m_targetCount = 0;
}

// FUNCTION: LEMBALL 0x00409ec0
void CInvisibleSwitch::VerifyObjects()
{
	unsigned short invalidObjectId;
	int i;

	invalidObjectId = 0xffff;
	i = 0;
	if (m_objectCount > 0) {
		do {
			CGameObject* object = m_objects[i];
			int x = object->m_position.m_xFixed >> 12;
			int y = object->m_position.m_yFixed >> 12;
			if (x < m_minCorner.m_x - 8 || x > m_maxCorner.m_x + 7 || y < m_minCorner.m_y - 8 ||
				y > m_maxCorner.m_y + 7) {
				object->m_invisibleSwitchId = invalidObjectId;
				int next = i + 1;
				if (next < m_objectCount) {
					do {
						m_objects[next - 1] = m_objects[next];
						next++;
					} while (next < m_objectCount);
				}
				i--;
				m_objectCount--;
			}
			i++;
		} while (i < m_objectCount);
	}
}

// FUNCTION: LEMBALL 0x00409f70
void CInvisibleSwitch::AddObject(CGameObject* p_object)
{
	if (m_objectCount < 24) {
		m_objects[m_objectCount] = p_object;
		m_objectCount++;
		p_object->m_invisibleSwitchId = GetId();
	}
}

// FUNCTION: LEMBALL 0x00409fa0
void CInvisibleSwitch::StepOn(const AiCoord& p_position, CGameObject* p_object)
{
	if (m_triggered == 0 && m_requestedAction == ACTION_0x18 && GetId() != (short) p_object->m_invisibleSwitchId) {
		int x = p_position.m_xFixed >> 12;
		int y = p_position.m_yFixed >> 12;
		if (x >= m_minCorner.m_x - 8 && x <= m_maxCorner.m_x + 7 && y >= m_minCorner.m_y - 8 &&
			y <= m_maxCorner.m_y + 7) {
			m_activator = p_object;
			RequestAction(ACTION_0x1a);
		}
	}
}

// FUNCTION: LEMBALL 0x0040a020
void CInvisibleSwitch::DoActivate()
{
	if (m_scoreAwarded == 0) {
		g_pAI->Score(50);
		m_scoreAwarded = 1;
	}
}

// FUNCTION: LEMBALL 0x0040a050
bool CInvisibleSwitch::Process()
{
	VerifyObjects();
	if (m_isRemoteObject) {
		if (m_pendingAction != m_action && m_action != ACTION_0x1a) {
			return 1;
		}
	}

	if (m_action == ACTION_0x1a) {
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
		SetSndEffect(SFX_SWITCH);
		Action(ACTION_0x18);
	}
	return 1;
}

// FUNCTION: LEMBALL 0x0040a110
void CInvisibleSwitch::Load(unsigned char*& p_data)
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
