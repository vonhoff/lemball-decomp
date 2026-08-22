#include "LevelLoader.h"

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
// STUB: LEMBALL 0x004087b0
bool LevelLoader::LocateStartOfLevelFile()
{
	// STRING: LEMBALL 0x0049cedc "rb"
	// STRING: LEMBALL 0x0049ce34 "You Stupid, Stupid Man !"
	// STRING: LEMBALL 0x0049ce50 "OK Smartarse,\nHow the hell do you expect me to load a level\nwhen you can't even
	// type the name\n in correctly !\n Quit out and try again...\n"
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
	// STRING: LEMBALL 0x0049cef4 " Name= <"
	// STRING: LEMBALL 0x0049cee8 " nLevel="
	// STRING: LEMBALL 0x0049cee0 "nSkill="
}

// 68K 0x10703218 CalcLevelID__12CLevelLoaderF6eSkilli
// STUB: LEMBALL 0x00408b00
unsigned int LevelLoader::CalcLevelId(eSkill p_skill, int p_level)
{
	return 0;
}

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
