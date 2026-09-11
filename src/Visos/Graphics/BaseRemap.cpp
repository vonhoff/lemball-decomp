#include "BaseRemap.h"

#include "../Resources/ResBase.h"
#include "../Resources/ResPalette.h"

#include <new.h>

BaseRemap::BaseRemap()
{
}

// 68K 0x10200920 __ct__10CBaseRemapFUlPUc13ePaletteTypes
// FUNCTION: LEMBALL 0x0046aa80
BaseRemap::BaseRemap(unsigned long p_arg0, unsigned char* p_arg1, ePaletteTypes p_arg2)
{
	m_paletteResource = ResPalette::Load(p_arg0);
	if (p_arg2 == 1) {
		CalculateGreyScale();
		return;
	}
	if (p_arg2 != 2) {
		m_remap = p_arg1;
		return;
	}
	MapRemap(p_arg1);
}

// 68K 0x1020099e __dt__10CBaseRemapFv
// FUNCTION: LEMBALL 0x0046aad0
BaseRemap::~BaseRemap()
{
	if (m_remap != 0) {
		operator delete(m_remap);
	}
	if (m_paletteResource != 0) {
		m_paletteResource->UnLoad();
	}
}

// 68K 0x102009fc MapRemap__10CBaseRemapFPCUc
// FUNCTION: LEMBALL 0x0046aaf0
void BaseRemap::MapRemap(const unsigned char* p_mapping)
{
	ResPalette* palette = m_paletteResource;
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

// 68K 0x10200aae CalculateGreyScale__10CBaseRemapFv
// FUNCTION: LEMBALL 0x0046ab70
void BaseRemap::CalculateGreyScale()
{
	volatile short grey;
	ResPalette* palette = m_paletteResource;
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
