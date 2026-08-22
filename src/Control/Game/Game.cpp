#include "Game.h"

// 68K 0x1070185c __ct__5CGameFPc
// ASSERT: _VSRELassert("IsValidResourceFile", "GAME.CPP", 366)
// STUB: LEMBALL 0x00406df0
Game::Game(char* p_arg0)
{
	// STRING: LEMBALL 0x0049cbc8 "Lemmings Paintball"
	// STRING: LEMBALL 0x0049cc50 "vsmem.dll"
	// STRING: LEMBALL 0x0049cca4 "Processing"
	// STRING: LEMBALL 0x0049ccb0 "Refreshing"
	// STRING: LEMBALL 0x0049ccbc "pbaimog.vsr"
	// STRING: LEMBALL 0x0049ccc8 "GAME.CPP"
	// STRING: LEMBALL 0x0049ccd4 "IsValidResourceFile"
	// STRING: LEMBALL 0x0049cce8 "Lemmings Paintball"
	// STRING: LEMBALL 0x0049ccfc "lemball"
	// STRING: LEMBALL 0x0049cd05 "evel\\"
}

// 68K 0x10701c2e __dt__5CGameFv
// STUB: LEMBALL 0x004071d0
Game::~Game()
{
	// STRING: LEMBALL 0x0049cd18 "Lemmings Paintball"
}

// 68K 0x10701daa IsValidResource__5CGameFv
// STUB: LEMBALL 0x00407300
bool Game::IsValidResource()
{
	// STRING: LEMBALL 0x0049cd54 "John Ketley is a Weatherman, and so is Michael Fish"
	// STRING: LEMBALL 0x0049cd55 "John Ketley is a Weatherman, and so is Michael Fish"
	// STRING: LEMBALL 0x0049cd3c "01234567890123456789"
	// STRING: LEMBALL 0x0049cd3d "01234567890123456789"
	// STRING: LEMBALL 0x0049cd3e "01234567890123456789"
	// STRING: LEMBALL 0x0049cd88 "Master v1.3"
	// STRING: LEMBALL 0x0049cd8a "Master v1.3"
	// STRING: LEMBALL 0x0049cd3f "01234567890123456789"
	// STRING: LEMBALL 0x0049cd89 "Master v1.3"
	// STRING: LEMBALL 0x0049cd8b "Master v1.3"
	return 0;
}

// 68K 0x10701e66 LoadFrontendResources__5CGameFi
// STUB: LEMBALL 0x004073b0
void Game::LoadFrontendResources(int p_mode)
{
}

// 68K 0x10701ecc UnLoadFrontendResources__5CGameFv
// STUB: LEMBALL 0x004073f0
void Game::UnLoadFrontendResources()
{
}

// 68K 0x10701f1c NextProcess__5CGameF14eFlowProcesses
// STUB: LEMBALL 0x00407420
void Game::NextProcess(eFlowProcesses p_flow)
{
}

// 68K 0x10702374 Process__5CGameFv
// STUB: LEMBALL 0x004077e0
void Game::Process()
{
}

// 68K 0x10702508 RefreshViews__5CGameFv
// STUB: LEMBALL 0x004078f0
void Game::RefreshViews()
{
}

// 68K 0x107025a0 Run__5CGameFv
// STUB: LEMBALL 0x00407950
void Game::Run()
{
}

// GLOBAL: LEMBALL 0x004a1bcc
int* g_pSentinel = 0;

// GLOBAL: LEMBALL 0x0049ce04
unsigned int g_dwGameTick = 0;
