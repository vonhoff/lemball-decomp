#include "Game.h"

#include "../Level/LevelLoader.h"
#include "../Support/PreInit.h"
#include "../../Frontend/Resources/FrontendResourceLoader.h"
#include "../../Frontend/Base/BaseFrontendDrawer.h"
#include "../../Frontend/Base/BaseFrontendProcess.h"
#include "../../Frontend/Drawers/IntroAnimDrawer.h"
#include "../../Frontend/Drawers/MainOptions1Drawer.h"
#include "../../Frontend/Processes/NetworkOptionsProc.h"
#include "../../Frontend/Processes/PasswordProc.h"
#include "../../Frontend/Processes/Preview.h"
#include "../../Frontend/Processes/SuccFail.h"
#include "../../Platform/Windows/Entry.h"
#include "../../Views/Display/Main2DDisplay.h"
#include "../../Views/Sound/SoundView.h"
#include "../../Visos/Animation/BaseStat.h"
#include "../../Visos/Animation/IntroAnim.h"
#include "../../Visos/Animation/StatManager.h"
#include "../../Visos/Foundation/MainOptions1.h"
#include "../../Visos/Foundation/MainOptions2.h"
#include "../../Visos/Foundation/Process.h"
#include "../../Visos/Foundation/VsDebug.h"
#include "../../Visos/Foundation/VsSound.h"
#include "../../Visos/Graphics/Drawer.h"
#include "../../Visos/Resources/MogRes.h"
#include "../../Visos/Resources/ResString.h"
#include "../../Visos/Sound/SoundManager.h"
#include "../../Visos/Target/TargetPlatformServices.h"
#include "Demo.h"
#include "GameMain.h"
#include "GameStatus.h"

#include <new.h>
#include <string.h>

#pragma intrinsic(strcpy, strcat, strcmp)

// GLOBAL: LEMBALL 0x0049cbc8
char g_szLemmingsPaintball[20] = "Lemmings Paintball";

// GLOBAL: LEMBALL 0x0049cc50
char g_szVsMemDll[12] = "vsmem.dll";

// GLOBAL: LEMBALL 0x0049cca4
char g_szProcessing[12] = "Processing";

// GLOBAL: LEMBALL 0x0049ccb0
char g_szRefreshing[12] = "Refreshing";

// GLOBAL: LEMBALL 0x0049ccbc
char g_szPbaimogVsr[12] = "pbaimog.vsr";

// GLOBAL: LEMBALL 0x0049ccc8
char g_szGameCpp[12] = "GAME.CPP";

// GLOBAL: LEMBALL 0x0049ccd4
char g_szIsValidResourceFile[20] = "IsValidResourceFile";

// GLOBAL: LEMBALL 0x0049cce8
char g_szLemmingsPaintballTitle[20] = "Lemmings Paintball";

// GLOBAL: LEMBALL 0x0049ccfc
char g_szMusicCdPath[8] = "lemball";

// GLOBAL: LEMBALL 0x0049cd04
char g_szDefaultRuntimeDir[8] = "level\\";

// GLOBAL: LEMBALL 0x0049cd0c
char g_szDefaultRuntimeFile[12] = "testlvl.lvl";

// GLOBAL: LEMBALL 0x0049cd3c
char g_szResourceDecodeBuffer[24] = "01234567890123456789";

// GLOBAL: LEMBALL 0x0049cd54
char g_szWeatherManKey[52] = "John Ketley is a Weatherman, and so is Michael Fish";

// GLOBAL: LEMBALL 0x0049cd88
char g_szMasterVersion[12] = "Master v1.3";

extern StatManager* g_pStatManager;

