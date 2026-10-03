#include "Frontend/Resources/CFrontendResourceLoader.h"

#include "Visos/Resources/CResBITMAP.h"

#include <stddef.h>

// FUNCTION: LEMBALL 0x00447ee0
void CFrontendResourceLoader::UnLoadBITMAP(unsigned long p_resourceId)
{
	unsigned int i;

	for (i = 0; i < (unsigned int) m_loadedBitmaps; i++) {
		if (m_bitmaps[i] != NULL && m_bitmaps[i]->m_resourceId == p_resourceId) {
			m_bitmaps[i]->UnLoad();
			m_bitmaps[i] = NULL;
			break;
		}
	}
}
