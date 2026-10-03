#include "CNetworkOptionsDrawer.h"

#include "../../Control/Game/CGameStatus.h"
#include "../../Network/Game/CNetworkManager.h"
#include "../../Network/Messages/CNetworkGameMessage.h"
#include "../../Views/Display/CMain2DDisplay.h"
#include "../../Views/Sound/CSoundView.h"
#include "../../Visos/Foundation/CTextManager.h"
#include "../../Visos/Foundation/VsTime.h"
#include "../../Visos/Graphics/CBasePalManager.h"
#include "../../Visos/Graphics/CGWnd.h"
#include "../../Visos/Graphics/CHotAreaList.h"
#include "../../Visos/Network/CConnect.h"
#include "../../Visos/Resources/Manifest.h"
#include "../Controls/CHiliteController.h"
#include "../Processes/CNetworkOptionsProc.h"
#include "../Support/CEditString.h"
#include "Frontend/Base/CBaseFrontendDrawer.h"
#include "Frontend/Base/CBaseFrontendProcess.h"
#include "Frontend/Base/FlowProcesses.h"
#include "Frontend/Support/CEntryHandler.h"
#include "Views/Sound/SoundEffects.h"
#include "Visos/Foundation/CVSPoint.h"
#include "Visos/Foundation/CVSRect.h"
#include "Visos/Foundation/Message.h"
#include "Visos/Graphics/CBaseRemap.h"

#include <string.h>

class CRemap;

#pragma intrinsic(strcpy)

extern "C" unsigned long __stdcall timeGetTime(void);

extern char* g_szBroadcastPeerName;

#define NETWORK_OPTIONS_BUTTON_MESSAGE_LAN 0xacef000c
#define NETWORK_OPTIONS_BUTTON_MESSAGE_SPECIFIC_HOST 0xacef000d
#define NETWORK_OPTIONS_BUTTON_MESSAGE_RETURN 0xacef000e

// GLOBAL: LEMBALL 0x004a0180
NetworkOptionsLayout g_networkOptionsLayoutIp = {
	{{60, 375}, {258, 375}, {455, 375}, {8, 11}, {624, 192}, {8, 213}, {624, 48}, {8, 271}, {624, 96}},
	{312, 283},
	34,
	21,
	{25, 318},
	76,
	222,
	97,
	67,
	16,
	0,
	70,
	0,
	265,
	0,
	500,
	{0, 0, 0, 0, 10, 0, 0, 0},
	32,
	609,
	29,
	0,
	271,
	0,
};

// GLOBAL: LEMBALL 0x004a0220
NetworkOptionsLayout g_networkOptionsLayoutLocal = {
	{{30, 186}, {129, 186}, {227, 186}, {8, 3}, {304, 96}, {8, 101}, {304, 30}, {8, 135}, {304, 48}},
	{159, 143},
	15,
	9,
	{19, 158},
	32,
	109,
	49,
	34,
	12,
	0,
	34,
	0,
	129,
	2,
	244,
	{0, 0, 0, 0, 0, 0, 0, 0},
	14,
	297,
	12,
	0,
	128,
	0,
};

// GLOBAL: LEMBALL 0x004a02c0
unsigned char g_networkOptionsRemap0[8] = {0x02, 0xf1, 0x51, 0x5d, 0x3d, 0x00, 0x00, 0x00};

// GLOBAL: LEMBALL 0x004a02c8
unsigned char g_networkOptionsRemap1[8] = {0x02, 0xf1, 0x51, 0x45, 0x20, 0x00, 0x00, 0x00};

// GLOBAL: LEMBALL 0x004a02d0
unsigned char g_networkOptionsRemap2[8] = {0x02, 0xf1, 0x51, 0x65, 0x31, 0x00, 0x00, 0x00};

// GLOBAL: LEMBALL 0x004a02d8
unsigned char g_networkOptionsRemap3[8] = {0x02, 0xf1, 0x51, 0xad, 0xbd, 0x00, 0x00, 0x00};

