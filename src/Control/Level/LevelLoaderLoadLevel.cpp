// Keep the manager declarations in this TU: they change MSVC 4.00 register
// allocation in the otherwise untouched level-file and preview functions.
#include "../../AI/Groups/EnemyGroupManager.h"
#include "../../AI/Groups/PlayerLemmingGroupManager.h"
#include "../../AI/Groups/SheepGroupManager.h"
#include "../../AI/Managers/BallManager.h"
#include "../../AI/Managers/CollectableManager.h"
#include "../../AI/Managers/DoorManager.h"
#include "../../AI/Managers/HandManager.h"
#include "../../AI/Managers/IceManager.h"
#include "../../AI/Managers/InvisibleSwitchManager.h"
#include "../../AI/Managers/LaserManager.h"
#include "../../AI/Managers/LiftManager.h"
#include "../../AI/Managers/MineManager.h"
#include "../../AI/Managers/ObjectManager.h"
#include "../../AI/Managers/PaintGunManager.h"
#include "../../AI/Managers/RocketManager.h"
#include "../../AI/Managers/SlinkyManager.h"
#include "../../AI/Managers/TrampolineManager.h"
#include "../../AI/Managers/TrapDoorManager.h"
#include "../../AI/Navigation/Ai.h"
#include "../../AI/Navigation/MoverManager.h"
#include "../../AI/Navigation/NodeManager.h"
#include "../../AI/Objects/BalloonPost.h"
#include "../../AI/Objects/GroundAnim.h"
#include "../../Map/Base/Map.h"
#include "../../Visos/Resources/ResBin.h"
#include "LevelLoader.h"
#include "LoadBlockHeader.h"

#include <new.h>

