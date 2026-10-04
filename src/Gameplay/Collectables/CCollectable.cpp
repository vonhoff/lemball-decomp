#include "CCollectable.h"

#include "Map/CMap.h"
#include "Visos/Network/CConnect.h"
#include "Gameplay/Simulation/CAI.h"
#include "Gameplay/Geometry/AICOORD.h"
#include "Gameplay/Objects/CGameObject.h"
#include "Gameplay/Objects/CGlobalGameObject.h"
#include "Gameplay/Geometry/CPt3.h"
#include "Gameplay/Objects/ObjectActions.h"
#include "Map/CGround.h"
#include "Map/CGroundArray.h"
#include "Visos/Math/CFixed.h"

#include <stddef.h>

inline static CFixed FixedGroundHeight(unsigned short p_height)
{
	CFixed height((int) p_height << FIXED_POINT_FRACTION_BITS);
	return height;
}

// FUNCTION: LEMBALL 0x00422870
CCollectable::CCollectable(int p_x, int p_y, int p_z, eObjectType p_objectType) : CGlobalGameObject(p_objectType, 0, 0)
{
	m_spawnPosition.m_xFixed = p_x << FIXED_POINT_FRACTION_BITS;
	m_spawnPosition.m_yFixed = p_y << FIXED_POINT_FRACTION_BITS;
	m_spawnPosition.m_zFixed = p_z << FIXED_POINT_FRACTION_BITS;
}

// FUNCTION: LEMBALL 0x004228b0
void CCollectable::Restart()
{
	CGlobalGameObject::Restart();
	m_position.m_xFixed = m_spawnPosition.m_xFixed;
	m_position.m_yFixed = m_spawnPosition.m_yFixed;
	m_position.m_zFixed = m_spawnPosition.m_zFixed;
	m_enabled = 1;
	m_action = ACTION_READY;
}

// FUNCTION: LEMBALL 0x004228f0
CCollectable::~CCollectable()
{
}

// FUNCTION: LEMBALL 0x00422900
bool CCollectable::Process()
{
	if (m_isRemoteObject != 0) {
		if (m_pendingAction != m_action) {
			switch (m_action) {
			case ACTION_DEAD:
				m_enabled = 0;
				break;
			case ACTION_ACTIVATED:
				SetSFX();
				break;
			}
			m_pendingAction = m_action;
		}
		return true;
	}
	if (m_enabled != 0) {
		switch (m_action) {
		case ACTION_DEAD:
			m_enabled = 0;
			break;
		case ACTION_READY: {
			if (g_pActiveConnection == NULL || m_requestedAction == ACTION_READY) {
				if (m_onMover == 0) {
					int y = m_position.m_yFixed >> FIXED_POINT_FRACTION_BITS;
					int x = m_position.m_xFixed >> FIXED_POINT_FRACTION_BITS;
					CMap* map = g_pMap;
					int blockY = y >> GROUND_BLOCK_PIXEL_SHIFT;
					int blockX = x >> GROUND_BLOCK_PIXEL_SHIFT;
					unsigned short z;
					if (x >= 0 && y >= 0 && blockX < map->m_ground.m_width && g_pMap->m_ground.m_height > blockY) {
						int cellX = x & GROUND_BLOCK_PIXEL_MASK;
						int cellY = y & GROUND_BLOCK_PIXEL_MASK;
						z = map->m_ground.m_ground[blockY * map->m_ground.m_width + blockX].GetZ(cellX, cellY);
					}
					else {
						z = 0;
					}
					m_position.m_zFixed = FixedGroundHeight(z).m_value;
				}
				CPt3 pt;
				pt.m_x = m_position.m_xFixed >> FIXED_POINT_FRACTION_BITS;
				pt.m_y = m_position.m_yFixed >> FIXED_POINT_FRACTION_BITS;
				pt.m_z = m_position.m_zFixed >> FIXED_POINT_FRACTION_BITS;
				CAI* ai = g_pAI;
				ai->m_collisionExclude = NULL;
				ai->m_collisionPoint = pt;
				ai->m_collisionIndex = 0;
				CGameObject* hit;
				if (ai->m_objectCount > 0) {
					do {
						CGameObject* obj = ai->m_objects[ai->m_collisionIndex];
						if (ai->m_collisionExclude != obj && obj->Collision(ai->m_collisionPoint)) {
							hit = ai->m_objects[ai->m_collisionIndex];
							ai->m_collisionIndex++;
							goto found;
						}
						ai->m_collisionIndex++;
					} while (ai->m_collisionIndex < ai->m_objectCount);
				}
				hit = NULL;
			found:
				if (hit != NULL && hit->m_objectType == OBJECT_PLAYER_2 && hit->HasObject(m_objectType) == 0) {
					m_activator = hit;
					RequestAction(ACTION_ACTIVATED);
				}
			}
			break;
		}
		case ACTION_ACTIVATED:
			Collected();
			SetSFX();
			Action(ACTION_DEAD);
			break;
		}
	}
	return true;
}

// FUNCTION: LEMBALL 0x00422fa0
void CCollectable::DoActivate()
{
}

// FUNCTION: LEMBALL 0x00423040
void CCollectable::SetSFX()
{
}

// FUNCTION: LEMBALL 0x00423050
int CCollectable::Collected()
{
	return 1;
}
