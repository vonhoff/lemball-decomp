#include "NetworkOptionsDrawer.h"

#include "../../Visos/Graphics/BasePalManager.h"
#include "../Controls/HiliteController.h"

extern "C" __declspec(dllimport) unsigned long __stdcall timeGetTime(void);

// 68K 0x10806468 __ct__21CNetworkOptionsDrawerFP14CMain2DDisplayP4CGDIRC7CVSRect
// STUB: LEMBALL 0x00453280
NetworkOptionsDrawer::NetworkOptionsDrawer(Main2DDisplay* p_arg0, Gdi* p_arg1, const VsRect& p_arg2)
	: BaseFrontendDrawer(p_arg0, p_arg1, p_arg2, (eFlowProcesses) 0xc, 0x32, 200, 0, 100, 0x28)
{
}

// 68K 0x10806646 Load__21CNetworkOptionsDrawerFv
// STUB: LEMBALL 0x00453450
void NetworkOptionsDrawer::Load()
{
}

// 68K 0x108067aa UnLoad__21CNetworkOptionsDrawerFv
// STUB: LEMBALL 0x004535a0
void NetworkOptionsDrawer::UnLoad()
{
}

// 68K 0x10806954 DrawBackGround__21CNetworkOptionsDrawerFv
// STUB: LEMBALL 0x00453690
void NetworkOptionsDrawer::DrawBackGround()
{
}

// 68K 0x108069ba DrawFrame__21CNetworkOptionsDrawerF17eNetOptsPositions
// STUB: LEMBALL 0x004536b0
void NetworkOptionsDrawer::DrawFrame(int p_position)
{
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
	// STRING: LEMBALL 0x004a04e4 "________________________________"
	// STRING: LEMBALL 0x004a0508 "_________________________________"
	// STRING: LEMBALL 0x004a04c8 "I.P. Address"
	// STRING: LEMBALL 0x004a04d8 "Computer"
	// STRING: LEMBALL 0x004a0410 "Looking for players on local network"
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
// STUB: LEMBALL 0x00454520
void NetworkOptionsDrawer::Start(unsigned char p_mode)
{
}

// 68K 0x10807afa StartBroadcast__21CNetworkOptionsDrawerFv
// STUB: LEMBALL 0x004545c0
void NetworkOptionsDrawer::StartBroadcast()
{
}

// 68K 0x10807ba4 Stop__21CNetworkOptionsDrawerFv
// STUB: LEMBALL 0x00454620
void NetworkOptionsDrawer::Stop()
{
}

// 68K 0x10807c12 SetMessage__21CNetworkOptionsDrawerF16eNetOptsMessages
// FUNCTION: LEMBALL 0x00454650
void NetworkOptionsDrawer::SetMessage(int p_message)
{
	unsigned long now;

	m_message = p_message;
	m_messageDuration = 0;
	m_backBufferNeeded = 1;
	now = timeGetTime();
	m_redrawPending = 1;
	m_lastDrawTime = now;
}

// 68K 0x10807c7c StartEditing__21CNetworkOptionsDrawerF13eEditingStageUc
// STUB: LEMBALL 0x00454690
void NetworkOptionsDrawer::StartEditing(int p_stage, unsigned char p_clear)
{
}

// 68K 0x10807d76 StopEditing__21CNetworkOptionsDrawerFv
// STUB: LEMBALL 0x00454740
void NetworkOptionsDrawer::StopEditing()
{
}

// 68K 0x10807e66 LastError__21CNetworkOptionsDrawerFv
// STUB: LEMBALL 0x00454830
void NetworkOptionsDrawer::LastError()
{
}

// 68K 0x10807ed6 StartMessageTimeout__21CNetworkOptionsDrawerF16eNetOptsMessagesUl
// FUNCTION: LEMBALL 0x00454870
void NetworkOptionsDrawer::StartMessageTimeout(int p_message, unsigned long p_duration)
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

// 68K 0x10807f58 Processing__21CNetworkOptionsDrawerFv
// STUB: LEMBALL 0x004548c0
void NetworkOptionsDrawer::Processing()
{
}

// 68K 0x10808166 RegisterRemaps__21CNetworkOptionsDrawerFv
// STUB: LEMBALL 0x00454ad0
void NetworkOptionsDrawer::RegisterRemaps()
{
}

// 68K 0x108081e8 UnRegisterRemaps__21CNetworkOptionsDrawerFv
// FUNCTION: LEMBALL 0x00454b10
void NetworkOptionsDrawer::UnRegisterRemaps()
{
	int i;
	void** remaps;

	remaps = m_remaps;
	i = 6;
	do {
		g_pBasePalManager->UnRegisterRemap((BaseRemap*) *remaps);
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
// STUB: LEMBALL 0x00454df0
void NetworkOptionsDrawer::InitialiseHandlers()
{
}

// 68K 0x10808792 ResetHandlers__21CNetworkOptionsDrawerFv
// STUB: LEMBALL 0x00454f00
void NetworkOptionsDrawer::ResetHandlers()
{
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
// STUB: LEMBALL 0x00454fc0
bool NetworkOptionsDrawer::AcceptingLock()
{
	return 0;
}

// 68K 0x10806800 __dt__21CNetworkOptionsDrawerFv
// SYNTHETIC: LEMBALL 0x00455de0
// NetworkOptionsDrawer::`scalar deleting destructor'
NetworkOptionsDrawer::~NetworkOptionsDrawer()
{
}
