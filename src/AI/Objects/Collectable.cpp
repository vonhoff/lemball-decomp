#include "Collectable.h"

#include "../../Map/Base/Map.h"
#include "../../Visos/Network/Connect.h"
#include "../Navigation/Ai.h"

Collectable::Collectable()
{
}

// 68K 0x10605b0e __ct__12CCollectableFiii11eObjectType
// FUNCTION: LEMBALL 0x00422870
Collectable::Collectable(int p_arg0, int p_arg1, int p_arg2, eObjectType p_arg3) : GlobalGameObject(p_arg3, 0, 0)
{
	m_spawnPosition.m_xFixed = p_arg0 << 12;
	m_spawnPosition.m_yFixed = p_arg1 << 12;
	m_spawnPosition.m_zFixed = p_arg2 << 12;
}

// 68K 0x10605bbc Restart__12CCollectableFv
// FUNCTION: LEMBALL 0x004228b0
void Collectable::Restart()
{
	GlobalGameObject::Restart();
	m_position.m_xFixed = m_spawnPosition.m_xFixed;
	m_position.m_yFixed = m_spawnPosition.m_yFixed;
	m_position.m_zFixed = m_spawnPosition.m_zFixed;
	m_enabled = 1;
	m_action = (eAction) 0x18;
}

// 68K 0x10605c74 Process__12CCollectableFv
// FUNCTION: LEMBALL 0x00422900
bool Collectable::Process()
{
	if (m_isRemoteObject != 0) {
		if (m_pendingAction != m_action) {
			if (m_action == 8) {
				m_enabled = 0;
			}
			else if (m_action == 26) {
				Collected();
			}
			m_pendingAction = m_action;
		}
		return 1;
	}
	if (m_enabled != 0) {
		if (m_action == 8) {
			m_enabled = 0;
		}
		else if (m_action == 24) {
			if (g_pActiveConnection == 0 || m_requestedAction == 24) {
				if (m_unk0x11c == 0) {
					int y = m_position.m_yFixed >> 12;
					int x = m_position.m_xFixed >> 12;
					int blockX = x >> 4;
					int blockY = y >> 4;
					unsigned short z;
					if (x >= 0 && y >= 0 && blockX < g_pMap->m_ground.m_width && g_pMap->m_ground.m_height > blockY) {
						z = g_pMap->m_ground.m_ground[blockY * g_pMap->m_ground.m_width + blockX].GetZ(x & 0xf,
																									   y & 0xf);
					}
					else {
						z = 0;
					}
					m_position.m_zFixed = (int) z << 12;
				}
				Pt3 pt;
				pt.m_x = m_position.m_xFixed >> 12;
				pt.m_y = m_position.m_yFixed >> 12;
				pt.m_z = m_position.m_zFixed >> 12;
				g_pAI->m_unk0x124[11] = 0;
				*(Pt3*) &g_pAI->m_unk0x124[0] = pt;
				g_pAI->m_unk0x124[3] = 0;
				GameObject* hit = 0;
				if (g_pAI->m_objectCount > 0) {
					do {
						GameObject* obj = g_pAI->m_objects[g_pAI->m_unk0x124[3]];
						if ((GameObject*) g_pAI->m_unk0x124[11] != obj &&
							obj->Collision(*(Pt3*) &g_pAI->m_unk0x124[0])) {
							hit = g_pAI->m_objects[g_pAI->m_unk0x124[3]];
							g_pAI->m_unk0x124[3]++;
							break;
						}
						g_pAI->m_unk0x124[3]++;
					} while ((int) g_pAI->m_unk0x124[3] < g_pAI->m_objectCount);
				}
				if (hit != 0 && hit->m_objectType == 2 && hit->HasObject(m_objectType) == 0) {
					m_activator = hit;
					RequestAction((eAction) 26);
				}
			}
		}
		else if (m_action == 26) {
			SetSfx();
			Collected();
			Action((eAction) 8);
		}
	}
	return 1;
}

// 68K 0x10119a66 DoActivate__12CCollectableFv
// FUNCTION: LEMBALL 0x00422fa0
void Collectable::DoActivate()
{
}

// 68K 0x101198c6 SetSFX__12CCollectableFv
// FUNCTION: LEMBALL 0x00423040
void Collectable::SetSfx()
{
}

// 68K 0x10119878 Collected__12CCollectableFv
// FUNCTION: LEMBALL 0x00423050
int Collectable::Collected()
{
	return 1;
}

// 68K 0x10605c14 __dt__12CCollectableFv
Collectable::~Collectable()
{
}
