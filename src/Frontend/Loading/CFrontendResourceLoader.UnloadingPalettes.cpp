#include "CFrontendResourceLoader.h"

#include "Visos/Resources/Types/CResPALETTE.h"

#include <stddef.h>

// FUNCTION: LEMBALL 0x00447f60
void CFrontendResourceLoader::UnLoadPALETTE(unsigned long p_resourceId)
{
	unsigned int i;

	for (i = 0; i < m_loadedPalettes; i++) {
		if (m_palettes[i] != NULL && m_palettes[i]->m_resourceId == p_resourceId) {
			m_palettes[i]->UnLoad();
			m_palettes[i] = NULL;
			break;
		}
	}
}
