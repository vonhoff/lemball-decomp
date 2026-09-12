#include "Maze.h"

#include "../../Map/Base/Map.h"
#include "../Base/Solution.h"

#include <string.h>

extern "C" unsigned long __stdcall timeGetTime(void);

extern const int g_unk0x00495b30[10];
extern const int g_unk0x00495b58[10];
extern const int g_unk0x00495b80[10];
extern const unsigned char g_unk0x00495ba8[32];

// 68K 0x10615900 __ct__5CMazeFP4CMap
// FUNCTION: LEMBALL 0x00423090
Maze::Maze(Map* p_arg0)
{
	m_map = p_arg0;
	m_distances = 0;
	m_changeSelect = 0;
	m_reserved = 0;
	m_width = 0;
	m_height = 0;
}

// 68K 0x1061593a __dt__5CMazeFv
// FUNCTION: LEMBALL 0x004230c0
Maze::~Maze()
{
	int row = 0;
	if (m_distances != 0) {
		if (m_height > 0) {
			do {
				delete[] m_distances[row];
				row++;
			} while (row < m_height);
		}
		delete[] m_distances;
	}
}

// 68K 0x106159a4 ReInitialise__5CMazeFv
// FUNCTION: LEMBALL 0x00423110
void Maze::ReInitialise()
{
	int y = 0;
	if (m_height > 0) {
		do {
			int x = 0;
			if (m_width > 0) {
				do {
					unsigned short collision;
					if (x < 0 || y < 0) {
						collision = 3;
					}
					else {
						int width;
						Map* map = m_map;
						width = map->m_ground.m_width;
						if (width <= x || map->m_ground.m_height <= y) {
							collision = 3;
						}
						else {
							collision = map->m_ground.m_ground[y * width + x].m_collision;
						}
					}
					if ((collision & 1) != 0) {
						m_distances[y][x] = 0xffff;
					}
					else {
						m_distances[y][x] = 0xff00;
					}
					x++;
				} while (x < m_width);
			}
			y++;
		} while (y < m_height);
	}
}

// 68K 0x10615a26 Initialise__5CMazeFv
// FUNCTION: LEMBALL 0x00423190
void Maze::Initialise()
{
	if (m_distances != 0) {
		int row = 0;
		if (m_height > 0) {
			do {
				delete[] m_distances[row];
				row++;
			} while (row < m_height);
		}
		delete[] m_distances;
	}

	m_width = m_map->m_ground.m_width;
	m_height = m_map->m_ground.m_height;
	m_distances = new unsigned short*[m_height];
	for (int row = 0; row < m_height; row++) {
		m_distances[row] = new unsigned short[m_width];
	}
	ReInitialise();
}

// 68K 0x10615ad8 CalcNewDistance__5CMazeFii
// FUNCTION: LEMBALL 0x00423230
bool Maze::CalcNewDistance(int p_x, int p_y)
{
	unsigned short distance = m_distances[p_y][p_x];
	unsigned char walk = m_map->GetWalk(p_x, p_y);
	bool changed = false;
	for (int i = 0; i < 9; i++) {
		if (g_unk0x00495b80[i] != 0 && (g_unk0x00495ba8[i] & walk) != 0 && (g_unk0x00495ba8[i + 16] & walk) != 0) {
			unsigned short newDistance = m_distances[p_y + g_unk0x00495b58[i]][p_x + g_unk0x00495b30[i]];
			if (newDistance < distance) {
				changed = true;
				distance = newDistance;
			}
		}
	}
	if (changed) {
		m_distances[p_y][p_x] = distance + 1;
	}
	return changed;
}

// 68K 0x10615bd0 FindSquare__5CMazeFUsRiRi
// FUNCTION: LEMBALL 0x004232e0
bool Maze::FindSquare(unsigned short p_distance, int& p_x, int& p_y)
{
	int x = p_x;
	int y = p_y;
	unsigned char walk = m_map->GetWalk(x, y);
	bool found = false;
	for (int i = 0; i < 9; i++) {
		if (g_unk0x00495b80[i] != 0 && (g_unk0x00495ba8[i] & walk) != 0) {
			int nextX = x + g_unk0x00495b30[i];
			int nextY = y + g_unk0x00495b58[i];
			if (m_distances[nextY][nextX] == p_distance) {
				p_x = nextX;
				p_y = nextY;
				found = true;
				break;
			}
		}
	}
	return found;
}

