#include "LevelLoader.h"

#include "../../Visos/Foundation/VsFile.h"

extern "C" __declspec(dllimport) int __stdcall MessageBoxA(void* p_window, char* p_text, char* p_caption, unsigned int p_type);

// 68K 0x1070267c __ct__12CLevelLoaderFP3CAI
// STUB: LEMBALL 0x00408210
LevelLoader::LevelLoader(Ai* p_arg0)
{
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
// STUB: LEMBALL 0x00408830
LoadBlockHeader* LevelLoader::GetNextBlockHeader(LoadBlockHeader* p_header)
{
	return 0;
}

// 68K 0x10702e0a RetrievePreviewData__12CLevelLoaderF6eSkilliP12tPreviewData
// STUB: LEMBALL 0x00408850
void LevelLoader::RetrievePreviewData(eSkill p_skill, int p_level, PreviewData* p_preview)
{
}

// 68K 0x10703218 CalcLevelID__12CLevelLoaderF6eSkilli
// STUB: LEMBALL 0x00408b00
unsigned int LevelLoader::CalcLevelId(eSkill p_skill, int p_level)
{
	return 0;
}

// GLOBAL: LEMBALL 0x0049ce34
char g_szYouStupidStupidMan[28] = "You Stupid, Stupid Man !";

// GLOBAL: LEMBALL 0x0049ce50
char g_szOkSmartarse[140] =
	"OK Smartarse,\nHow the hell do you expect me to load a level\nwhen you can't even type the name\n in correctly !\n Quit out and try again...\n";

// GLOBAL: LEMBALL 0x0049cedc
char g_szReadBinaryMode[4] = "rb";

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
