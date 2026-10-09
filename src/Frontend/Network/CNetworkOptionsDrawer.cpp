#include "CNetworkOptionsDrawer.h"

#include "Application/CGameStatus.h"
#include "Application/FlowProcesses.h"
#include "Application/SoundEffects.h"
#include "CEditString.h"
#include "CEntryHandler.h"
#include "CNetworkOptionsProc.h"
#include "Engine/Graphics/Palettes/CBasePalManager.h"
#include "Engine/Graphics/Palettes/CBaseRemap.h"
#include "Engine/Input/CHotAreaList.h"
#include "Engine/Math/CVSPoint.h"
#include "Engine/Math/CVSRect.h"
#include "Engine/Queues/Message.h"
#include "Engine/Resources/Manifest.h"
#include "Engine/Text/CTextManager.h"
#include "Engine/Text/TextAdvanceFlags.h"
#include "Engine/Time/VsTime.h"
#include "Frontend/CBaseFrontendDrawer.h"
#include "Frontend/CBaseFrontendProcess.h"
#include "Frontend/Controls/CHiliteController.h"
#include "Frontend/FrontendLayoutMode.h"
#include "GameView/Display/CMain2DDisplay.h"
#include "GameView/Sound/CSoundView.h"
#include "Multiplayer/CNetworkGameMessage.h"
#include "Multiplayer/CNetworkManager.h"
#include "Multiplayer/Transport/CConnect.h"
#include "Platform/Windows/Windowing/CGWnd.h"

#include <string.h>

class CRemap;

#pragma intrinsic(strcpy)

extern char* g_szBroadcastPeerName;

#define NETWORK_OPTIONS_MODE_LAN 0
#define NETWORK_OPTIONS_MODE_SPECIFIC_HOST 1
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
char g_szNetworkOptionsDividerIp[] = "________________________________";

// GLOBAL: LEMBALL 0x004a0508
char g_szNetworkOptionsDividerLocal[] = "_________________________________";

// GLOBAL: LEMBALL 0x004a052c
char g_szNetworkOptionsCursor[] = "_";

