#include "NetworkOptionsDrawer.h"

#include "../../Control/Game/GameStatus.h"
#include "../../Network/Game/NetworkManager.h"
#include "../../Network/Messages/NetworkGameMessage.h"
#include "../../Views/Display/Main2DDisplay.h"
#include "../../Views/Sound/SoundView.h"
#include "../../Visos/Foundation/VsTime.h"
#include "../../Visos/Graphics/GWnd.h"
#include "../../Visos/Graphics/HotAreaList.h"
#include "../../Visos/Graphics/BasePalManager.h"
#include "../../Visos/Network/Connect.h"
#include "../../Visos/Network/NetworkAddress.h"
#include "../Controls/HiliteController.h"
#include "../Processes/NetworkOptionsProc.h"
#include "../Support/EditString.h"

#include <new.h>
#include <string.h>

#pragma intrinsic(strcpy)

extern char* g_szBroadcastPeerName;

struct BroadcastAddressDispatch {
	virtual char* GetStr() = 0;
};

// GLOBAL: LEMBALL 0x004a0180
unsigned char g_abNetworkOptionsLayoutIp[0xa0] = {
	0x3c, 0x00, 0x00, 0x00, 0x77, 0x01, 0x00, 0x00, 0x02, 0x01, 0x00, 0x00, 0x77, 0x01, 0x00, 0x00,
	0xc7, 0x01, 0x00, 0x00, 0x77, 0x01, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00,
	0x70, 0x02, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0xd5, 0x00, 0x00, 0x00,
	0x70, 0x02, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x0f, 0x01, 0x00, 0x00,
	0x70, 0x02, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x38, 0x01, 0x00, 0x00, 0x1b, 0x01, 0x00, 0x00,
	0x22, 0x00, 0x00, 0x00, 0x15, 0x00, 0x00, 0x00, 0x19, 0x00, 0x00, 0x00, 0x3e, 0x01, 0x00, 0x00,
	0x4c, 0x00, 0x00, 0x00, 0xde, 0x00, 0x00, 0x00, 0x61, 0x00, 0x00, 0x00, 0x43, 0x00, 0x00, 0x00,
	0x10, 0x00, 0x00, 0x00, 0x46, 0x00, 0x00, 0x00, 0x09, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xf4, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00,
	0x61, 0x02, 0x00, 0x00, 0x1d, 0x00, 0x00, 0x00, 0x0f, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

// GLOBAL: LEMBALL 0x004a0220
unsigned char g_abNetworkOptionsLayoutLocal[0xa0] = {
	0x1e, 0x00, 0x00, 0x00, 0xba, 0x00, 0x00, 0x00, 0x81, 0x00, 0x00, 0x00, 0xba, 0x00, 0x00, 0x00,
	0xe3, 0x00, 0x00, 0x00, 0xba, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00,
	0x30, 0x01, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x65, 0x00, 0x00, 0x00,
	0x30, 0x01, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x87, 0x00, 0x00, 0x00,
	0x30, 0x01, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x9f, 0x00, 0x00, 0x00, 0x8f, 0x00, 0x00, 0x00,
	0x0f, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x13, 0x00, 0x00, 0x00, 0x9e, 0x00, 0x00, 0x00,
	0x20, 0x00, 0x00, 0x00, 0x6d, 0x00, 0x00, 0x00, 0x31, 0x00, 0x00, 0x00, 0x22, 0x00, 0x00, 0x00,
	0x0c, 0x00, 0x00, 0x00, 0x22, 0x00, 0x00, 0x00, 0x81, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00,
	0xf4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x00,
	0x29, 0x01, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
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
unsigned long g_anNetworkOptionsAnimIds[6] = {0x1bf, 0x1c0, 0x1b3, 0x1f3, 0x1f4, 0x1e7};

char g_szNetworkOptionsNoMessage[] = "No Message";

// GLOBAL: LEMBALL 0x004a0344
int g_anNetworkOptionsEditMessages[4] = {(int) g_szNetworkOptionsNoMessage, 2, 3, 0};

// GLOBAL: LEMBALL 0x004a0354
int g_anNetworkOptionsEditMaxLength[4] = {0, 8, 0x14, 0};

// GLOBAL: LEMBALL 0x004a0368
char g_szNetworkGameName[16];

// GLOBAL: LEMBALL 0x004a0378
char g_szNetworkBroadcastAddress[16];

// 68K 0x10806468 __ct__21CNetworkOptionsDrawerFP14CMain2DDisplayP4CGDIRC7CVSRect
// STUB: LEMBALL 0x00453280
NetworkOptionsDrawer::NetworkOptionsDrawer(Main2DDisplay* p_arg0, Gdi* p_arg1, const VsRect& p_arg2)
	: BaseFrontendDrawer(p_arg0, p_arg1, p_arg2, (eFlowProcesses) 0xc, 0x32, 200, 0, 100, 0x28)
{
}

// 68K 0x10806646 Load__21CNetworkOptionsDrawerFv
// FUNCTION: LEMBALL 0x00453450
void NetworkOptionsDrawer::Load()
{
	unsigned long* animIds1;
	unsigned long* animIds0;
	unsigned long* animIds2;

	if (m_mode == 1) {
		animIds1 = &g_anNetworkOptionsAnimIds[4];
		animIds0 = &g_anNetworkOptionsAnimIds[3];
		animIds2 = &g_anNetworkOptionsAnimIds[5];
		m_layoutTable = (NetworkOptionsLayout*) g_abNetworkOptionsLayoutLocal;
	}
	else {
		animIds1 = &g_anNetworkOptionsAnimIds[1];
		animIds0 = &g_anNetworkOptionsAnimIds[0];
		animIds2 = &g_anNetworkOptionsAnimIds[2];
		m_layoutTable = (NetworkOptionsLayout*) g_abNetworkOptionsLayoutIp;
	}
	m_handlerCount = 0;
	m_hiliteController = new HiliteController((GWnd*) m_display, m_gdi, 4, m_mode, 0);
	m_hiliteController->AddButton(
		m_layoutTable->m_framePos[0].m_x, m_layoutTable->m_framePos[0].m_y, animIds0, 1, 0, 0, 0, &m_handlerCount, 0xacef000c);
	m_hiliteController->AddButton(
		m_layoutTable->m_framePos[1].m_x, m_layoutTable->m_framePos[1].m_y, animIds1, 1, 0, 0, 0, &m_handlerCount, 0xacef000d);
	m_hiliteController->AddButton(
		m_layoutTable->m_framePos[2].m_x, m_layoutTable->m_framePos[2].m_y, animIds2, 1, 0, 0, 0, &m_handlerCount, 0xacef000e);
	m_hiliteController->SetHilite(0);
	m_hiliteController->SetHiliteWindow();
	InitialiseHandlers();
}

// 68K 0x108067aa UnLoad__21CNetworkOptionsDrawerFv
// FUNCTION: LEMBALL 0x004535a0
void NetworkOptionsDrawer::UnLoad()
{
	if (m_hiliteController != 0) {
		delete m_hiliteController;
	}
}

// 68K 0x108067f4 __dt__21CNetworkOptionsDrawerFv
// FUNCTION: LEMBALL 0x004535c0
NetworkOptionsDrawer::~NetworkOptionsDrawer()
{
	int index;
	EditString* editor;

	if (m_returnState == 0) {
		if (g_pNetworkOptionsProc != 0) {
			delete g_pNetworkOptionsProc;
		}
	}
	else {
		if (g_pNetworkOptionsProc != 0) {
			g_pNetworkOptionsProc->StopBroadcast();
		}
	}
	index = 0;
	do {
		((GWnd*) m_display)->m_hotAreaList->RemoveFromList(&m_playerEntries[index]);
		index = index + 1;
	} while (index < 10);
	delete[] m_playerEntries;
	editor = m_editor;
	if (editor != 0) {
		operator delete(editor->m_text);
		operator delete(editor);
	}
}

// 68K 0x10806954 DrawBackGround__21CNetworkOptionsDrawerFv
// FUNCTION: LEMBALL 0x00453690
void NetworkOptionsDrawer::DrawBackGround()
{
	DrawFrame(3);
	DrawFrame(5);
	DrawFrame(7);
}

// 68K 0x108069ba DrawFrame__21CNetworkOptionsDrawerF17eNetOptsPositions
// FUNCTION: LEMBALL 0x004536b0
void NetworkOptionsDrawer::DrawFrame(int p_position)
{
	NetworkOptionsFramePos* pos = &m_layoutTable->m_framePos[p_position];
	BaseFrontendDrawer::DrawFrame(VsRect(pos[0].m_x, pos[0].m_y, pos[1].m_x, pos[1].m_y));
}

// 68K 0x10806ab0 DrawEntry__21CNetworkOptionsDrawerFUlRi7eRemaps
// STUB: LEMBALL 0x004536f0
void NetworkOptionsDrawer::DrawEntry(unsigned long p_index, int& p_value, int p_remap)
{
}

// 68K 0x10806d8c DrawText__21CNetworkOptionsDrawerFv
// STUB: LEMBALL 0x00453940
void NetworkOptionsDrawer::DrawText()
{
}

// 68K 0x108075c6 DrawAnims__21CNetworkOptionsDrawerFv
// FUNCTION: LEMBALL 0x00454050
void NetworkOptionsDrawer::DrawAnims()
{
}

// 68K 0x108075f6 ProcessMessages__21CNetworkOptionsDrawerFP10tagMESSAGE
// STUB: LEMBALL 0x00454060
bool NetworkOptionsDrawer::ProcessMessages(Message* p_message)
{
	return 0;
}

// 68K 0x10807a3c Start__21CNetworkOptionsDrawerFUc
// FUNCTION: LEMBALL 0x00454520
void NetworkOptionsDrawer::Start(unsigned int p_mode)
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
	g_pNetworkOptionsProc->StopBroadcast();
	if (g_szNetworkGameName[0] != 0) {
		*m_editor = g_szNetworkGameName;
		StartEditing(1, 0);
		return;
	}
	StartEditing(1, 1);
}

// 68K 0x10807afa StartBroadcast__21CNetworkOptionsDrawerFv
// FUNCTION: LEMBALL 0x004545c0
void NetworkOptionsDrawer::StartBroadcast()
{
	m_broadcasting = 1;
	g_pNetworkOptionsProc->Start();
	if (g_pNetworkOptionsProc->m_started != 0 && g_pNetworkOptionsProc->m_startFailed == 0) {
		g_pNetworkManager->StartBroadcast(g_szNetworkBroadcastAddress);
		SetMessage(4);
		return;
	}
	StartMessageTimeout(9, 6000);
}

// 68K 0x10807ba4 Stop__21CNetworkOptionsDrawerFv
// FUNCTION: LEMBALL 0x00454620
void NetworkOptionsDrawer::Stop()
{
	if (g_pNetworkOptionsProc->m_started != 0 && g_pNetworkOptionsProc->m_startFailed == 0) {
		g_pNetworkOptionsProc->Stop();
	}
	SetMessage(0);
	m_editingActive = 0;
}

// 68K 0x10807c12 SetMessage__21CNetworkOptionsDrawerF16eNetOptsMessages
// FUNCTION: LEMBALL 0x00454650
void NetworkOptionsDrawer::SetMessage(int p_message)
{
	unsigned long now;

	m_message = p_message;
	m_messageDuration = 0;
	m_backBufferNeeded = 1;
	now = CurrentMilliTimer();
	m_redrawPending = 1;
	m_lastDrawTime = now;
}

// 68K 0x10807c7c StartEditing__21CNetworkOptionsDrawerF13eEditingStageUc
// FUNCTION: LEMBALL 0x00454690
void NetworkOptionsDrawer::StartEditing(int p_stage, unsigned int p_clear)
{
	int stage;
	EditString* editor;

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
		g_pNetworkOptionsProc->Start();
		if (g_pNetworkOptionsProc->m_started == 0 || g_pNetworkOptionsProc->m_startFailed != 0) {
			StartMessageTimeout(9, 6000);
			return;
		}
	}
	m_editor->m_maxLength = g_anNetworkOptionsEditMaxLength[m_editingStage];
	SetMessage(g_anNetworkOptionsEditMessages[m_editingStage]);
	m_editingActive = 1;
}

