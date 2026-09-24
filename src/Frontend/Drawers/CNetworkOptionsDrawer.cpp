#include "CNetworkOptionsDrawer.h"

#include "../../Control/Game/CGameStatus.h"
#include "../../Network/Game/CNetworkManager.h"
#include "../../Network/Messages/CNetworkGameMessage.h"
#include "../../Views/Display/CMain2DDisplay.h"
#include "../../Views/Sound/CSoundView.h"
#include "../../Visos/Foundation/CString.h"
#include "../../Visos/Foundation/CTextManager.h"
#include "../../Visos/Foundation/VsTime.h"
#include "../../Visos/Graphics/CBasePalManager.h"
#include "../../Visos/Graphics/CGWnd.h"
#include "../../Visos/Graphics/CHotAreaList.h"
#include "../../Visos/Network/CConnect.h"
#include "../../Visos/Network/CNetworkAddress.h"
#include "../../Visos/Resources/CResFONT.h"
#include "../../Visos/Resources/Manifest.h"
#include "../Controls/CHiliteController.h"
#include "../Processes/CNetworkOptionsProc.h"
#include "../Support/CEditString.h"
#include "Frontend/Base/CBaseFrontendDrawer.h"
#include "Frontend/Base/CBaseFrontendProcess.h"
#include "Frontend/Base/FlowProcesses.h"
#include "Frontend/Support/CEntryHandler.h"
#include "Views/Sound/SoundEffects.h"
#include "Visos/Foundation/CVsPoint.h"
#include "Visos/Foundation/CVsRect.h"
#include "Visos/Foundation/CVsSize.h"
#include "Visos/Foundation/Message.h"
#include "Visos/Graphics/CBaseRemap.h"

#include <string.h>

class CRemap;

#pragma intrinsic(strcpy)

extern "C" unsigned long __stdcall timeGetTime(void);

extern char* g_szBroadcastPeerName;

