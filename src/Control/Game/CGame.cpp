#include "CGame.h"

#include "../../AI/Navigation/CAI.h"
#include "../../Frontend/Base/CBaseFrontendDrawer.h"
#include "../../Frontend/Base/CBaseFrontendProcess.h"
#include "../../Frontend/Processes/CAbout.h"
#include "../../Frontend/Processes/CNetworkOptionsProc.h"
#include "../../Frontend/Processes/CPasswordProc.h"
#include "../../Frontend/Processes/CPreview.h"
#include "../../Frontend/Processes/CSuccFail.h"
#include "../../Frontend/Resources/CFrontendResourceLoader.h"
#include "../../Network/Game/CNetworkManager.h"
#include "../../Platform/Windows/Entry.h"
#include "../../Views/Display/CMain2DDisplay.h"
#include "../../Views/Sound/CSoundView.h"
#include "../../Visos/Animation/CIntroAnim.h"
#include "../../Visos/Animation/CStatManager.h"
#include "../../Visos/Animation/CTimeStat.h"
#include "../../Visos/Foundation/CBaseProcess.h"
#include "../../Visos/Foundation/CDebugOStream.h"
#include "../../Visos/Foundation/CMainOptions1.h"
#include "../../Visos/Foundation/CMainOptions2.h"
#include "../../Visos/Foundation/VsDebug.h"
#include "../../Visos/Foundation/VsSound.h"
#include "../../Visos/Foundation/VsTime.h"
#include "../../Visos/Graphics/CDrawer.h"
#include "../../Visos/Network/CBaseNetwork.h"
#include "../../Visos/Resources/CMogRes.h"
#include "../../Visos/Resources/CMogloadArena.h"
#include "../../Visos/Resources/CResSTRING.h"
#include "../../Visos/Resources/Manifest.h"
#include "../../Visos/Sound/CSoundManager.h"
#include "../../Visos/Target/System/CPlatformServices.h"
#include "../Level/CLevelLoader.h"
#include "CDemo.h"
#include "CGameStatus.h"
#include "Frontend/Base/FlowProcesses.h"
#include "GameMain.h"

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

extern CStatManager* g_pStatManager;

