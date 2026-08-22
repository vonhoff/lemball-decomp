#include "GameStatus.h"

// 68K 0x107013de __ct__11CGameStatusFv
// STUB: LEMBALL 0x00406a90
GameStatus::GameStatus()
{
}

// 68K 0x10701440 JiggleLevelData__11CGameStatusFv
// STUB: LEMBALL 0x00406ad0
unsigned int GameStatus::JiggleLevelData()
{
	return 0;
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
// STUB: LEMBALL 0x00406c00
char* GameStatus::EncodePassword()
{
	// STRING: LEMBALL 0x0049cb90 "00000000"
	// STRING: LEMBALL 0x0049cb91 "00000000"
	return 0;
}

// 68K 0x107016a0 DecodePassword__11CGameStatusFPc
// STUB: LEMBALL 0x00406ca0
bool GameStatus::DecodePassword(char* p_password)
{
	// STRING: LEMBALL 0x0049cb9c "9913454278"
	// STRING: LEMBALL 0x0049cb9e "9913454278"
	// STRING: LEMBALL 0x0049cb9d "9913454278"
	// STRING: LEMBALL 0x0049cb9f "9913454278"
	// STRING: LEMBALL 0x0049cba8 "Oh, oh, someones cheating !!!\n"
	return 0;
}

// 68K 0x107017b8 StringToDWord__11CGameStatusFv
// STUB: LEMBALL 0x00406d80
int GameStatus::StringToDWord()
{
	return 0;
}

// 68K 0x1070181a GotoLastLevels__11CGameStatusFv
// STUB: LEMBALL 0x00406dd0
void GameStatus::GotoLastLevels()
{
}

// 68K 0x10703296 IncLevel__11CGameStatusFv
// STUB: LEMBALL 0x00408dc0
void GameStatus::IncLevel()
{
}

// 68K 0x10703332 DecLevel__11CGameStatusFv
// STUB: LEMBALL 0x00408e40
bool GameStatus::DecLevel()
{
	return 0;
}

// 68K 0x10703374 IncSkill__11CGameStatusFUc
// STUB: LEMBALL 0x00408e60
void GameStatus::IncSkill(unsigned char p_wrap)
{
}

// 68K 0x107033e6 NoOfLevelsInSkill__11CGameStatusFi
// STUB: LEMBALL 0x00408f30
int GameStatus::NoOfLevelsInSkill(int p_skill)
{
	return 0;
}

// 68K 0x1070344a NextLevelAvailable__11CGameStatusFv
// STUB: LEMBALL 0x00408fa0
bool GameStatus::NextLevelAvailable()
{
	return 0;
}

// 68K 0x107034a8 LastLevelAvailable__11CGameStatusFv
// STUB: LEMBALL 0x00408fd0
bool GameStatus::LastLevelAvailable()
{
	return 0;
}

// 68K 0x107034e6 SetMaxLevel__11CGameStatusFii
// STUB: LEMBALL 0x00408fe0
void GameStatus::SetMaxLevel(int p_skill, int p_level)
{
}

// 68K 0x1070356e Level__11CGameStatusFi
// STUB: LEMBALL 0x00409070
void GameStatus::Level(int p_level)
{
}

// 68K 0x107035a4 Level__11CGameStatusFv
// STUB: LEMBALL 0x00409080
int GameStatus::Level()
{
	return 0;
}

// GLOBAL: LEMBALL 0x0049cb68
GameStatus* g_pGameStatus = 0;

// GLOBAL: LEMBALL 0x0049cb70
int g_anPasswordPermutation[8] = {2, 0, 7, 4, 6, 1, 5, 3};
