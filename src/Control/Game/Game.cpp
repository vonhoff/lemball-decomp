#include "Game.h"

#include "../../AI/Navigation/Ai.h"
#include "../../Frontend/Base/BaseFrontendDrawer.h"
#include "../../Frontend/Base/BaseFrontendProcess.h"
#include "../../Frontend/Drawers/IntroAnimDrawer.h"
#include "../../Frontend/Drawers/MainOptions1Drawer.h"
#include "../../Frontend/Processes/NetworkOptionsProc.h"
#include "../../Frontend/Processes/PasswordProc.h"
#include "../../Frontend/Processes/Preview.h"
#include "../../Frontend/Processes/SuccFail.h"
#include "../../Frontend/Processes/TargetAbout.h"
#include "../../Frontend/Resources/FrontendResourceLoader.h"
#include "../../Network/Game/NetworkManager.h"
#include "../../Platform/Windows/Entry.h"
#include "../../Views/Display/Main2DDisplay.h"
#include "../../Views/Sound/SoundView.h"
#include "../../Visos/Animation/IntroAnim.h"
#include "../../Visos/Animation/StatManager.h"
#include "../../Visos/Animation/TimeStat.h"
#include "../../Visos/Foundation/BaseProcess.h"
#include "../../Visos/Foundation/MainOptions1.h"
#include "../../Visos/Foundation/MainOptions2.h"
#include "../../Visos/Foundation/VsDebug.h"
#include "../../Visos/Foundation/VsDebugStreambuf.h"
#include "../../Visos/Foundation/VsOStream.h"
#include "../../Visos/Foundation/VsSound.h"
#include "../../Visos/Foundation/VsTime.h"
#include "../../Visos/Graphics/Drawer.h"
#include "../../Visos/Network/BaseNetwork.h"
#include "../../Visos/Resources/Manifest.h"
#include "../../Visos/Resources/MogRes.h"
#include "../../Visos/Resources/MogloadArena.h"
#include "../../Visos/Resources/ResString.h"
#include "../../Visos/Sound/SoundManager.h"
#include "../../Visos/Target/TargetPlatformServices.h"
#include "../Level/LevelLoader.h"
#include "../Support/PreInit.h"
#include "Demo.h"
#include "GameMain.h"
#include "GameStatus.h"

#include <new.h>
#include <string.h>

#pragma intrinsic(strcpy, strcat, strcmp)

extern "C" unsigned long __stdcall timeGetTime(void);
extern "C" __declspec(dllimport) int __stdcall MessageBoxA(void* hWnd,
														   const char* lpText,
														   const char* lpCaption,
														   unsigned int uType);

// GLOBAL: LEMBALL 0x0049cbc8
char g_szLemmingsPaintball[20] = "Lemmings Paintball";

// GLOBAL: LEMBALL 0x0049cbdc
char g_szPaintballNotInstalled[24] = "Paintball Not Installed";

// GLOBAL: LEMBALL 0x0049cbf4
char g_szInstallPrompt[92] =
	"To play Lemmings Paintball, you must first install it.  Run the SETUP.EXE program on the CD";

// GLOBAL: LEMBALL 0x0049cc50
char g_szVsMemDll[12] = "vsmem.dll";

// GLOBAL: LEMBALL 0x0049cc5c
char g_szUnableToFindCd[20] = "Unable to find CD";

// GLOBAL: LEMBALL 0x0049cc70
char g_szInsertCdPrompt[52] = "Please insert the Paintball CD in a local CD Drive";

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

