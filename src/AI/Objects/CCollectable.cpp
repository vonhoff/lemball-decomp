#include "CCollectable.h"

#include "../../Map/Base/CMap.h"
#include "../../Visos/Network/CConnect.h"
#include "../Navigation/CAI.h"
#include "AI/Base/AiCoord.h"
#include "AI/Base/CGameObject.h"
#include "AI/Base/CGlobalGameObject.h"
#include "AI/Base/CPt3.h"
#include "AI/Base/ObjectActions.h"
#include "Map/Ground/CGround.h"
#include "Map/Ground/CGroundArray.h"

// FUNCTION: LEMBALL 0x00422870
CCollectable::CCollectable(int p_x, int p_y, int p_z, eObjectType p_objectType) : CGlobalGameObject(p_objectType, 0, 0)
{
	m_spawnPosition.m_xFixed = p_x << 12;
	m_spawnPosition.m_yFixed = p_y << 12;
	m_spawnPosition.m_zFixed = p_z << 12;
}

// FUNCTION: LEMBALL 0x004228b0
void CCollectable::Restart()
{
	CGlobalGameObject::Restart();
	m_position.m_xFixed = m_spawnPosition.m_xFixed;
	m_position.m_yFixed = m_spawnPosition.m_yFixed;
	m_position.m_zFixed = m_spawnPosition.m_zFixed;
	m_enabled = 1;
	m_action = ACTION_0x18;
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
			case 8:
				m_enabled = 0;
				break;
			case 26:
				SetSFX();
				break;
			}
			m_pendingAction = m_action;
		}
		return 1;
	}
	if (m_enabled != 0) {
		switch (m_action) {
		case 8:
			m_enabled = 0;
			break;
		case 24: {
			if (g_pActiveConnection == 0 || m_requestedAction == 24) {
				if (m_onMover == 0) {
					int y = m_position.m_yFixed >> 12;
					int x = m_position.m_xFixed >> 12;
					CMap* map = g_pMap;
					int blockY = y >> 4;
					int blockX = x >> 4;
					unsigned short z;
					if (x >= 0 && y >= 0 && blockX < map->m_ground.m_width && g_pMap->m_ground.m_height > blockY) {
						int cellX = x & 0xf;
						int cellY = y & 0xf;
						z = map->m_ground.m_ground[blockY * map->m_ground.m_width + blockX].GetZ(cellX, cellY);
					}
					else {
						z = 0;
					}
					m_position.m_zFixed = (int) z << 12;
				}
				CPt3 pt;
				pt.m_x = m_position.m_xFixed >> 12;
				pt.m_y = m_position.m_yFixed >> 12;
				pt.m_z = m_position.m_zFixed >> 12;
				CAI* ai = g_pAI;
				ai->m_collisionExclude = 0;
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
					} while ((int) ai->m_collisionIndex < ai->m_objectCount);
				}
				hit = 0;
			found:
				if (hit != 0 && hit->m_objectType == 2 && hit->HasObject(m_objectType) == 0) {
					m_activator = hit;
					RequestAction(ACTION_0x1a);
				}
			}
			break;
		}
		case 26:
			Collected();
			SetSFX();
			Action(ACTION_8);
			break;
		}
	}
	return 1;
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
