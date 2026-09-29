#include "../CNetworkOptionsDrawer.h"

#include "../../../Control/Game/CGameStatus.h"
#include "../../../Network/Game/CNetworkManager.h"
#include "../../../Network/Messages/CNetworkGameMessage.h"
#include "../../../Views/Display/CMain2DDisplay.h"
#include "../../../Views/Sound/CSoundView.h"
#include "../../../Visos/Foundation/CString.h"
#include "../../../Visos/Foundation/CTextManager.h"
#include "../../../Visos/Foundation/VsTime.h"
#include "../../../Visos/Graphics/CBasePalManager.h"
#include "../../../Visos/Graphics/CGWnd.h"
#include "../../../Visos/Graphics/CHotAreaList.h"
#include "../../../Visos/Network/CConnect.h"
#include "../../../Visos/Network/CNetworkAddress.h"
#include "../../../Visos/Resources/CResFONT.h"
#include "../../../Visos/Resources/Manifest.h"
#include "../../Controls/CHiliteController.h"
#include "../../Processes/CNetworkOptionsProc.h"
#include "../../Support/CEditString.h"
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
			CVsPoint namePosition((short) layout->m_headerNameX, (short) layout->m_playerListY);
			CVsPoint addressPosition((short) layout->m_headerIpX, (short) layout->m_playerListY);
			CVsPoint peerPosition((short) layout->m_headerComputerX, (short) layout->m_playerListY);
			CVsPoint& posName = namePosition;
			CVsPoint& posAddress = addressPosition;
			CVsPoint& posPeer = peerPosition;
			short yOffset = (short) layout->m_rowStride * (short) p_value;
			posName.m_y += yOffset;
			posAddress.m_y += yOffset;
			posPeer.m_y += yOffset;
			remap = 0;
			if (p_remap != 6) {
				remap = (CRemap*) m_remaps[p_remap];
			}
			gameName = entries[p_index].m_gameName;
			addressStr = connections[p_index]->m_destinationAddress->GetStr();
			peerName = entries[p_index].m_peerName;
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
