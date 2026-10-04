#include "../CPreviewDrawer.h"

#include "Visos/Graphics/Palettes/CBasePalManager.h"
#include "Visos/Resources/Types/CResPALETTE.h"
#include "../../../Visos/Resources/Manifest.h"
#include "Visos/Graphics/Palettes/CBaseRemap.h"

class CGWnd;
class CRemap;

extern int g_previewRemapSourceIndices[10];
extern int g_previewRemapTargetIndices[10];

// FUNCTION: LEMBALL 0x0044a330
void CPreviewDrawer::RegisterRemaps()
{
	CResPALETTE* palette;
	int i;

	palette = CResPALETTE::Load(RES_PALETTES_TITLEPALETTE);
	m_remapTable = (unsigned char*) operator new(0x100);
	i = 0;
	do {
		m_remapTable[i] = (unsigned char) i;
		i = i + 1;
	} while (i < 0x100);
	i = 0;
	do {
		int target = g_previewRemapTargetIndices[i];
		int source = g_previewRemapSourceIndices[i];
		if (target != 0) {
			m_remapTable[source] = (unsigned char) target;
		}
		i = i + 1;
	} while (i < 10);
	m_remap = g_pBasePalManager->RegisterRemap(RES_PALETTES_TITLEPALETTE, m_remapTable, PALETTE_DEFAULT);
	palette->UnLoad();
}
