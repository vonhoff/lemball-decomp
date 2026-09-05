#include "../../Map/Base/Map.h"
#include "Ai.h"

// 68K 0x10602750 LoadLevel__3CAIFPUciUc
// FUNCTION: LEMBALL 0x00412f10
void Ai::LoadLevel(unsigned char* p_data, int p_dataSize, unsigned char p_skip)
{
	unsigned short* data;
	if (p_dataSize > 4) {
		data = (unsigned short*) p_data;
		m_levelVersion = *data++;
	}
	else {
		m_levelVersion = 0;
		data = (unsigned short*) p_data;
	}
	unsigned int mapType = *data++;
	m_unk0x60 = mapType;
	m_unk0xec = *data++;
	Map* map = m_map;
	map->m_reserved = mapType;
	map->m_ground.Clear();
	if (m_levelVersion >= 4) {
		m_lemmingCount = data[0];
		m_flagCounts[0] = data[1];
	}
	else {
		m_lemmingCount = 4;
		m_flagCounts[0] = 1;
	}
	m_unk0xe4 = m_unk0xec;
}

// 68K 0x10602b0a LoadFlagInfo__3CAIFPUci
// FUNCTION: LEMBALL 0x00413300
void Ai::LoadFlagInfo(unsigned char* p_data, int p_size)
{
	unsigned short* data = (unsigned short*) p_data;
	if (m_unk0x64 == 1) {
		if (m_unk0x7c == 1) {
			m_flagCounts[0] = data[0];
			m_flagCounts[1] = data[1];
		}
		else {
			m_flagCounts[1] = data[0];
			m_flagCounts[0] = data[1];
		}
	}
	else {
		m_flagCounts[0] = data[0];
		m_flagCounts[1] = data[1];
	}
}