// GLOBAL: LEMBALL 0x004a02e0
unsigned char g_networkOptionsRemap4[8] = {0x02, 0xf1, 0x51, 0x8c, 0xad, 0x00, 0x00, 0x00};

// GLOBAL: LEMBALL 0x004a02e8
unsigned char g_networkOptionsRemap5[8] = {0x02, 0xf1, 0x51, 0xa8, 0x6c, 0x00, 0x00, 0x00};

// GLOBAL: LEMBALL 0x004a02f0
unsigned char* g_apNetworkOptionsRemaps[6] = {
	g_networkOptionsRemap0,
	g_networkOptionsRemap1,
	g_networkOptionsRemap2,
	g_networkOptionsRemap3,
	g_networkOptionsRemap4,
	g_networkOptionsRemap5,
};

// GLOBAL: LEMBALL 0x004a0308
unsigned long g_anNetworkOptionsAnimIds[6] = {RES_NEWFRONT_ICONS_HIRES_START_LOCAL,
											  RES_NEWFRONT_ICONS_HIRES_START_SPEC,
											  RES_NEWFRONT_ICONS_HIRES_RETURN,
											  RES_NEWFRONT_ICONS_LORES_START_LOCAL,
											  RES_NEWFRONT_ICONS_LORES_START_SPEC,
											  RES_NEWFRONT_ICONS_LORES_RETURN};

// GLOBAL: LEMBALL 0x004a0398
char g_szNetworkOptionsMsg1[] = "Play with a LAN or specific computer";

// GLOBAL: LEMBALL 0x004a03c0
char g_szNetworkOptionsMsg2[] = "Enter your name:";

// GLOBAL: LEMBALL 0x004a03d4
char g_szNetworkOptionsMsg3[] = "Enter your opponents I.P. Address:";

// GLOBAL: LEMBALL 0x004a03f8
char g_szNetworkOptionsMsg4[] = "Looking for player at ";

// GLOBAL: LEMBALL 0x004a0410
char g_szNetworkOptionsMsg5[] = "Looking for players on local network";

// GLOBAL: LEMBALL 0x004a0438
char g_szNetworkOptionsMsg6[] = "Initialising Network...";

// GLOBAL: LEMBALL 0x004a0450
char g_szNetworkOptionsMsg7[] = "Invalid name or address specified";

// GLOBAL: LEMBALL 0x004a0474
char g_szNetworkOptionsMsg8[] = "You must enter your name";

// GLOBAL: LEMBALL 0x004a0490
char g_szNetworkOptionsMsg9[] = "Network Facilities not available";

// GLOBAL: LEMBALL 0x004a04b4
char g_szNetworkOptionsMsg10[] = "No Message";

// GLOBAL: LEMBALL 0x004a04c0
char g_szNetworkOptionsHeaderName[] = "Name";

// GLOBAL: LEMBALL 0x004a04c8
char g_szNetworkOptionsHeaderIp[] = "I.P. Address";

// GLOBAL: LEMBALL 0x004a04d8
char g_szNetworkOptionsHeaderComputer[] = "Computer";

// GLOBAL: LEMBALL 0x004a04e4
char g_szNetworkOptionsDividerIp[] = "__________________________________";

// GLOBAL: LEMBALL 0x004a0508
char g_szNetworkOptionsDividerLocal[] = "__________________________________";

// GLOBAL: LEMBALL 0x004a052c
char g_szNetworkOptionsCursor[] = "_";

// GLOBAL: LEMBALL 0x004a0320
char* g_apNetworkOptionsMessages[10] = {
	g_szNetworkOptionsMsg1,
	g_szNetworkOptionsMsg2,
	g_szNetworkOptionsMsg3,
	g_szNetworkOptionsMsg4,
	g_szNetworkOptionsMsg5,
	g_szNetworkOptionsMsg6,
	g_szNetworkOptionsMsg7,
	g_szNetworkOptionsMsg8,
	g_szNetworkOptionsMsg9,
	g_szNetworkOptionsMsg10,
};

