#ifndef LEMBALL_CONTROL_LEVEL_LEVELLOADER_H
#define LEMBALL_CONTROL_LEVEL_LEVELLOADER_H

#include "../../Common.h"

// SIZE 0x08
class LevelLoader {
public:
	LevelLoader(Ai* p_arg0);
	LoadBlockHeader* GetNextBlockHeader(LoadBlockHeader* p_header);
	bool LocateStartOfLevelFile();
	unsigned int CalcLevelId(eSkill p_skill, int p_level);
	void LoadLevel(eSkill p_skill, int p_level, unsigned char p_skip);
	void RetrievePreviewData(eSkill p_skill, int p_level, PreviewData* p_preview);

private:
	Ai* m_ai;            // 0x00
	int m_fallbackLevel; // 0x04
};

// Confirmed class-scoped globals.
extern int g_nEditLevelMode;
extern int g_nPlayLevelMode;
extern char g_szCommandLineLevelFile[232];
extern char* g_pActiveLevelFile;
extern void* g_pLevelFileData;
#endif
