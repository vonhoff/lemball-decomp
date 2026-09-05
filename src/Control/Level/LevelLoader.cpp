#include "LevelLoader.h"

#include "../../AI/Navigation/Ai.h"
#include "../../Visos/Foundation/VsFile.h"
#include "../../Visos/Foundation/VsOStream.h"
#include "../../Visos/Network/Connect.h"
#include "../../Visos/Resources/Manifest.h"
#include "../../Visos/Resources/ResBin.h"
#include "../Support/PreviewData.h"
#include "LoadBlockHeader.h"

#include <new.h>
#include <string.h>

extern "C" __declspec(dllimport) int __stdcall MessageBoxA(void* p_window,
														   char* p_text,
														   char* p_caption,
														   unsigned int p_type);

extern char g_szNSkillFormat[];
extern char g_szNLevelFormat[];
extern char g_szNameBracketFormat[];
extern char g_szCloseBracketNewline[];

// 68K 0x1070267c __ct__12CLevelLoaderFP3CAI
// FUNCTION: LEMBALL 0x00408210
LevelLoader::LevelLoader(Ai* p_ai)
{
	m_ai = p_ai;
	m_fallbackLevel = 0;
	g_pLevelFileData = 0;
	g_pActiveLevelFile = (char*) m_ai->LevelName();
}

// 68K 0x10702cdc LocateStartOfLevelFile__12CLevelLoaderFv
// FUNCTION: LEMBALL 0x004087b0
bool LevelLoader::LocateStartOfLevelFile()
{
	_Filet* file;
	unsigned int size;

	file = VsOpen(g_pActiveLevelFile, g_szReadBinaryMode);
	if (file != 0) {
		size = VsSeek(file, 0, 2);
		VsSeek(file, 0, 0);
		g_pLevelFileData = operator new(size);
		VsRead(file, g_pLevelFileData, size);
		VsClose(file);
		return 1;
	}
	MessageBoxA(0, g_szOkSmartarse, g_szYouStupidStupidMan, 0);
	return 0;
}

// 68K 0x10702d8c GetNextBlockHeader__12CLevelLoaderFP18tagLoadBlockHeader
// FUNCTION: LEMBALL 0x00408830
LoadBlockHeader* LevelLoader::GetNextBlockHeader(LoadBlockHeader* p_header)
{
	unsigned int size;
	unsigned char* raw;

	if (p_header == 0) {
		return (LoadBlockHeader*) g_pLevelFileData;
	}
	size = p_header->m_size;
	if ((size & 3) != 0) {
		size = (size - (size & 3)) + 4;
	}
	raw = (unsigned char*) p_header;
	return (LoadBlockHeader*) (raw + size);
}

// 68K 0x10702e0a RetrievePreviewData__12CLevelLoaderF6eSkilliP12tPreviewData
// FUNCTION: LEMBALL 0x00408850
void LevelLoader::RetrievePreviewData(eSkill p_skill, int p_level, PreviewData* p_preview)
{
	bool endFound = false;
	ResBin* binResource = 0;
	LoadBlockHeader* header;
	unsigned short* data16;
	unsigned int dataSize;
	unsigned int blockType;
	int count;
	unsigned int total;

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
		g_pActiveLevelFile = g_szCommandLineLevelFile;
		LocateStartOfLevelFile();
	}

	header = GetNextBlockHeader(0);
	do {
		dataSize = header->m_size;
		data16 = (unsigned short*) (header + 1);
		dataSize -= 8;
		blockType = header->m_type;

		switch (blockType) {
		case LEVEL_BLOCK_AI: {
			unsigned short version = 0;
			if (dataSize > 4) {
				version = *data16++;
			}
			data16++;
			p_preview->m_timeLimit = *data16;
			data16++;
			if (version >= 4) {
				p_preview->m_lemmingCount = data16[0];
				p_preview->m_playerCount = data16[1];
			}
			else {
				p_preview->m_lemmingCount = 4;
				p_preview->m_playerCount = 1;
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
			p_preview->m_name[0x20] = 0;
			break;
		}
		case LEVEL_BLOCK_NETWORK_STARTS: {
			total = 0;
			count = (unsigned int) *data16++;
			while (count > 0) {
				data16 += 3;
				total += (unsigned int) *data16++;
				count--;
			}
			if (g_pActiveConnection == 0 || g_pActiveConnection->m_isHost == 1) {
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
				data16 += 3;
				total += (unsigned int) *data16++;
				count--;
			}
			if (g_pActiveConnection == 0) {
				p_preview->m_lemmingCount = total;
			}
			else {
				if (g_pActiveConnection->m_isHost != 1) {
					p_preview->m_opponentLemmingCount = total;
				}
				else {
					p_preview->m_lemmingCount = total;
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

// 68K 0x10703218 CalcLevelID__12CLevelLoaderF6eSkilli
// FUNCTION: LEMBALL 0x00408b00
unsigned int LevelLoader::CalcLevelId(eSkill p_skill, int p_level)
{
	switch (p_skill) {
	case 0:
		return p_level + RES_FUN_LEVEL_00;
	case 1:
		return p_level + RES_TRICKY_LEVEL_00;
	case 2:
		return p_level + RES_TAXING_LEVEL_00;
	case 3:
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
