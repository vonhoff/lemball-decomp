#include "CGame.h"

#include "Application/GameTime.h"

#include "Application/GameMain.h"

#include "Gameplay/Simulation/CAI.h"
#include "Frontend/CBaseFrontendDrawer.h"
#include "Frontend/CBaseFrontendProcess.h"
#include "Frontend/About/CAbout.h"
#include "Frontend/Network/CNetworkOptionsProc.h"
#include "Frontend/Password/CPasswordProc.h"
#include "Frontend/Preview/CPreview.h"
#include "Frontend/Results/CSuccFail.h"
#include "Frontend/Loading/CFrontendResourceLoader.h"
#include "Multiplayer/CNetworkManager.h"
#include "Platform/Windows/Entry.h"
#include "GameView/Display/CMain2DDisplay.h"
#include "GameView/Display/DisplayQuitState.h"
#include "GameView/CSoundView.h"
#include "Frontend/Intro/CIntroAnim.h"
#include "Engine/Statistics/CStatManager.h"
#include "Engine/Statistics/CTimeStat.h"
#include "CBaseProcess.h"
#include "Engine/Diagnostics/CDebugOStream.h"
#include "Frontend/Options/CMainOptions1.h"
#include "Frontend/Options/CMainOptions2.h"
#include "Engine/Diagnostics/VsDebug.h"
#include "Engine/Sound/VsSound.h"
#include "Engine/VsTime.h"
#include "Engine/Windows/CDrawer.h"
#include "Engine/Network/CBaseNetwork.h"
#include "Engine/Network/NetworkConstants.h"
#include "Engine/Resources/Archive/CMogRes.h"
#include "Engine/Resources/Archive/CMogloadArena.h"
#include "Engine/Resources/Types/CResSTRING.h"
#include "Engine/Resources/Manifest.h"
#include "Engine/Sound/CSoundManager.h"
#include "Platform/Windows/CPlatformServices.h"
#include "Level/CLevelLoader.h"
#include "CDemo.h"
#include "CGameStatus.h"
#include "Application/FlowProcesses.h"
#include "GameMain.h"

#include <new.h>
#include <string.h>

enum {
	GAME_FLOW_TIMING_WARMUP_TICKS = 50,
	GAME_RESOURCE_ARENA_SIZE_BYTES = 0x177000,
	GAME_SOUND_CHANNEL_COUNT = 50
};

#ifndef LEMBALL_ENFORCE_STARTUP_CHECKS
#define LEMBALL_ENFORCE_STARTUP_CHECKS 1
#endif

#pragma intrinsic(strcpy, strcat, strcmp)

enum eEventPumpResult {
	EVENT_PUMP_CONTINUE = 0,
	EVENT_PUMP_QUIT = 1
};

extern "C" unsigned long __stdcall timeGetTime(void);
extern "C" __declspec(dllimport) int __stdcall MessageBoxA(void* p_hWnd,
														   const char* p_lpText,
														   const char* p_lpCaption,
														   unsigned int p_uType);

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

extern CStatManager* g_pStatManager;

