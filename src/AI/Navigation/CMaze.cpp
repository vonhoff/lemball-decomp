#include "CMaze.h"

#include "../../Map/Base/CMap.h"
#include "../Base/Solution.h"

#include <string.h>

extern "C" unsigned long __stdcall timeGetTime(void);

extern const int g_mazeNeighborOffsetsX[10];
extern const int g_mazeNeighborOffsetsY[10];
extern const int g_mazeCardinalNeighbors[10];
extern const unsigned char g_mazeWalkMasks[32];

// FUNCTION: LEMBALL 0x00423090
CMaze::CMaze(CMap* p_map)
{
	m_map = p_map;
	m_distances = 0;
	m_changeSelect = 0;
	m_routeSearchBusy = 0;
	m_width = 0;
	m_height = 0;
}

// FUNCTION: LEMBALL 0x00423110
void CMaze::ReInitialise()
{
	int width;
	CMap* map;
	unsigned short collision;
	int y = 0;
	int x;
	if (m_height > 0) {
		do {
			x = 0;
			if (m_width > 0) {
				do {
					if (x < 0 || y < 0) {
						collision = 3;
					}
					else {
						map = m_map;
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

// FUNCTION: LEMBALL 0x00423190
void CMaze::Initialise()
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

// FUNCTION: LEMBALL 0x00423230
bool CMaze::CalcNewDistance(int p_x, int p_y)
{
	unsigned short distance = m_distances[p_y][p_x];
	unsigned char walk = m_map->GetWalk(p_x, p_y);
	bool changed = false;
	for (int i = 0; i < 9; i++) {
		if (g_mazeCardinalNeighbors[i] != 0 && (g_mazeWalkMasks[i] & walk) != 0 &&
			(g_mazeWalkMasks[i + 16] & walk) != 0) {
			unsigned short newDistance = m_distances[p_y + g_mazeNeighborOffsetsY[i]][p_x + g_mazeNeighborOffsetsX[i]];
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

// FUNCTION: LEMBALL 0x004232e0
bool CMaze::FindSquare(unsigned short p_distance, int& p_x, int& p_y)
{
	int x = p_x;
	int y = p_y;
	unsigned char walk = m_map->GetWalk(x, y);
	bool found = false;
	for (int i = 0; i < 9; i++) {
		if (g_mazeCardinalNeighbors[i] != 0 && (g_mazeWalkMasks[i] & walk) != 0) {
			int nextX = x + g_mazeNeighborOffsetsX[i];
			int nextY = y + g_mazeNeighborOffsetsY[i];
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
extern const unsigned char g_aChangeBitMasks[8][4] = {{0x80, 0, 0, 0},
													  {0x40, 0, 0, 0},
													  {0x20, 0, 0, 0},
													  {0x10, 0, 0, 0},
													  {0x08, 0, 0, 0},
													  {0x04, 0, 0, 0},
													  {0x02, 0, 0, 0},
													  {0x01, 0, 0, 0}};

// GLOBAL: LEMBALL 0x00495b30
const int g_mazeNeighborOffsetsX[10] = {-1, 0, 1, -1, 0, 1, -1, 0, 1, 0};

// GLOBAL: LEMBALL 0x00495b58
const int g_mazeNeighborOffsetsY[10] = {-1, -1, -1, 0, 0, 0, 1, 1, 1, 0};

// GLOBAL: LEMBALL 0x00495b80
const int g_mazeCardinalNeighbors[10] = {0, 1, 0, 1, 0, 1, 0, 1, 0, 0};

// GLOBAL: LEMBALL 0x00495ba8
const unsigned char g_mazeWalkMasks[32] = {0, 1,  0, 8,   0, 4,  0, 2,  0, 0, 0, 0, 0, 0, 0, 0,
										   0, 16, 0, 128, 0, 64, 0, 32, 0, 0, 0, 0, 0, 0, 0, 0};

// FUNCTION: LEMBALL 0x00423380
void CMaze::UpdateChangeNext(int p_x, int p_y)
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

	unsigned int mask;
	memcpy(&mask, &g_aChangeBitMasks[xMin & 7][0], 1);
	for (int y = yMin; y <= yMax; y++) {
		unsigned char currentMask;
		memcpy(&currentMask, &mask, 1);
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

// FUNCTION: LEMBALL 0x004234a0
void CMaze::Clear(unsigned char* p_change)
{
	int row = 0;
	if (m_height > 0) {
		do {
			int rowBytes = m_width / 8;
			memset(p_change, 0, rowBytes);
			p_change += 0x10;
			row++;
		} while (row < m_height);
	}
}

// FUNCTION: LEMBALL 0x004234f0
void CMaze::SwapChange()
{
	if (m_changeSelect != 0) {
		Clear(m_changeA);
	}
	else {
		Clear(m_changeB);
	}
	m_changeSelect = m_changeSelect == 0;
}

// FUNCTION: LEMBALL 0x00423890
int Direction(int p_x0, int p_y0, int p_x1, int p_y1)
{
	return ((p_y0 - p_y1) * 3 - p_x1) + 4 + p_x0;
}

// FUNCTION: LEMBALL 0x004238b0
void CMaze::BSolution(int& p_count, Solution* p_solution)
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
CMaze* g_pMaze = 0;

// GLOBAL: LEMBALL 0x0049cf60
int g_anDefaultTrapDoorLemmings[4][4] = {{4, 0, 0, 0}, {3, 1, 0, 0}, {2, 1, 1, 0}, {1, 1, 1, 1}};
