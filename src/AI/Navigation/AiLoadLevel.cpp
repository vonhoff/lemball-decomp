#include "../../Map/Base/Map.h"
#include "Ai.h"

// Four rows of four 32-bit entries, indexed by trapdoor count minus one.
// The x86 folds the -1 into its 0x0049cf50 displacement; the data starts at 0x0049cf60.
// GLOBAL: LEMBALL 0x0049cf60
static int g_unk0049cf60[4][4] = {{4, 0, 0, 0}, {3, 1, 0, 0}, {2, 1, 1, 0}, {1, 1, 1, 1}};

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

// 68K 0x10602a54 SetNetworkTrapDoors__3CAIFiiiii
// FUNCTION: LEMBALL 0x004131e0
void Ai::SetNetworkTrapDoors(int p_count, int p_first, int p_second, int p_third, int p_fourth)
{
	m_networkTrapDoorCount = p_count;
	if (p_first == -1) {
		m_networkTrapDoors[0] = g_unk0049cf60[p_count - 1][0];
		m_networkTrapDoors[1] = g_unk0049cf60[p_count - 1][1];
		m_networkTrapDoors[2] = g_unk0049cf60[p_count - 1][2];
		m_networkTrapDoors[3] = g_unk0049cf60[p_count - 1][3];
	}
	else {
		m_networkTrapDoors[0] = p_first;
		m_networkTrapDoors[2] = p_third;
		m_networkTrapDoors[1] = p_second;
		m_networkTrapDoors[3] = p_fourth;
	}
	for (int i = 0; i < p_count; i++) {
		if (m_networkStartsX[i] > 1024 || m_networkStartsX[i] < 0) {
			m_networkStartsX[i] = i * 16;
		}
		if (m_networkStartsY[i] > 1024 || m_networkStartsY[i] < 0) {
			m_networkStartsY[i] = i * 16;
		}
	}
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
