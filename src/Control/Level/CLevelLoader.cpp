#include "CLevelLoader.h"

#include "../../AI/Groups/CEnemyGroupManager.h"
#include "../../AI/Groups/CPlayerLemmingGroupManager.h"
#include "../../AI/Groups/CSheepGroupManager.h"
#include "../../AI/Managers/CBallManager.h"
#include "../../AI/Managers/CCollectableManager.h"
#include "../../AI/Managers/CDoorManager.h"
#include "../../AI/Managers/CHandManager.h"
#include "../../AI/Managers/CIceManager.h"
#include "../../AI/Managers/CInvisibleSwitchManager.h"
#include "../../AI/Managers/CLaserManager.h"
#include "../../AI/Managers/CLiftManager.h"
#include "../../AI/Managers/CMineManager.h"
#include "../../AI/Managers/CObjectManager.h"
#include "../../AI/Managers/CPaintGunManager.h"
#include "../../AI/Managers/CRocketManager.h"
#include "../../AI/Managers/CSlinkyManager.h"
#include "../../AI/Managers/CTrampolineManager.h"
#include "../../AI/Managers/CTrapDoorManager.h"
#include "../../AI/Navigation/CAI.h"
#include "../../AI/Navigation/CMoverManager.h"
#include "../../AI/Navigation/CNodeManager.h"
#include "../../AI/Objects/CBalloonPost.h"
#include "../../AI/Objects/CGroundAnim.h"
#include "../../Map/Base/CMap.h"
#include "../../Visos/Foundation/CVSOStream.h"
#include "../../Visos/Foundation/VsFile.h"
#include "../../Visos/Network/CConnect.h"
#include "../../Visos/Resources/CResBIN.h"
#include "../../Visos/Resources/Manifest.h"
#include "../Support/tPreviewData.h"
#include "tagLoadBlockHeader.h"

#include <string.h>
struct tagLoadEnemyData;
struct tagLoadGroundName;
struct tagLoadGroundSurfaceData;
struct tagLoadSheepData;
struct _Filet;

extern "C" __declspec(dllimport) int __stdcall MessageBoxA(void* p_window,
														   char* p_text,
														   char* p_caption,
														   unsigned int p_type);

#define LEVEL_AI_UNVERSIONED_DATA_BYTES (2 * sizeof(unsigned short))
#define LEVEL_AI_FIRST_VERSION_WITH_COUNTS 4
#define LEVEL_AI_LEGACY_LEMMING_COUNT 4
#define LEVEL_AI_LEGACY_PLAYER_COUNT 1
#define LEVEL_START_COORDINATE_WORDS 3

extern char g_szNSkillFormat[];
extern char g_szNLevelFormat[];
extern char g_szNameBracketFormat[];
extern char g_szCloseBracketNewline[];

// FUNCTION: LEMBALL 0x00408210
CLevelLoader::CLevelLoader(CAI* p_ai)
{
	m_ai = p_ai;
	m_fallbackLevel = 0;
	g_pLevelFileData = 0;
	g_pActiveLevelFile = (char*) m_ai->LevelName();
}

