#include "NetworkOptionsDrawer.h"

#include "../../Control/Game/GameStatus.h"
#include "../../Network/Game/NetworkManager.h"
#include "../../Network/Messages/NetworkGameMessage.h"
#include "../../Views/Display/Main2DDisplay.h"
#include "../../Views/Sound/SoundView.h"
#include "../../Visos/Foundation/String.h"
#include "../../Visos/Foundation/TextManager.h"
#include "../../Visos/Foundation/VsTime.h"
#include "../../Visos/Graphics/BasePalManager.h"
#include "../../Visos/Graphics/GWnd.h"
#include "../../Visos/Graphics/HotAreaList.h"
#include "../../Visos/Network/Connect.h"
#include "../../Visos/Network/NetworkAddress.h"
#include "../../Visos/Resources/Manifest.h"
#include "../../Visos/Resources/ResFont.h"
#include "../Controls/HiliteController.h"
#include "../Processes/NetworkOptionsProc.h"
#include "../Support/EditString.h"

#include <new.h>
#include <string.h>

#pragma intrinsic(strcpy)

extern "C" unsigned long __stdcall timeGetTime(void);

extern char* g_szBroadcastPeerName;

struct BroadcastAddressDispatch {
	virtual char* GetStr() = 0;
};

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

// GLOBAL: LEMBALL 0x004a031c
char* g_apNetworkOptionsMessages[11] = {
	0,
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

// 68K 0x10806468 __ct__21CNetworkOptionsDrawerFP14CMain2DDisplayP4CGDIRC7CVSRect
// FUNCTION: LEMBALL 0x00453280
NetworkOptionsDrawer::NetworkOptionsDrawer(Main2DDisplay* p_arg0, Gdi* p_arg1, const VsRect& p_arg2)
	: BaseFrontendDrawer(p_arg0, p_arg1, p_arg2, (eFlowProcesses) 0xc, 0x32, 200, 0, 100, 0x28)
{
	int i;

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
	m_pendingEvent = 0;
	m_editor = new EditString(0x28);
	m_playerEntries = new EntryHandler[10];
	m_acceptedPlayer = -1;
	m_highlightedPlayer = -1;
	RegisterRemaps();
	i = 0;
	do {
		((GWnd*) m_display)->m_hotAreaList->AddToList(&m_playerEntries[i]);
		i = i + 1;
	} while (i < 10);
	m_drawBackground = 1;
	m_drawFrame = 1;
	m_drawSolid = 0;
	Setup();
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
// FUNCTION: LEMBALL 0x004536f0
void NetworkOptionsDrawer::DrawEntry(unsigned long p_index, int& p_value, int p_remap)
{
	VsPoint size;
	VsSize advance;
	VsPoint posName;
	VsPoint posAddress;
	VsPoint posPeer;
	char* gameName;
	char* peerName;
	char* addressStr;
	char trimmedPeerName[24];
	ResFont* font;
	Remap* remap;
	int len;

	if (g_pNetworkManager != 0) {
		if (g_pNetworkManager->m_gameMessages[p_index].m_valid != 0) {
			int* layout = (int*) m_layoutTable;
			font = m_textManager->GetFont(m_chalkFontId);
			posName.m_x = (short) layout[0x68 / 4];
			posAddress.m_x = (short) layout[0x78 / 4];
			posPeer.m_x = (short) layout[0x80 / 4];
			remap = 0;
			int yOffset = (short) layout[0x8c / 4] * (short) p_value;
			posName.m_y = (short) layout[0x6c / 4] + yOffset;
			posAddress.m_y = (short) layout[0x6c / 4] + yOffset;
			posPeer.m_y = (short) layout[0x6c / 4] + yOffset;
			if (p_remap != 6) {
				remap = (Remap*) m_remaps[p_remap];
			}
			gameName = g_pNetworkManager->m_gameMessages[p_index].m_gameName;
			peerName = g_pNetworkManager->m_gameMessages[p_index].m_peerName;
			if (g_pNetworkManager->m_connections[p_index] == 0 ||
				g_pNetworkManager->m_connections[p_index]->m_address == 0) {
				return;
			}
			addressStr = g_pNetworkManager->m_connections[p_index]->m_address->GetStr();
			memcpy(trimmedPeerName, peerName, 0x14);
			len = 0x14;
			do {
				trimmedPeerName[len] = 0;
				size = font->GetSize(trimmedPeerName, 0x20);
				len--;
			} while (layout[0x98 / 4] < (int) size.m_x);

			size = font->GetSize(gameName, 0x20);
			posName.m_x -= size.m_x / 2;
			size = font->GetSize(addressStr, 0x20);
			posAddress.m_x -= size.m_x / 2;
			size = font->GetSize(peerName, 0x20);
			posPeer.m_x -= size.m_x / 2;

			advance.m_width = 0;
			advance.m_height = 0;
			m_textManager->DrawString(m_gdi, posName, advance, m_chalkFontId, gameName, 0x20, remap);
			advance.m_width = 0;
			advance.m_height = 0;
			m_textManager->DrawString(m_gdi, posAddress, advance, m_chalkFontId, addressStr, 0x20, remap);
			advance.m_width = 0;
			advance.m_height = 0;
			m_textManager->DrawString(m_gdi, posPeer, advance, m_chalkFontId, peerName, 0x20, remap);
			p_value++;
		}
	}
}

// 68K 0x10806d8c DrawText__21CNetworkOptionsDrawerFv
// FUNCTION: LEMBALL 0x00453940
void NetworkOptionsDrawer::DrawText()
{
	VsPoint pos;
	VsPoint posLabel;
	VsPoint posIp;
	VsPoint posComputer;
	VsPoint posDivider;
	VsPoint posMyName;
	VsPoint posMyIp;
	VsPoint posMyComputer;
	VsPoint size;
	VsSize advance;
	ResFont* font;
	int row;
	int* layout = (int*) m_layoutTable;

	pos.m_y = (short) layout[0x5c / 4];
	pos.m_x = (short) layout[0x58 / 4];

	if (m_drawingBackBuffer == 0) {
		if (m_message != 0) {
			VsPoint msgPos;
			msgPos.m_y = (short) layout[0x4c / 4];
			msgPos.m_x = (short) layout[0x48 / 4];
			String msgText = g_apNetworkOptionsMessages[m_message];
			bool special = false;
			if (m_message == 4) {
				if (g_szNetworkBroadcastAddress[0] == 0) {
					msgText = g_szNetworkOptionsMsg5;
				}
				else {
					msgText += g_szNetworkBroadcastAddress;
				}
				special = true;
			}
			Remap* remap = 0;
			if (m_message < 7) {
				if (special) {
					remap = (Remap*) m_remaps[5];
				}
			}
			else {
				remap = (Remap*) m_remaps[3];
				special = true;
			}
			if (m_redrawPending != 0 || !special) {
				font = m_textManager->GetFont(m_chalkFontId);
				size = font->GetSize(msgText.m_text, 0x20);
				msgPos.m_x -= size.m_x / 2;
				advance.m_width = 0;
				advance.m_height = 0;
				m_textManager->DrawString(m_gdi, msgPos, advance, m_chalkFontId, msgText, 0x20, remap);
			}
		}

		m_messageDirty = m_message;
		if (g_pNetworkManager != 0) {
			int searchIndex;
			row = 0;
			int fallbackHighlighted = -1;
			if (m_highlightedPlayer != -1 && m_playerEntries[m_highlightedPlayer].m_active == 0) {
				searchIndex = 0;
				do {
					if (m_playerEntries[searchIndex].m_active != 0) {
						m_highlightedPlayer = searchIndex;
						break;
					}
					searchIndex = searchIndex + 1;
				} while (searchIndex < 10);
				if (searchIndex == 10) {
					fallbackHighlighted = m_highlightedPlayer;
				}
			}
			for (int idx = 0; idx < 10; idx++) {
				if (g_pNetworkManager->m_gameMessages[idx].m_valid != 0) {
					int isAccepted = 0;
					int state = 1;
					if (m_playerEntries[idx].m_active != 0 || fallbackHighlighted == idx) {
						isAccepted = 1;
					}
					if (m_acceptedPlayer == idx) {
						if (m_playerEntries[idx].m_entered == 0 || m_redrawPending != 0) {
							state = 3;
						}
					}
					else if (m_playerEntries[idx].m_entered != 0 && m_redrawPending == 0) {
						state = 3;
					}
					if (state + isAccepted == 1) {
						row++;
					}
					else {
						DrawEntry(idx, row, state + isAccepted);
					}
					if (row == 4) {
						break;
					}
				}
			}
		}

		if (m_editingActive != 0) {
			String editText = m_editor->m_text;
			if (m_redrawPending == 0) {
				editText += g_szNetworkOptionsCursor;
			}
			if (editText.Getlength() > 0) {
				Remap* remap = (Remap*) m_remaps[1];
				advance.m_width = 0;
				advance.m_height = 0;
				m_textManager->DrawString(m_gdi, pos, advance, m_chalkFontId, editText, 0x20, remap);
			}
		}
	}
	else {
		char* divider = (m_mode == 0) ? g_szNetworkOptionsDividerLocal : g_szNetworkOptionsDividerIp;
		font = m_textManager->GetFont(m_chalkFontId);
		posDivider.m_y = (short) layout[0x50 / 4];
		posDivider.m_x = 0;
		posLabel.m_y = (short) layout[0x54 / 4];
		posLabel.m_x = (short) layout[0x68 / 4];
		posIp.m_y = (short) layout[0x54 / 4];
		posIp.m_x = (short) layout[0x78 / 4];
		posComputer.m_y = (short) layout[0x54 / 4];
		posComputer.m_x = (short) layout[0x80 / 4];

		size = font->GetSize(g_szNetworkOptionsHeaderName, 0x20);
		posLabel.m_x -= size.m_x / 2;
		advance.m_width = 0;
		advance.m_height = 0;
		m_textManager->DrawString(m_gdi, posLabel, advance, m_chalkFontId, g_szNetworkOptionsHeaderName, 0x20, 0);

		size = font->GetSize(g_szNetworkOptionsHeaderIp, 0x20);
		posIp.m_x -= size.m_x / 2;
		advance.m_width = 0;
		advance.m_height = 0;
		m_textManager->DrawString(m_gdi, posIp, advance, m_chalkFontId, g_szNetworkOptionsHeaderIp, 0x20, 0);

		size = font->GetSize(g_szNetworkOptionsHeaderComputer, 0x20);
		posComputer.m_x -= size.m_x / 2;
		advance.m_width = 0;
		advance.m_height = 0;
		m_textManager->DrawString(m_gdi, posComputer, advance, m_chalkFontId, g_szNetworkOptionsHeaderComputer, 0x20, 0);

		size = font->GetSize(divider, 0x20);
		posDivider.m_x = (short) (((int) m_width - (int) size.m_x) / 2);
		advance.m_width = 0;
		advance.m_height = 0;
		m_textManager->DrawString(m_gdi, posDivider, advance, m_chalkFontId, divider, 0x20, 0);

		if (g_szNetworkGameName[0] != 0) {
			posMyName.m_y = (short) layout[0x64 / 4];
			posMyName.m_x = (short) layout[0x68 / 4];
			posMyIp.m_y = (short) layout[0x64 / 4];
			posMyIp.m_x = (short) layout[0x78 / 4];
			posMyComputer.m_y = (short) layout[0x64 / 4];
			posMyComputer.m_x = (short) layout[0x80 / 4];

			size = font->GetSize(g_szNetworkGameName, 0x20);
			posMyName.m_x -= size.m_x / 2;
			advance.m_width = 0;
			advance.m_height = 0;
			m_textManager
				->DrawString(m_gdi, posMyName, advance, m_chalkFontId, g_szNetworkGameName, 0x20, (Remap*) m_remaps[0]);

			char* myIp = (char*) m_stopPending;
			if (myIp != 0 && *myIp != 0) {
				size = font->GetSize(myIp, 0x20);
				posMyIp.m_x -= size.m_x / 2;
				advance.m_width = 0;
				advance.m_height = 0;
				m_textManager->DrawString(m_gdi, posMyIp, advance, m_chalkFontId, myIp, 0x20, (Remap*) m_remaps[0]);
			}

			char* myPeer = (char*) m_connectionState;
			if (myPeer != 0 && *myPeer != 0) {
				char trimmed[24];
				int len = 0x14;
				memcpy(trimmed, myPeer, 0x14);
				do {
					trimmed[len] = 0;
					size = font->GetSize(trimmed, 0x20);
					len--;
				} while (layout[0x98 / 4] < (int) size.m_x);

				String lowerPeer = String(trimmed).Lower();
				size = font->GetSize(trimmed, 0x20);
				posMyComputer.m_x -= size.m_x / 2;
				advance.m_width = 0;
				advance.m_height = 0;
				m_textManager
					->DrawString(m_gdi, posMyComputer, advance, m_chalkFontId, lowerPeer, 0x20, (Remap*) m_remaps[0]);
			}
		}

		if (g_pNetworkManager != 0) {
			row = 0;
			for (unsigned long i = 0; i < 10; i++) {
				DrawEntry(i, row, 1);
				if (row == 4) {
					break;
				}
			}
		}
	}
}

// 68K 0x108075c6 DrawAnims__21CNetworkOptionsDrawerFv
// FUNCTION: LEMBALL 0x00454050
void NetworkOptionsDrawer::DrawAnims()
{
}

// 68K 0x108075f6 ProcessMessages__21CNetworkOptionsDrawerFP10tagMESSAGE
// FUNCTION: LEMBALL 0x00454060
bool NetworkOptionsDrawer::ProcessMessages(Message* p_message)
{
	int code;

	if (m_startPending != 0 || (unsigned int) m_message != m_messageDirty) {
		return 0;
	}

	if (p_message->type == 3) {
		code = p_message->code;
		if (code == 0x49) {
			g_nNetworkOptionsCapsOrShift &= ~1;
			g_nNetworkOptionsShiftHeld = 0;
			return 1;
		}
		if (m_editingActive != 0) {
			if (code > 4 && code < 0x1f) {
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
	}

	if (p_message->type != 4) {
		if (p_message->type != 0xc) {
			m_processedCount++;
			return 0;
		}
		code = p_message->code;
		if (code == 0xacef000c) {
			if (m_locked == 0) {
				Start(0);
				return 0;
			}
		}
		else if (code == 0xacef000d) {
			if (m_locked == 0) {
				Start(1);
				return 0;
			}
		}
		else if (code == 0xacef000e) {
			Stop();
			m_quitYet = 1;
			m_returnState = 2;
			return 1;
		}
		return 0;
	}

	code = p_message->code;
	if (code == 0x49) {
		g_nNetworkOptionsShiftHeld = 1;
		g_nNetworkOptionsCapsOrShift |= 1;
		return 1;
	}

	if (m_editingActive != 0) {
		bool handled = false;
		if (code > 4 && code < 0x1f) {
			if (m_editor->m_length != m_editor->m_maxLength) {
				char ch = (char) code - (((g_nNetworkOptionsShiftHeld == 0) ? 0xe0 : 0) - 0x3c);
				*m_editor += ch;
				handled = true;
			}
			else {
				g_pSoundView->PlayEffect((eSoundEffect) 0x19);
			}
		}
		else if (code > 0x38 && code < 0x43) {
			if (m_editor->m_length != m_editor->m_maxLength) {
				*m_editor += (char) (code - 9);
				handled = true;
			}
			else {
				g_pSoundView->PlayEffect((eSoundEffect) 0x19);
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
					g_pSoundView->PlayEffect((eSoundEffect) 0x19);
				}
				break;
			case 0x20:
				if (m_editor->m_length != m_editor->m_maxLength) {
					*m_editor += '.';
					handled = true;
				}
				else {
					g_pSoundView->PlayEffect((eSoundEffect) 0x19);
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
			case 0x4e:
				if (m_editor->m_length > 0) {
					m_editor->m_length--;
					m_editor->m_text[m_editor->m_length] = 0;
					handled = true;
				}
				else {
					g_pSoundView->PlayEffect((eSoundEffect) 0x19);
				}
				break;
			}
		}

		if (handled) {
			m_lastDrawTime = CurrentMilliTimer();
			m_redrawPending = 0;
			g_pSoundView->PlayEffect((eSoundEffect) 0x25);
			return 1;
		}
	}

	switch (code) {
	case 1:
		if (HighlightPreviousEntry()) {
			g_pSoundView->PlayEffect((eSoundEffect) 0x1b);
			return 1;
		}
		break;
	case 2:
		if (HighlightNextEntry()) {
			g_pSoundView->PlayEffect((eSoundEffect) 0x1b);
			return 1;
		}
		break;
	case 0x1f:
	case 0x22:
	case 0x4c:
		if (m_highlightedPlayer != -1) {
			VsPoint pt;
			pt.m_x = 0;
			pt.m_y = 0;
			m_playerEntries[m_highlightedPlayer].OnButtonDown(pt, 0);
			return 1;
		}
		break;
	}

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
	((NetworkOptionsProc*) g_pCurrentFrontendProcess)->StopBroadcast();
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
	((NetworkOptionsProc*) g_pCurrentFrontendProcess)->Start();
	if (((NetworkOptionsProc*) g_pCurrentFrontendProcess)->m_started != 0 &&
		((NetworkOptionsProc*) g_pCurrentFrontendProcess)->m_startFailed == 0) {
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
	if (((NetworkOptionsProc*) g_pCurrentFrontendProcess)->m_started != 0 &&
		((NetworkOptionsProc*) g_pCurrentFrontendProcess)->m_startFailed == 0) {
		((NetworkOptionsProc*) g_pCurrentFrontendProcess)->Stop();
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
	now = timeGetTime();
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
		((NetworkOptionsProc*) g_pCurrentFrontendProcess)->Start();
		if (((NetworkOptionsProc*) g_pCurrentFrontendProcess)->m_started == 0 ||
			((NetworkOptionsProc*) g_pCurrentFrontendProcess)->m_startFailed != 0) {
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
		if (((NetworkOptionsProc*) g_pCurrentFrontendProcess)->m_started == 0 ||
			((NetworkOptionsProc*) g_pCurrentFrontendProcess)->m_startFailed != 0) {
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
		((NetworkOptionsProc*) g_pCurrentFrontendProcess)->StopBroadcast();
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
	now = timeGetTime();
	m_backBufferNeeded = 1;
	m_messageStartTime = now;
	m_messageDuration = p_duration;
	now = timeGetTime();
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
		if (g_pBroadcastAddress != 0) {
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
		if (g_pNetworkOptionsProc != 0) {
			connections = g_pNetworkManager->m_connections;
			current = connections;
			index = 0;
			do {
				if (m_playerEntries[index].m_pressed != 0 && m_acceptedPlayer != index) {
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
	m_visibleEntryCount = 0;
	offset = 0;
	if (messages != 0 && connections != 0) {
		valid = &messages->m_valid;
		do {
			entry = &m_playerEntries[offset];
			if (m_visibleEntryCount < 4 && *connections != 0 && *valid != 0) {
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
	}
	else {
		do {
			m_playerEntries[offset].SetActive(0);
			offset = offset + 1;
		} while (offset < 10);
	}
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
		messages = g_pNetworkManager->m_gameMessages;
		if (messages != 0 && connections != 0) {
			index = 0;
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
