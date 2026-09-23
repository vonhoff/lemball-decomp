#ifndef LEMBALL_CONTROL_GAME_CGAMESTATUS_H
#define LEMBALL_CONTROL_GAME_CGAMESTATUS_H

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
	int m_level;               // 0x00
	unsigned int m_levelState; // 0x04
	int m_skill;               // 0x08
	unsigned int m_skillState; // 0x0c
	char m_password[10];       // 0x10
	unsigned short m_pad0x1a;  // 0x1a
	unsigned int m_pad0x1c;    // 0x1c
	unsigned int m_status0;    // 0x20
	unsigned int m_status1;    // 0x24
	int m_maxLevels[5];        // 0x28
	int m_lastLevels[5];       // 0x3c
};

extern CGameStatus* g_pGameStatus;
extern int g_anPasswordPermutation[8];
#endif