// FUNCTION: LEMBALL 0x00408240
void CLevelLoader::LoadLevel(eSkill p_skill, int p_level, unsigned int p_skip)
{
	bool endFound = false;
	CResBIN* binResource = 0;
	tagLoadBlockHeader* header;
	unsigned int dataSize;
	unsigned int blockType;

	if (p_level == 9999) {
		p_level = m_fallbackLevel;
	}
	if (g_nEditLevelMode == 0 && g_nPlayLevelMode == 0) {
		unsigned int resourceId = CalcLevelID(p_skill, p_level);
		binResource = CResBIN::Load(resourceId);
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
		dataSize -= sizeof(*header);
		switch (blockType) {
		case LEVEL_BLOCK_AI:
			m_ai->LoadLevel((unsigned char*) (header + 1), dataSize, p_skip);
			break;
		case LEVEL_BLOCK_GROUND_ANIMS:
			m_ai->m_groundAnim->LoadLevel((unsigned char*) (header + 1), dataSize, p_skip);
			break;
		case LEVEL_BLOCK_BALLS:
			m_ai->m_ballManager->LoadLevel((unsigned char*) (header + 1), dataSize, p_skip);
			break;
		case LEVEL_BLOCK_BALLOON_POSTS:
			m_ai->m_balloonPost->LoadLevel((unsigned char*) (header + 1), dataSize, p_skip);
			break;
		case LEVEL_BLOCK_COLLECTABLES:
			m_ai->m_collectableManager->LoadLevel((unsigned char*) (header + 1), dataSize, p_skip);
			break;
		case LEVEL_BLOCK_DEFAULT_BLOX:
			m_ai->m_map->LoadDefaultBlox((tagLoadDefaultBlox*) (header + 1), dataSize);
			break;
		case LEVEL_BLOCK_DOORS:
			m_ai->m_doorManager->LoadLevel((unsigned char*) (header + 1), dataSize, p_skip);
			break;
		case LEVEL_BLOCK_END:
			endFound = true;
			break;
		case LEVEL_BLOCK_ENEMY_GROUPS:
			m_ai->m_enemyGroupManager->LoadLevel((tagLoadEnemyData*) (header + 1), dataSize, p_skip);
			break;
		case LEVEL_BLOCK_FLAGS:
			m_ai->LoadFlagInfo((unsigned char*) (header + 1), dataSize);
			break;
		case LEVEL_BLOCK_GROUND_SURFACE:
			m_ai->m_map->LoadLevel((tagLoadGroundSurfaceData*) (header + 1), dataSize, p_skip);
			break;
		case LEVEL_BLOCK_OBJECTS:
			m_ai->m_objectManager->LoadLevel((unsigned char*) (header + 1), dataSize, p_skip);
			break;
		case LEVEL_BLOCK_HANDS:
			m_ai->m_handManager->LoadLevel((unsigned char*) (header + 1), dataSize, p_skip);
			break;
		case LEVEL_BLOCK_ICE:
			m_ai->m_iceManager->LoadLevel((unsigned char*) (header + 1), dataSize, p_skip);
			break;
		case LEVEL_BLOCK_INVISIBLE_SWITCHES:
			m_ai->m_invisibleSwitchManager->LoadLevel((unsigned char*) (header + 1), dataSize, p_skip);
			break;
		case LEVEL_BLOCK_LASERS:
			m_ai->m_laserManager->LoadLevel((unsigned char*) (header + 1), dataSize, p_skip);
			break;
		case LEVEL_BLOCK_LIFTS:
			m_ai->m_liftManager->LoadLevel((unsigned char*) (header + 1), dataSize, p_skip);
			break;
		case LEVEL_BLOCK_MINES:
			m_ai->m_mineManager->LoadLevel((unsigned char*) (header + 1), dataSize, p_skip);
			break;
		case LEVEL_BLOCK_MOVERS:
			m_ai->m_moverManager->LoadLevel((unsigned char*) (header + 1), dataSize, p_skip);
			break;
		case LEVEL_BLOCK_NAME:
			m_ai->m_map->LoadLevelName((tagLoadGroundName*) (header + 1), dataSize);
			break;
		case LEVEL_BLOCK_NETWORK_STARTS:
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
		case LEVEL_BLOCK_NODES:
			m_ai->m_nodeManager->LoadLevel((unsigned char*) (header + 1), dataSize, p_skip);
			break;
		case LEVEL_BLOCK_PAINT_GUNS:
			m_ai->m_paintGunManager->LoadLevel((unsigned char*) (header + 1), dataSize, p_skip);
			break;
		case LEVEL_BLOCK_PLAYER_GROUPS:
			m_ai->m_playerGroupManager->LoadLevel((unsigned char*) (header + 1), dataSize, p_skip);
			break;
		case LEVEL_BLOCK_PLAYER_STARTS:
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
		case LEVEL_BLOCK_ROCKETS:
			m_ai->m_rocketManager->LoadLevel((unsigned char*) (header + 1), dataSize, p_skip);
			break;
		case LEVEL_BLOCK_SHEEP_GROUPS:
			m_ai->m_sheepGroupManager->LoadLevel((tagLoadSheepData*) (header + 1), dataSize, p_skip);
			break;
		case LEVEL_BLOCK_SLINKIES:
			m_ai->m_slinkyManager->LoadLevel((unsigned char*) (header + 1), dataSize, p_skip);
			break;
		case LEVEL_BLOCK_TRAMPOLINES:
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

// FUNCTION: LEMBALL 0x004087b0
bool CLevelLoader::LocateStartOfLevelFile()
{
	_Filet* file;
	unsigned int size;

	file = vsOpen(g_pActiveLevelFile, g_szReadBinaryMode);
	if (file != 0) {
		size = vsSeek(file, 0, 2);
		vsSeek(file, 0, 0);
		g_pLevelFileData = operator new(size);
		vsRead(file, g_pLevelFileData, size);
		vsClose(file);
		return 1;
	}
	MessageBoxA(0, g_szOkSmartarse, g_szYouStupidStupidMan, 0);
	return 0;
}

// FUNCTION: LEMBALL 0x00408830
tagLoadBlockHeader* CLevelLoader::GetNextBlockHeader(tagLoadBlockHeader* p_header)
{
	unsigned int size;

	if (p_header == 0) {
		return (tagLoadBlockHeader*) g_pLevelFileData;
	}
	size = p_header->m_size;
	unsigned int remainder = size & LEVEL_BLOCK_ALIGNMENT_MASK;
	if (remainder != 0) {
		size = (size - remainder) + LEVEL_BLOCK_ALIGNMENT;
	}
	p_header = (tagLoadBlockHeader*) ((unsigned char*) p_header + size);
	return p_header;
}

// FUNCTION: LEMBALL 0x00408850
void CLevelLoader::RetrievePreviewData(eSkill p_skill, int p_level, tPreviewData* p_preview)
{
	bool endFound = false;
	CResBIN* binResource = 0;
	tagLoadBlockHeader* header;
	unsigned short* data16;
	unsigned int dataSize;
	unsigned int blockType;
	int count;
	unsigned int total;

	if (g_nEditLevelMode == 0 && g_nPlayLevelMode == 0) {
		unsigned int resourceId = CalcLevelID(p_skill, p_level);
		binResource = CResBIN::Load(resourceId);
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
		g_pActiveLevelFile = g_szCommandLineLevelFile;
		LocateStartOfLevelFile();
	}

	header = GetNextBlockHeader(0);
	do {
		dataSize = header->m_size - sizeof(*header);
		data16 = (unsigned short*) (header + 1);
		blockType = header->m_type;

		switch (blockType) {
		case LEVEL_BLOCK_AI: {
			unsigned short version = dataSize > LEVEL_AI_UNVERSIONED_DATA_BYTES ? *data16++ : 0;
			data16++;
			p_preview->m_timeLimit = *data16;
			data16++;
			if (version >= LEVEL_AI_FIRST_VERSION_WITH_COUNTS) {
				p_preview->m_lemmingCount = data16[0];
				p_preview->m_playerCount = data16[1];
			}
			else {
				p_preview->m_lemmingCount = LEVEL_AI_LEGACY_LEMMING_COUNT;
				p_preview->m_playerCount = LEVEL_AI_LEGACY_PLAYER_COUNT;
			}
			break;
		}
		case LEVEL_BLOCK_GROUND_ANIMS:
		case LEVEL_BLOCK_BALLS:
		case LEVEL_BLOCK_BALLOON_POSTS:
		case LEVEL_BLOCK_COLLECTABLES:
		case LEVEL_BLOCK_DEFAULT_BLOX:
		case LEVEL_BLOCK_DOORS:
			break;
		case LEVEL_BLOCK_END:
			endFound = true;
			break;
		case LEVEL_BLOCK_ENEMY_GROUPS:
		case LEVEL_BLOCK_FLAGS:
		case LEVEL_BLOCK_GROUND_SURFACE:
		case LEVEL_BLOCK_OBJECTS:
		case LEVEL_BLOCK_HANDS:
		case LEVEL_BLOCK_ICE:
		case LEVEL_BLOCK_INVISIBLE_SWITCHES:
		case LEVEL_BLOCK_LASERS:
		case LEVEL_BLOCK_LIFTS:
		case LEVEL_BLOCK_MINES:
		case LEVEL_BLOCK_MOVERS:
			break;
		case LEVEL_BLOCK_NAME: {
			strcpy(p_preview->m_name, (char*) data16);
			p_preview->m_name[sizeof(p_preview->m_name) - 1] = 0;
			break;
		}
		case LEVEL_BLOCK_NETWORK_STARTS: {
			total = 0;
			count = (unsigned int) *data16++;
			while (count > 0) {
				data16 += LEVEL_START_COORDINATE_WORDS;
				total += (unsigned int) *data16++;
				count--;
			}
			if (g_pActiveConnection == 0) {
				p_preview->m_opponentLemmingCount = total;
			}
			else if (g_pActiveConnection->m_isHost == 1) {
				p_preview->m_opponentLemmingCount = total;
			}
			else {
				p_preview->m_lemmingCount = total;
			}
			break;
		}
		case LEVEL_BLOCK_NODES:
		case LEVEL_BLOCK_PAINT_GUNS:
		case LEVEL_BLOCK_PLAYER_GROUPS:
			break;
		case LEVEL_BLOCK_PLAYER_STARTS: {
			total = 0;
			count = (unsigned int) *data16++;
			while (count > 0) {
				data16 += LEVEL_START_COORDINATE_WORDS;
				total += (unsigned int) *data16++;
				count--;
			}
			if (g_pActiveConnection == 0) {
				p_preview->m_lemmingCount = total;
			}
			else {
				if (g_pActiveConnection->m_isHost == 1) {
					p_preview->m_lemmingCount = total;
				}
				else {
					p_preview->m_opponentLemmingCount = total;
				}
			}
			break;
		}
		case LEVEL_BLOCK_ROCKETS:
		case LEVEL_BLOCK_SHEEP_GROUPS:
		case LEVEL_BLOCK_SLINKIES:
		case LEVEL_BLOCK_TRAMPOLINES:
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
	*g_pDebugOutput << g_szNSkillFormat << (int) p_skill << g_szNLevelFormat << p_level << g_szNameBracketFormat
					<< p_preview->m_name << g_szCloseBracketNewline;
}

// FUNCTION: LEMBALL 0x00408b00
unsigned int CLevelLoader::CalcLevelID(eSkill p_skill, int p_level)
{
	switch (p_skill) {
	case SKILL_FUN:
		return p_level + RES_FUN_LEVEL_00;
	case SKILL_TRICKY:
		return p_level + RES_TRICKY_LEVEL_00;
	case SKILL_TAXING:
		return p_level + RES_TAXING_LEVEL_00;
	case SKILL_MAYHEM:
		return p_level + RES_MAYHEM_LEVEL_00;
	default:
		return p_level + RES_NETWORK_LEVEL_00;
	}
}

// GLOBAL: LEMBALL 0x0049ce34
char g_szYouStupidStupidMan[28] = "You Stupid, Stupid Man !";

// GLOBAL: LEMBALL 0x0049ce50
char g_szOkSmartarse[140] = "OK Smartarse,\nHow the hell do you expect me to load a level\nwhen you can't even type "
							"the name\n in correctly !\n Quit out and try again...\n";

// GLOBAL: LEMBALL 0x0049cedc
char g_szReadBinaryMode[4] = "rb";

// GLOBAL: LEMBALL 0x0049cee0
char g_szNSkillFormat[] = "nSkill=";

// GLOBAL: LEMBALL 0x0049cee8
char g_szNLevelFormat[] = " nLevel=";

// GLOBAL: LEMBALL 0x0049cef4
char g_szNameBracketFormat[] = " Name= <";

// GLOBAL: LEMBALL 0x0049cf00
char g_szCloseBracketNewline[] = ">\n";

// GLOBAL: LEMBALL 0x004a6304
int g_nEditLevelMode = 0;

// GLOBAL: LEMBALL 0x004a6308
int g_nPlayLevelMode = 0;

// GLOBAL: LEMBALL 0x004a6314
char g_szCommandLineLevelFile[232] = {0};

// GLOBAL: LEMBALL 0x004a63fc
char* g_pActiveLevelFile = 0;

// GLOBAL: LEMBALL 0x004a6400
void* g_pLevelFileData;
