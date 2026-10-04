#include "../CNetworkOptionsDrawer.h"

#include "Network/CNetworkManager.h"
#include "../../../Network/Messages/CNetworkGameMessage.h"
#include "Visos/Text/CTextManager.h"
#include "../../../Visos/Network/CConnect.h"
#include "../../../Visos/Network/CNetworkAddress.h"
#include "Visos/Resources/Types/CResFONT.h"
#include "../../Processes/CNetworkOptionsProc.h"
#include "Frontend/Base/CBaseFrontendDrawer.h"
#include "Frontend/Base/CBaseFrontendProcess.h"
#include "Visos/Math/CVSPoint.h"
#include "Visos/Math/CVSSize.h"

#include <string.h>

class CRemap;

extern "C" unsigned long __stdcall timeGetTime(void);

extern char* g_szBroadcastPeerName;

enum {
	NETWORK_OPTIONS_REMAP_NONE = 6
};

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