// FUNCTION: LEMBALL 0x00406df0
CGame::CGame(char* p_runtimeFileName)
{
	void* storage;
	char titleBuf[80];

	m_frontendResources = NULL;
	g_pGameStatus = NULL;
	m_mainDisplay = NULL;
	g_pMogRes = NULL;
	g_pSoundView = NULL;
	m_quit = 1;
	m_process = NULL;

#if LEMBALL_ENFORCE_STARTUP_CHECKS
	enum {
		CD_PROMPT_BUTTONS_OK_CANCEL = 1,
		CD_PROMPT_RESULT_OK = 1,
		CD_PROMPT_RESULT_CANCEL = 2
	};

	if (g_pTargetPlatformServices->WriteRegistryFlag(g_szLemmingsPaintball, 1) == 0) {
		MessageBoxA(NULL, g_szInstallPrompt, g_szPaintballNotInstalled, 0);
		return;
	}

	int cdResponse = 0;
	while (g_pTargetPlatformServices->GetCDDir(g_szVsMemDll) == NULL) {
		cdResponse = MessageBoxA(NULL, g_szInsertCdPrompt, g_szUnableToFindCd, CD_PROMPT_BUTTONS_OK_CANCEL);
		if (cdResponse != CD_PROMPT_RESULT_OK) {
			break;
		}
	}
	if (cdResponse == CD_PROMPT_RESULT_CANCEL) {
		return;
	}
#endif

	g_pGameStatus = new CGameStatus();
	CTimeStat*& processingStat = m_processingStat;
	processingStat = new CTimeStat(g_szProcessing);
	CTimeStat*& refreshingStat = m_refreshingStat;
	refreshingStat = new CTimeStat(g_szRefreshing);

	m_flowTicks = 0;
	g_pStatManager->Register(processingStat);
	g_pStatManager->Register(refreshingStat);

	storage = CMogloadArena::operator new(sizeof(CMogRes));
	if (storage != NULL) {
		g_pMogRes = new (storage) CMogRes(g_szPbaimogVsr, GAME_RESOURCE_ARENA_SIZE_BYTES);
	}
	else {
		g_pMogRes = NULL;
	}

	if (IsValidResource() == 0) {
		_VSRELassert(g_szIsValidResourceFile, g_szGameCpp, 0x16e);
	}

	m_mainDisplay = new CMain2DDisplay(this);

	CDebugOStream stream(titleBuf, 80);
	stream << g_szLemmingsPaintballTitle;

	m_mainDisplay->Create(m_mainDisplay->GetUseRect(DISPLAY_COORDINATE_AUTO_CENTER, DISPLAY_COORDINATE_AUTO_CENTER),
						  NULL,
						  titleBuf);

	InitSound(g_nMusicVolume, g_nEffectsVolume, GAME_SOUND_CHANNEL_COUNT, m_mainDisplay, 0);
	if (g_nMusicVolume != 0) {
		g_pSoundManager->UseMusicCD(1);
		g_pSoundManager->SetMusicCdPath(g_szMusicCdPath);
	}

	g_pSoundView = new CSoundView();

	m_process = NULL;
	m_currentFlow = FLOW_INTRO_ANIM;
	NextProcess(FLOW_INTRO_ANIM);

	strcpy(m_runtimeName, g_szDefaultRuntimeDir);
	if (p_runtimeFileName == NULL) {
		strcat(m_runtimeName, g_szDefaultRuntimeFile);
	}
	else {
		strcat(m_runtimeName, p_runtimeFileName);
	}

	m_quit = 0;
}

// FUNCTION: LEMBALL 0x004071d0
CGame::~CGame()
{
	CFrontendResourceLoader* resources;
	CSoundView* soundView;
	CMogRes* mogRes;
	unsigned long started;
	unsigned long now;

	resources = (CFrontendResourceLoader*) m_frontendResources;
	if (resources != NULL) {
		resources->~CFrontendResourceLoader();
		operator delete(resources);
	}
	if (g_pNetworkManager != NULL) {
		g_pNetworkManager->Stop();
	}
	if (g_pBaseNetwork != NULL) {
		started = CurrentMilliTimer();
		do {
			now = CurrentMilliTimer();
			if (now - started >= NETWORK_QUEUE_TRANSITION_TIMEOUT_MS) {
				break;
			}
		} while (g_pBaseNetwork->m_queueTransitionPending != 0);
	}
	if (g_pNetworkManager != NULL) {
		delete g_pNetworkManager;
		g_pNetworkManager = NULL;
	}
	if (m_mainDisplay != NULL) {
		m_mainDisplay->Destroy();
		if (m_mainDisplay != NULL) {
			delete m_mainDisplay;
		}
	}
	if (m_process != NULL) {
		delete m_process;
	}
	if (g_pSoundView != NULL) {
		soundView = g_pSoundView;
		soundView->~CSoundView();
		operator delete(soundView);
		EndSound();
	}
	if (g_pMogRes != NULL) {
		mogRes = g_pMogRes;
		mogRes->~CMogRes();
		CMogloadArena::operator delete(mogRes);
		g_pMogRes = NULL;
	}
	if (g_pGameStatus != NULL) {
		operator delete(g_pGameStatus);
		g_pGameStatus = NULL;
	}
	g_pTargetPlatformServices->WriteRegistryFlag(g_szLemmingsPaintballRegistry, 0);
}

