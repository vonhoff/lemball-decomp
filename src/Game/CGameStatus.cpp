#include "CGameStatus.h"

#include "Visos/Streams/CVSOStream.h"
#include "Visos/Strings/VsString.h"
#include "Level/CLevelLoader.h"

#include <string.h>

#pragma intrinsic(strlen, strcpy, strcmp, memcpy)

extern char g_szPasswordZeroes[9];
extern char g_szUnlockPassword[11];
extern char g_szPasswordCheatMessage[31];

// FUNCTION: LEMBALL 0x00406a90
CGameStatus::CGameStatus()
{
	int zero = 0;
	int* p;

	m_level = zero;
	m_levelState = zero;
	m_skill = zero;
	m_skillState = zero;
	p = (int*) &m_bulletHitHandlingDisabled;
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

// FUNCTION: LEMBALL 0x00406ad0
unsigned int CGameStatus::JiggleLevelData()
{
	unsigned int chunks[PASSWORD_LEVEL_DATA_CHUNK_COUNT];
	unsigned int mixed[PASSWORD_LEVEL_DATA_CHUNK_COUNT];
	unsigned int result = 0;
	int* levels = m_maxLevels;
	unsigned int* dest = chunks;

	while (1) {
		unsigned int* end = chunks + PASSWORD_LEVEL_DATA_CHUNK_COUNT;
		unsigned int value;
		unsigned int high;

		if (dest >= end) {
			break;
		}
		value = *levels;
		dest = dest + 2;
		high = value;
		high = high & PASSWORD_LEVEL_DATA_HIGH_CHUNK_MASK;
		levels = levels + 1;
		value = value & PASSWORD_LEVEL_DATA_CHUNK_MASK;
		high = high >> PASSWORD_LEVEL_DATA_CHUNK_BITS;
		dest[-2] = high;
		dest[-1] = value;
	}

	for (int i = 0; i < PASSWORD_LEVEL_DATA_CHUNK_COUNT; i++) {
		unsigned int value;
		unsigned int perm;

		result = result << PASSWORD_LEVEL_DATA_CHUNK_BITS;
		perm = (unsigned int) g_anPasswordPermutation[i];
		value = chunks[perm] ^ perm;
		result = result | value;
		mixed[i] = value;
	}
	return result;
}

// FUNCTION: LEMBALL 0x00406b30
void CGameStatus::UnJiggleLevelData(unsigned int p_value)
{
	unsigned int mixed[PASSWORD_LEVEL_DATA_CHUNK_COUNT];
	unsigned int chunks[PASSWORD_LEVEL_DATA_CHUNK_COUNT];
	int i;

	unsigned int value;
	int remaining = PASSWORD_LEVEL_DATA_CHUNK_COUNT;
	unsigned int* dest = &mixed[7];
	do {
		value = p_value;
		*dest-- = value & PASSWORD_LEVEL_DATA_CHUNK_MASK;
		p_value >>= PASSWORD_LEVEL_DATA_CHUNK_BITS;
		remaining--;
	} while (remaining != 0);

	for (i = 0; i < PASSWORD_LEVEL_DATA_CHUNK_COUNT; i++) {
		unsigned int perm = g_anPasswordPermutation[i];
		chunks[perm] = mixed[i] ^ perm;
	}

	for (i = 0; i < PASSWORD_ENCODED_SKILL_COUNT; i++) {
		SetMaxLevel(i,
					(chunks[i * PASSWORD_LEVEL_CHUNKS_PER_SKILL] << PASSWORD_LEVEL_DATA_CHUNK_BITS) |
						chunks[i * PASSWORD_LEVEL_CHUNKS_PER_SKILL + 1]);
	}
}

// FUNCTION: LEMBALL 0x00406ba0
unsigned int CGameStatus::CalcCheckSum(unsigned int p_value)
{
	return ((p_value >> 16) + (p_value >> 8) + p_value) & PASSWORD_CHECKSUM_MASK;
}

// FUNCTION: LEMBALL 0x00406c00
char* CGameStatus::EncodePassword()
{
	char buffer[12];
	unsigned int levelData = JiggleLevelData();
	unsigned int checksum = CalcCheckSum(levelData);
	vsLtoa((checksum << PASSWORD_CHECKSUM_SHIFT) | levelData, buffer, 10);
	strcpy(m_password, g_szPasswordZeroes);
	strcpy(m_password + PASSWORD_CODE_DIGIT_COUNT - strlen(buffer), buffer);
	return m_password;
}

// FUNCTION: LEMBALL 0x00406ca0
bool CGameStatus::DecodePassword(char* p_password)
{
	strcpy(m_password, p_password);
	if (strcmp(m_password, g_szUnlockPassword) == 0) {
		*g_pDebugOutput << g_szPasswordCheatMessage;
		int i = 0;
		do {
			g_pGameStatus->SetMaxLevel(i, PASSWORD_UNLOCKED_LEVEL_LIMIT);
			i++;
		} while (i < PASSWORD_ENCODED_SKILL_COUNT);
		return true;
	}

	unsigned int value = StringToDWord();
	if (value == 0) {
		return false;
	}
	unsigned int checksum = CalcCheckSum(value);
	if (((value & PASSWORD_CHECKSUM_FIELD_MASK) >> PASSWORD_CHECKSUM_SHIFT) != checksum) {
		return false;
	}
	value &= PASSWORD_LEVEL_DATA_PAYLOAD_MASK;
	UnJiggleLevelData(value);
	return true;
}

// FUNCTION: LEMBALL 0x00406d80
int CGameStatus::StringToDWord()
{
	int result = 0;
	char* password = m_password;
	for (unsigned int digitIndex = 0; digitIndex < strlen(password); digitIndex++) {
		result = result * 10 + (m_password[digitIndex] - '0');
	}
	return result;
}

// FUNCTION: LEMBALL 0x00406dd0
void CGameStatus::GotoLastLevels()
{
	int remaining = 4;
	int* last = m_lastLevels;
	do {
		last[0] = last[-5];
		last = last + 1;
		remaining = remaining - 1;
	} while (remaining != 0);
}

// FUNCTION: LEMBALL 0x00408dc0
void CGameStatus::IncLevel()
{
	int skill = m_skill;
	int maxLevel;

	switch (skill) {
	case SKILL_FUN:
		maxLevel = SKILL_LEVEL_COUNT_FUN;
		break;
	case SKILL_TRICKY:
		maxLevel = SKILL_LEVEL_COUNT_TRICKY;
		break;
	case SKILL_TAXING:
		maxLevel = SKILL_LEVEL_COUNT_TAXING;
		break;
	case SKILL_MAYHEM:
		maxLevel = SKILL_LEVEL_COUNT_MAYHEM;
		break;
	case SKILL_NETWORK:
		maxLevel = SKILL_LEVEL_COUNT_NETWORK;
		break;
	}
	if (m_level < maxLevel) {
		m_level = m_level + 1;
	}
	if (skill != SKILL_NETWORK) {
		if (m_maxLevels[skill] < m_level) {
			m_maxLevels[skill] = m_level;
		}
	}
	m_lastLevels[m_skill] = m_level;
	NextLevelAvailable();
}

// FUNCTION: LEMBALL 0x00408e40
bool CGameStatus::DecLevel()
{
	if (m_level > 0) {
		m_level = m_level - 1;
	}
	m_lastLevels[m_skill] = m_level;
	return (unsigned int) m_level >= 1;
}

// FUNCTION: LEMBALL 0x00408e60
void CGameStatus::IncSkill(unsigned int p_wrap)
{
	switch (m_skill) {
	case SKILL_FUN:
		m_skill = SKILL_TRICKY;
		break;
	case SKILL_TRICKY:
		m_skill = SKILL_TAXING;
		break;
	case SKILL_TAXING:
		m_skill = SKILL_MAYHEM;
		break;
	case SKILL_MAYHEM:
		if (p_wrap != 0) {
			m_skill = SKILL_FUN;
		}
		break;
	}
	m_level = 0;
	m_lastLevels[m_skill] = 0;
}

// FUNCTION: LEMBALL 0x00408ec0
void CGameStatus::DecSkill(unsigned int p_wrap)
{
	switch (m_skill) {
	case SKILL_FUN:
		if (p_wrap != 0) {
			m_skill = SKILL_MAYHEM;
			m_level = 0;
			return;
		}
		break;
	case SKILL_TRICKY:
		m_skill = SKILL_FUN;
		m_level = 0;
		return;
	case SKILL_TAXING:
		m_skill = SKILL_TRICKY;
		m_level = 0;
		return;
	case SKILL_MAYHEM:
		m_skill = SKILL_TAXING;
		break;
	}
	m_level = 0;
}

// FUNCTION: LEMBALL 0x00408f30
int CGameStatus::NoOfLevelsInSkill(int p_skill)
{
	int count;

	switch (p_skill) {
	case SKILL_FUN:
		count = SKILL_LEVEL_COUNT_FUN;
		break;
	case SKILL_TRICKY:
		count = SKILL_LEVEL_COUNT_TRICKY;
		break;
	case SKILL_TAXING:
		count = SKILL_LEVEL_COUNT_TAXING;
		break;
	case SKILL_MAYHEM:
		count = SKILL_LEVEL_COUNT_MAYHEM;
		break;
	case SKILL_NETWORK:
		count = SKILL_LEVEL_COUNT_NETWORK;
		break;
	}
	return count;
}

// FUNCTION: LEMBALL 0x00408fa0
bool CGameStatus::NextLevelAvailable()
{
	if (m_skill == SKILL_NETWORK) {
		if (m_level < SKILL_LEVEL_COUNT_NETWORK) {
			return true;
		}
		return false;
	}
	return m_maxLevels[m_skill] != m_level;
}

// FUNCTION: LEMBALL 0x00408fd0
bool CGameStatus::LastLevelAvailable()
{
	return (unsigned int) m_level >= 1;
}

// FUNCTION: LEMBALL 0x00408fe0
void CGameStatus::SetMaxLevel(int p_skill, int p_level)
{
	int maxLevel;

	switch (p_skill) {
	case SKILL_FUN:
		maxLevel = SKILL_LEVEL_COUNT_FUN;
		break;
	case SKILL_TRICKY:
		maxLevel = SKILL_LEVEL_COUNT_TRICKY;
		break;
	case SKILL_TAXING:
		maxLevel = SKILL_LEVEL_COUNT_TAXING;
		break;
	case SKILL_MAYHEM:
		maxLevel = SKILL_LEVEL_COUNT_MAYHEM;
		break;
	case SKILL_NETWORK:
		maxLevel = SKILL_LEVEL_COUNT_NETWORK;
		break;
	}
	if (m_skill == p_skill) {
		if (m_level > p_level) {
			p_level = m_level;
		}
	}
	if (maxLevel >= p_level) {
		m_maxLevels[p_skill] = p_level;
	}
	else {
		m_maxLevels[p_skill] = maxLevel;
	}
}

// FUNCTION: LEMBALL 0x00409070
void CGameStatus::Level(int p_level)
{
	m_level = p_level;
	m_lastLevels[m_skill] = p_level;
}

// FUNCTION: LEMBALL 0x00409080
int CGameStatus::Level()
{
	return m_lastLevels[m_skill];
}

// GLOBAL: LEMBALL 0x0049cb68
CGameStatus* g_pGameStatus = NULL;

// GLOBAL: LEMBALL 0x0049cb70
int g_anPasswordPermutation[PASSWORD_LEVEL_DATA_CHUNK_COUNT] = {2, 0, 7, 4, 6, 1, 5, 3};

// GLOBAL: LEMBALL 0x0049cb90
char g_szPasswordZeroes[9] = "00000000";

// GLOBAL: LEMBALL 0x0049cb9c
char g_szUnlockPassword[11] = "9913454278";

// GLOBAL: LEMBALL 0x0049cba8
char g_szPasswordCheatMessage[31] = "Oh, oh, someones cheating !!!\n";