// GLOBAL: LEMBALL 0x004a0348
int g_anNetworkOptionsEditMessages[3] = {2, 3, 0};

// GLOBAL: LEMBALL 0x004a0358
int g_anNetworkOptionsEditMaxLength[3] = {8, NETWORK_OPTIONS_ADDRESS_MAX_LENGTH, 0};

// GLOBAL: LEMBALL 0x004a0368
char g_szNetworkGameName[16];

// GLOBAL: LEMBALL 0x004a0378
char g_szNetworkBroadcastAddress[NETWORK_OPTIONS_ADDRESS_MAX_LENGTH + 1];

// GLOBAL: LEMBALL 0x004a0390
int g_nNetworkOptionsShiftHeld = 0;

// GLOBAL: LEMBALL 0x004a0394
int g_nNetworkOptionsCapsOrShift = 0;

// FUNCTION: LEMBALL 0x00453280
CNetworkOptionsDrawer::CNetworkOptionsDrawer(CMain2DDisplay* p_display, CGDI* p_gdi, const CVSRect& p_rect)
	: CBaseFrontendDrawer(p_display, p_gdi, p_rect, FLOW_NETWORK_OPTIONS, 0x32, 200, 0, 100, 0x28)
{
	int playerEntryIndex;

	m_editingActive = 0;
	m_message = 1;
	m_drawnMessage = 1;
	m_messageDuration = 0;
	m_pendingEvent = 0;
	m_broadcasting = 0;
	m_networkState = 0;
	m_redrawPending = 0;
	m_lastDrawTime = CurrentMilliTimer();
	m_localAddressText = 0;
	m_localComputerName = 0;
	m_locked = 0;
	m_startPending = 0;
	m_pendingStage = 0;
	m_visibleEntryCount = 0;
	m_editor = new CEditString(0x28);
	m_playerEntries = new CEntryHandler[10];
	m_acceptedPlayer = -1;
	m_highlightedPlayer = -1;
	RegisterRemaps();
	playerEntryIndex = 0;
	do {
		((CGWnd*) m_display)->m_hotAreaList->AddToList(&m_playerEntries[playerEntryIndex]);
		playerEntryIndex = playerEntryIndex + 1;
	} while (playerEntryIndex < 10);
	m_drawBackground = 1;
	m_drawFrame = 1;
	m_drawSolid = 0;
	Setup();
}

// FUNCTION: LEMBALL 0x00453450
void CNetworkOptionsDrawer::Load()
{
	unsigned long* animIds1;
	unsigned long* animIds0;
	unsigned long* animIds2;

	if (m_mode == 1) {
		animIds2 = &g_anNetworkOptionsAnimIds[5];
		animIds1 = &g_anNetworkOptionsAnimIds[4];
		animIds0 = &g_anNetworkOptionsAnimIds[3];
		m_layoutTable = &g_networkOptionsLayoutLocal;
	}
	else {
		animIds2 = &g_anNetworkOptionsAnimIds[2];
		animIds1 = &g_anNetworkOptionsAnimIds[1];
		animIds0 = &g_anNetworkOptionsAnimIds[0];
		m_layoutTable = &g_networkOptionsLayoutIp;
	}
	m_handlerCount = 0;
	m_hiliteController = new CHiliteController((CGWnd*) m_display, m_gdi, 4, m_mode, 0);
	m_hiliteController->AddButton(m_layoutTable->m_framePos[0].m_x,
								  m_layoutTable->m_framePos[0].m_y,
								  animIds0,
								  1,
								  0,
								  0,
								  0,
								  &m_handlerCount,
								  NETWORK_OPTIONS_BUTTON_MESSAGE_LAN);
	m_hiliteController->AddButton(m_layoutTable->m_framePos[1].m_x,
								  m_layoutTable->m_framePos[1].m_y,
								  animIds1,
								  1,
								  0,
								  0,
								  0,
								  &m_handlerCount,
								  NETWORK_OPTIONS_BUTTON_MESSAGE_SPECIFIC_HOST);
	m_hiliteController->AddButton(m_layoutTable->m_framePos[2].m_x,
								  m_layoutTable->m_framePos[2].m_y,
								  animIds2,
								  1,
								  0,
								  0,
								  0,
								  &m_handlerCount,
								  NETWORK_OPTIONS_BUTTON_MESSAGE_RETURN);
	m_hiliteController->SetHilite(0);
	m_hiliteController->SetHiliteWindow();
	InitialiseHandlers();
}

