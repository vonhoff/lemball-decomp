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

extern unsigned char* g_apNetworkOptionsRemaps[6];

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