// GLOBAL: LEMBALL 0x0049cd18
char g_szLemmingsPaintballRegistry[20] = "Lemmings Paintball";

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
// FUNCTION: LEMBALL 0x00406df0
Game::Game(char* p_arg0)
{
	void* storage;
	char titleBuf[80];

	m_frontendResources = 0;
	g_pGameStatus = 0;
	m_mainDisplay = 0;
	g_pMogRes = 0;
	g_pSoundView = 0;
	m_quit = 1;
	m_process = 0;

	if (g_pTargetPlatformServices->WriteRegistryFlag(g_szLemmingsPaintball, 1) == 0) {
		if (0) {
			MessageBoxA(0, g_szInstallPrompt, g_szPaintballNotInstalled, 0);
			return;
		}
	}

	int cdResult = 0;
	do {
		if (g_pTargetPlatformServices->GetCDDir(g_szVsMemDll) != 0) {
			break;
		}
		if (0) {
			cdResult = MessageBoxA(0, g_szInsertCdPrompt, g_szUnableToFindCd, 1);
			if (cdResult == 1) {
				continue;
			}
		}
	} while (cdResult != 2);
	if (cdResult == 2) {
		return;
	}

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
		m_processingStat = new (storage) TimeStat(g_szProcessing);
	}

	storage = operator new(0x28);
	if (storage == 0) {
		m_refreshingStat = 0;
	}
	else {
		m_refreshingStat = new (storage) TimeStat(g_szRefreshing);
	}

	m_flowTicks = 0;
	g_pStatManager->Register(m_processingStat);
	g_pStatManager->Register(m_refreshingStat);

	storage = MogloadArena::operator new(0x28);
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

	{
		VsDebugStreambuf streambuf(titleBuf, 80, 0);
		VsOStream stream(&streambuf);
		stream << g_szLemmingsPaintballTitle;

		m_mainDisplay->Create(m_mainDisplay->GetUseRect(-1, -1), 0, titleBuf);
	}

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
// FUNCTION: LEMBALL 0x004071d0
Game::~Game()
{
	FrontendResourceLoader* resources;
	SoundView* soundView;
	MogRes* mogRes;
	unsigned long started;
	unsigned long now;

	resources = (FrontendResourceLoader*) m_frontendResources;
	if (resources != 0) {
		resources->~FrontendResourceLoader();
		operator delete(resources);
	}
	if (g_pNetworkManager != 0) {
		g_pNetworkManager->Stop();
	}
	if (g_pBaseNetwork != 0) {
		started = CurrentMilliTimer();
		do {
			now = CurrentMilliTimer();
			if (now - started >= 2000) {
				break;
			}
		} while (g_pBaseNetwork->m_queueTransitionPending != 0);
	}
	if (g_pNetworkManager != 0) {
		delete g_pNetworkManager;
		g_pNetworkManager = 0;
	}
	if (m_mainDisplay != 0) {
		m_mainDisplay->Destroy();
		if (m_mainDisplay != 0) {
			delete (BaseQueueHandler*) m_mainDisplay;
		}
	}
	if (m_process != 0) {
		delete m_process;
	}
	if (g_pSoundView != 0) {
		soundView = g_pSoundView;
		soundView->~SoundView();
		operator delete(soundView);
		EndSound();
	}
	if (g_pMogRes != 0) {
		mogRes = g_pMogRes;
		mogRes->~MogRes();
		MogloadArena::operator delete(mogRes);
		g_pMogRes = 0;
	}
	if (g_pGameStatus != 0) {
		operator delete(g_pGameStatus);
		g_pGameStatus = 0;
	}
	g_pTargetPlatformServices->WriteRegistryFlag(g_szLemmingsPaintballRegistry, 0);
}

// 68K 0x10701daa IsValidResource__5CGameFv
// FUNCTION: LEMBALL 0x00407300
bool Game::IsValidResource()
{
	const char* key;
	ResString* resource;
	unsigned char* data;
	int i;
	char c;

	key = g_szWeatherManKey;
	resource = ResString::Load(RES_REGISTRATION_FINGERPRINT);
	if (resource == 0) {
		return 0;
	}
	if (resource->m_loaded != 0) {
		resource->m_age = 0;
	}
	else {
		resource->LoadData();
	}
	i = 0;
	resource->m_directUseCount++;
	data = resource->m_data;
	if (*data != 0) {
		do {
			c = (char) (data[i++] - 1);
			c = c ^ *key++;
			g_szResourceDecodeBuffer[i - 1] = c;
		} while (data[i] != 0);
	}
	g_szResourceDecodeBuffer[i] = 0;
	resource->m_directUseCount--;
	resource->UnLoad();
	return strcmp(g_szResourceDecodeBuffer, g_szMasterVersion) == 0;
}

// 68K 0x10701e66 LoadFrontendResources__5CGameFi
// FUNCTION: LEMBALL 0x004073b0
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
// FUNCTION: LEMBALL 0x004073f0
void Game::UnLoadFrontendResources()
{
	if (m_frontendResources != 0) {
		delete (FrontendResourceLoader*) m_frontendResources;
		m_frontendResources = 0;
	}
}