// FUNCTION: LEMBALL 0x004535a0
void CNetworkOptionsDrawer::UnLoad()
{
	if (m_hiliteController != 0) {
		delete m_hiliteController;
	}
}

// FUNCTION: LEMBALL 0x004535c0
CNetworkOptionsDrawer::~CNetworkOptionsDrawer()
{
	int index;
	CEditString* editor;

	if (m_returnState == 0) {
		if (g_pCurrentFrontendProcess != 0) {
			Stop();
		}
	}
	else {
		if (g_pCurrentFrontendProcess != 0) {
			((CNetworkOptionsProc*) g_pCurrentFrontendProcess)->StopBroadcast();
		}
	}
	index = 0;
	do {
		((CGWnd*) m_display)->m_hotAreaList->RemoveFromList(&m_playerEntries[index]);
		index = index + 1;
	} while (index < 10);
	delete[] m_playerEntries;
	editor = m_editor;
	if (editor != 0) {
		operator delete(editor->m_text);
		operator delete(editor);
	}
	UnRegisterRemaps();
	if (m_loaded != 0) {
		UnLoad();
	}
}

// FUNCTION: LEMBALL 0x00453690
void CNetworkOptionsDrawer::DrawBackGround()
{
	DrawFrame(3);
	DrawFrame(5);
	DrawFrame(7);
}

