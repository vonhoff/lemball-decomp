#include "../../CNetworkOptionsDrawer.h"

#include "../../../../Control/Game/CGameStatus.h"
#include "../../../../Network/Game/CNetworkManager.h"
#include "../../../../Network/Messages/CNetworkGameMessage.h"
#include "../../../../Views/Display/CMain2DDisplay.h"
#include "../../../../Views/Sound/CSoundView.h"
#include "../../../../Visos/Foundation/CString.h"
#include "../../../../Visos/Foundation/CTextManager.h"
#include "../../../../Visos/Foundation/VsTime.h"
#include "../../../../Visos/Graphics/CBasePalManager.h"
#include "../../../../Visos/Graphics/CGWnd.h"
#include "../../../../Visos/Graphics/CHotAreaList.h"
#include "../../../../Visos/Network/CConnect.h"
#include "../../../../Visos/Network/CNetworkAddress.h"
#include "../../../../Visos/Resources/CResFONT.h"
#include "../../../../Visos/Resources/Manifest.h"
#include "../../../Controls/CHiliteController.h"
#include "../../../Processes/CNetworkOptionsProc.h"
#include "../../../Support/CEditString.h"
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

extern char* g_apNetworkOptionsMessages[9];
extern char g_szNetworkGameName[16];
extern char g_szNetworkBroadcastAddress[16];
extern char g_szNetworkOptionsCursor[];
extern char g_szNetworkOptionsDividerIp[];
extern char g_szNetworkOptionsDividerLocal[];
extern char g_szNetworkOptionsHeaderComputer[];
extern char g_szNetworkOptionsHeaderIp[];
extern char g_szNetworkOptionsHeaderName[];

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
		{
			short sizeStorage[2];
			CVsSize& size = *(CVsSize*) sizeStorage;
			size.m_height = 0;
			size.m_width = 0;
			m_textManager->DrawString(m_gdi, posLabel, size, m_chalkFontId, g_szNetworkOptionsHeaderName, 0x20, 0);
		}

		posIp.m_x -= font->GetSize(g_szNetworkOptionsHeaderIp, 0x20).m_width / 2;
		{
			short sizeStorage[2];
			CVsSize& size = *(CVsSize*) sizeStorage;
			size.m_height = 0;
			size.m_width = 0;
			m_textManager->DrawString(m_gdi, posIp, size, m_chalkFontId, g_szNetworkOptionsHeaderIp, 0x20, 0);
		}

		posComputer.m_x -= font->GetSize(g_szNetworkOptionsHeaderComputer, 0x20).m_width / 2;
		{
			short sizeStorage[2];
			CVsSize& size = *(CVsSize*) sizeStorage;
			size.m_height = 0;
			size.m_width = 0;
			m_textManager
				->DrawString(m_gdi, posComputer, size, m_chalkFontId, g_szNetworkOptionsHeaderComputer, 0x20, 0);
		}

		short dividerWidth = font->GetSize(divider, 0x20).m_width;
		posDivider.m_x = (short) (((int) m_width - (int) dividerWidth) / 2);
		{
			short sizeStorage[2];
			CVsSize& size = *(CVsSize*) sizeStorage;
			size.m_height = 0;
			size.m_width = 0;
			m_textManager->DrawString(m_gdi, posDivider, size, m_chalkFontId, divider, 0x20, 0);
		}

		if (g_szNetworkGameName[0] != 0) {
			CVsPoint posMyName((short) m_layoutTable->m_headerNameX, (short) m_layoutTable->m_localPlayerY);
			CVsPoint posMyIp((short) m_layoutTable->m_headerIpX, (short) m_layoutTable->m_localPlayerY);
			CVsPoint posMyComputer((short) m_layoutTable->m_headerComputerX, (short) m_layoutTable->m_localPlayerY);
			posMyName.m_x -= font->GetSize(g_szNetworkGameName, 0x20).m_width / 2;
			{
				short sizeStorage[2];
				CVsSize& size = *(CVsSize*) sizeStorage;
				size.m_height = 0;
				size.m_width = 0;
				m_textManager->DrawString(m_gdi,
										  posMyName,
										  size,
										  m_chalkFontId,
										  g_szNetworkGameName,
										  0x20,
										  (CRemap*) m_remaps[0]);
			}

			char* myIp = m_stopPending;
			if (myIp != 0 && *myIp != 0) {
				posMyIp.m_x -= font->GetSize(myIp, 0x20).m_width / 2;
				{
					short sizeStorage[2];
					CVsSize& size = *(CVsSize*) sizeStorage;
					size.m_height = 0;
					size.m_width = 0;
					m_textManager
						->DrawString(m_gdi, posMyIp, size, m_chalkFontId, m_stopPending, 0x20, (CRemap*) m_remaps[0]);
				}
			}

			char* myPeer = m_connectionState;
			if (myPeer != 0 && *myPeer != 0) {
				char trimmed[21];
				strncpy(trimmed, myPeer, 0x14);
				int len = 0x14;
				do {
					trimmed[len--] = 0;
				} while (m_layoutTable->m_peerNameWidth < font->GetSize(trimmed, 0x20).m_width);

				CString lowerPeer(trimmed);
				lowerPeer.lower();
				posMyComputer.m_x -= font->GetSize(trimmed, 0x20).m_width / 2;
				{
					short sizeStorage[2];
					CVsSize& size = *(CVsSize*) sizeStorage;
					size.m_height = 0;
					size.m_width = 0;
					m_textManager
						->DrawString(m_gdi, posMyComputer, size, m_chalkFontId, lowerPeer, 0x20, (CRemap*) m_remaps[0]);
				}
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
			CString msgText = g_apNetworkOptionsMessages[m_message - 1];
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
			int idx;
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
			idx = 0;
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