// FUNCTION: LEMBALL 0x00406df0
CGame::CGame(char* p_arg0)
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

	g_pGameStatus = new CGameStatus();
	m_processingStat = new CTimeStat(g_szProcessing);
	m_refreshingStat = new CTimeStat(g_szRefreshing);

	m_flowTicks = 0;
	g_pStatManager->Register(m_processingStat);
	g_pStatManager->Register(m_refreshingStat);

	storage = CMogloadArena::operator new(0x28);
	if (storage != 0) {
		g_pMogRes = new (storage) CMogRes(g_szPbaimogVsr, 0x177000);
	}
	else {
		g_pMogRes = 0;
	}

	if (IsValidResource() == 0) {
		_VSRELassert(g_szIsValidResourceFile, g_szGameCpp, 0x16e);
	}

	m_mainDisplay = new CMain2DDisplay(this);

	CDebugOStream stream(titleBuf, 80);
	stream << g_szLemmingsPaintballTitle;

	m_mainDisplay->Create(m_mainDisplay->GetUseRect(-1, -1), 0, titleBuf);

	InitSound(g_nMusicVolume, g_nEffectsVolume, 0x32, m_mainDisplay, 0);
	if (g_nMusicVolume != 0) {
		g_pSoundManager->UseMusicCD(1);
		g_pSoundManager->SetMusicCdPath(g_szMusicCdPath);
	}

	g_pSoundView = new CSoundView();

	m_process = 0;
	m_currentFlow = FLOW_INTRO_ANIM;
	NextProcess(FLOW_INTRO_ANIM);

	strcpy(m_runtimeName, g_szDefaultRuntimeDir);
	if (p_arg0 == 0) {
		strcat(m_runtimeName, g_szDefaultRuntimeFile);
	}
	else {
		strcat(m_runtimeName, p_arg0);
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
	if (resources != 0) {
		resources->~CFrontendResourceLoader();
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
			delete m_mainDisplay;
		}
	}
	if (m_process != 0) {
		delete m_process;
	}
	if (g_pSoundView != 0) {
		soundView = g_pSoundView;
		soundView->~CSoundView();
		operator delete(soundView);
		EndSound();
	}
	if (g_pMogRes != 0) {
		mogRes = g_pMogRes;
		mogRes->~CMogRes();
		CMogloadArena::operator delete(mogRes);
		g_pMogRes = 0;
	}
	if (g_pGameStatus != 0) {
		operator delete(g_pGameStatus);
		g_pGameStatus = 0;
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

// FUNCTION: LEMBALL 0x004073b0
void CGame::LoadFrontendResources(int p_mode)
{
	void* storage;

	if (m_frontendResources == 0) {
		storage = operator new(0x58);
		if (storage != 0) {
			m_frontendResources = new (storage) CFrontendResourceLoader(m_mainDisplay, p_mode);
		}
		else {
			m_frontendResources = 0;
		}
	}
}

// FUNCTION: LEMBALL 0x004073f0
void CGame::UnLoadFrontendResources()
{
	if (m_frontendResources != 0) {
		delete (CFrontendResourceLoader*) m_frontendResources;
		m_frontendResources = 0;
	}
}

// FUNCTION: LEMBALL 0x00407420
void CGame::NextProcess(eFlowProcesses p_flow)
{

	if (m_mainDisplay->m_drawer != 0) {
		m_mainDisplay->m_drawer->ShutDown();
	}
	if (m_process != 0) {
		delete m_process;
		m_process = 0;
	}

	if (p_flow == FLOW_INTRO_ANIM && g_nAnimationsDisabled == 1) {
		p_flow = FLOW_MAIN_OPTIONS_1;
	}
	if (p_flow == FLOW_LEVEL_INTRO && g_nAnimationsDisabled == 1) {
		p_flow = FLOW_PREVIEW;
	}

	switch (p_flow) {
	case 1:
		UnLoadFrontendResources();
		m_currentFlow = p_flow;
		m_mainDisplay->KillDrawer(p_flow);
		m_process = new CIntroAnim(this);
		m_flowTicks = 0;
		goto done;
	case 2:
		m_currentFlow = p_flow;
		m_mainDisplay->KillDrawer(p_flow);
		LoadFrontendResources(3);
		if (g_nDemoMode != 0) {
			g_nDemoMode = g_nStoredLevelDemoModeEnabled;
		}
		m_process = new CMainOptions1(this);
		goto done;
	case 3:
		m_currentFlow = p_flow;
		m_mainDisplay->KillDrawer(p_flow);
		m_process = new CMainOptions2(this);
		goto done;
	case 4:
		m_currentFlow = p_flow;
		m_mainDisplay->KillDrawer(p_flow);
		m_process = new CPreview(this);
		goto done;
	case 0x13:
		if (g_nDemoMode != 0) {
			g_pDemo->m_filePath = g_szDemoFilePath;
		}
		else {
			g_pDemo->m_filePath = 0;
		}
		g_nDemoMode = 1;
	case 5:
		UnLoadFrontendResources();
		m_currentFlow = p_flow;
		m_mainDisplay->KillDrawer(p_flow);
		m_process = new CAI(this);
		goto done;
	case 10:
		m_currentFlow = p_flow;
		m_mainDisplay->KillDrawer(p_flow);
		m_process = new CAbout(this);
		goto done;
	case 0xc:
		m_currentFlow = p_flow;
		m_mainDisplay->KillDrawer(p_flow);
		m_process = new CNetworkOptionsProc(this);
		goto done;
	case 0xe:
		m_currentFlow = p_flow;
		m_mainDisplay->KillDrawer(p_flow);
		LoadFrontendResources(2);
		m_process = new CSuccFail(this, 1);
		m_flowTicks = 0;
		goto done;
	case 0xf:
		m_currentFlow = p_flow;
		m_mainDisplay->KillDrawer(p_flow);
		LoadFrontendResources(2);
		m_process = new CSuccFail(this, 0);
		m_flowTicks = 0;
		goto done;
	case 0x10:
		m_currentFlow = p_flow;
		m_mainDisplay->KillDrawer(p_flow);
		m_process = new CPasswordProc(this);
		goto done;
	case 0x12:
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
			NextProcess((eFlowProcesses) m_process->m_returnState);
			break;
		case 2:
			m_quit = 1;
			break;
		}
	}

	quitState = m_mainDisplay->QuitYet();
	switch (quitState) {
	case 1:
		NextProcess((eFlowProcesses) m_mainDisplay->GetReturnState());
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

// FUNCTION: LEMBALL 0x00407950
void CGame::Run()
{
	if (g_pDemo != 0 && g_nDemoMode == 0) {
		CDemo* demo = g_pDemo;
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
		switch (PumpEvents()) {
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

// FUNCTION: LEMBALL 0x004079e0
void CGame::StreamRuntimeStats()
{
	m_processingStat->StreamOut(*g_pDebugOutput) << '\n';
	m_refreshingStat->StreamOut(*g_pDebugOutput) << '\n';
}

// GLOBAL: LEMBALL 0x004a1bcc
int* g_pSentinel = 0;

// GLOBAL: LEMBALL 0x0049ce04
unsigned int g_dwGameTick = 0;