// FUNCTION: LEMBALL 0x00454060
bool CNetworkOptionsDrawer::ProcessMessages(Message* p_message)
{
	bool handled;
	unsigned int code;

	if (m_startPending != 0 || (unsigned int) m_message != m_drawnMessage) {
		return 0;
	}

	switch ((int) p_message->m_type) {
	case 4: {

		code = p_message->m_code;
		if (code == INPUT_KEY_SHIFT) {
			g_nNetworkOptionsShiftHeld = 1;
			g_nNetworkOptionsCapsOrShift |= 1;
			return 1;
		}

		if (m_editingActive != 0) {
			handled = false;
			if (code >= INPUT_KEY_A && code <= INPUT_KEY_Z) {
				if (m_editor->m_length != m_editor->m_maxLength) {
					char offset = ((g_nNetworkOptionsShiftHeld == 0) ? 0xe0 : 0) - 0x3c;
					char ch = (char) code - offset;
					*m_editor += ch;
					goto input_accepted;
				}
				else {
					g_pSoundView->PlayEffect(SFX_CHINK);
				}
			}
			else if (code >= INPUT_KEY_0 && code <= INPUT_KEY_9) {
				if (m_editor->m_length != m_editor->m_maxLength) {
					*m_editor += (char) (code - INPUT_DIGIT_ASCII_OFFSET);
					goto input_accepted;
				}
				else {
					g_pSoundView->PlayEffect(SFX_CHINK);
				}
			}
			else {
				switch (code) {
				case INPUT_KEY_SPACE:
					if (m_editor->m_length != m_editor->m_maxLength) {
						*m_editor += ' ';
						goto input_accepted;
					}
					else {
						g_pSoundView->PlayEffect(SFX_CHINK);
					}
					break;
				case INPUT_KEY_PERIOD:
					if (m_editor->m_length != m_editor->m_maxLength) {
						*m_editor += '.';
						goto input_accepted;
					}
					else {
						g_pSoundView->PlayEffect(SFX_CHINK);
					}
					break;
				case INPUT_KEY_ESCAPE:
					m_broadcasting = 0;
					m_editingActive = 0;
					m_pendingEvent = 0;
					SetMessage(1);
					goto input_accepted;
				case INPUT_KEY_RETURN:
					StopEditing();
					goto input_accepted;
				case INPUT_KEY_DELETE:
				case INPUT_KEY_BACKSPACE: {
					CEditString* editor = m_editor;
					if (editor->m_length != 0) {
						if (editor->m_length > 0) {
							editor->m_length--;
							editor->m_text[editor->m_length] = 0;
						}
						goto input_accepted;
					}
					else {
						g_pSoundView->PlayEffect(SFX_CHINK);
					}
					break;
				}
				}
			}

			goto input_done;
		input_accepted:
			handled = true;
		input_done:
			if (handled) {
				m_lastDrawTime = CurrentMilliTimer();
				m_redrawPending = 0;
				g_pSoundView->PlayEffect(SFX_DRUM1);
				return 1;
			}
		}

		switch (p_message->m_code) {
		case INPUT_KEY_UP:
			if (HighlightPreviousEntry()) {
				g_pSoundView->PlayEffect(SFX_CHANGEOP);
				return 1;
			}
			break;
		case INPUT_KEY_DOWN:
			if (HighlightNextEntry()) {
				g_pSoundView->PlayEffect(SFX_CHANGEOP);
				return 1;
			}
			break;
		case INPUT_KEY_SPACE:
		case 0x22:
		case INPUT_KEY_RETURN:
			if (m_highlightedPlayer != -1) {
				CVSPoint pt;
				m_playerEntries[m_highlightedPlayer].OnButtonDown(pt, 0);
				return 1;
			}
			break;
		}
	}
	case 3:
		code = p_message->m_code;
		if (code == INPUT_KEY_SHIFT) {
			g_nNetworkOptionsCapsOrShift &= ~1;
			g_nNetworkOptionsShiftHeld = 0;
			return 1;
		}
		if (m_editingActive != 0) {
			if (code >= INPUT_KEY_A && code <= INPUT_KEY_Z) {
				return 1;
			}
			switch (code) {
			case INPUT_KEY_UP:
			case INPUT_KEY_SPACE:
			case INPUT_KEY_RETURN:
			case INPUT_KEY_DELETE:
			case INPUT_KEY_BACKSPACE:
				return 1;
			}
		}
		return 0;
	case MESSAGE_BUTTON_RELEASED:
		switch (p_message->m_code) {
		case NETWORK_OPTIONS_BUTTON_MESSAGE_LAN:
			if (m_locked == 0) {
				Start(0);
			}
			break;
		case NETWORK_OPTIONS_BUTTON_MESSAGE_SPECIFIC_HOST:
			if (m_locked == 0) {
				Start(1);
			}
			break;
		case NETWORK_OPTIONS_BUTTON_MESSAGE_RETURN:
			Stop();
			m_quitYet = 1;
			m_returnState = 2;
			return 1;
		}
		return 0;
	default:
		m_processedCount++;
		return 0;
	}
}

// FUNCTION: LEMBALL 0x00454520
void CNetworkOptionsDrawer::Start(unsigned int p_mode)
{
	unsigned int mode;

	if (m_editingActive != 0) {
		mode = m_networkMode;
		if (mode != 0) {
			if (p_mode != 0) {
				goto stop_editing;
			}
		}
		if (mode == 0 && p_mode == 0) {
		stop_editing:
			StopEditing();
			return;
		}
	}
	m_networkMode = p_mode;
	m_broadcasting = 0;
	m_editingActive = 0;
	m_pendingEvent = 0;
	((CNetworkOptionsProc*) g_pCurrentFrontendProcess)->StopBroadcast();
	if (g_szNetworkGameName[0] != 0) {
		*m_editor = g_szNetworkGameName;
		StartEditing(1, 0);
		return;
	}
	StartEditing(1, 1);
}

