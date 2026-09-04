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
LevelLoader::LevelLoader(Ai* p_arg0)
{
	m_ai = p_arg0;
	m_fallbackLevel = 0;
	g_pLevelFileData = 0;
	g_pActiveLevelFile = (char*) m_ai->LevelName();
}

// 68K 0x107026ca LoadLevel__12CLevelLoaderF6eSkilliUc
// STUB: LEMBALL 0x00408240
void LevelLoader::LoadLevel(eSkill p_skill, int p_level, unsigned char p_skip)
{
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
	unsigned short version;
	unsigned int count;
	unsigned short total;

	if (g_nEditLevelMode == 0 && g_nPlayLevelMode == 0) {
		unsigned int resourceId = CalcLevelId(p_skill, p_level);
		binResource = ResBin::Load(resourceId);
		if (binResource->m_loaded == 0) {
			binResource->LoadData();
		}
		else {
			binResource->m_age = 0;
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
		unsigned short* data16 = (unsigned short*) (header + 1);
		unsigned int blockType = header->m_type;

		switch (blockType) {
		case 0x41492020: {
			unsigned short* cursor = (unsigned short*) (header + 1);
			version = 0;
			if (header->m_size - 8 > 4) {
				version = *cursor++;
			}
			p_preview->m_timeLimit = cursor[1];
			cursor += 2;
			if (version < 4) {
				p_preview->m_lemmingCount = 4;
				p_preview->m_playerCount = 1;
			}
			else {
				p_preview->m_lemmingCount = cursor[0];
				p_preview->m_playerCount = cursor[1];
			}
			break;
		}
		case 0x42414c4c:
			break;
		case 0x454e443f:
			endFound = true;
			break;
		case 0x464c4147:
			break;
		case 0x4e414d45: {
			char* src = (char*) data16;
			char* dst = p_preview->m_name;
			int len = 0;
			while (src[len] != 0 && len < 32) {
				dst[len] = src[len];
				len++;
			}
			dst[len] = 0;
			p_preview->m_name[0x20] = 0;
			break;
		}
		case 0x4e455457: {
			unsigned char* edi = (unsigned char*) (header + 1);
			total = 0;
			for (count = (unsigned int) *(unsigned short*) edi; count != 0; count--) {
				edi += 8;
				total = (unsigned short) (total + *(short*) edi);
			}
			if (g_pActiveConnection == 0 || g_pActiveConnection->m_isHost == 1) {
				p_preview->m_opponentLemmingCount = total;
			}
			else {
				p_preview->m_lemmingCount = total;
			}
			break;
		}
		case 0x504c5331: {
			unsigned char* edi = (unsigned char*) (header + 1);
			total = 0;
			for (count = (unsigned int) *(unsigned short*) edi; count != 0; count--) {
				edi += 8;
				total = (unsigned short) (total + *(short*) edi);
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
		}

		header = GetNextBlockHeader(header);
		if (endFound) {
			if (g_nEditLevelMode == 0 && g_nPlayLevelMode == 0) {
				binResource->m_directUseCount--;
				binResource->UnLoad();
			}
			else {
				operator delete(g_pLevelFileData);
			}
			*g_pDebugOutput << g_szNSkillFormat << (int) p_skill << g_szNLevelFormat << p_level << g_szNameBracketFormat
							<< p_preview->m_name << g_szCloseBracketNewline;
			return;
		}
	} while (1);
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
