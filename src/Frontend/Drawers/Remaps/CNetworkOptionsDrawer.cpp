#include "../CNetworkOptionsDrawer.h"

#include "../../../Views/Display/CMain2DDisplay.h"
#include "Visos/Graphics/Palettes/CBasePalManager.h"
#include "Frontend/Base/CBaseFrontendDrawer.h"
#include "Visos/Math/CVSRect.h"
#include "Visos/Graphics/Palettes/CBaseRemap.h"

class CRemap;

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