// FUNCTION: LEMBALL 0x004545c0
void CNetworkOptionsDrawer::StartBroadcast()
{
	m_broadcasting = 1;
	((CNetworkOptionsProc*) g_pCurrentFrontendProcess)->Start();
	if (((CNetworkOptionsProc*) g_pCurrentFrontendProcess)->m_started != 0 &&
		((CNetworkOptionsProc*) g_pCurrentFrontendProcess)->m_startFailed == 0) {
		g_pNetworkManager->StartBroadcast(g_szNetworkBroadcastAddress);
		SetMessage(4);
		return;
	}
	StartMessageTimeout(9, 6000);
}

// FUNCTION: LEMBALL 0x00454620
void CNetworkOptionsDrawer::Stop()
{
	if (((CNetworkOptionsProc*) g_pCurrentFrontendProcess)->m_started != 0 &&
		((CNetworkOptionsProc*) g_pCurrentFrontendProcess)->m_startFailed == 0) {
		((CNetworkOptionsProc*) g_pCurrentFrontendProcess)->Stop();
	}
	SetMessage(0);
	m_editingActive = 0;
}

// FUNCTION: LEMBALL 0x00454650
void CNetworkOptionsDrawer::SetMessage(int p_message)
{
	unsigned long now;

	m_message = p_message;
	m_messageDuration = 0;
	m_backBufferNeeded = 1;
	now = timeGetTime();
	m_redrawPending = 1;
	m_lastDrawTime = now;
}

// FUNCTION: LEMBALL 0x00454690
void CNetworkOptionsDrawer::StartEditing(int p_stage, unsigned int p_clear)
{
	int stage;
	CEditString* editor;

	stage = p_stage;
	m_editingActive = 0;
	if (stage != 3) {
		m_editingStage = stage;
	}
	if (p_clear != 0) {
		editor = m_editor;
		editor->m_length = 0;
		editor->m_text[0] = 0;
	}
	if (m_editingStage == 2) {
		((CNetworkOptionsProc*) g_pCurrentFrontendProcess)->Start();
		if (((CNetworkOptionsProc*) g_pCurrentFrontendProcess)->m_started == 0 ||
			((CNetworkOptionsProc*) g_pCurrentFrontendProcess)->m_startFailed != 0) {
			StartMessageTimeout(9, 6000);
			return;
		}
	}
	m_editor->m_maxLength = g_anNetworkOptionsEditMaxLength[m_editingStage - 1];
	SetMessage(g_anNetworkOptionsEditMessages[m_editingStage - 1]);
	m_editingActive = 1;
}

// FUNCTION: LEMBALL 0x00454740
void CNetworkOptionsDrawer::StopEditing()
{
	int stage;
	char* text;

	stage = m_editingStage;
	m_editingActive = 0;
	switch (stage) {
	case 1:
		text = m_editor->m_text;
		if (*text == 0) {
			StartMessageTimeout(8, 6000);
			return;
		}
		strcpy(g_szNetworkGameName, text);
		if (m_networkMode != 0) {
			m_pendingStage = 2;
		}
		else {
			g_szNetworkBroadcastAddress[0] = 0;
			m_startPending = 1;
		}
		if (((CNetworkOptionsProc*) g_pCurrentFrontendProcess)->m_started == 0 ||
			((CNetworkOptionsProc*) g_pCurrentFrontendProcess)->m_startFailed != 0) {
			SetMessage(6);
			return;
		}
		break;
	case 2:
		strcpy(g_szNetworkBroadcastAddress, m_editor->m_text);
		m_startPending = 1;
		break;
	}
}

// FUNCTION: LEMBALL 0x00454830
void CNetworkOptionsDrawer::LastError()
{
	if (m_pendingEvent == NETWORK_OPTIONS_MESSAGE_HOST_LOOKUP_FAILED) {
		((CNetworkOptionsProc*) g_pCurrentFrontendProcess)->StopBroadcast();
		if (m_broadcasting == 0) {
			return;
		}
	}
	StartMessageTimeout(m_pendingEvent, 6000);
	m_pendingEvent = 0;
}