// 68K 0x10701f1c NextProcess__5CGameF14eFlowProcesses
// FUNCTION: LEMBALL 0x00407420
void Game::NextProcess(eFlowProcesses p_flow)
{
	void* storage;
	Ai* ai;

	if (m_mainDisplay->m_drawer != 0) {
		m_mainDisplay->m_drawer->ShutDown();
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
		goto done;
	case 2:
		m_currentFlow = p_flow;
		m_mainDisplay->KillDrawer(p_flow);
		LoadFrontendResources(3);
		if (g_nDemoMode != 0) {
			g_nDemoMode = g_nStoredLevelDemoModeEnabled;
		}
		storage = operator new(0x28);
		if (storage != 0) {
			m_process = new (storage) MainOptions1(this);
			goto done;
		}
		break;
	case 3:
		m_currentFlow = p_flow;
		m_mainDisplay->KillDrawer(p_flow);
		storage = operator new(0x28);
		if (storage != 0) {
			m_process = new (storage) MainOptions2(this);
			goto done;
		}
		break;
	case 4:
		m_currentFlow = p_flow;
		m_mainDisplay->KillDrawer(p_flow);
		storage = operator new(0x28);
		if (storage != 0) {
			m_process = new (storage) Preview(this);
			goto done;
		}
		break;
	case 0x13:
		if (g_nDemoMode == 0) {
			g_pDemo->m_filePath = 0;
		}
		else {
			g_pDemo->m_filePath = g_szDemoFilePath;
		}
		g_nDemoMode = 1;
	case 5:
		UnLoadFrontendResources();
		m_currentFlow = p_flow;
		m_mainDisplay->KillDrawer(p_flow);
		storage = operator new(0x1f0);
		ai = (Ai*) 0;
		if (storage != 0) {
			ai = new (storage) Ai(this);
		}
		if (ai != 0) {
			m_process = ai;
			goto done;
		}
		break;
	case 10:
		m_currentFlow = p_flow;
		m_mainDisplay->KillDrawer(p_flow);
		storage = operator new(0x10);
		if (storage != 0) {
			m_process = new (storage) TargetAbout(this);
			goto done;
		}
		break;
	case 0xc:
		m_currentFlow = p_flow;
		m_mainDisplay->KillDrawer(p_flow);
		storage = operator new(0x38);
		if (storage != 0) {
			m_process = new (storage) NetworkOptionsProc(this);
			goto done;
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
		goto done;
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
		goto done;
	case 0x10:
		m_currentFlow = p_flow;
		m_mainDisplay->KillDrawer(p_flow);
		storage = operator new(0x28);
		if (storage != 0) {
			m_process = new (storage) PasswordProc(this);
			goto done;
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
		goto done;
	default:
		goto done;
	}

	m_process = 0;
done:
	m_mainDisplay->StatusUpdate(m_currentFlow);
}

// 68K 0x10702374 Process__5CGameFv
// FUNCTION: LEMBALL 0x004077e0
void Game::Process()
{
	int timing;
	TimeStat* stat;
	unsigned long now;
	int quitState;

	m_mainDisplay->Process();
	if (m_process != 0) {
		timing = 0;
		if ((m_currentFlow == 5 || m_currentFlow == 0x13) && 0x32 < (int) m_flowTicks) {
			timing = 1;
			stat = m_processingStat;
			stat->m_timingStart = timeGetTime();
			stat->m_timingActive = 1;
		}
		m_process->Process();
		if (timing != 0) {
			stat = m_processingStat;
			if (stat->m_timingActive != 0) {
				now = timeGetTime();
				stat->Update(now - stat->m_timingStart);
				stat->m_timingActive = 0;
			}
		}
		if (g_pNetworkManager != 0) {
			g_pNetworkManager->GameProcess();
		}
		switch (m_process->m_processState) {
		case 1:
			NextProcess(m_process->m_returnState);
			break;
		case 2:
			m_quit = 1;
			break;
		}
	}

	quitState = m_mainDisplay->QuitYet();
	switch (quitState) {
	case 1:
		NextProcess(m_mainDisplay->GetReturnState());
		break;
	case 2:
		m_quit = 1;
		break;
	}

	if (m_quit != 0) {
		if (m_mainDisplay->m_drawer != 0) {
			m_mainDisplay->m_drawer->ShutDown();
		}
		if (m_process != 0) {
			delete m_process;
			m_process = 0;
		}
		m_mainDisplay->KillDrawer(0);
	}
}

// 68K 0x10702508 RefreshViews__5CGameFv
// FUNCTION: LEMBALL 0x004078f0
void Game::RefreshViews()
{
	int timing;
	TimeStat* stat;
	unsigned long now;

	timing = 0;
	if ((m_currentFlow == 5 || m_currentFlow == 0x13) && 0x32 < (int) m_flowTicks) {
		timing = 1;
		stat = m_refreshingStat;
		now = timeGetTime();
		stat->m_timingStart = now;
		stat->m_timingActive = timing;
	}
	m_mainDisplay->RefreshView();
	if (timing != 0) {
		stat = m_refreshingStat;
		if (stat->m_timingActive != 0) {
			now = timeGetTime();
			stat->Update(now - stat->m_timingStart);
			stat->m_timingActive = 0;
		}
	}
}

// 68K 0x107025a0 Run__5CGameFv
// FUNCTION: LEMBALL 0x00407950
void Game::Run()
{
	if (g_pDemo != 0 && g_nDemoMode == 0) {
		Demo* demo = g_pDemo;
		demo->m_currentResourceId = RES_DEMOS_DEMO_00;
		demo->m_firstResourceId = RES_DEMOS_DEMO_00;
		demo->m_resourceCount = 8;
	}

	while (m_quit == 0) {
		if (m_currentFlow == 5 || m_currentFlow == 0x13) {
			m_flowTicks = m_flowTicks + 1;
		}
		if (g_pDemo != 0) {
			g_pDemo->Process();
		}
		switch (TargetPumpEvents()) {
		case 0:
			Process();
			if (m_quit != 0) {
				return;
			}
			RefreshViews();
			break;
		case 1:
			m_quit = 1;
			break;
		}
	}
}

// GLOBAL: LEMBALL 0x004a1bcc
int* g_pSentinel = 0;

// GLOBAL: LEMBALL 0x0049ce04
unsigned int g_dwGameTick = 0;