// GLOBAL: LEMBALL 0x00495b10
static const unsigned char g_aChangeBitMasks[8][4] = {{0x80, 0, 0, 0},
													  {0x40, 0, 0, 0},
													  {0x20, 0, 0, 0},
													  {0x10, 0, 0, 0},
													  {0x08, 0, 0, 0},
													  {0x04, 0, 0, 0},
													  {0x02, 0, 0, 0},
													  {0x01, 0, 0, 0}};

// GLOBAL: LEMBALL 0x00495b30
const int g_unk0x00495b30[10] = {-1, 0, 1, -1, 0, 1, -1, 0, 1, 0};

// GLOBAL: LEMBALL 0x00495b58
const int g_unk0x00495b58[10] = {-1, -1, -1, 0, 0, 0, 1, 1, 1, 0};

// GLOBAL: LEMBALL 0x00495b80
const int g_unk0x00495b80[10] = {0, 1, 0, 1, 0, 1, 0, 1, 0, 0};

// GLOBAL: LEMBALL 0x00495ba8
const unsigned char g_unk0x00495ba8[32] = {0, 1,  0, 8,   0, 4,  0, 2,  0, 0, 0, 0, 0, 0, 0, 0,
										   0, 16, 0, 128, 0, 64, 0, 32, 0, 0, 0, 0, 0, 0, 0, 0};

// 68K 0x10615c8c UpdateChangeNext__5CMazeFii
// FUNCTION: LEMBALL 0x00423380
void Maze::UpdateChangeNext(int p_x, int p_y)
{
	int xMin = p_x;
	if (p_x > 0) {
		xMin = p_x - 1;
	}
	int yMin = p_y;
	if (p_y > 0) {
		yMin = p_y - 1;
	}
	int xMax = p_x;
	if (p_x < m_width - 1) {
		xMax = p_x + 1;
	}
	int yMax = p_y;
	if (p_y < m_height - 1) {
		yMax = p_y + 1;
	}

	int offset = (yMin * 0x80 + xMin) >> 3;
	unsigned char* pChange;
	unsigned char* pOther;
	if (m_changeSelect == 0) {
		pChange = m_changeA + offset;
		pOther = m_changeB + offset;
	}
	else {
		pChange = m_changeB + offset;
		pOther = m_changeA + offset;
	}

	unsigned char mask = g_aChangeBitMasks[xMin & 7][0];
	for (int y = yMin; y <= yMax; y++) {
		unsigned char currentMask = mask;
		unsigned char* pChangeRow = pChange;
		unsigned char* pOtherRow = pOther;
		int x = xMin;
		while (x <= xMax) {
			if (x != p_x || y != p_y) {
				*pChangeRow |= currentMask;
				*pOtherRow |= currentMask;
			}
			currentMask >>= 1;
			if (currentMask == 0) {
				pChangeRow++;
				pOtherRow++;
				currentMask = 0x80;
			}
			x++;
		}
		pChange += 0x10;
		pOther += 0x10;
	}
}

// 68K 0x10615da4 Clear__5CMazeFPUc
// FUNCTION: LEMBALL 0x004234a0
void Maze::Clear(unsigned char* p_change)
{
	int row = 0;
	if (m_height > 0) {
		do {
			row++;
			int rowBytes = (m_width + (m_width >> 31 & 7)) >> 3;
			memset(p_change, 0, rowBytes);
			p_change += 0x10;
		} while (row < m_height);
	}
}

// 68K 0x10615e04 SwapChange__5CMazeFv
// FUNCTION: LEMBALL 0x004234f0
void Maze::SwapChange()
{
	if (m_changeSelect != 0) {
		Clear(m_changeA);
	}
	else {
		Clear(m_changeB);
	}
	m_changeSelect = m_changeSelect == 0;
}

// 68K 0x10615e5a BInitialise__5CMazeFUciiii
// FUNCTION: LEMBALL 0x00423530
void Maze::BInitialise(unsigned char p_resetStats, int p_startX, int p_startY, int p_endX, int p_endY)
{
	if (p_resetStats != 0) {
		m_totalTime = 0;
		m_solutionCount = 0;
	}
	m_startTime = timeGetTime();
	m_radius = 0;
	m_startX = p_startX;
	m_startY = p_startY;
	m_endX = p_endX;
	m_endY = p_endY;

	int y = 0;
	if (m_height > 0) {
		do {
			int x = 0;
			if (m_width > 0) {
				do {
					unsigned short collision;
					if (x < 0 || y < 0) {
						collision = 3;
					}
					else {
						Map* map = m_map;
						int width = map->m_ground.m_width;
						if (width <= x || map->m_ground.m_height <= y) {
							collision = 3;
						}
						else {
							collision = map->m_ground.m_ground[y * width + x].m_collision;
						}
					}
					if ((collision & 1) != 0) {
						m_distances[y][x] = 0xffff;
					}
					else {
						m_distances[y][x] = 0xff00;
					}
					x++;
				} while (x < m_width);
			}
			y++;
		} while (y < m_height);
	}

	m_changeSelect = 0;
	Clear(m_changeA);
	Clear(m_changeB);
	UpdateChangeNext(m_startX, m_startY);
	m_distances[m_startY][m_startX] = 0;
}