// FUNCTION: LEMBALL 0x00454870
void CNetworkOptionsDrawer::StartMessageTimeout(int p_message, unsigned long p_duration)
{
	unsigned long now;

	m_message = p_message;
	now = timeGetTime();
	m_backBufferNeeded = 1;
	m_messageStartTime = now;
	m_messageDuration = p_duration;
	now = timeGetTime();
	m_redrawPending = 1;
	m_lastDrawTime = now;
}

// FUNCTION: LEMBALL 0x00454b10
void CNetworkOptionsDrawer::UnRegisterRemaps()
{
	int i;
	CBaseRemap** remaps;

	remaps = m_remaps;
	i = 6;
	do {
		g_pBasePalManager->UnRegisterRemap(*remaps);
		remaps = remaps + 1;
		i = i - 1;
	} while (i != 0);
}

// FUNCTION: LEMBALL 0x00454b40
void CNetworkOptionsDrawer::GameReady(int p_index)
{
	if (p_index != -1 && p_index < 10) {
		m_playerEntries[p_index].m_activationState = 1;
	}
}

// FUNCTION: LEMBALL 0x00454b70
void CNetworkOptionsDrawer::GameNotReady(int p_index)
{
	if (p_index != -1 && p_index < 10 &&
		(m_playerEntries[p_index].m_activationState = 0, m_acceptedPlayer == p_index)) {
		UnLock();
	}
}

// FUNCTION: LEMBALL 0x00454bb0
void CNetworkOptionsDrawer::UpdateHighlightedEntry()
{
	if (m_highlightedPlayer != -1) {
		if (m_playerEntries[m_highlightedPlayer].m_active == 0) {
			if (HighlightPreviousEntry() == 0) {
				HighlightNextEntry();
			}
		}
		else {
			m_playerEntries[m_highlightedPlayer].m_entered = 1;
			m_playerEntries[m_highlightedPlayer].OnEnter();
		}
	}
}

// FUNCTION: LEMBALL 0x00454c10
bool CNetworkOptionsDrawer::HighlightPreviousEntry()
{
	int selected;
	int i;

	if (m_visibleEntryCount == 0 || m_highlightedPlayer == 0) {
		return 0;
	}

	if (m_highlightedPlayer == -1) {
		m_hiliteController->m_active = 0;
		m_highlightedPlayer = 10;
	}

	selected = m_highlightedPlayer - 1;
	if (selected >= 0) {
		do {
			if (m_playerEntries[selected].m_active != 0) {
				break;
			}
			--selected;
		} while (selected >= 0);
	}

	if (selected < 0) {
		return 0;
	}

	m_highlightedPlayer = selected;
	i = 0;
	do {
		m_playerEntries[i].m_entered = 0;
		m_playerEntries[i].OnExit();
		++i;
	} while (i < 10);

	m_playerEntries[m_highlightedPlayer].m_entered = 1;
	m_playerEntries[m_highlightedPlayer].OnEnter();
	return 1;
}

// FUNCTION: LEMBALL 0x00454cf0
bool CNetworkOptionsDrawer::HighlightNextEntry()
{
	int selected;
	int i;

	if (m_visibleEntryCount == 0) {
		if (m_highlightedPlayer == -1) {
			return 0;
		}
		m_hiliteController->m_active = 1;
		m_highlightedPlayer = -1;
	}

	if (m_highlightedPlayer == -1) {
		return 0;
	}

	selected = m_highlightedPlayer + 1;
	if (selected < 10) {
		do {
			if (m_playerEntries[selected].m_active != 0) {
				break;
			}
			++selected;
		} while (selected < 10);
	}

	m_highlightedPlayer = selected;
	i = 0;
	do {
		m_playerEntries[i].m_entered = 0;
		m_playerEntries[i].OnExit();
		++i;
	} while (i < 10);

	if (m_highlightedPlayer == 10) {
		m_hiliteController->m_active = 1;
		m_highlightedPlayer = -1;
		return 1;
	}

	m_playerEntries[m_highlightedPlayer].m_entered = 1;
	m_playerEntries[m_highlightedPlayer].OnEnter();
	return 1;
}