// 68K 0x1070185c __ct__5CGameFPc
// ASSERT: _VSRELassert("IsValidResourceFile", "GAME.CPP", 366)
// STUB: LEMBALL 0x00406df0
Game::Game(char* p_arg0)
{
	void* storage;
	VsRect useRect;

	m_frontendResources = 0;
	g_pGameStatus = 0;
	m_mainDisplay = 0;
	g_pMogRes = 0;
	g_pSoundView = 0;
	m_quit = 1;
	m_process = 0;

	g_pTargetPlatformServices->WriteRegistryFlag(g_szLemmingsPaintball, 1);
	g_pTargetPlatformServices->GetCDDir(g_szVsMemDll);

	storage = operator new(0x50);
	if (storage == 0) {
		g_pGameStatus = 0;
	}
	else {
		g_pGameStatus = new (storage) GameStatus();
	}

	storage = operator new(0x28);
	if (storage == 0) {
		m_processingStat = 0;
	}
	else {
		m_processingStat = new (storage) BaseStat(g_szProcessing);
	}

	storage = operator new(0x28);
	if (storage == 0) {
		m_refreshingStat = 0;
	}
	else {
		m_refreshingStat = new (storage) BaseStat(g_szRefreshing);
	}

	m_flowTicks = 0;
	g_pStatManager->Register(m_processingStat);
	g_pStatManager->Register(m_refreshingStat);

	storage = operator new(0x28);
	if (storage == 0) {
		g_pMogRes = 0;
	}
	else {
		g_pMogRes = new (storage) MogRes(g_szPbaimogVsr, 0x177000);
	}

	if (IsValidResource() == 0) {
		VsRelAssert(g_szIsValidResourceFile, g_szGameCpp, 0x16e);
	}

	storage = operator new(0xe4);
	if (storage == 0) {
		m_mainDisplay = 0;
	}
	else {
		m_mainDisplay = new (storage) Main2DDisplay(this);
	}

	useRect = m_mainDisplay->GetUseRect(-1, -1);
	m_mainDisplay->Create(useRect, 0, g_szLemmingsPaintballTitle);

	InitSound(g_nMusicVolume, g_nEffectsVolume, 0x32, m_mainDisplay, 0);
	if (g_nMusicVolume != 0) {
		g_pSoundManager->UseMusicCd(1);
		g_pSoundManager->SetMusicCdPath(g_szMusicCdPath);
	}

	storage = operator new(0x2cc);
	if (storage == 0) {
		g_pSoundView = 0;
	}
	else {
		g_pSoundView = new (storage) SoundView();
	}

	m_process = 0;
	m_currentFlow = 1;
	NextProcess(1);

	strcpy(m_runtimeName, g_szDefaultRuntimeDir);
	if (p_arg0 == 0) {
		strcat(m_runtimeName, g_szDefaultRuntimeFile);
	}
	else {
		strcat(m_runtimeName, p_arg0);
	}

	m_quit = 0;
}

// 68K 0x10701c2e __dt__5CGameFv
// STUB: LEMBALL 0x004071d0
Game::~Game()
{
}

// 68K 0x10701daa IsValidResource__5CGameFv
// STUB: LEMBALL 0x00407300
bool Game::IsValidResource()
{
	ResString* resource;
	unsigned char* data;
	char* key;
	int i;
	unsigned char next;

	key = g_szWeatherManKey;
	resource = ResString::Load(0x100);
	if (resource == 0) {
		return 0;
	}
	if (resource->m_loaded == 0) {
		resource->LoadData();
	}
	else {
		resource->m_age = 0;
	}
	resource->m_directUseCount = resource->m_directUseCount + 1;
	data = resource->m_data;
	next = *data;
	i = 0;
	while (next != 0) {
		next = *key;
		key = key + 1;
		g_szResourceDecodeBuffer[i] = (char) ((data[i] - 1) ^ next);
		next = data[i + 1];
		i = i + 1;
	}
	g_szResourceDecodeBuffer[i] = 0;
	resource->m_directUseCount = resource->m_directUseCount - 1;
	resource->UnLoad();
	return strcmp(g_szResourceDecodeBuffer, g_szMasterVersion) == 0;
}

