#include "BasePalManager.h"

#include "BaseRemap.h"
#include "Remap.h"

#include <new.h>

BasePalManager::BasePalManager()
{
}

// 68K 0x10200c72 __ct__15CBasePalManagerFi
// FUNCTION: LEMBALL 0x0046acd0
BasePalManager::BasePalManager(int p_arg0)
{
	int i;

	m_capacity = p_arg0;
	m_remapCount = 0;
	m_remaps = (BaseRemap**) operator new(p_arg0 * sizeof(BaseRemap*));
	i = 0;
	if (0 < m_capacity) {
		do {
			m_remaps[i] = 0;
			i = i + 1;
		} while (i < m_capacity);
	}
}

// 68K 0x10200cd2 __dt__15CBasePalManagerFv
// FUNCTION: LEMBALL 0x0046ad10
BasePalManager::~BasePalManager()
{
	int i;
	BaseRemap* remap;

	if (m_remaps != 0 && 0 < m_remapCount) {
		i = 0;
		do {
			remap = m_remaps[i];
			if (remap != 0) {
				remap->~BaseRemap();
				operator delete(remap);
			}
			m_remaps[i] = 0;
			i = i + 1;
		} while (i < m_remapCount);
	}
	if (m_remaps != 0) {
		operator delete(m_remaps);
	}
}

// 68K 0x10200d5a RegisterRemap__15CBasePalManagerFUlPUc13ePaletteTypes
// FUNCTION: LEMBALL 0x0046ad70
BaseRemap* BasePalManager::RegisterRemap(unsigned int p_resourceId, unsigned char* p_mapping, ePaletteTypes p_type)
{
	BaseRemap** slot;
	void* storage;
	BaseRemap* remap;
	int i;

	slot = m_remaps;
	i = 0;
	remap = *slot;
	while (remap != 0) {
		slot = slot + 1;
		i = i + 1;
		remap = *slot;
	}
	storage = operator new(sizeof(Remap));
	if (storage == 0) {
		m_remaps[i] = 0;
	}
	else {
		m_remaps[i] = new (storage) Remap(p_resourceId, p_mapping, p_type);
	}
	m_remapCount = m_remapCount + 1;
	return m_remaps[i];
}

// 68K 0x10200df0 UnRegisterRemap__15CBasePalManagerFP6CRemap
// FUNCTION: LEMBALL 0x0046add0
void BasePalManager::UnRegisterRemap(BaseRemap* p_remap)
{
	int i;
	int counted;
	int remapCount;
	BaseRemap** slots;
	BaseRemap* current;

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
			current->~BaseRemap();
			operator delete(current);
		}
		m_remaps[i] = 0;
		m_remapCount = m_remapCount - 1;
	}
}

// GLOBAL: LEMBALL 0x004a2000
BasePalManager* g_pBasePalManager = 0;
