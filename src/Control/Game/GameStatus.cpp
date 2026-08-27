#include "GameStatus.h"

#include "../../Visos/Foundation/VsOStream.h"
#include "../../Visos/Foundation/VsString.h"

#include <string.h>

#pragma intrinsic(strlen, strcpy, strcmp, memcpy)

extern char g_szPasswordZeroes[9];
extern char g_szUnlockPassword[11];
extern char g_szPasswordCheatMessage[31];

// 68K 0x107013de __ct__11CGameStatusFv
// FUNCTION: LEMBALL 0x00406a90
GameStatus::GameStatus()
{
	int zero = 0;
	int* p;

	m_level = zero;
	m_levelState = zero;
	m_skill = zero;
	m_skillState = zero;
	p = (int*) &m_status0;
	p[0] = zero;
	p[1] = zero;
	p = m_maxLevels;
	p[0] = zero;
	p[1] = zero;
	p[2] = zero;
	p[3] = zero;
	p[4] = zero;
	p = m_lastLevels;
	p[0] = zero;
	p[1] = zero;
	p[2] = zero;
	p[3] = zero;
	p[4] = zero;
}

// 68K 0x10701440 JiggleLevelData__11CGameStatusFv
// FUNCTION: LEMBALL 0x00406ad0
unsigned int GameStatus::JiggleLevelData()
{
	unsigned int chunks[8];
	unsigned int mixed[8];
	unsigned int result = 0;
	int* levels = m_maxLevels;
	unsigned int* dest = chunks;

	while (1) {
		unsigned int* end = mixed;
		unsigned int value;
		unsigned int high;

		if (dest >= end) {
			break;
		}
		value = *levels;
		dest = dest + 2;
		high = value;
		levels = levels + 1;
		high = high & 0x38;
		value = value & 7;
		high = high >> 3;
		dest[-2] = high;
		dest[-1] = value;
	}

	{
		int offset = 0;
		do {
			unsigned int perm;
			unsigned int value;

			result = result << 3;
			perm = (unsigned int) g_anPasswordPermutation[offset / 4];
			offset = offset + 4;
			value = chunks[perm] ^ perm;
			result = result | value;
			mixed[(offset / 4) - 1] = value;
		} while (offset < 0x20);
	}
	return result;
}

// 68K 0x107014de UnJiggleLevelData__11CGameStatusFUl
// STUB: LEMBALL 0x00406b30
void GameStatus::UnJiggleLevelData(unsigned int p_value)
{
}

// 68K 0x1070159e CalcCheckSum__11CGameStatusFUl
// STUB: LEMBALL 0x00406ba0
unsigned int GameStatus::CalcCheckSum(unsigned int p_value)
{
	return 0;
}

// 68K 0x10701600 EncodePassword__11CGameStatusFv
// FUNCTION: LEMBALL 0x00406c00
char* GameStatus::EncodePassword()
{
	char buffer[12];
	unsigned int levelData = JiggleLevelData();
	unsigned int checksum = CalcCheckSum(levelData);
	VsLtoa((checksum << 24) | levelData, buffer, 10);
	strcpy(m_password, g_szPasswordZeroes);
	strcpy(m_password + 10 - strlen(buffer), buffer);
	return m_password;
}

// 68K 0x107016a0 DecodePassword__11CGameStatusFPc
// FUNCTION: LEMBALL 0x00406ca0
bool GameStatus::DecodePassword(char* p_password)
{
	strcpy(m_password, p_password);
	if (strcmp(m_password, g_szUnlockPassword) == 0) {
		*g_pDebugOutput << g_szPasswordCheatMessage;
		int i = 0;
		do {
			g_pGameStatus->SetMaxLevel(i, 0x40);
			i++;
		} while (i < 4);
		return 1;
	}

	unsigned int value = StringToDWord();
	if (value == 0) {
		return 0;
	}
	unsigned int checksum = CalcCheckSum(value);
	if (((value & 0x1f000000) >> 24) != checksum) {
		return 0;
	}
	value &= 0xffffff;
	UnJiggleLevelData(value);
	return 1;
}

// 68K 0x107017b8 StringToDWord__11CGameStatusFv
// STUB: LEMBALL 0x00406d80
int GameStatus::StringToDWord()
{
	return 0;
}

// 68K 0x1070181a GotoLastLevels__11CGameStatusFv
// FUNCTION: LEMBALL 0x00406dd0
void GameStatus::GotoLastLevels()
{
	int remaining = 4;
	int* last = m_lastLevels;
	do {
		last[0] = last[-5];
		last = last + 1;
		remaining = remaining - 1;
	} while (remaining != 0);
}