// 68K 0x10615f52 BIteration__5CMazeFRUcRUc
// FUNCTION: LEMBALL 0x00423650
bool Maze::BIteration(unsigned int& p_reached, unsigned int& p_noChanges)
{
	p_reached = 0;
	if (m_endY < 0 || m_endX < 0 || m_height <= m_endY || m_width <= m_endX || m_distances[m_endY][m_endX] == 0xffff) {
		return 1;
	}

	bool changed = false;
	SwapChange();
	int radius = m_radius + 1;
	int xMin = m_startX - radius;
	int xMax = m_startX + radius;
	int yMax = m_startY + radius;
	int yMin = m_startY - radius;
	m_radius = radius;
	if (xMin < 0) {
		xMin = 0;
	}
	if (m_width - 1 < xMax) {
		xMax = m_width - 1;
	}
	if (yMin < 0) {
		yMin = 0;
	}
	if (m_height - 1 < yMax) {
		yMax = m_height - 1;
	}

	int offset = (yMin * 0x80 + xMin) >> 3;
	unsigned char* pChange;
	if (m_changeSelect == 0) {
		pChange = m_changeB + offset;
	}
	else {
		pChange = m_changeA + offset;
	}

	if (yMin <= yMax) {
		unsigned char mask = g_aChangeBitMasks[xMin & 7][0];
		for (int y = yMin; y <= yMax; y++) {
			unsigned short* pDistance = m_distances[y] + xMin;
			unsigned char currentMask = mask;
			unsigned char* pChangeRow = pChange;
			for (int x = xMin; x <= xMax; x++) {
				if ((currentMask & *pChangeRow) != 0 && *pDistance != 0xffff && CalcNewDistance(x, y)) {
					changed = true;
					UpdateChangeNext(x, y);
				}
				currentMask >>= 1;
				if (currentMask == 0) {
					currentMask = 0x80;
					pChangeRow++;
				}
				pDistance++;
			}
			pChange += 0x10;
		}
	}

	p_reached = m_distances[m_endY][m_endX] != 0xff00;
	p_noChanges = !p_reached || changed ? 0 : 1;
	if (!p_reached && changed && m_radius < 0x14) {
		return 0;
	}
	return 1;
}

// 68K 0x10616122 Direction__Fiiii
// FUNCTION: LEMBALL 0x00423890
int Direction(int p_x0, int p_y0, int p_x1, int p_y1)
{
	return ((p_y0 - p_y1) * 3 - p_x1) + 4 + p_x0;
}

// 68K 0x1061616a BSolution__5CMazeFRiP9tSolution
// FUNCTION: LEMBALL 0x004238b0
void Maze::BSolution(int& p_count, Solution* p_solution)
{
	p_count = 0;
	int x = m_endX;
	int y = m_endY;
	int distance = m_distances[y][x];
	p_solution->m_x = (short) x;
	p_solution[p_count].m_y = (short) y;
	p_count++;

	int previousX = x;
	int previousY = y;
	if (FindSquare(--distance, x, y)) {
		distance--;
		int direction = Direction(previousX, previousY, x, y);
		while (distance >= 0) {
			previousX = x;
			previousY = y;
			FindSquare(distance, x, y);
			distance--;
			int nextDirection = Direction(previousX, previousY, x, y);
			if (direction != nextDirection) {
				p_solution[p_count].m_x = (short) previousX;
				p_solution[p_count].m_y = (short) previousY;
				p_count++;
				direction = nextDirection;
			}
		}
	}

	unsigned long elapsed = timeGetTime() - m_startTime;
	m_startTime = elapsed;
	m_totalTime += elapsed;
	m_solutionCount++;
}

// GLOBAL: LEMBALL 0x0049cf58
Maze* g_pMaze = 0;
