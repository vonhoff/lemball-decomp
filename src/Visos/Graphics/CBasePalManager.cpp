#include "CBasePalManager.h"

#include "CBaseRemap.h"
#include "CRemap.h"

#include <new.h>

// FUNCTION: LEMBALL 0x0046acd0
CBasePalManager::CBasePalManager(int p_capacity)
{
	int i;

	m_capacity = p_capacity;
	m_remapCount = 0;
	m_remaps = (CBaseRemap**) operator new(p_capacity * sizeof(CBaseRemap*));
	i = 0;
	if (0 < m_capacity) {
		do {
			m_remaps[i] = 0;
			i = i + 1;
		} while (i < m_capacity);
	}
}

// FUNCTION: LEMBALL 0x0046ad10
CBasePalManager::~CBasePalManager()
{
	int i = 0;
	CBasePalManager* manager = this;
	int offset;
	CBaseRemap* remap;

	if (manager->m_remaps != 0 && 0 < manager->m_remapCount) {
		offset = 0;
		do {
			CBaseRemap** slot = (CBaseRemap**) ((char*) manager->m_remaps + offset);
			remap = *slot;
			if (remap != 0) {
				remap->~CBaseRemap();
				operator delete(remap);
			}
			offset += sizeof(CBaseRemap*);
			i = i + 1;
			CBaseRemap** previousSlot = (CBaseRemap**) ((char*) manager->m_remaps + offset - sizeof(CBaseRemap*));
			*previousSlot = 0;
		} while (i < manager->m_remapCount);
	}
	if (manager->m_remaps != 0) {
		operator delete(manager->m_remaps);
	}
}

// FUNCTION: LEMBALL 0x0046ad70
CBaseRemap* CBasePalManager::RegisterRemap(unsigned int p_resourceId, unsigned char* p_mapping, ePaletteTypes p_type)
{
	CBaseRemap** slot;
	void* storage;
	CBaseRemap* remap;
	int i;

	slot = m_remaps;
	i = 0;
	remap = *slot;
	while (remap != 0) {
		slot = slot + 1;
		i = i + 1;
		remap = *slot;
	}
	storage = operator new(sizeof(CRemap));
	if (storage == 0) {
		m_remaps[i] = 0;
	}
	else {
		m_remaps[i] = new (storage) CRemap(p_resourceId, p_mapping, p_type);
	}
	m_remapCount = m_remapCount + 1;
	return m_remaps[i];
}

// FUNCTION: LEMBALL 0x0046add0
void CBasePalManager::UnRegisterRemap(CBaseRemap* p_remap)
{
	int i;
	int counted;
	int remapCount;
	CBaseRemap** slots;
	CBaseRemap* current;

	counted = 0;
	i = 0;
	remapCount = m_remapCount;
	if (counted < remapCount) {
		slots = m_remaps;
		while (true) {
			current = slots[i];
			while (current == 0) {
				i = i + 1;
				current = slots[i];
			}
			if (slots[i] == p_remap) {
				break;
			}
			counted = counted + 1;
			i = i + 1;
			if (remapCount <= counted) {
				return;
			}
		}
		current = slots[i];
		if (current != 0) {
			current->~CBaseRemap();
			operator delete(current);
		}
		m_remaps[i] = 0;
		m_remapCount = m_remapCount - 1;
	}
}

// GLOBAL: LEMBALL 0x004a2000
CBasePalManager* g_pBasePalManager = 0;