// 68K 0x10701e66 LoadFrontendResources__5CGameFi
// STUB: LEMBALL 0x004073b0
void Game::LoadFrontendResources(int p_mode)
{
	void* storage;

	if (m_frontendResources == 0) {
		storage = operator new(0x58);
		if (storage == 0) {
			m_frontendResources = 0;
		}
		else {
			m_frontendResources = new (storage) FrontendResourceLoader(m_mainDisplay, p_mode);
		}
	}
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
	void* storage;

	if (m_mainDisplay->m_drawer != 0) {
		((Drawer*) m_mainDisplay->m_drawer)->DestroyDrawer();
	}
	if (m_process != 0) {
		delete m_process;
		m_process = 0;
	}

	if (p_flow == 1 && g_nAnimationsDisabled == 1) {
		p_flow = 2;
	}
	if (p_flow == 0x12 && g_nAnimationsDisabled == 1) {
		p_flow = 4;
	}

	switch (p_flow) {
	case 1:
		UnLoadFrontendResources();
		m_currentFlow = p_flow;
		m_mainDisplay->KillDrawer(p_flow);
		storage = operator new(0x28);
		if (storage == 0) {
			m_process = 0;
		}
		else {
			m_process = new (storage) IntroAnim(this);
		}
		m_flowTicks = 0;
		break;
	case 2:
		m_currentFlow = p_flow;
		m_mainDisplay->KillDrawer(p_flow);
		LoadFrontendResources(3);
		if (g_nDemoMode != 0) {
			g_nDemoMode = g_nStoredLevelDemoModeEnabled;
		}
		storage = operator new(0x28);
		if (storage == 0) {
			m_process = 0;
		}
		else {
			m_process = new (storage) MainOptions1(this);
		}
		break;
	case 3:
		m_currentFlow = p_flow;
		m_mainDisplay->KillDrawer(p_flow);
		storage = operator new(0x28);
		if (storage == 0) {
			m_process = 0;
		}
		else {
			m_process = new (storage) MainOptions2(this);
		}
		break;
	case 4:
		m_currentFlow = p_flow;
		m_mainDisplay->KillDrawer(p_flow);
		storage = operator new(0x28);
		if (storage == 0) {
			m_process = 0;
		}
		else {
			m_process = new (storage) Preview(this);
		}
		break;
	case 0xc:
		m_currentFlow = p_flow;
		m_mainDisplay->KillDrawer(p_flow);
		storage = operator new(0x38);
		if (storage == 0) {
			m_process = 0;
		}
		else {
			m_process = new (storage) NetworkOptionsProc(this);
		}
		break;
	case 0xe:
		m_currentFlow = p_flow;
		m_mainDisplay->KillDrawer(p_flow);
		LoadFrontendResources(2);
		storage = operator new(0x2c);
		if (storage == 0) {
			m_process = 0;
		}
		else {
			m_process = new (storage) SuccFail(this, 1);
		}
		m_flowTicks = 0;
		break;
	case 0xf:
		m_currentFlow = p_flow;
		m_mainDisplay->KillDrawer(p_flow);
		LoadFrontendResources(2);
		storage = operator new(0x2c);
		if (storage == 0) {
			m_process = 0;
		}
		else {
			m_process = new (storage) SuccFail(this, 0);
		}
		m_flowTicks = 0;
		break;
	case 0x10:
		m_currentFlow = p_flow;
		m_mainDisplay->KillDrawer(p_flow);
		storage = operator new(0x28);
		if (storage == 0) {
			m_process = 0;
		}
		else {
			m_process = new (storage) PasswordProc(this);
		}
		break;
	case 0x12:
		UnLoadFrontendResources();
		m_currentFlow = p_flow;
		m_mainDisplay->KillDrawer(p_flow);
		storage = operator new(0x28);
		if (storage == 0) {
			m_process = 0;
		}
		else {
			m_process = new (storage) IntroAnim(this);
		}
		m_flowTicks = 0;
		break;
	default:
		m_process = 0;
		break;
	}

	m_mainDisplay->StatusUpdate(m_currentFlow);
}

// 68K 0x10702374 Process__5CGameFv
// STUB: LEMBALL 0x004077e0
void Game::Process()
{
	unsigned char quitState;

	m_mainDisplay->Process();
	if (m_process != 0) {
		((BaseFrontendProcess*) m_process)->Process();
		if (m_process->m_processState == 1) {
			NextProcess(m_process->m_returnState);
		}
		else if (m_process->m_processState == 2) {
			m_quit = 1;
		}
	}

	quitState = m_mainDisplay->QuitYet();
	if (quitState == 1) {
		NextProcess(m_mainDisplay->GetReturnState());
	}
	else if (quitState == 2) {
		m_quit = 1;
	}

	if (m_quit != 0) {
		if (m_mainDisplay->m_drawer != 0) {
			((Drawer*) m_mainDisplay->m_drawer)->DestroyDrawer();
		}
		if (m_process != 0) {
			delete m_process;
			m_process = 0;
		}
		m_mainDisplay->KillDrawer(0);
	}
}

// 68K 0x10702508 RefreshViews__5CGameFv
// STUB: LEMBALL 0x004078f0
void Game::RefreshViews()
{
	m_mainDisplay->RefreshView();
}

// 68K 0x107025a0 Run__5CGameFv
// STUB: LEMBALL 0x00407950
void Game::Run()
{
	Demo* demo;
	int pumpResult;

	demo = g_pDemo;
	if (g_pDemo != 0 && g_nDemoMode == 0) {
		g_pDemo->m_currentResourceId = 0x14;
		demo->m_firstResourceId = 0x14;
		demo->m_resourceCount = 8;
	}

	while (m_quit == 0) {
		if (m_currentFlow == 5 || m_currentFlow == 0x13) {
			m_flowTicks = m_flowTicks + 1;
		}
		if (g_pDemo != 0) {
			g_pDemo->Process();
		}
		pumpResult = TargetPumpEvents();
		if (pumpResult == 0) {
			Process();
			if (m_quit != 0) {
				return;
			}
			RefreshViews();
		}
		else if (pumpResult == 1) {
			m_quit = 1;
		}
	}
}

// GLOBAL: LEMBALL 0x004a1bcc
int* g_pSentinel = 0;

// GLOBAL: LEMBALL 0x0049ce04
unsigned int g_dwGameTick = 0;
