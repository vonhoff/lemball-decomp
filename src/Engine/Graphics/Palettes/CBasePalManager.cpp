#include "CBasePalManager.h"

#include <stddef.h>

#include "CRemap.h"
#include "Engine/Graphics/Palettes/CBaseRemap.h"

// FUNCTION: LEMBALL 0x0046acd0
CBasePalManager::CBasePalManager(int p_capacity)
{
	m_capacity = p_capacity;
	m_remapCount = 0;
	m_remaps = new CRemap*[p_capacity];
	for (int i = 0; i < m_capacity; ++i) {
		m_remaps[i] = NULL;
	}
}

// FUNCTION: LEMBALL 0x0046ad10
CBasePalManager::~CBasePalManager()
{
	if (m_remaps != NULL) {
		for (int i = 0; i < m_remapCount; ++i) {
			delete m_remaps[i];
			m_remaps[i] = NULL;
		}
		delete[] m_remaps;
	}
}

// FUNCTION: LEMBALL 0x0046ad70
CRemap* CBasePalManager::RegisterRemap(unsigned long p_resourceId, unsigned char* p_mapping, ePaletteTypes p_type)
{
	int i = 0;
	while (m_remaps[i] != NULL) {
		++i;
	}
	m_remaps[i] = new CRemap(p_resourceId, p_mapping, p_type);
	++m_remapCount;
	return m_remaps[i];
}

// FUNCTION: LEMBALL 0x0046add0
void CBasePalManager::UnRegisterRemap(CRemap* p_remap)
{
	int remapCount = m_remapCount;
	CRemap** slots = m_remaps;
	int i = 0;
	for (int counted = 0; counted < remapCount; ++counted, ++i) {
		while (slots[i] == NULL) {
			++i;
		}
		if (slots[i] == p_remap) {
			delete slots[i];
			m_remaps[i] = NULL;
			--m_remapCount;
			return;
		}
	}
}

// GLOBAL: LEMBALL 0x004a2000
CPaletteManager* g_pBasePalManager = NULL;