// x86 0x004082bb reads and forwards the full 32-bit skip argument slot.
// 68K 0x107026ca LoadLevel__12CLevelLoaderF6eSkilliUc
// FUNCTION: LEMBALL 0x00408240
void LevelLoader::LoadLevel(eSkill p_skill, int p_level, unsigned int p_skip)
{
	bool endFound = false;
	ResBin* binResource = 0;
	LoadBlockHeader* header;
	unsigned int dataSize;
	unsigned int blockType;

	if (p_level == 9999) {
		p_level = m_fallbackLevel;
	}
	if (g_nEditLevelMode == 0 && g_nPlayLevelMode == 0) {
		unsigned int resourceId = CalcLevelId(p_skill, p_level);
		binResource = ResBin::Load(resourceId);
		if (binResource->m_loaded != 0) {
			binResource->m_age = 0;
		}
		else {
			binResource->LoadData();
		}
		binResource->m_directUseCount++;
		g_pLevelFileData = binResource->GetData();
	}
	else {
		LocateStartOfLevelFile();
	}

	header = GetNextBlockHeader(0);
	do {
		dataSize = header->m_size;
		blockType = header->m_type;
		dataSize -= 8;
		switch (blockType) {
		case LEVEL_BLOCK_AI: // AI
			m_ai->LoadLevel((unsigned char*) (header + 1), dataSize, p_skip);
			break;
		case LEVEL_BLOCK_GROUND_ANIMS: // ANIM
			m_ai->m_groundAnim->LoadLevel((unsigned char*) (header + 1), dataSize, p_skip);
			break;
		case LEVEL_BLOCK_BALLS: // BALL
			m_ai->m_ballManager->LoadLevel((unsigned char*) (header + 1), dataSize, p_skip);
			break;
		case LEVEL_BLOCK_BALLOON_POSTS: // BOON
			m_ai->m_balloonPost->LoadLevel((unsigned char*) (header + 1), dataSize, p_skip);
			break;
		case LEVEL_BLOCK_COLLECTABLES: // COLL
			m_ai->m_collectableManager->LoadLevel((unsigned char*) (header + 1), dataSize, p_skip);
			break;
		case LEVEL_BLOCK_DEFAULT_BLOX: // DEFT
			m_ai->m_map->LoadDefaultBlox((class LoadDefaultBlox*) (header + 1), dataSize);
			break;
		case LEVEL_BLOCK_DOORS: // DOOR
			m_ai->m_doorManager->LoadLevel((unsigned char*) (header + 1), dataSize, p_skip);
			break;
		case LEVEL_BLOCK_END: // END?
			endFound = true;
			break;
		case LEVEL_BLOCK_ENEMY_GROUPS: // ENMY
			m_ai->m_enemyGroupManager->LoadLevel((LoadEnemyData*) (header + 1), dataSize, p_skip);
			break;
		case LEVEL_BLOCK_FLAGS: // FLAG
			m_ai->LoadFlagInfo((unsigned char*) (header + 1), dataSize);
			break;
		case LEVEL_BLOCK_GROUND_SURFACE: // GDSF
			m_ai->m_map->LoadLevel((LoadGroundSurfaceData*) (header + 1), dataSize, p_skip);
			break;
		case LEVEL_BLOCK_OBJECTS: // GMOB
			m_ai->m_objectManager->LoadLevel((unsigned char*) (header + 1), dataSize, p_skip);
			break;
		case LEVEL_BLOCK_HANDS: // HAND
			m_ai->m_handManager->LoadLevel((unsigned char*) (header + 1), dataSize, p_skip);
			break;
		case LEVEL_BLOCK_ICE: // ICE
			m_ai->m_iceManager->LoadLevel((unsigned char*) (header + 1), dataSize, p_skip);
			break;
		case LEVEL_BLOCK_INVISIBLE_SWITCHES: // INVS
			m_ai->m_invisibleSwitchManager->LoadLevel((unsigned char*) (header + 1), dataSize, p_skip);
			break;
		case LEVEL_BLOCK_LASERS: // LASR
			m_ai->m_laserManager->LoadLevel((unsigned char*) (header + 1), dataSize, p_skip);
			break;
		case LEVEL_BLOCK_LIFTS: // LIFT
			m_ai->m_liftManager->LoadLevel((unsigned char*) (header + 1), dataSize, p_skip);
			break;
		case LEVEL_BLOCK_MINES: // MINE
			m_ai->m_mineManager->LoadLevel((unsigned char*) (header + 1), dataSize, p_skip);
			break;
		case LEVEL_BLOCK_MOVERS: // MOVE
			m_ai->m_moverManager->LoadLevel((unsigned char*) (header + 1), dataSize, p_skip);
			break;
		case LEVEL_BLOCK_NAME: // NAME
			m_ai->m_map->LoadLevelName((LoadGroundName*) (header + 1), dataSize);
			break;
		case LEVEL_BLOCK_NETWORK_STARTS: // NETW
			if (m_ai->m_networkMode != 1) {
				m_ai->m_trapDoorManager->LoadLevel((unsigned char*) (header + 1), dataSize, p_skip);
			}
			else {
				if (m_ai->m_isHost == 0) {
					m_ai->m_playerGroupManager->LoadAdditionalPlayerStartPositions((unsigned char*) (header + 1),
																				   dataSize,
																				   p_skip);
				}
				else {
					m_ai->m_trapDoorManager->LoadLevel((unsigned char*) (header + 1), dataSize, p_skip);
				}
			}
			break;
		case LEVEL_BLOCK_NODES: // NODE
			m_ai->m_nodeManager->LoadLevel((unsigned char*) (header + 1), dataSize, p_skip);
			break;
		case LEVEL_BLOCK_PAINT_GUNS: // PGUN
			m_ai->m_paintGunManager->LoadLevel((unsigned char*) (header + 1), dataSize, p_skip);
			break;
		case LEVEL_BLOCK_PLAYER_GROUPS: // PLAS
			m_ai->m_playerGroupManager->LoadLevel((unsigned char*) (header + 1), dataSize, p_skip);
			break;
		case LEVEL_BLOCK_PLAYER_STARTS: // PLS1
			if (m_ai->m_networkMode != 1) {
				m_ai->m_playerGroupManager->LoadAdditionalPlayerStartPositions((unsigned char*) (header + 1),
																			   dataSize,
																			   p_skip);
			}
			else {
				if (m_ai->m_isHost == 1) {
					m_ai->m_playerGroupManager->LoadAdditionalPlayerStartPositions((unsigned char*) (header + 1),
																				   dataSize,
																				   p_skip);
				}
				else {
					m_ai->m_trapDoorManager->LoadLevel((unsigned char*) (header + 1), dataSize, p_skip);
				}
			}
			break;
		case LEVEL_BLOCK_ROCKETS: // ROCK
			m_ai->m_rocketManager->LoadLevel((unsigned char*) (header + 1), dataSize, p_skip);
			break;
		case LEVEL_BLOCK_SHEEP_GROUPS: // SHPG
			m_ai->m_sheepGroupManager->LoadLevel((LoadSheepData*) (header + 1), dataSize, p_skip);
			break;
		case LEVEL_BLOCK_SLINKIES: // SLNK
			m_ai->m_slinkyManager->LoadLevel((unsigned char*) (header + 1), dataSize, p_skip);
			break;
		case LEVEL_BLOCK_TRAMPOLINES: // TRAM
			m_ai->m_trampolineManager->LoadLevel((unsigned char*) (header + 1), dataSize, p_skip);
			break;
		}

		header = GetNextBlockHeader(header);
	} while (!endFound);

	if (g_nEditLevelMode == 0 && g_nPlayLevelMode == 0) {
		binResource->m_directUseCount--;
		binResource->UnLoad();
	}
	else {
		operator delete(g_pLevelFileData);
	}
}
