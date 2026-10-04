#include "../../CNetworkOptionsDrawer.h"

#include "../../../../Network/Game/CNetworkManager.h"
#include "../../../../Network/Messages/CNetworkGameMessage.h"
#include "Visos/Strings/CString.h"
#include "Visos/Text/CTextManager.h"
#include "Visos/Resources/Types/CResFONT.h"
#include "../../../Support/CEditString.h"
#include "Frontend/Base/CBaseFrontendDrawer.h"
#include "Frontend/Base/FrontendLayoutMode.h"
#include "Frontend/Support/CEntryHandler.h"
#include "Visos/Math/CVSPoint.h"
#include "Visos/Math/CVSSize.h"

#include <string.h>

class CRemap;

extern "C" unsigned long __stdcall timeGetTime(void);

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