// FUNCTION: LEMBALL 0x00407300
bool CGame::IsValidResource()
{
	const char* key;
	CResSTRING* resource;
	unsigned char* data;
	int i;
	char c;

	key = g_szWeatherManKey;
	resource = CResSTRING::Load(RES_REGISTRATION_FINGERPRINT);
	if (resource == NULL) {
		return false;
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

// FUNCTION: LEMBALL 0x004073b0
void CGame::LoadFrontendResources(int p_mode)
{
	void* storage;

	if (m_frontendResources == NULL) {
		storage = operator new(sizeof(CFrontendResourceLoader));
		if (storage != NULL) {
			m_frontendResources = new (storage) CFrontendResourceLoader(m_mainDisplay, p_mode);
		}
		else {
			m_frontendResources = NULL;
		}
	}
}

// FUNCTION: LEMBALL 0x004073f0
void CGame::UnLoadFrontendResources()
{
	if (m_frontendResources != NULL) {
		delete (CFrontendResourceLoader*) m_frontendResources;
		m_frontendResources = NULL;
	}
}

// FUNCTION: LEMBALL 0x00407420
void CGame::NextProcess(eFlowProcesses p_flow)
{

	if (m_mainDisplay->m_drawer != NULL) {
		m_mainDisplay->m_drawer->ShutDown();
	}
	if (m_process != NULL) {
		delete m_process;
		m_process = NULL;
	}

	if (p_flow == FLOW_INTRO_ANIM && g_nAnimationsDisabled == 1) {
		p_flow = FLOW_MAIN_OPTIONS_1;
	}
	if (p_flow == FLOW_LEVEL_INTRO && g_nAnimationsDisabled == 1) {
		p_flow = FLOW_PREVIEW;
	}

	switch (p_flow) {
	case FLOW_INTRO_ANIM:
		UnLoadFrontendResources();
		m_currentFlow = p_flow;
		m_mainDisplay->KillDrawer(p_flow);
		m_process = new CIntroAnim(this);
		m_flowTicks = 0;
		goto done;
	case FLOW_MAIN_OPTIONS_1:
		m_currentFlow = p_flow;
		m_mainDisplay->KillDrawer(p_flow);
		LoadFrontendResources(SOUND_STATE_FRONTEND);
		if (g_nDemoMode != 0) {
			g_nDemoMode = g_nStoredLevelDemoModeEnabled;
		}
		m_process = new CMainOptions1(this);
		goto done;
	case FLOW_MAIN_OPTIONS_2:
		m_currentFlow = p_flow;
		m_mainDisplay->KillDrawer(p_flow);
		m_process = new CMainOptions2(this);
		goto done;
	case FLOW_PREVIEW:
		m_currentFlow = p_flow;
		m_mainDisplay->KillDrawer(p_flow);
		m_process = new CPreview(this);
		goto done;
	case FLOW_DEMO:
		if (g_nDemoMode != 0) {
			g_pDemo->m_filePath = g_szDemoFilePath;
		}
		else {
			g_pDemo->m_filePath = NULL;
		}
		g_nDemoMode = 1;
	case FLOW_GAMEPLAY:
		UnLoadFrontendResources();
		m_currentFlow = p_flow;
		m_mainDisplay->KillDrawer(p_flow);
		m_process = new CAI(this);
		goto done;
	case FLOW_ABOUT:
		m_currentFlow = p_flow;
		m_mainDisplay->KillDrawer(p_flow);
		m_process = new CAbout(this);
		goto done;
	case FLOW_NETWORK_OPTIONS:
		m_currentFlow = p_flow;
		m_mainDisplay->KillDrawer(p_flow);
		m_process = new CNetworkOptionsProc(this);
		goto done;
	case FLOW_SUCCESS:
		m_currentFlow = p_flow;
		m_mainDisplay->KillDrawer(p_flow);
		LoadFrontendResources(SOUND_STATE_RESULTS);
		m_process = new CSuccFail(this, 1);
		m_flowTicks = 0;
		goto done;
	case FLOW_FAILURE:
		m_currentFlow = p_flow;
		m_mainDisplay->KillDrawer(p_flow);
		LoadFrontendResources(SOUND_STATE_RESULTS);
		m_process = new CSuccFail(this, 0);
		m_flowTicks = 0;
		goto done;
	case FLOW_PASSWORD:
		m_currentFlow = p_flow;
		m_mainDisplay->KillDrawer(p_flow);
		m_process = new CPasswordProc(this);
		goto done;
	case FLOW_LEVEL_INTRO:
		UnLoadFrontendResources();
		m_currentFlow = p_flow;
		m_mainDisplay->KillDrawer(p_flow);
		m_process = new CIntroAnim(this);
		m_flowTicks = 0;
		goto done;
	default:
		goto done;
	}

done:
	m_mainDisplay->StatusUpdate(m_currentFlow);
}

// FUNCTION: LEMBALL 0x004077e0
void CGame::Process()
{
	int timing;
	CTimeStat* stat;
	unsigned long now;
	int quitState;

	m_mainDisplay->Process();
	if (m_process != NULL) {
		timing = 0;
		if ((m_currentFlow == FLOW_GAMEPLAY || m_currentFlow == FLOW_DEMO) &&
			GAME_FLOW_TIMING_WARMUP_TICKS < (int) m_flowTicks) {
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
		if (g_pNetworkManager != NULL) {
			g_pNetworkManager->GameProcess();
		}
		switch (m_process->m_processState) {
		case PROCESS_RESULT_CONTINUE:
			break;
		case PROCESS_RESULT_CHANGE_FLOW:
			NextProcess((eFlowProcesses) m_process->m_returnState);
			break;
		case PROCESS_RESULT_QUIT:
			m_quit = 1;
			break;
		}
	}

	quitState = m_mainDisplay->QuitYet();
	switch (quitState) {
	case DISPLAY_QUIT_NONE:
		break;
	case DISPLAY_QUIT_CHANGE_FLOW:
		NextProcess((eFlowProcesses) m_mainDisplay->GetReturnState());
		break;
	case DISPLAY_QUIT_APPLICATION:
		m_quit = 1;
		break;
	}

	if (m_quit != 0) {
		if (m_mainDisplay->m_drawer != NULL) {
			m_mainDisplay->m_drawer->ShutDown();
		}
		if (m_process != NULL) {
			delete m_process;
			m_process = NULL;
		}
		m_mainDisplay->KillDrawer(FLOW_NONE);
	}
}

// FUNCTION: LEMBALL 0x004078f0
void CGame::RefreshViews()
{
	int timing;
	CTimeStat* stat;
	unsigned long now;

	timing = 0;
	if ((m_currentFlow == FLOW_GAMEPLAY || m_currentFlow == FLOW_DEMO) &&
		GAME_FLOW_TIMING_WARMUP_TICKS < (int) m_flowTicks) {
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

// FUNCTION: LEMBALL 0x00407950
void CGame::Run()
{
	if (g_pDemo != NULL && g_nDemoMode == 0) {
		CDemo* demo = g_pDemo;
		demo->m_currentResourceId = RES_DEMOS_DEMO_00;
		demo->m_firstResourceId = RES_DEMOS_DEMO_00;
		demo->m_resourceCount = 8;
	}

	while (m_quit == 0) {
		if (m_currentFlow == FLOW_GAMEPLAY || m_currentFlow == FLOW_DEMO) {
			m_flowTicks = m_flowTicks + 1;
		}
		if (g_pDemo != NULL) {
			g_pDemo->Process();
		}
		switch (PumpEvents()) {
		case EVENT_PUMP_CONTINUE:
			Process();
			if (m_quit != 0) {
				return;
			}
			RefreshViews();
			break;
		case EVENT_PUMP_QUIT:
			m_quit = 1;
			break;
		}
	}
}

// FUNCTION: LEMBALL 0x004079e0
void CGame::StreamRuntimeStats()
{
	m_processingStat->StreamOut(*g_pDebugOutput) << '\n';
	m_refreshingStat->StreamOut(*g_pDebugOutput) << '\n';
}
