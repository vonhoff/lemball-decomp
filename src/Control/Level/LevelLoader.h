#ifndef LEMBALL_CONTROL_LEVEL_LEVELLOADER_H
#define LEMBALL_CONTROL_LEVEL_LEVELLOADER_H

class Ai;
struct LoadBlockHeader;
struct PreviewData;
enum eSkill {
	SKILL_FUN = 0,
	SKILL_TRICKY = 1,
	SKILL_TAXING = 2,
	SKILL_MAYHEM = 3,
	SKILL_NETWORK = 4
};

// SIZE 0x08
class LevelLoader {
public:
	LevelLoader(Ai* p_ai);
	static LoadBlockHeader* GetNextBlockHeader(LoadBlockHeader* p_header);
	static bool LocateStartOfLevelFile();
	static unsigned int CalcLevelId(eSkill p_skill, int p_level);
	void LoadLevel(eSkill p_skill, int p_level, unsigned int p_skip);
	static void RetrievePreviewData(eSkill p_skill, int p_level, PreviewData* p_preview);

private:
	Ai* m_ai;            // 0x00
	int m_fallbackLevel; // 0x04
};

extern int g_nEditLevelMode;
extern int g_nPlayLevelMode;
extern char g_szCommandLineLevelFile[232];
extern char* g_pActiveLevelFile;
extern void* g_pLevelFileData;
extern char g_szYouStupidStupidMan[28];
extern char g_szOkSmartarse[140];
extern char g_szReadBinaryMode[4];
#endif