// FUNCTION: LEMBALL 0x00454df0
void CNetworkOptionsDrawer::InitialiseHandlers()
{
	CVSRect rect;
	CConnect** connections;
	CNetworkGameMessage* messages;
	int index;

	connections = 0;
	messages = 0;
	if (g_pNetworkManager != 0) {
		connections = g_pNetworkManager->m_connections;
		messages = g_pNetworkManager->m_gameMessages;
	}
	rect.m_height = m_layoutTable->m_entryHeight;
	rect.m_y = (short) m_layoutTable->m_entryY;
	rect.m_x = (short) m_layoutTable->m_entryX;
	rect.m_width = (short) m_layoutTable->m_entryWidth;
	index = 0;
	m_visibleEntryCount = 0;
	do {
		if (m_visibleEntryCount < 4 && connections != 0 && connections[index] != 0 && messages[index].m_valid != 0) {
			CEntryHandler* entry = &m_playerEntries[index];
			entry->m_bounds.m_width = rect.m_width;
			entry->m_bounds.m_height = rect.m_height;
			entry->m_bounds.m_x = rect.m_x;
			entry->m_bounds.m_y = rect.m_y;
			entry->SetActive(1);
			rect.m_y += (short) m_layoutTable->m_rowStride;
			m_visibleEntryCount++;
		}
		else {
			CEntryHandler* entry = &m_playerEntries[index];
			entry->SetActive(0);
		}
		index++;
	} while (index < 10);
	UpdateHighlightedEntry();
}

// FUNCTION: LEMBALL 0x00454f00
void CNetworkOptionsDrawer::ResetHandlers()
{
	CConnect** connections;
	unsigned int* valid;
	int index;

	if (g_pNetworkManager != 0) {
		connections = g_pNetworkManager->m_connections;
		index = 0;
		valid = &g_pNetworkManager->m_gameMessages->m_valid;
		do {
			if (*connections == 0 || *valid == 0) {
				m_playerEntries[index].Reset();
				if (m_acceptedPlayer == index) {
					m_acceptedPlayer = -1;
				}
			}
			valid = (unsigned int*) ((char*) valid + sizeof(CNetworkGameMessage));
			connections++;
			index++;
		} while (index < 10);
	}
	InitialiseHandlers();
}

// FUNCTION: LEMBALL 0x00454f80
void CNetworkOptionsDrawer::Lock()
{
	int i;

	m_locked = 1;
	i = 0;
	do {
		m_playerEntries[i].SetActive(0);
		++i;
	} while (i < 10);
}

// FUNCTION: LEMBALL 0x00454fb0
void CNetworkOptionsDrawer::UnLock()
{
	m_locked = 0;
	InitialiseHandlers();
}

// FUNCTION: LEMBALL 0x00454fc0
bool CNetworkOptionsDrawer::AcceptingLock()
{
	int unlocked;
	int skill;
	CConnect** connections;
	CGameStatus* status;

	unlocked = m_locked == 0;
	if (g_pNetworkManager != 0) {
		connections = g_pNetworkManager->m_connections;
		if (m_acceptedPlayer != -1 && connections[m_acceptedPlayer] != 0) {
			Lock();
			g_pActiveConnection = connections[m_acceptedPlayer];
			skill = 4;
			m_quitYet = 1;
			m_returnState = skill;
			status = g_pGameStatus;
			status->m_level = status->m_lastLevels[4];
			status->m_skill = skill;
			return unlocked;
		}
	}
	m_locked = 0;
	UnLock();
	return 0;
}
