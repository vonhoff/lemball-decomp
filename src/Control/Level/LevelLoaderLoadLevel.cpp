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
		case 0x41492020: // AI
			m_ai->LoadLevel((unsigned char*) (header + 1), dataSize, p_skip);
			break;
		case 0x414e494d: // ANIM
			m_ai->m_groundAnim->LoadLevel((unsigned char*) (header + 1), dataSize, p_skip);
			break;
		case 0x42414c4c: // BALL
			m_ai->m_ballManager->LoadLevel((unsigned char*) (header + 1), dataSize, p_skip);
			break;
		case 0x424f4f4e: // BOON
			m_ai->m_balloonPost->LoadLevel((unsigned char*) (header + 1), dataSize, p_skip);
			break;
		case 0x434f4c4c: // COLL
			m_ai->m_collectableManager->LoadLevel((unsigned char*) (header + 1), dataSize, p_skip);
			break;
		case 0x44454654: // DEFT
			m_ai->m_map->LoadDefaultBlox((class LoadDefaultBlox*) (header + 1), dataSize);
			break;
		case 0x444f4f52: // DOOR
			m_ai->m_doorManager->LoadLevel((unsigned char*) (header + 1), dataSize, p_skip);
			break;
		case 0x454e443f: // END?
			endFound = true;
			break;
		case 0x454e4d59: // ENMY
			m_ai->m_enemyGroupManager->LoadLevel((LoadEnemyData*) (header + 1), dataSize, p_skip);
			break;
		case 0x464c4147: // FLAG
			m_ai->LoadFlagInfo((unsigned char*) (header + 1), dataSize);
			break;
		case 0x47445346: // GDSF
			m_ai->m_map->LoadLevel((LoadGroundSurfaceData*) (header + 1), dataSize, p_skip);
			break;
		case 0x474d4f42: // GMOB
			m_ai->m_objectManager->LoadLevel((unsigned char*) (header + 1), dataSize, p_skip);
			break;
		case 0x48414e44: // HAND
			m_ai->m_handManager->LoadLevel((unsigned char*) (header + 1), dataSize, p_skip);
			break;
		case 0x49434520: // ICE
			m_ai->m_iceManager->LoadLevel((unsigned char*) (header + 1), dataSize, p_skip);
			break;
		case 0x494e5653: // INVS
			m_ai->m_invisibleSwitchManager->LoadLevel((unsigned char*) (header + 1), dataSize, p_skip);
			break;
		case 0x4c415352: // LASR
			m_ai->m_laserManager->LoadLevel((unsigned char*) (header + 1), dataSize, p_skip);
			break;
		case 0x4c494654: // LIFT
			m_ai->m_liftManager->LoadLevel((unsigned char*) (header + 1), dataSize, p_skip);
			break;
		case 0x4d494e45: // MINE
			m_ai->m_mineManager->LoadLevel((unsigned char*) (header + 1), dataSize, p_skip);
			break;
		case 0x4d4f5645: // MOVE
			m_ai->m_moverManager->LoadLevel((unsigned char*) (header + 1), dataSize, p_skip);
			break;
		case 0x4e414d45: // NAME
			m_ai->m_map->LoadLevelName((LoadGroundName*) (header + 1), dataSize);
			break;
		case 0x4e455457: // NETW
			if (m_ai->m_unk0x64 != 1) {
				m_ai->m_trapDoorManager->LoadLevel((unsigned char*) (header + 1), dataSize, p_skip);
			}
			else {
				if (m_ai->m_unk0x7c == 0) {
					m_ai->m_playerGroupManager->LoadAdditionalPlayerStartPositions((unsigned char*) (header + 1),
																				   dataSize,
																				   p_skip);
				}
				else {
					m_ai->m_trapDoorManager->LoadLevel((unsigned char*) (header + 1), dataSize, p_skip);
				}
			}
			break;
		case 0x4e4f4445: // NODE
			m_ai->m_nodeManager->LoadLevel((unsigned char*) (header + 1), dataSize, p_skip);
			break;
		case 0x5047554e: // PGUN
			m_ai->m_paintGunManager->LoadLevel((unsigned char*) (header + 1), dataSize, p_skip);
			break;
		case 0x504c4153: // PLAS
			m_ai->m_playerGroupManager->LoadLevel((unsigned char*) (header + 1), dataSize, p_skip);
			break;
		case 0x504c5331: // PLS1
			if (m_ai->m_unk0x64 != 1) {
				m_ai->m_playerGroupManager->LoadAdditionalPlayerStartPositions((unsigned char*) (header + 1),
																			   dataSize,
																			   p_skip);
			}
			else {
				if (m_ai->m_unk0x7c == 1) {
					m_ai->m_playerGroupManager->LoadAdditionalPlayerStartPositions((unsigned char*) (header + 1),
																				   dataSize,
																				   p_skip);
				}
				else {
					m_ai->m_trapDoorManager->LoadLevel((unsigned char*) (header + 1), dataSize, p_skip);
				}
			}
			break;
		case 0x524f434b: // ROCK
			m_ai->m_rocketManager->LoadLevel((unsigned char*) (header + 1), dataSize, p_skip);
			break;
		case 0x53485047: // SHPG
			m_ai->m_sheepGroupManager->LoadLevel((LoadSheepData*) (header + 1), dataSize, p_skip);
			break;
		case 0x534c4e4b: // SLNK
			m_ai->m_slinkyManager->LoadLevel((unsigned char*) (header + 1), dataSize, p_skip);
			break;
		case 0x5452414d: // TRAM
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