// 68K 0x10807d76 StopEditing__21CNetworkOptionsDrawerFv
// FUNCTION: LEMBALL 0x00454740
void NetworkOptionsDrawer::StopEditing()
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
		if (g_pNetworkOptionsProc->m_started == 0 || g_pNetworkOptionsProc->m_startFailed != 0) {
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

// 68K 0x10807e66 LastError__21CNetworkOptionsDrawerFv
// FUNCTION: LEMBALL 0x00454830
void NetworkOptionsDrawer::LastError()
{
	if (m_pendingEvent == 7) {
		g_pNetworkOptionsProc->StopBroadcast();
		if (m_broadcasting == 0) {
			return;
		}
	}
	StartMessageTimeout(m_pendingEvent, 6000);
	m_pendingEvent = 0;
}

// 68K 0x10807ed6 StartMessageTimeout__21CNetworkOptionsDrawerF16eNetOptsMessagesUl
// FUNCTION: LEMBALL 0x00454870
void NetworkOptionsDrawer::StartMessageTimeout(int p_message, unsigned long p_duration)
{
	unsigned long now;

	m_message = p_message;
	now = CurrentMilliTimer();
	m_backBufferNeeded = 1;
	m_messageStartTime = now;
	m_messageDuration = p_duration;
	now = CurrentMilliTimer();
	m_redrawPending = 1;
	m_lastDrawTime = now;
}

// 68K 0x10807f58 Processing__21CNetworkOptionsDrawerFv
// FUNCTION: LEMBALL 0x004548c0
void NetworkOptionsDrawer::Processing()
{
	unsigned long now;
	unsigned long duration;
	unsigned int ident;
	char* peer;
	Connect* connection;
	Connect** current;
	Connect** connections;
	int index;
	int activation;

	if (m_messageDirty != (unsigned int) m_message) {
		return;
	}
	if (m_startPending != 0) {
		StartBroadcast();
		m_startPending = 0;
	}
	if (m_pendingStage != 0) {
		StartEditing(m_pendingStage, 1);
		m_pendingStage = 0;
	}
	if (m_pendingEvent != 0) {
		LastError();
	}
	now = CurrentMilliTimer();
	if (now - m_lastDrawTime >= 500) {
		m_redrawPending = m_redrawPending == 0;
		now = CurrentMilliTimer();
		m_lastDrawTime = now;
	}
	if (g_pNetworkManager != 0) {
		ident = (unsigned int) ((BroadcastAddressDispatch*) g_pBroadcastAddress)->GetStr();
		peer = g_szBroadcastPeerName;
		if (m_stopPending != ident) {
			m_backBufferNeeded = 1;
			m_stopPending = ident;
		}
		if (m_connectionState != (unsigned int) peer) {
			m_backBufferNeeded = 1;
			m_connectionState = (unsigned int) peer;
		}
		if (m_networkState == 0) {
			if (g_pNetworkManager->m_connectionsChanged != 0) {
				g_pNetworkManager->m_connectionsChanged = 0;
				m_networkState = 0;
				m_backBufferNeeded = 1;
				InitialiseHandlers();
			}
		}
		else {
			m_networkState = 0;
			m_backBufferNeeded = 1;
			InitialiseHandlers();
		}
		connections = g_pNetworkManager->m_connections;
		current = connections;
		index = 0;
		do {
			if (m_playerEntries[index].m_pressed != 0 &&
				m_acceptedPlayer != index) {
				g_pSoundView->PlayEffect((eSoundEffect) 0x25);
				if (m_acceptedPlayer != -1) {
					connection = connections[m_acceptedPlayer];
					if (connection != 0) {
						g_pNetworkOptionsProc->Reject(connection);
					}
				}
				m_acceptedPlayer = index;
				if (*current != 0) {
					activation = m_playerEntries[index].m_activationState;
					if (activation != 0) {
						Lock();
					}
					g_pNetworkOptionsProc->Accept(*current, activation);
				}
			}
			current = current + 1;
			index = index + 1;
			m_playerEntries[index - 1].m_pressed = 0;
		} while (index < 10);
	}
	if (m_message != 0) {
		duration = m_messageDuration;
		if (duration != 0) {
			now = CurrentMilliTimer();
			if (now - m_messageStartTime > duration) {
				m_message = 1;
				m_backBufferNeeded = 1;
				m_messageDuration = 0;
			}
		}
	}
}

// 68K 0x10808166 RegisterRemaps__21CNetworkOptionsDrawerFv
// FUNCTION: LEMBALL 0x00454ad0
void NetworkOptionsDrawer::RegisterRemaps()
{
	BaseRemap** remaps;
	unsigned char** mappings;

	remaps = m_remaps;
	mappings = g_apNetworkOptionsRemaps;
	do {
		mappings++;
		remaps++;
		*(remaps - 1) = g_pBasePalManager->RegisterRemap(m_display->m_paletteResourceId, *(mappings - 1), 2);
	} while (mappings < g_apNetworkOptionsRemaps + 6);
}

// 68K 0x108081e8 UnRegisterRemaps__21CNetworkOptionsDrawerFv
// FUNCTION: LEMBALL 0x00454b10
void NetworkOptionsDrawer::UnRegisterRemaps()
{
	int i;
	BaseRemap** remaps;

	remaps = m_remaps;
	i = 6;
	do {
		g_pBasePalManager->UnRegisterRemap(*remaps);
		remaps = remaps + 1;
		i = i - 1;
	} while (i != 0);
}

// 68K 0x10808252 GameReady__21CNetworkOptionsDrawerFi
// FUNCTION: LEMBALL 0x00454b40
void NetworkOptionsDrawer::GameReady(int p_index)
{
	if (p_index != -1 && p_index < 10) {
		m_playerEntries[p_index].m_activationState = 1;
	}
}

// 68K 0x108082a8 GameNotReady__21CNetworkOptionsDrawerFi
// FUNCTION: LEMBALL 0x00454b70
void NetworkOptionsDrawer::GameNotReady(int p_index)
{
	if (p_index != -1 && p_index < 10 &&
		(m_playerEntries[p_index].m_activationState = 0, m_acceptedPlayer == p_index)) {
		UnLock();
	}
}

// 68K 0x10808318 UpdateHighlightedEntry__21CNetworkOptionsDrawerFv
// FUNCTION: LEMBALL 0x00454bb0
void NetworkOptionsDrawer::UpdateHighlightedEntry()
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

// 68K 0x108083c8 HighlightPreviousEntry__21CNetworkOptionsDrawerFv
// FUNCTION: LEMBALL 0x00454c10
bool NetworkOptionsDrawer::HighlightPreviousEntry()
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

// 68K 0x108084d6 HighlightNextEntry__21CNetworkOptionsDrawerFv
// FUNCTION: LEMBALL 0x00454cf0
bool NetworkOptionsDrawer::HighlightNextEntry()
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

// 68K 0x10808604 InitialiseHandlers__21CNetworkOptionsDrawerFv
// FUNCTION: LEMBALL 0x00454df0
void NetworkOptionsDrawer::InitialiseHandlers()
{
	int offset;
	Connect** connections;
	NetworkGameMessage* messages;
	unsigned int* valid;
	EntryHandler* entry;
	short height;
	short y;
	short x;
	short width;
	int stride;

	connections = 0;
	messages = 0;
	if (g_pNetworkManager != 0) {
		connections = g_pNetworkManager->m_connections;
		messages = g_pNetworkManager->m_gameMessages;
	}
	y = (short) m_layoutTable->m_entryY;
	height = m_layoutTable->m_entryHeight;
	width = m_layoutTable->m_entryWidth;
	x = (short) m_layoutTable->m_entryX;
	valid = &messages->m_valid;
	m_visibleEntryCount = 0;
	offset = 0;
	do {
		entry = &m_playerEntries[offset];
		if (m_visibleEntryCount < 4 && connections != 0 && *connections != 0 && *valid != 0) {
			entry->m_x = x;
			entry->m_y = y;
			entry->m_width = width;
			entry->m_height = height;
			entry->SetActive(1);
			stride = m_layoutTable->m_rowStride;
			y = y + (short) stride;
			m_visibleEntryCount = m_visibleEntryCount + 1;
		}
		else {
			entry->SetActive(0);
		}
		offset = offset + 1;
		valid = &messages[offset].m_valid;
		connections = connections + 1;
	} while (offset < 10);
	UpdateHighlightedEntry();
}

// 68K 0x10808792 ResetHandlers__21CNetworkOptionsDrawerFv
// FUNCTION: LEMBALL 0x00454f00
void NetworkOptionsDrawer::ResetHandlers()
{
	int index;
	Connect** connections;
	NetworkGameMessage* messages;
	unsigned int* valid;

	if (g_pNetworkManager != 0) {
		connections = g_pNetworkManager->m_connections;
		index = 0;
		messages = g_pNetworkManager->m_gameMessages;
		valid = &messages->m_valid;
		do {
			if (*connections == 0 || *valid == 0) {
				m_playerEntries[index].Reset();
				if (m_acceptedPlayer == index) {
					m_acceptedPlayer = -1;
				}
			}
			index = index + 1;
			valid = &messages[index].m_valid;
			connections = connections + 1;
		} while (index < 10);
	}
	InitialiseHandlers();
}

// 68K 0x10808834 Lock__21CNetworkOptionsDrawerFv
// FUNCTION: LEMBALL 0x00454f80
void NetworkOptionsDrawer::Lock()
{
	int i;

	m_locked = 1;
	i = 0;
	do {
		m_playerEntries[i].SetActive(0);
		++i;
	} while (i < 10);
}

// 68K 0x10808894 UnLock__21CNetworkOptionsDrawerFv
// FUNCTION: LEMBALL 0x00454fb0
void NetworkOptionsDrawer::UnLock()
{
	m_locked = 0;
	InitialiseHandlers();
}

// 68K 0x108088d2 AcceptingLock__21CNetworkOptionsDrawerFv
// FUNCTION: LEMBALL 0x00454fc0
bool NetworkOptionsDrawer::AcceptingLock()
{
	int unlocked;
	int skill;
	Connect** connections;
	GameStatus* status;

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

