#include "Frontend/Resources/CFrontendResourceLoader.h"

#include "Visos/Resources/Types/CResSTRING.h"

#include <stddef.h>

// FUNCTION: LEMBALL 0x00447fe0
void CFrontendResourceLoader::UnLoadSTRING(unsigned long p_resourceId)
{
	unsigned int count = m_loadedStrings;
	CResSTRING** slot;
	unsigned int i;

	for (i = 0; i < count; i++) {
		slot = &m_strings[i];
		if (*slot != NULL && (*slot)->m_resourceId == p_resourceId) {
			m_strings[i]->UnLoad();
			m_strings[i] = NULL;
			break;
		}
	}
}