// 68K 0x10703296 IncLevel__11CGameStatusFv
// FUNCTION: LEMBALL 0x00408dc0
void GameStatus::IncLevel()
{
	int maxLevel;

	switch (m_skill) {
	case 0:
		maxLevel = 0x18;
		break;
	case 1:
		maxLevel = 0x19;
		break;
	case 2:
		maxLevel = 0x1c;
		break;
	case 3:
		maxLevel = 0x15;
		break;
	case 4:
		maxLevel = 0xb;
		break;
	}
	if (m_level < maxLevel) {
		m_level = m_level + 1;
	}
	if (m_skill != 4) {
		if (m_maxLevels[m_skill] < m_level) {
			m_maxLevels[m_skill] = m_level;
		}
	}
	m_lastLevels[m_skill] = m_level;
	NextLevelAvailable();
}

// 68K 0x10703332 DecLevel__11CGameStatusFv
// FUNCTION: LEMBALL 0x00408e40
bool GameStatus::DecLevel()
{
	if (m_level > 0) {
		m_level = m_level - 1;
	}
	m_lastLevels[m_skill] = m_level;
	return (unsigned int) m_level >= 1;
}

// 68K 0x10703374 IncSkill__11CGameStatusFUc
// FUNCTION: LEMBALL 0x00408e60
void GameStatus::IncSkill(unsigned int p_wrap)
{
	switch (m_skill) {
	case 0:
		m_skill = 1;
		break;
	case 1:
		m_skill = 2;
		break;
	case 2:
		m_skill = 3;
		break;
	case 3:
		if (p_wrap != 0) {
			m_skill = 0;
		}
		break;
	}
	m_level = 0;
	m_lastLevels[m_skill] = 0;
}

// 68K 0x107033e6 NoOfLevelsInSkill__11CGameStatusFi
// FUNCTION: LEMBALL 0x00408f30
int GameStatus::NoOfLevelsInSkill(int p_skill)
{
	int count;

	switch (p_skill) {
	case 0:
		count = 0x18;
		break;
	case 1:
		count = 0x19;
		break;
	case 2:
		count = 0x1c;
		break;
	case 3:
		count = 0x15;
		break;
	case 4:
		count = 0xb;
		break;
	}
	return count;
}

// 68K 0x1070344a NextLevelAvailable__11CGameStatusFv
// FUNCTION: LEMBALL 0x00408fa0
bool GameStatus::NextLevelAvailable()
{
	if (m_skill == 4) {
		if (m_level < 11) {
			return 1;
		}
		return 0;
	}
	return m_maxLevels[m_skill] != m_level;
}

// 68K 0x107034a8 LastLevelAvailable__11CGameStatusFv
// FUNCTION: LEMBALL 0x00408fd0
bool GameStatus::LastLevelAvailable()
{
	return (unsigned int) m_level >= 1;
}

// 68K 0x107034e6 SetMaxLevel__11CGameStatusFii
// FUNCTION: LEMBALL 0x00408fe0
void GameStatus::SetMaxLevel(int p_skill, int p_level)
{
	int maxLevel;

	switch (p_skill) {
	case 0:
		maxLevel = 0x18;
		break;
	case 1:
		maxLevel = 0x19;
		break;
	case 2:
		maxLevel = 0x1c;
		break;
	case 3:
		maxLevel = 0x15;
		break;
	case 4:
		maxLevel = 0xb;
		break;
	}
	if (m_skill == p_skill && p_level < m_level) {
		p_level = m_level;
	}
	if (maxLevel < p_level) {
		m_maxLevels[p_skill] = maxLevel;
		return;
	}
	m_maxLevels[p_skill] = p_level;
}

// 68K 0x1070356e Level__11CGameStatusFi
// FUNCTION: LEMBALL 0x00409070
void GameStatus::Level(int p_level)
{
	m_level = p_level;
	m_lastLevels[m_skill] = p_level;
}

// 68K 0x107035a4 Level__11CGameStatusFv
// FUNCTION: LEMBALL 0x00409080
int GameStatus::Level()
{
	return m_lastLevels[m_skill];
}

// GLOBAL: LEMBALL 0x0049cb68
GameStatus* g_pGameStatus = 0;

// GLOBAL: LEMBALL 0x0049cb70
int g_anPasswordPermutation[8] = {2, 0, 7, 4, 6, 1, 5, 3};

// GLOBAL: LEMBALL 0x0049cb90
char g_szPasswordZeroes[9] = "00000000";

// GLOBAL: LEMBALL 0x0049cb9c
char g_szUnlockPassword[11] = "9913454278";

// GLOBAL: LEMBALL 0x0049cba8
char g_szPasswordCheatMessage[31] = "Oh, oh, someones cheating !!!\n";
