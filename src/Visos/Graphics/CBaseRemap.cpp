#include "CBaseRemap.h"

#include "../Resources/CResPALETTE.h"

// FUNCTION: LEMBALL 0x0046aa80
CBaseRemap::CBaseRemap(unsigned long p_resourceId, unsigned char* p_mapping, ePaletteTypes p_type)
{
	m_paletteResource = CResPALETTE::Load(p_resourceId);
	switch (p_type) {
	case PALETTE_GREYSCALE:
		CalculateGreyScale();
		break;
	case PALETTE_MAPPED:
		MapRemap(p_mapping);
		break;
	default:
		m_remap = p_mapping;
		break;
	}
}

// FUNCTION: LEMBALL 0x0046aad0
CBaseRemap::~CBaseRemap()
{
	if (m_remap != 0) {
		operator delete(m_remap);
	}
	if (m_paletteResource != 0) {
		m_paletteResource->UnLoad();
	}
}

// FUNCTION: LEMBALL 0x0046aaf0
void CBaseRemap::MapRemap(const unsigned char* p_mapping)
{
	CResPALETTE* palette = m_paletteResource;
	if (palette->m_loaded != 0) {
		palette->m_age = 0;
	}
	else {
		palette->LoadData();
	}
	++palette->m_directUseCount;

	unsigned int paletteSize = (unsigned int) m_paletteResource->m_paletteState;
	m_remap = new unsigned char[paletteSize];
	unsigned int mappingCount = *p_mapping++;
	int i;
	for (i = 0; i < (int) paletteSize; ++i) {
		m_remap[i] = (unsigned char) i;
	}
	for (i = 0; i < (int) mappingCount; ++i) {
		unsigned char mappedColour = p_mapping[mappingCount + i];
		if (mappedColour != 0) {
			m_remap[p_mapping[i]] = mappedColour;
		}
	}

	--m_paletteResource->m_directUseCount;
}

#include <stdlib.h>

// FUNCTION: LEMBALL 0x0046ab70
void CBaseRemap::CalculateGreyScale()
{
	volatile short grey;
	CResPALETTE* palette = m_paletteResource;
	if (palette->m_loaded != 0) {
		palette->m_age = 0;
	}
	else {
		palette->LoadData();
	}
	++palette->m_directUseCount;

	unsigned int paletteSize = (unsigned int) m_paletteResource->m_paletteState;
	m_remap = new unsigned char[paletteSize];
	unsigned char* greyIndices = new unsigned char[paletteSize];
	int greyCount = 0;
	int paletteIndex;
	int greyIndex;

	for (paletteIndex = 1; paletteIndex < (int) paletteSize - 1; ++paletteIndex) {
		unsigned char* colour = m_paletteResource->m_data + paletteIndex * 4;
		if (colour[0] == colour[1] && colour[2] == colour[1]) {
			greyIndices[greyCount++] = (unsigned char) paletteIndex;
		}
	}

	for (paletteIndex = 1; paletteIndex < (int) paletteSize - 1; ++paletteIndex) {
		unsigned char* colour = m_paletteResource->m_data + paletteIndex * 4;
		grey = (short) (((short) ((unsigned short) colour[0] * 2) + (short) ((unsigned short) colour[1] * 7) +
						 (int) colour[2]) /
						10);
		unsigned int bestIndex = (unsigned int) -1;
		int bestDistance = 0x7fffffff;

		greyIndex = 0;
		for (;;) {
			if (greyIndex >= greyCount) {
				break;
			}
			unsigned int candidate = greyIndices[greyIndex];
			int distance = abs((int) m_paletteResource->m_data[candidate * 4] - grey);
			if (bestDistance > distance) {
				bestDistance = distance;
				bestIndex = candidate;
			}
			++greyIndex;
		}
		m_remap[paletteIndex] = (unsigned char) bestIndex;
	}

	operator delete(greyIndices);
	--m_paletteResource->m_directUseCount;
}