// GLOBAL: LEMBALL 0x004a0180
unsigned char g_abNetworkOptionsLayoutIp[0xa0] = {
	0x3c, 0x00, 0x00, 0x00, 0x77, 0x01, 0x00, 0x00, 0x02, 0x01, 0x00, 0x00, 0x77, 0x01, 0x00, 0x00, 0xc7, 0x01,
	0x00, 0x00, 0x77, 0x01, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00, 0x70, 0x02, 0x00, 0x00,
	0xc0, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0xd5, 0x00, 0x00, 0x00, 0x70, 0x02, 0x00, 0x00, 0x30, 0x00,
	0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x0f, 0x01, 0x00, 0x00, 0x70, 0x02, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00,
	0x38, 0x01, 0x00, 0x00, 0x1b, 0x01, 0x00, 0x00, 0x22, 0x00, 0x00, 0x00, 0x15, 0x00, 0x00, 0x00, 0x19, 0x00,
	0x00, 0x00, 0x3e, 0x01, 0x00, 0x00, 0x4c, 0x00, 0x00, 0x00, 0xde, 0x00, 0x00, 0x00, 0x61, 0x00, 0x00, 0x00,
	0x43, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x46, 0x00, 0x00, 0x00, 0x09, 0x01, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0xf4, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00,
	0x61, 0x02, 0x00, 0x00, 0x1d, 0x00, 0x00, 0x00, 0x0f, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

// GLOBAL: LEMBALL 0x004a0220
unsigned char g_abNetworkOptionsLayoutLocal[0xa0] = {
	0x1e, 0x00, 0x00, 0x00, 0xba, 0x00, 0x00, 0x00, 0x81, 0x00, 0x00, 0x00, 0xba, 0x00, 0x00, 0x00, 0xe3, 0x00,
	0x00, 0x00, 0xba, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x30, 0x01, 0x00, 0x00,
	0x60, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x65, 0x00, 0x00, 0x00, 0x30, 0x01, 0x00, 0x00, 0x1e, 0x00,
	0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x87, 0x00, 0x00, 0x00, 0x30, 0x01, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00,
	0x9f, 0x00, 0x00, 0x00, 0x8f, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x13, 0x00,
	0x00, 0x00, 0x9e, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x6d, 0x00, 0x00, 0x00, 0x31, 0x00, 0x00, 0x00,
	0x22, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x22, 0x00, 0x00, 0x00, 0x81, 0x00, 0x00, 0x00, 0x02, 0x00,
	0x00, 0x00, 0xf4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x00,
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
char* g_apNetworkOptionsMessages[9] = {
	g_szNetworkOptionsMsg1,
	g_szNetworkOptionsMsg2,
	g_szNetworkOptionsMsg3,
	g_szNetworkOptionsMsg4,
	g_szNetworkOptionsMsg5,
	g_szNetworkOptionsMsg6,
	g_szNetworkOptionsMsg7,
	g_szNetworkOptionsMsg8,
	g_szNetworkOptionsMsg9,
};

// GLOBAL: LEMBALL 0x004a0344
int g_anNetworkOptionsEditMessages[4] = {(int) g_szNetworkOptionsMsg10, 2, 3, 0};

// GLOBAL: LEMBALL 0x004a0354
int g_anNetworkOptionsEditMaxLength[4] = {0, 8, 0x14, 0};

// GLOBAL: LEMBALL 0x004a0368
char g_szNetworkGameName[16];

// GLOBAL: LEMBALL 0x004a0378
char g_szNetworkBroadcastAddress[16];

// GLOBAL: LEMBALL 0x004a0390
int g_nNetworkOptionsShiftHeld = 0;

// GLOBAL: LEMBALL 0x004a0394
int g_nNetworkOptionsCapsOrShift = 0;

// FUNCTION: LEMBALL 0x00453280
CNetworkOptionsDrawer::CNetworkOptionsDrawer(CMain2DDisplay* p_display, CGDI* p_gdi, const CVsRect& p_rect)
	: CBaseFrontendDrawer(p_display, p_gdi, p_rect, FLOW_NETWORK_OPTIONS, 0x32, 200, 0, 100, 0x28)
{
	int playerEntryIndex;

	m_editingActive = 0;
	m_message = 1;
	m_messageDirty = 1;
	m_messageDuration = 0;
	m_pendingEvent = 0;
	m_broadcasting = 0;
	m_networkState = 0;
	m_redrawPending = 0;
	m_lastDrawTime = CurrentMilliTimer();
	m_stopPending = 0;
	m_connectionState = 0;
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
		m_layoutTable = (NetworkOptionsLayout*) g_abNetworkOptionsLayoutLocal;
	}
	else {
		animIds2 = &g_anNetworkOptionsAnimIds[2];
		animIds1 = &g_anNetworkOptionsAnimIds[1];
		animIds0 = &g_anNetworkOptionsAnimIds[0];
		m_layoutTable = (NetworkOptionsLayout*) g_abNetworkOptionsLayoutIp;
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
								  0xacef000c);
	m_hiliteController->AddButton(m_layoutTable->m_framePos[1].m_x,
								  m_layoutTable->m_framePos[1].m_y,
								  animIds1,
								  1,
								  0,
								  0,
								  0,
								  &m_handlerCount,
								  0xacef000d);
	m_hiliteController->AddButton(m_layoutTable->m_framePos[2].m_x,
								  m_layoutTable->m_framePos[2].m_y,
								  animIds2,
								  1,
								  0,
								  0,
								  0,
								  &m_handlerCount,
								  0xacef000e);
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

// FUNCTION: LEMBALL 0x004536b0
void CNetworkOptionsDrawer::DrawFrame(int p_position)
{
	NetworkOptionsFramePos* pos = &m_layoutTable->m_framePos[p_position];
	CBaseFrontendDrawer::DrawFrame(CVsRect(pos[0].m_x, pos[0].m_y, pos[1].m_x, pos[1].m_y));
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

	if (g_pNetworkManager != 0) {
		CConnect** connections = g_pNetworkManager->m_connections;
		if (g_pNetworkManager->m_gameMessages[p_index].m_valid != 0) {
			font = m_textManager->GetFont(m_chalkFontId);
			NetworkOptionsLayout* layout = m_layoutTable;
			CNetworkGameMessage* entries = g_pNetworkManager->m_gameMessages;
			CVsPoint posName((short) layout->m_headerNameX, (short) layout->m_playerListY);
			CVsPoint posAddress((short) layout->m_headerIpX, (short) layout->m_playerListY);
			CVsPoint posPeer((short) layout->m_headerComputerX, (short) layout->m_playerListY);
			remap = 0;
			short yOffset = (short) layout->m_rowStride * (short) p_value;
			posName.m_y += yOffset;
			posAddress.m_y += yOffset;
			posPeer.m_y += yOffset;
			if (p_remap != 6) {
				remap = (CRemap*) m_remaps[p_remap];
			}
			gameName = entries[p_index].m_gameName;
			peerName = entries[p_index].m_peerName;
			addressStr = connections[p_index]->m_destinationAddress->GetStr();
			strncpy(trimmedPeerName, peerName, 0x14);
			len = 0x14;
			do {
				trimmedPeerName[len] = 0;
				short measuredWidth = font->GetSize(trimmedPeerName, 0x20).m_width;
				len--;
				if (m_layoutTable->m_peerNameWidth >= (int) measuredWidth) {
					break;
				}
			} while (1);

			posName.m_x -= font->GetSize(gameName, 0x20).m_width / 2;
			posAddress.m_x -= font->GetSize(addressStr, 0x20).m_width / 2;
			posPeer.m_x -= font->GetSize(peerName, 0x20).m_width / 2;

			m_textManager->DrawString(m_gdi, posName, CVsSize(), m_chalkFontId, gameName, 0x20, remap);
			m_textManager->DrawString(m_gdi, posAddress, CVsSize(), m_chalkFontId, addressStr, 0x20, remap);
			m_textManager->DrawString(m_gdi, posPeer, CVsSize(), m_chalkFontId, peerName, 0x20, remap);
			p_value++;
		}
	}
}

// FUNCTION: LEMBALL 0x00453940
void CNetworkOptionsDrawer::DrawText()
{
	CVsPoint pos((short) m_layoutTable->m_editPos.m_x, (short) m_layoutTable->m_editPos.m_y);

	if (m_drawingBackBuffer != 0) {
		char* divider = g_szNetworkOptionsDividerIp;
		if (m_mode == 0) {
			divider = g_szNetworkOptionsDividerLocal;
		}
		CResFONT* font = m_textManager->GetFont(m_chalkFontId);
		CVsPoint posDivider(0, (short) m_layoutTable->m_dividerY);
		CVsPoint posLabel((short) m_layoutTable->m_headerNameX, (short) m_layoutTable->m_headerY);
		CVsPoint posIp((short) m_layoutTable->m_headerIpX, (short) m_layoutTable->m_headerY);
		CVsPoint posComputer((short) m_layoutTable->m_headerComputerX, (short) m_layoutTable->m_headerY);
		posLabel.m_x -= font->GetSize(g_szNetworkOptionsHeaderName, 0x20).m_width / 2;
		m_textManager->DrawString(m_gdi, posLabel, CVsSize(), m_chalkFontId, g_szNetworkOptionsHeaderName, 0x20, 0);

		posIp.m_x -= font->GetSize(g_szNetworkOptionsHeaderIp, 0x20).m_width / 2;
		m_textManager->DrawString(m_gdi, posIp, CVsSize(), m_chalkFontId, g_szNetworkOptionsHeaderIp, 0x20, 0);

		posComputer.m_x -= font->GetSize(g_szNetworkOptionsHeaderComputer, 0x20).m_width / 2;
		m_textManager
			->DrawString(m_gdi, posComputer, CVsSize(), m_chalkFontId, g_szNetworkOptionsHeaderComputer, 0x20, 0);

		posDivider.m_x = (short) (((int) m_width - (int) font->GetSize(divider, 0x20).m_width) / 2);
		m_textManager->DrawString(m_gdi, posDivider, CVsSize(), m_chalkFontId, divider, 0x20, 0);

		if (g_szNetworkGameName[0] != 0) {
			CVsPoint posMyName((short) m_layoutTable->m_headerNameX, (short) m_layoutTable->m_localPlayerY);
			CVsPoint posMyIp((short) m_layoutTable->m_headerIpX, (short) m_layoutTable->m_localPlayerY);
			CVsPoint posMyComputer((short) m_layoutTable->m_headerComputerX, (short) m_layoutTable->m_localPlayerY);
			posMyName.m_x -= font->GetSize(g_szNetworkGameName, 0x20).m_width / 2;
			m_textManager->DrawString(m_gdi,
									  posMyName,
									  CVsSize(),
									  m_chalkFontId,
									  g_szNetworkGameName,
									  0x20,
									  (CRemap*) m_remaps[0]);

			char* myIp = m_stopPending;
			if (myIp != 0 && *myIp != 0) {
				posMyIp.m_x -= font->GetSize(myIp, 0x20).m_width / 2;
				m_textManager
					->DrawString(m_gdi, posMyIp, CVsSize(), m_chalkFontId, m_stopPending, 0x20, (CRemap*) m_remaps[0]);
			}

			char* myPeer = m_connectionState;
			if (myPeer != 0 && *myPeer != 0) {
				char trimmed[21];
				int len = 0x14;
				strncpy(trimmed, myPeer, 0x14);
				do {
					trimmed[len] = 0;
					len--;
				} while (m_layoutTable->m_peerNameWidth < font->GetSize(trimmed, 0x20).m_width);

				CString lowerPeer(trimmed);
				lowerPeer.lower();
				posMyComputer.m_x -= font->GetSize(trimmed, 0x20).m_width / 2;
				m_textManager->DrawString(m_gdi,
										  posMyComputer,
										  CVsSize(),
										  m_chalkFontId,
										  lowerPeer,
										  0x20,
										  (CRemap*) m_remaps[0]);
			}
		}

		if (g_pNetworkManager != 0) {
			int i;
			int row = 0;
			for (i = 0; i < 10; i++) {
				DrawEntry(i, row, 1);
				if (row == 4) {
					break;
				}
			}
		}
	}
	else {
		if (m_message != 0) {
			CVsPoint msgPos((short) m_layoutTable->m_messagePos.m_x, (short) m_layoutTable->m_messagePos.m_y);
			CString msgText = g_apNetworkOptionsMessages[m_message];
			bool special = false;
			if (m_message == 4) {
				if (g_szNetworkBroadcastAddress[0] != 0) {
					msgText += g_szNetworkBroadcastAddress;
				}
				else {
					msgText = g_apNetworkOptionsMessages[4];
				}
				special = true;
			}
			CRemap* remap = 0;
			if (m_message >= 7) {
				remap = (CRemap*) m_remaps[3];
				special = true;
			}
			else if (special) {
				remap = (CRemap*) m_remaps[5];
			}
			if (m_redrawPending != 0 || !special) {
				CResFONT* font = m_textManager->GetFont(m_chalkFontId);
				msgPos.m_x -= font->GetSize(msgText.m_text, 0x20).m_width / 2;
				m_textManager->DrawString(m_gdi, msgPos, CVsSize(), m_chalkFontId, msgText, 0x20, remap);
			}
		}

		m_messageDirty = m_message;
		if (g_pNetworkManager != 0) {
			int searchIndex;
			int row = 0;
			int fallbackHighlighted = -1;
			CNetworkGameMessage* messages = g_pNetworkManager->m_gameMessages;
			if (m_highlightedPlayer != -1 && m_playerEntries[m_highlightedPlayer].m_hoverState == 0) {
				searchIndex = 0;
				do {
					if (m_playerEntries[searchIndex].m_hoverState != 0) {
						m_highlightedPlayer = searchIndex;
						break;
					}
					searchIndex = searchIndex + 1;
				} while (searchIndex < 10);
				if (searchIndex == 10) {
					fallbackHighlighted = m_highlightedPlayer;
				}
			}
			unsigned int* valid;
			int idx = 0;
			valid = &messages->m_valid;
			for (; idx < 10; valid += sizeof(CNetworkGameMessage) / sizeof(*valid), idx++) {
				if (*valid != 0) {
					int state = 1;
					int isAccepted = 0;
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
					if (row == 4) {
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
				m_textManager->DrawString(m_gdi, pos, CVsSize(), m_chalkFontId, editText, 0x20, remap);
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

	if (m_startPending != 0 || (unsigned int) m_message != m_messageDirty) {
		return 0;
	}

	switch ((int) p_message->type) {
	case 4: {

		code = p_message->code;
		if (code == 0x49) {
			g_nNetworkOptionsShiftHeld = 1;
			g_nNetworkOptionsCapsOrShift |= 1;
			return 1;
		}

		if (m_editingActive != 0) {
			handled = false;
			if (code >= 5 && code <= 0x1e) {
				if (m_editor->m_length != m_editor->m_maxLength) {
					char offset = ((g_nNetworkOptionsShiftHeld == 0) ? 0xe0 : 0) - 0x3c;
					char ch = (char) code - offset;
					*m_editor += ch;
					handled = true;
				}
				else {
					g_pSoundView->PlayEffect(SFX_CHINK);
				}
			}
			else if (code >= 0x39 && code <= 0x42) {
				if (m_editor->m_length != m_editor->m_maxLength) {
					*m_editor += (char) (code - 9);
					handled = true;
				}
				else {
					g_pSoundView->PlayEffect(SFX_CHINK);
				}
			}
			else {
				switch (code) {
				case 0x1f:
					if (m_editor->m_length != m_editor->m_maxLength) {
						*m_editor += ' ';
						handled = true;
					}
					else {
						g_pSoundView->PlayEffect(SFX_CHINK);
					}
					break;
				case 0x20:
					if (m_editor->m_length != m_editor->m_maxLength) {
						*m_editor += '.';
						handled = true;
					}
					else {
						g_pSoundView->PlayEffect(SFX_CHINK);
					}
					break;
				case 0x23:
					m_broadcasting = 0;
					m_editingActive = 0;
					m_pendingEvent = 0;
					SetMessage(1);
					handled = true;
					break;
				case 0x4c:
					StopEditing();
					handled = true;
					break;
				case 0x4d:
				case 0x4e: {
					CEditString* editor = m_editor;
					if (editor->m_length != 0) {
						if (editor->m_length > 0) {
							editor->m_length--;
							editor->m_text[editor->m_length] = 0;
						}
						handled = true;
					}
					else {
						g_pSoundView->PlayEffect(SFX_CHINK);
					}
					break;
				}
				}
			}

			if (handled) {
				m_lastDrawTime = CurrentMilliTimer();
				m_redrawPending = 0;
				g_pSoundView->PlayEffect(SFX_DRUM1);
				return 1;
			}
		}

		switch (p_message->code) {
		case 1:
			if (HighlightPreviousEntry()) {
				g_pSoundView->PlayEffect(SFX_CHANGEOP);
				return 1;
			}
			break;
		case 2:
			if (HighlightNextEntry()) {
				g_pSoundView->PlayEffect(SFX_CHANGEOP);
				return 1;
			}
			break;
		case 0x1f:
		case 0x22:
		case 0x4c:
			if (m_highlightedPlayer != -1) {
				CVsPoint pt;
				m_playerEntries[m_highlightedPlayer].OnButtonDown(pt, 0);
				return 1;
			}
			break;
		}
	}
	case 3:
		code = p_message->code;
		if (code == 0x49) {
			g_nNetworkOptionsCapsOrShift &= ~1;
			g_nNetworkOptionsShiftHeld = 0;
			return 1;
		}
		if (m_editingActive != 0) {
			if (code >= 5 && code <= 0x1e) {
				return 1;
			}
			switch (code) {
			case 1:
			case 0x1f:
			case 0x4c:
			case 0x4d:
			case 0x4e:
				return 1;
			}
		}
		return 0;
	case 0xc:
		switch (p_message->code) {
		case 0xacef000c:
			if (m_locked == 0) {
				Start(0);
			}
			break;
		case 0xacef000d:
			if (m_locked == 0) {
				Start(1);
			}
			break;
		case 0xacef000e:
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
	m_editor->m_maxLength = g_anNetworkOptionsEditMaxLength[m_editingStage];
	SetMessage(g_anNetworkOptionsEditMessages[m_editingStage]);
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
	if (m_pendingEvent == 7) {
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

// FUNCTION: LEMBALL 0x004548c0
void CNetworkOptionsDrawer::Processing()
{
	unsigned long now;
	unsigned long duration;
	char* ident;
	char* peer;
	CConnect* connection;
	int index;
	CConnect** current;
	CConnect** connections;
	int activation;
	int acceptedPlayer;

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
		ident = g_pBroadcastAddress->GetStr();
		peer = g_szBroadcastPeerName;
		if (m_stopPending != ident) {
			m_backBufferNeeded = 1;
			m_stopPending = ident;
		}
		if (m_connectionState != peer) {
			m_backBufferNeeded = 1;
			m_connectionState = peer;
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
			if (m_playerEntries[index].m_pressed != 0 && m_acceptedPlayer != index) {
				g_pSoundView->PlayEffect(SFX_DRUM1);
				acceptedPlayer = m_acceptedPlayer;
				if (acceptedPlayer != -1) {
					connection = connections[acceptedPlayer];
					if (connection != 0) {
						((CNetworkOptionsProc*) g_pCurrentFrontendProcess)->Reject(connection);
					}
				}
				m_acceptedPlayer = index;
				if (*current != 0) {
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
	short rect[3];
	volatile short rowY;
	CConnect** connections;
	CNetworkGameMessage* messages;
	int index;

	connections = 0;
	messages = 0;
	if (g_pNetworkManager != 0) {
		connections = g_pNetworkManager->m_connections;
		messages = g_pNetworkManager->m_gameMessages;
	}
	rect[1] = m_layoutTable->m_entryHeight;
	rowY = (short) m_layoutTable->m_entryY;
	rect[2] = (short) m_layoutTable->m_entryX;
	rect[0] = (short) m_layoutTable->m_entryWidth;
	index = 0;
	m_visibleEntryCount = 0;
	do {
		if (m_visibleEntryCount < 4 && connections != 0 && connections[index] != 0 && messages[index].m_valid != 0) {
			CEntryHandler* entry = &m_playerEntries[index];
			entry->m_bounds.m_width = rect[0];
			entry->m_bounds.m_height = rect[1];
			entry->m_bounds.m_x = rect[2];
			entry->m_bounds.m_y = rowY;
			entry->SetActive(1);
			rowY += (short) m_layoutTable->m_rowStride;
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