// GLOBAL: LEMBALL 0x004a0320
char* g_apNetworkOptionsMessages[NETWORK_OPTIONS_MESSAGE_COUNT] = {
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
eNetOptsMessages g_anNetworkOptionsEditMessages[3] = {NETWORK_OPTIONS_MESSAGE_ENTER_NAME,
													  NETWORK_OPTIONS_MESSAGE_ENTER_IP_ADDRESS,
													  NETWORK_OPTIONS_MESSAGE_NONE};

// GLOBAL: LEMBALL 0x004a0358
int g_anNetworkOptionsEditMaxLength[3] = {8, NETWORK_OPTIONS_ADDRESS_MAX_LENGTH, 0};

enum {
	NETWORK_OPTIONS_EDIT_BUFFER_CAPACITY = 0x28
};

// GLOBAL: LEMBALL 0x004a0368
char g_szNetworkGameName[16] = {0};

// GLOBAL: LEMBALL 0x004a0378
char g_szNetworkBroadcastAddress[NETWORK_OPTIONS_ADDRESS_MAX_LENGTH + 1] = {0};

// GLOBAL: LEMBALL 0x004a0390
int g_nNetworkOptionsShiftHeld = 0;

// GLOBAL: LEMBALL 0x004a0394
int g_nNetworkOptionsCapsOrShift = 0;

#include "Multiplayer/Transport/CNetworkAddress.h"

extern char* g_szBroadcastPeerName;

enum {
	NETWORK_OPTIONS_REDRAW_INTERVAL_MS = 500
};

extern char* g_szBroadcastPeerName;

extern unsigned char* g_apNetworkOptionsRemaps[6];

#include "Engine/Math/CVSSize.h"
#include "Engine/Resources/Types/CResFONT.h"

extern char* g_szBroadcastPeerName;

enum {
	NETWORK_OPTIONS_REMAP_NONE = 6
};

#include "Engine/Strings/CString.h"

extern char* g_szBroadcastPeerName;

extern char* g_apNetworkOptionsMessages[NETWORK_OPTIONS_MESSAGE_COUNT];
extern char g_szNetworkGameName[16];
extern char g_szNetworkBroadcastAddress[NETWORK_OPTIONS_ADDRESS_MAX_LENGTH + 1];
extern char g_szNetworkOptionsCursor[];
extern char g_szNetworkOptionsDividerIp[];
extern char g_szNetworkOptionsDividerLocal[];
extern char g_szNetworkOptionsHeaderComputer[];
extern char g_szNetworkOptionsHeaderIp[];
extern char g_szNetworkOptionsHeaderName[];

// FUNCTION: LEMBALL 0x00453280
CNetworkOptionsDrawer::CNetworkOptionsDrawer(CMain2DDisplay* p_display, CGDI* p_gdi, const CVSRect& p_rect)
	: CBaseFrontendDrawer(p_display,
						  p_gdi,
						  p_rect,
						  FLOW_NETWORK_OPTIONS,
						  0x32,
						  200,
						  0,
						  100,
						  NETWORK_OPTIONS_EDIT_BUFFER_CAPACITY)
{
	int playerEntryIndex;

	m_editingActive = 0;
	m_message = NETWORK_OPTIONS_MESSAGE_NETWORK_TYPE_PROMPT;
	m_drawnMessage = NETWORK_OPTIONS_MESSAGE_NETWORK_TYPE_PROMPT;
	m_messageDuration = 0;
	m_pendingEvent = NETWORK_OPTIONS_MESSAGE_NONE;
	m_broadcasting = 0;
	m_networkState = NETWORK_OPTIONS_HANDLERS_CURRENT;
	m_redrawPending = 0;
	m_lastDrawTime = CurrentMilliTimer();
	m_localAddressText = NULL;
	m_localComputerName = NULL;
	m_locked = 0;
	m_startPending = 0;
	m_pendingStage = NETWORK_OPTIONS_EDIT_NONE;
	m_visibleEntryCount = 0;
	m_editor = new CEditString(NETWORK_OPTIONS_EDIT_BUFFER_CAPACITY);
	m_playerEntries = new CEntryHandler[NETWORK_OPTIONS_PLAYER_ENTRY_COUNT];
	m_acceptedPlayer = NETWORK_OPTIONS_NO_PLAYER_INDEX;
	m_highlightedPlayer = NETWORK_OPTIONS_NO_PLAYER_INDEX;
	RegisterRemaps();
	playerEntryIndex = 0;
	do {
		((CGWnd*) m_display)->m_hotAreaList->AddToList(&m_playerEntries[playerEntryIndex]);
		playerEntryIndex = playerEntryIndex + 1;
	} while (playerEntryIndex < NETWORK_OPTIONS_PLAYER_ENTRY_COUNT);
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

	if (m_mode == FRONTEND_LAYOUT_COMPACT) {
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
								  HILITE_BUTTON_MODE_ACTION_MESSAGE,
								  0,
								  0,
								  0,
								  &m_handlerCount,
								  NETWORK_OPTIONS_BUTTON_MESSAGE_LAN);
	m_hiliteController->AddButton(m_layoutTable->m_framePos[1].m_x,
								  m_layoutTable->m_framePos[1].m_y,
								  animIds1,
								  HILITE_BUTTON_MODE_ACTION_MESSAGE,
								  0,
								  0,
								  0,
								  &m_handlerCount,
								  NETWORK_OPTIONS_BUTTON_MESSAGE_SPECIFIC_HOST);
	m_hiliteController->AddButton(m_layoutTable->m_framePos[2].m_x,
								  m_layoutTable->m_framePos[2].m_y,
								  animIds2,
								  HILITE_BUTTON_MODE_ACTION_MESSAGE,
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
	if (m_hiliteController != NULL) {
		delete m_hiliteController;
	}
}

// FUNCTION: LEMBALL 0x004535c0
CNetworkOptionsDrawer::~CNetworkOptionsDrawer()
{
	int index;
	CEditString* editor;

	if (m_returnState == FLOW_NONE) {
		if (g_pCurrentFrontendProcess != NULL) {
			Stop();
		}
	}
	else {
		if (g_pCurrentFrontendProcess != NULL) {
			((CNetworkOptionsProc*) g_pCurrentFrontendProcess)->StopBroadcast();
		}
	}
	index = 0;
	do {
		((CGWnd*) m_display)->m_hotAreaList->RemoveFromList(&m_playerEntries[index]);
		index = index + 1;
	} while (index < NETWORK_OPTIONS_PLAYER_ENTRY_COUNT);
	delete[] m_playerEntries;
	editor = m_editor;
	if (editor != NULL) {
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

// FUNCTION: LEMBALL 0x004536b0
void CNetworkOptionsDrawer::DrawFrame(int p_position)
{
	NetworkOptionsFramePos* pos = &m_layoutTable->m_framePos[p_position];
	CBaseFrontendDrawer::DrawFrame(CVSRect(pos[0].m_x, pos[0].m_y, pos[1].m_x, pos[1].m_y));
}

// FUNCTION: LEMBALL 0x004536f0
void CNetworkOptionsDrawer::DrawEntry(unsigned long p_index, int& p_value, int p_remap)
{
	char* gameName;
	char* peerName;
	char* addressStr;
	char trimmedPeerName[24];
	CResFONT* font;
	CRemap* remap;
	int len;

	if (g_pNetworkManager != NULL) {
		CConnect** connections = g_pNetworkManager->m_connections;
		if (g_pNetworkManager->m_gameMessages[p_index].m_valid != 0) {
			font = m_textManager->GetFont(m_chalkFontId);
			NetworkOptionsLayout* layout = m_layoutTable;
			CNetworkGameMessage* entries = g_pNetworkManager->m_gameMessages;
			CVSPoint namePosition((short) layout->m_headerNameX, (short) layout->m_playerListY);
			CVSPoint addressPosition((short) layout->m_headerIpX, (short) layout->m_playerListY);
			CVSPoint peerPosition((short) layout->m_headerComputerX, (short) layout->m_playerListY);
			CVSPoint& posName = namePosition;
			CVSPoint& posAddress = addressPosition;
			CVSPoint& posPeer = peerPosition;
			short yOffset = (short) layout->m_rowStride * (short) p_value;
			posName.m_y += yOffset;
			posAddress.m_y += yOffset;
			posPeer.m_y += yOffset;
			remap = NULL;
			if (p_remap != NETWORK_OPTIONS_REMAP_NONE) {
				remap = (CRemap*) m_remaps[p_remap];
			}
			gameName = entries[p_index].m_gameName;
			addressStr = connections[p_index]->m_destinationAddress->GetStr();
			peerName = entries[p_index].m_peerName;
			strncpy(trimmedPeerName, peerName, 0x14);
			len = 0x14;
			do {
				trimmedPeerName[len] = 0;
				short measuredWidth = font->GetSize(trimmedPeerName, TEXT_ADVANCE_X_POSITIVE).m_width;
				len--;
				if (m_layoutTable->m_peerNameWidth >= (int) measuredWidth) {
					break;
				}
			} while (1);

			posName.m_x -= font->GetSize(gameName, TEXT_ADVANCE_X_POSITIVE).m_width / 2;
			posAddress.m_x -= font->GetSize(addressStr, TEXT_ADVANCE_X_POSITIVE).m_width / 2;
			posPeer.m_x -= font->GetSize(peerName, TEXT_ADVANCE_X_POSITIVE).m_width / 2;

			m_textManager
				->DrawString(m_gdi, posName, CVSSize(), m_chalkFontId, gameName, TEXT_ADVANCE_X_POSITIVE, remap);
			m_textManager
				->DrawString(m_gdi, posAddress, CVSSize(), m_chalkFontId, addressStr, TEXT_ADVANCE_X_POSITIVE, remap);
			m_textManager
				->DrawString(m_gdi, posPeer, CVSSize(), m_chalkFontId, peerName, TEXT_ADVANCE_X_POSITIVE, remap);
			p_value++;
		}
	}
}

// FUNCTION: LEMBALL 0x00453940
void CNetworkOptionsDrawer::DrawText()
{
	int idx;
	int searchIndex;
	CVSPoint pos((short) m_layoutTable->m_editPos.m_x, (short) m_layoutTable->m_editPos.m_y);

	if (m_drawingBackBuffer != 0) {
		char* divider = g_szNetworkOptionsDividerIp;
		if (m_mode == FRONTEND_LAYOUT_STANDARD) {
			divider = g_szNetworkOptionsDividerLocal;
		}
		CResFONT* font = m_textManager->GetFont(m_chalkFontId);
		CVSPoint posDivider(0, (short) m_layoutTable->m_dividerY);
		CVSPoint posLabel((short) m_layoutTable->m_headerNameX, (short) m_layoutTable->m_headerY);
		CVSPoint posIp((short) m_layoutTable->m_headerIpX, (short) m_layoutTable->m_headerY);
		CVSPoint posComputer((short) m_layoutTable->m_headerComputerX, (short) m_layoutTable->m_headerY);
		posLabel.m_x -= font->GetSize(g_szNetworkOptionsHeaderName, TEXT_ADVANCE_X_POSITIVE).m_width / 2;
		m_textManager->DrawString(m_gdi,
								  posLabel,
								  CVSSize(),
								  m_chalkFontId,
								  g_szNetworkOptionsHeaderName,
								  TEXT_ADVANCE_X_POSITIVE,
								  NULL);

		posIp.m_x -= font->GetSize(g_szNetworkOptionsHeaderIp, TEXT_ADVANCE_X_POSITIVE).m_width / 2;
		m_textManager->DrawString(m_gdi,
								  posIp,
								  CVSSize(),
								  m_chalkFontId,
								  g_szNetworkOptionsHeaderIp,
								  TEXT_ADVANCE_X_POSITIVE,
								  NULL);

		posComputer.m_x -= font->GetSize(g_szNetworkOptionsHeaderComputer, TEXT_ADVANCE_X_POSITIVE).m_width / 2;
		m_textManager->DrawString(m_gdi,
								  posComputer,
								  CVSSize(),
								  m_chalkFontId,
								  g_szNetworkOptionsHeaderComputer,
								  TEXT_ADVANCE_X_POSITIVE,
								  NULL);

		short dividerWidth = font->GetSize(divider, TEXT_ADVANCE_X_POSITIVE).m_width;
		posDivider.m_x = (short) (((int) m_size.m_width - (int) dividerWidth) / 2);
		m_textManager->DrawString(m_gdi, posDivider, CVSSize(), m_chalkFontId, divider, TEXT_ADVANCE_X_POSITIVE, NULL);

		if (g_szNetworkGameName[0] != 0) {
			CVSPoint posMyName((short) m_layoutTable->m_headerNameX, (short) m_layoutTable->m_localPlayerY);
			CVSPoint posMyIp((short) m_layoutTable->m_headerIpX, (short) m_layoutTable->m_localPlayerY);
			CVSPoint posMyComputer((short) m_layoutTable->m_headerComputerX, (short) m_layoutTable->m_localPlayerY);
			posMyName.m_x -= font->GetSize(g_szNetworkGameName, TEXT_ADVANCE_X_POSITIVE).m_width / 2;
			m_textManager->DrawString(m_gdi,
									  posMyName,
									  CVSSize(),
									  m_chalkFontId,
									  g_szNetworkGameName,
									  0x20,
									  (CRemap*) m_remaps[0]);

			char* myIp = m_localAddressText;
			if (myIp != NULL && *myIp != 0) {
				posMyIp.m_x -= font->GetSize(myIp, TEXT_ADVANCE_X_POSITIVE).m_width / 2;
				m_textManager->DrawString(m_gdi,
										  posMyIp,
										  CVSSize(),
										  m_chalkFontId,
										  m_localAddressText,
										  0x20,
										  (CRemap*) m_remaps[0]);
			}

			char* myPeer = m_localComputerName;
			if (myPeer != NULL && *myPeer != 0) {
				char trimmed[21];
				strncpy(trimmed, myPeer, 0x14);
				int len = 0x14;
				do {
					trimmed[len--] = 0;
				} while (m_layoutTable->m_peerNameWidth < font->GetSize(trimmed, TEXT_ADVANCE_X_POSITIVE).m_width);

				CString lowerPeer(trimmed);
				lowerPeer.lower();
				posMyComputer.m_x -= font->GetSize(trimmed, TEXT_ADVANCE_X_POSITIVE).m_width / 2;
				m_textManager->DrawString(m_gdi,
										  posMyComputer,
										  CVSSize(),
										  m_chalkFontId,
										  lowerPeer,
										  0x20,
										  (CRemap*) m_remaps[0]);
			}
		}

		if (g_pNetworkManager != NULL) {
			int i;
			int row = 0;
			for (i = 0; i < NETWORK_OPTIONS_PLAYER_ENTRY_COUNT; i++) {
				DrawEntry(i, row, 1);
				if (row == NETWORK_OPTIONS_VISIBLE_PLAYER_ROW_COUNT) {
					break;
				}
			}
		}
	}
	else {
		if (m_message != NETWORK_OPTIONS_MESSAGE_NONE) {
			CVSPoint msgPos((short) m_layoutTable->m_messagePos.m_x, (short) m_layoutTable->m_messagePos.m_y);
			CString msgText = g_apNetworkOptionsMessages[m_message - 1];
			bool special = false;
			if (m_message == NETWORK_OPTIONS_MESSAGE_SEARCHING_FOR_HOST) {
				if (g_szNetworkBroadcastAddress[0] != 0) {
					msgText += g_szNetworkBroadcastAddress;
				}
				else {
					msgText = g_apNetworkOptionsMessages[NETWORK_OPTIONS_MESSAGE_SEARCHING_LOCAL_NETWORK - 1];
				}
				special = true;
			}
			CRemap* remap = NULL;
			if (m_message >= NETWORK_OPTIONS_MESSAGE_FIRST_ERROR) {
				remap = (CRemap*) m_remaps[3];
				special = true;
			}
			else if (special) {
				remap = (CRemap*) m_remaps[5];
			}
			if (m_redrawPending != 0 || !special) {
				CResFONT* font = m_textManager->GetFont(m_chalkFontId);
				msgPos.m_x -= font->GetSize(msgText.m_text, TEXT_ADVANCE_X_POSITIVE).m_width / 2;
				m_textManager
					->DrawString(m_gdi, msgPos, CVSSize(), m_chalkFontId, msgText, TEXT_ADVANCE_X_POSITIVE, remap);
			}
		}

		m_drawnMessage = m_message;
		if (g_pNetworkManager != NULL) {
			int row = 0;
			int fallbackHighlighted = NETWORK_OPTIONS_NO_PLAYER_INDEX;
			CNetworkGameMessage* messages = g_pNetworkManager->m_gameMessages;
			if (m_highlightedPlayer != NETWORK_OPTIONS_NO_PLAYER_INDEX &&
				m_playerEntries[m_highlightedPlayer].m_hoverState == 0) {
				searchIndex = 0;
				do {
					if (m_playerEntries[searchIndex].m_hoverState != 0) {
						m_highlightedPlayer = searchIndex;
						break;
					}
					searchIndex = searchIndex + 1;
				} while (searchIndex < NETWORK_OPTIONS_PLAYER_ENTRY_COUNT);
				if (searchIndex == NETWORK_OPTIONS_PLAYER_ENTRY_COUNT) {
					fallbackHighlighted = m_highlightedPlayer;
				}
			}
			idx = 0;
			for (; idx < NETWORK_OPTIONS_PLAYER_ENTRY_COUNT; idx++) {
				if (messages[idx].m_valid != 0) {
					int isAccepted = 0;
					int state = 1;
					if (m_playerEntries[idx].m_hoverState != 0 || fallbackHighlighted == idx) {
						isAccepted = 1;
					}
					if (m_acceptedPlayer != idx) {
						if (m_playerEntries[idx].m_activationState != 0 && m_redrawPending == 0) {
							state = 3;
						}
					}
					else if (m_playerEntries[idx].m_activationState == 0 || m_redrawPending != 0) {
						state = 3;
					}
					state += isAccepted;
					if (state != 1) {
						DrawEntry(idx, row, state);
					}
					else {
						row++;
					}
					if (row == NETWORK_OPTIONS_VISIBLE_PLAYER_ROW_COUNT) {
						break;
					}
				}
			}
		}

		if (m_editingActive != 0) {
			CString editText = m_editor->m_text;
			if (m_redrawPending == 0) {
				editText += g_szNetworkOptionsCursor;
			}
			if (editText.getlength() > 0) {
				CRemap* remap = (CRemap*) m_remaps[1];
				m_textManager
					->DrawString(m_gdi, pos, CVSSize(), m_chalkFontId, editText, TEXT_ADVANCE_X_POSITIVE, remap);
			}
		}
	}
}

// FUNCTION: LEMBALL 0x00454050
void CNetworkOptionsDrawer::DrawAnims()
{
}

// FUNCTION: LEMBALL 0x00454060
bool CNetworkOptionsDrawer::ProcessMessages(Message* p_message)
{
	bool handled;
	unsigned int code;

	if (m_startPending != 0 || m_message != m_drawnMessage) {
		return false;
	}

	switch ((int) p_message->m_type) {
	case MESSAGE_KEY_DOWN: {

		code = p_message->m_code;
		if (code == INPUT_KEY_SHIFT) {
			g_nNetworkOptionsShiftHeld = 1;
			g_nNetworkOptionsCapsOrShift |= 1;
			return true;
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
					m_pendingEvent = NETWORK_OPTIONS_MESSAGE_NONE;
					SetMessage(NETWORK_OPTIONS_MESSAGE_NETWORK_TYPE_PROMPT);
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
				return true;
			}
		}

		switch (p_message->m_code) {
		case INPUT_KEY_UP:
			if (HighlightPreviousEntry()) {
				g_pSoundView->PlayEffect(SFX_CHANGEOP);
				return true;
			}
			break;
		case INPUT_KEY_DOWN:
			if (HighlightNextEntry()) {
				g_pSoundView->PlayEffect(SFX_CHANGEOP);
				return true;
			}
			break;
		case INPUT_KEY_SPACE:
		case INPUT_KEY_ACTIVATE:
		case INPUT_KEY_RETURN:
			if (m_highlightedPlayer != NETWORK_OPTIONS_NO_PLAYER_INDEX) {
				CVSPoint pt;
				m_playerEntries[m_highlightedPlayer].OnButtonDown(pt, MOUSE_BUTTON_INDEX_LEFT);
				return true;
			}
			break;
		}
	}
	case MESSAGE_KEY_UP:
		code = p_message->m_code;
		if (code == INPUT_KEY_SHIFT) {
			g_nNetworkOptionsCapsOrShift &= ~1;
			g_nNetworkOptionsShiftHeld = 0;
			return true;
		}
		if (m_editingActive != 0) {
			if (code >= INPUT_KEY_A && code <= INPUT_KEY_Z) {
				return true;
			}
			switch (code) {
			case INPUT_KEY_UP:
			case INPUT_KEY_SPACE:
			case INPUT_KEY_RETURN:
			case INPUT_KEY_DELETE:
			case INPUT_KEY_BACKSPACE:
				return true;
			}
		}
		return false;
	case MESSAGE_BUTTON_RELEASED:
		switch ((unsigned int) p_message->m_code) {
		case NETWORK_OPTIONS_BUTTON_MESSAGE_LAN:
			if (m_locked == 0) {
				Start(NETWORK_OPTIONS_MODE_LAN);
			}
			break;
		case NETWORK_OPTIONS_BUTTON_MESSAGE_SPECIFIC_HOST:
			if (m_locked == 0) {
				Start(NETWORK_OPTIONS_MODE_SPECIFIC_HOST);
			}
			break;
		case NETWORK_OPTIONS_BUTTON_MESSAGE_RETURN:
			Stop();
			m_quitYet = 1;
			m_returnState = FLOW_MAIN_OPTIONS_1;
			return true;
		}
		return false;
	default:
		m_processedCount++;
		return false;
	}
}

// FUNCTION: LEMBALL 0x00454520
void CNetworkOptionsDrawer::Start(unsigned int p_mode)
{
	unsigned int mode;

	if (m_editingActive != 0) {
		mode = m_networkMode;
		if (mode != NETWORK_OPTIONS_MODE_LAN) {
			if (p_mode != NETWORK_OPTIONS_MODE_LAN) {
				goto stop_editing;
			}
		}
		if (mode == NETWORK_OPTIONS_MODE_LAN && p_mode == NETWORK_OPTIONS_MODE_LAN) {
		stop_editing:
			StopEditing();
			return;
		}
	}
	m_networkMode = p_mode;
	m_broadcasting = 0;
	m_editingActive = 0;
	m_pendingEvent = NETWORK_OPTIONS_MESSAGE_NONE;
	((CNetworkOptionsProc*) g_pCurrentFrontendProcess)->StopBroadcast();
	if (g_szNetworkGameName[0] != 0) {
		*m_editor = g_szNetworkGameName;
		StartEditing(NETWORK_OPTIONS_EDIT_GAME_NAME, 0);
		return;
	}
	StartEditing(NETWORK_OPTIONS_EDIT_GAME_NAME, 1);
}

// FUNCTION: LEMBALL 0x004545c0
void CNetworkOptionsDrawer::StartBroadcast()
{
	m_broadcasting = 1;
	((CNetworkOptionsProc*) g_pCurrentFrontendProcess)->Start();
	if (((CNetworkOptionsProc*) g_pCurrentFrontendProcess)->m_started != 0 &&
		((CNetworkOptionsProc*) g_pCurrentFrontendProcess)->m_startFailed == 0) {
		g_pNetworkManager->StartBroadcast(g_szNetworkBroadcastAddress);
		SetMessage(NETWORK_OPTIONS_MESSAGE_SEARCHING_FOR_HOST);
		return;
	}
	StartMessageTimeout(NETWORK_OPTIONS_MESSAGE_NETWORK_UNAVAILABLE, 6000);
}

// FUNCTION: LEMBALL 0x00454620
void CNetworkOptionsDrawer::Stop()
{
	if (((CNetworkOptionsProc*) g_pCurrentFrontendProcess)->m_started != 0 &&
		((CNetworkOptionsProc*) g_pCurrentFrontendProcess)->m_startFailed == 0) {
		((CNetworkOptionsProc*) g_pCurrentFrontendProcess)->Stop();
	}
	SetMessage(NETWORK_OPTIONS_MESSAGE_NONE);
	m_editingActive = 0;
}

// FUNCTION: LEMBALL 0x00454650
void CNetworkOptionsDrawer::SetMessage(eNetOptsMessages p_message)
{
	unsigned long now;

	m_message = p_message;
	m_messageDuration = 0;
	m_backBufferNeeded = 1;
	now = CurrentMilliTimer();
	m_redrawPending = 1;
	m_lastDrawTime = now;
}

// FUNCTION: LEMBALL 0x00454690
void CNetworkOptionsDrawer::StartEditing(eEditingStage p_stage, unsigned int p_clear)
{
	eEditingStage stage;
	CEditString* editor;

	stage = p_stage;
	m_editingActive = 0;
	if (stage != NETWORK_OPTIONS_EDIT_KEEP_CURRENT) {
		m_editingStage = stage;
	}
	if (p_clear != 0) {
		editor = m_editor;
		editor->m_length = 0;
		editor->m_text[0] = 0;
	}
	if (m_editingStage == NETWORK_OPTIONS_EDIT_BROADCAST_ADDRESS) {
		((CNetworkOptionsProc*) g_pCurrentFrontendProcess)->Start();
		if (((CNetworkOptionsProc*) g_pCurrentFrontendProcess)->m_started == 0 ||
			((CNetworkOptionsProc*) g_pCurrentFrontendProcess)->m_startFailed != 0) {
			StartMessageTimeout(NETWORK_OPTIONS_MESSAGE_NETWORK_UNAVAILABLE, 6000);
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
	eEditingStage stage;
	char* text;

	stage = m_editingStage;
	m_editingActive = 0;
	switch (stage) {
	case NETWORK_OPTIONS_EDIT_GAME_NAME:
		text = m_editor->m_text;
		if (*text == 0) {
			StartMessageTimeout(NETWORK_OPTIONS_MESSAGE_NAME_REQUIRED, 6000);
			return;
		}
		strcpy(g_szNetworkGameName, text);
		if (m_networkMode != NETWORK_OPTIONS_MODE_LAN) {
			m_pendingStage = NETWORK_OPTIONS_EDIT_BROADCAST_ADDRESS;
		}
		else {
			g_szNetworkBroadcastAddress[0] = 0;
			m_startPending = 1;
		}
		if (((CNetworkOptionsProc*) g_pCurrentFrontendProcess)->m_started == 0 ||
			((CNetworkOptionsProc*) g_pCurrentFrontendProcess)->m_startFailed != 0) {
			SetMessage(NETWORK_OPTIONS_MESSAGE_INITIALISING_NETWORK);
			return;
		}
		break;
	case NETWORK_OPTIONS_EDIT_BROADCAST_ADDRESS:
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
	m_pendingEvent = NETWORK_OPTIONS_MESSAGE_NONE;
}

// FUNCTION: LEMBALL 0x00454870
void CNetworkOptionsDrawer::StartMessageTimeout(eNetOptsMessages p_message, unsigned long p_duration)
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

// FUNCTION: LEMBALL 0x004548c0
void CNetworkOptionsDrawer::Processing()
{
	unsigned long now;
	unsigned long duration;
	char* ident;
	char* peer;
	CConnect** current;
	CConnect** connections;
	int index;
	int activation;
	int acceptedPlayer;

	if (m_drawnMessage != m_message) {
		return;
	}
	if (m_startPending != 0) {
		StartBroadcast();
		m_startPending = 0;
	}
	if (m_pendingStage != NETWORK_OPTIONS_EDIT_NONE) {
		StartEditing(m_pendingStage, 1);
		m_pendingStage = NETWORK_OPTIONS_EDIT_NONE;
	}
	if (m_pendingEvent != NETWORK_OPTIONS_MESSAGE_NONE) {
		LastError();
	}
	now = CurrentMilliTimer();
	if (now - m_lastDrawTime >= NETWORK_OPTIONS_REDRAW_INTERVAL_MS) {
		m_redrawPending = m_redrawPending == 0;
		now = CurrentMilliTimer();
		m_lastDrawTime = now;
	}
	if (g_pNetworkManager != NULL) {
		ident = g_pBroadcastAddress->GetStr();
		peer = g_szBroadcastPeerName;
		if (m_localAddressText != ident) {
			m_backBufferNeeded = 1;
			m_localAddressText = ident;
		}
		if (m_localComputerName != peer) {
			m_backBufferNeeded = 1;
			m_localComputerName = peer;
		}
		if (m_networkState == NETWORK_OPTIONS_HANDLERS_CURRENT) {
			if (g_pNetworkManager->m_connectionsChanged != 0) {
				g_pNetworkManager->m_connectionsChanged = 0;
				m_networkState = NETWORK_OPTIONS_HANDLERS_CURRENT;
				m_backBufferNeeded = 1;
				InitialiseHandlers();
			}
		}
		else {
			m_networkState = NETWORK_OPTIONS_HANDLERS_CURRENT;
			m_backBufferNeeded = 1;
			InitialiseHandlers();
		}
		connections = g_pNetworkManager->m_connections;
		current = connections;
		index = 0;
		do {
			if (m_playerEntries[index].m_pressed != 0 && m_acceptedPlayer != index) {
				g_pSoundView->PlayEffect(SFX_DRUM1);
				acceptedPlayer = m_acceptedPlayer;
				if (acceptedPlayer != NETWORK_OPTIONS_NO_PLAYER_INDEX) {
					CConnect* connection = connections[acceptedPlayer];
					if (connection != NULL) {
						((CNetworkOptionsProc*) g_pCurrentFrontendProcess)->Reject(connection);
					}
				}
				m_acceptedPlayer = index;
				if (*current != NULL) {
					activation = m_playerEntries[index].m_activationState;
					if (activation != 0) {
						Lock();
					}
					((CNetworkOptionsProc*) g_pCurrentFrontendProcess)->Accept(*current, activation);
				}
			}
			m_playerEntries[index].m_pressed = 0;
			current = current + 1;
			index = index + 1;
		} while (index < 10);
	}
	if (m_message != NETWORK_OPTIONS_MESSAGE_NONE) {
		duration = m_messageDuration;
		if (duration != 0) {
			now = CurrentMilliTimer();
			if (now - m_messageStartTime > duration) {
				m_message = NETWORK_OPTIONS_MESSAGE_NETWORK_TYPE_PROMPT;
				m_backBufferNeeded = 1;
				m_messageDuration = 0;
			}
		}
	}
}

// FUNCTION: LEMBALL 0x00454ad0
void CNetworkOptionsDrawer::RegisterRemaps()
{
	CBaseRemap** remaps;
	unsigned char** mappings;

	remaps = m_remaps;
	mappings = g_apNetworkOptionsRemaps;
	do {
		mappings++;
		remaps++;
		*(remaps - 1) =
			g_pBasePalManager->RegisterRemap(m_display->m_paletteResourceId, *(mappings - 1), PALETTE_MAPPED);
	} while (mappings < g_apNetworkOptionsRemaps + 6);
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
	if (p_index != NETWORK_OPTIONS_NO_PLAYER_INDEX && p_index < NETWORK_OPTIONS_PLAYER_ENTRY_COUNT) {
		m_playerEntries[p_index].m_activationState = 1;
	}
}

// FUNCTION: LEMBALL 0x00454b70
void CNetworkOptionsDrawer::GameNotReady(int p_index)
{
	if (p_index != NETWORK_OPTIONS_NO_PLAYER_INDEX && p_index < NETWORK_OPTIONS_PLAYER_ENTRY_COUNT &&
		(m_playerEntries[p_index].m_activationState = 0, m_acceptedPlayer == p_index)) {
		UnLock();
	}
}

// FUNCTION: LEMBALL 0x00454bb0
void CNetworkOptionsDrawer::UpdateHighlightedEntry()
{
	if (m_highlightedPlayer != NETWORK_OPTIONS_NO_PLAYER_INDEX) {
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
		return false;
	}

	if (m_highlightedPlayer == NETWORK_OPTIONS_NO_PLAYER_INDEX) {
		m_hiliteController->m_active = 0;
		m_highlightedPlayer = NETWORK_OPTIONS_PLAYER_ENTRY_COUNT;
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
		return false;
	}

	m_highlightedPlayer = selected;
	i = 0;
	do {
		m_playerEntries[i].m_entered = 0;
		m_playerEntries[i].OnExit();
		++i;
	} while (i < NETWORK_OPTIONS_PLAYER_ENTRY_COUNT);

	m_playerEntries[m_highlightedPlayer].m_entered = 1;
	m_playerEntries[m_highlightedPlayer].OnEnter();
	return true;
}

// FUNCTION: LEMBALL 0x00454cf0
bool CNetworkOptionsDrawer::HighlightNextEntry()
{
	int selected;
	int i;

	if (m_visibleEntryCount == 0) {
		if (m_highlightedPlayer == NETWORK_OPTIONS_NO_PLAYER_INDEX) {
			return false;
		}
		m_hiliteController->m_active = 1;
		m_highlightedPlayer = NETWORK_OPTIONS_NO_PLAYER_INDEX;
	}

	if (m_highlightedPlayer == NETWORK_OPTIONS_NO_PLAYER_INDEX) {
		return false;
	}

	selected = m_highlightedPlayer + 1;
	if (selected < NETWORK_OPTIONS_PLAYER_ENTRY_COUNT) {
		do {
			if (m_playerEntries[selected].m_active != 0) {
				break;
			}
			++selected;
		} while (selected < NETWORK_OPTIONS_PLAYER_ENTRY_COUNT);
	}

	m_highlightedPlayer = selected;
	i = 0;
	do {
		m_playerEntries[i].m_entered = 0;
		m_playerEntries[i].OnExit();
		++i;
	} while (i < NETWORK_OPTIONS_PLAYER_ENTRY_COUNT);

	if (m_highlightedPlayer == NETWORK_OPTIONS_PLAYER_ENTRY_COUNT) {
		m_hiliteController->m_active = 1;
		m_highlightedPlayer = NETWORK_OPTIONS_NO_PLAYER_INDEX;
		return true;
	}

	m_playerEntries[m_highlightedPlayer].m_entered = 1;
	m_playerEntries[m_highlightedPlayer].OnEnter();
	return true;
}

// FUNCTION: LEMBALL 0x00454df0
void CNetworkOptionsDrawer::InitialiseHandlers()
{
	CConnect** connections;
	CNetworkGameMessage* messages;
	int index;

	connections = NULL;
	messages = NULL;
	if (g_pNetworkManager != NULL) {
		connections = g_pNetworkManager->m_connections;
		messages = g_pNetworkManager->m_gameMessages;
	}
	CVSRect rect(m_layoutTable->m_entryX,
				 m_layoutTable->m_entryY,
				 (short) m_layoutTable->m_entryWidth,
				 m_layoutTable->m_entryHeight);
	short& rowY = rect.m_y;
	index = 0;
	m_visibleEntryCount = 0;
	do {
		if (m_visibleEntryCount < 4 && connections != NULL && connections[index] != NULL &&
			messages[index].m_valid != 0) {
			CEntryHandler* entry = &m_playerEntries[index];
			entry->m_bounds.m_width = rect.m_width;
			entry->m_bounds.m_height = rect.m_height;
			entry->m_bounds.m_x = rect.m_x;
			entry->m_bounds.m_y = rowY;
			entry->SetActive(1);
			rowY += (short) m_layoutTable->m_rowStride;
			m_visibleEntryCount++;
		}
		else {
			m_playerEntries[index].SetActive(0);
		}
		index++;
	} while (index < NETWORK_OPTIONS_PLAYER_ENTRY_COUNT);
	UpdateHighlightedEntry();
}

// FUNCTION: LEMBALL 0x00454f00
void CNetworkOptionsDrawer::ResetHandlers()
{
	CConnect** connections;
	unsigned int* valid;
	int index;

	if (g_pNetworkManager != NULL) {
		connections = g_pNetworkManager->m_connections;
		index = 0;
		valid = &g_pNetworkManager->m_gameMessages->m_valid;
		do {
			if (*connections == NULL || *valid == 0) {
				m_playerEntries[index].Reset();
				if (m_acceptedPlayer == index) {
					m_acceptedPlayer = NETWORK_OPTIONS_NO_PLAYER_INDEX;
				}
			}
			valid = (unsigned int*) ((char*) valid + sizeof(CNetworkGameMessage));
			connections++;
			index++;
		} while (index < NETWORK_OPTIONS_PLAYER_ENTRY_COUNT);
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
	} while (i < NETWORK_OPTIONS_PLAYER_ENTRY_COUNT);
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
	if (g_pNetworkManager != NULL) {
		connections = g_pNetworkManager->m_connections;
		if (m_acceptedPlayer != NETWORK_OPTIONS_NO_PLAYER_INDEX && connections[m_acceptedPlayer] != NULL) {
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
	return false;
}
