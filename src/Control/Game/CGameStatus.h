#ifndef LEMBALL_CONTROL_GAME_CGAMESTATUS_H
#define LEMBALL_CONTROL_GAME_CGAMESTATUS_H

enum eGameResultMessage {
	GAME_RESULT_MESSAGE_UNSET = 0,
	GAME_RESULT_MESSAGE_BEST_SCORE = 1,
	GAME_RESULT_MESSAGE_FLAGS_COLLECTED = 2,
	GAME_RESULT_MESSAGE_LEMMING_ELIMINATION = 3,
	GAME_RESULT_MESSAGE_TIME_EXPIRED = 4,
	GAME_RESULT_MESSAGE_GAVE_UP = 5
};

enum eSkillLevelCount {
	SKILL_LEVEL_COUNT_FUN = 24,
	SKILL_LEVEL_COUNT_TRICKY = 25,
	SKILL_LEVEL_COUNT_TAXING = 28,
	SKILL_LEVEL_COUNT_MAYHEM = 21,
	SKILL_LEVEL_COUNT_NETWORK = 11
};

enum {
	GAME_SCORE_MAX_DISPLAY_VALUE = 9999999,
	GAME_SCORE_DISPLAY_VALUE_LIMIT = GAME_SCORE_MAX_DISPLAY_VALUE + 1
};

#define PASSWORD_ENCODED_SKILL_COUNT 4
#define PASSWORD_LEVEL_CHUNKS_PER_SKILL 2
#define PASSWORD_LEVEL_DATA_CHUNK_COUNT 8
#define PASSWORD_LEVEL_DATA_CHUNK_BITS 3
#define PASSWORD_LEVEL_DATA_CHUNK_MASK 7
#define PASSWORD_LEVEL_DATA_HIGH_CHUNK_MASK 0x38
#define PASSWORD_LEVEL_DATA_PAYLOAD_MASK 0x00ffffff
#define PASSWORD_CHECKSUM_MASK 0x1f
#define PASSWORD_CHECKSUM_FIELD_MASK 0x1f000000
#define PASSWORD_CHECKSUM_SHIFT 24
#define PASSWORD_CODE_DIGIT_COUNT 10
#define PASSWORD_UNLOCKED_LEVEL_LIMIT 0x40

// SIZE 0x50
class CGameStatus {
public:
	CGameStatus();
	bool DecLevel();
	bool DecodePassword(char* p_password);
	bool LastLevelAvailable();
	bool NextLevelAvailable();
	char* EncodePassword();
	int Level();
	int NoOfLevelsInSkill(int p_skill);
	int StringToDWord();
	unsigned int CalcCheckSum(unsigned int p_value);
	unsigned int JiggleLevelData();
	void GotoLastLevels();
	void IncLevel();
	void IncSkill(unsigned int p_wrap);
	void DecSkill(unsigned int p_wrap);
	void Level(int p_level);
	void SetMaxLevel(int p_skill, int p_level);
	void UnJiggleLevelData(unsigned int p_value);

	friend class CNetworkOptionsDrawer;
	friend class CPlayerLemming;
	friend class CMainOptions1Drawer;
	friend class CPasswordDrawer;
	friend class CPreviewDrawer;
	friend class CSuccFailDrawer;
	friend class CIntroAnimAnimWindow;
	friend class CBaseFrontendProcess;
	friend class CBaseFrontendDrawer;
	friend class CAI;
	friend class C2D;

private:
	int m_level;                              // 0x00
	unsigned int m_levelState;                // 0x04
	int m_skill;                              // 0x08
	unsigned int m_skillState;                // 0x0c
	char m_password[16];                      // 0x10
	unsigned int m_bulletHitHandlingDisabled; // 0x20
	unsigned int m_unlimitedAmmo;             // 0x24
	int m_maxLevels[5];                       // 0x28
	int m_lastLevels[5];                      // 0x3c
};

extern CGameStatus* g_pGameStatus;
extern int g_anPasswordPermutation[PASSWORD_LEVEL_DATA_CHUNK_COUNT];
#endif
