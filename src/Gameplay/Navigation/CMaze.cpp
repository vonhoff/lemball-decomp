#include "CMaze.h"

#include "Engine/Time/VsTime.h"
#include "Map/CGround.h"
#include "Map/CGroundArray.h"
#include "Map/CMap.h"
#include "tSolution.h"

#include <string.h>

extern const int g_mazeNeighborOffsetsX[10];
extern const int g_mazeNeighborOffsetsY[10];
extern const int g_mazeCardinalNeighbors[10];
extern const unsigned char g_mazeWalkMasks[32];

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

// GLOBAL: LEMBALL 0x0049cf58
CMaze* g_pMaze = NULL;

extern const unsigned char g_aChangeBitMasks[8][4];

// FUNCTION: LEMBALL 0x00423090
CMaze::CMaze(CMap* p_map)
{
	m_map = p_map;
	m_distances = NULL;
	m_changeSelect = 0;
	m_routeSearchBusy = false;
	m_width = 0;
	m_height = 0;
}

// FUNCTION: LEMBALL 0x004230c0
CMaze::~CMaze()
{
	int row = 0;
	if (m_distances != NULL) {
		if (m_height > 0) {
			do {
				delete[] m_distances[row];
				row++;
			} while (row < m_height);
		}
		delete[] m_distances;
	}
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
						collision = GROUND_COLLISION_OUT_OF_BOUNDS;
					}
					else {
						map = m_map;
						width = map->m_ground.m_width;
						if (width <= x || map->m_ground.m_height <= y) {
							collision = GROUND_COLLISION_OUT_OF_BOUNDS;
						}
						else {
							collision = map->m_ground.m_ground[y * width + x].m_collision;
						}
					}
					if ((collision & GROUND_COLLISION_BLOCKS_WALKING) != 0) {
						m_distances[y][x] = MAZE_DISTANCE_BLOCKED;
					}
					else {
						m_distances[y][x] = MAZE_DISTANCE_UNREACHED;
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
	if (m_distances != NULL) {
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
	int yMax;
	if (p_y < m_height - 1) {
		yMax = p_y + 1;
	}
	else {
		yMax = p_y;
	}

	unsigned char* pChange;
	if (m_changeSelect != 0) {
		pChange =
			m_changeB + ((yMin * MAZE_CHANGE_BITMAP_ROW_WIDTH_BITS + xMin) >> MAZE_CHANGE_BITMAP_BYTE_INDEX_SHIFT);
	}
	else {
		pChange =
			m_changeA + ((yMin * MAZE_CHANGE_BITMAP_ROW_WIDTH_BITS + xMin) >> MAZE_CHANGE_BITMAP_BYTE_INDEX_SHIFT);
	}
	unsigned char* pOther;
	if (m_changeSelect != 0) {
		pOther = m_changeA + ((yMin * MAZE_CHANGE_BITMAP_ROW_WIDTH_BITS + xMin) >> MAZE_CHANGE_BITMAP_BYTE_INDEX_SHIFT);
	}
	else {
		pOther = m_changeB + ((yMin * MAZE_CHANGE_BITMAP_ROW_WIDTH_BITS + xMin) >> MAZE_CHANGE_BITMAP_BYTE_INDEX_SHIFT);
	}

	unsigned char mask = g_aChangeBitMasks[xMin & MAZE_CHANGE_BITMAP_BIT_INDEX_MASK][0];
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
				currentMask = MAZE_CHANGE_BITMAP_FIRST_COLUMN_MASK;
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
			int rowBytes = m_width / MAZE_CHANGE_BITMAP_BITS_PER_BYTE;
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

// FUNCTION: LEMBALL 0x00423530
void CMaze::BInitialise(unsigned int p_resetStats, int p_startX, int p_startY, int p_endX, int p_endY)
{
	int x;
	int width;
	CMap* map;
	if (p_resetStats != 0) {
		m_totalTime = 0;
		m_solutionCount = 0;
	}
	m_startTime = CurrentMilliTimer();
	m_radius = 0;
	m_startX = p_startX;
	m_startY = p_startY;
	m_endX = p_endX;
	m_endY = p_endY;

	int y = 0;
	if (m_height > 0) {
		do {
			x = 0;
			if (m_width > 0) {
				do {
					unsigned short collision;
					if (x < 0 || y < 0) {
						collision = GROUND_COLLISION_OUT_OF_BOUNDS;
					}
					else {
						map = m_map;
						width = map->m_ground.m_width;
						if (width <= x || map->m_ground.m_height <= y) {
							collision = GROUND_COLLISION_OUT_OF_BOUNDS;
						}
						else {
							collision = map->m_ground.m_ground[y * width + x].m_collision;
						}
					}
					if ((collision & GROUND_COLLISION_BLOCKS_WALKING) != 0) {
						m_distances[y][x] = MAZE_DISTANCE_BLOCKED;
					}
					else {
						m_distances[y][x] = MAZE_DISTANCE_UNREACHED;
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

// FUNCTION: LEMBALL 0x00423650
bool CMaze::BIteration(bool& p_reached, bool& p_noChanges)
{
	p_reached = false;
	if (m_endY < 0 || m_endX < 0 || m_height <= m_endY || m_width <= m_endX ||
		m_distances[m_endY][m_endX] == MAZE_DISTANCE_BLOCKED) {
		return true;
	}

	bool changed = false;
	SwapChange();
	int radius = ++m_radius;
	int xMin = m_startX - radius;
	int xMax = m_startX + radius;
	int yMin = m_startY - radius;
	int yMax = m_startY + radius;
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

	unsigned char* pChange;
	if (m_changeSelect != 0) {
		pChange =
			m_changeA + ((yMin * MAZE_CHANGE_BITMAP_ROW_WIDTH_BITS + xMin) >> MAZE_CHANGE_BITMAP_BYTE_INDEX_SHIFT);
	}
	else {
		pChange =
			m_changeB + ((yMin * MAZE_CHANGE_BITMAP_ROW_WIDTH_BITS + xMin) >> MAZE_CHANGE_BITMAP_BYTE_INDEX_SHIFT);
	}

	if (yMin <= yMax) {
		unsigned int mask;
		memcpy(&mask, &g_aChangeBitMasks[xMin & MAZE_CHANGE_BITMAP_BIT_INDEX_MASK][0], 1);
		int y = yMin;
		do {
			unsigned short* pDistance = m_distances[y] + xMin;
			unsigned char* pChangeRow = pChange;
			int x;
			unsigned char currentMask;
			memcpy(&currentMask, &mask, 1);
			for (x = xMin; x <= xMax; x++, pDistance++) {
				if ((currentMask & *pChangeRow) != 0 && *pDistance != MAZE_DISTANCE_BLOCKED && CalcNewDistance(x, y)) {
					changed = true;
					UpdateChangeNext(x, y);
				}
				currentMask >>= 1;
				if (currentMask == 0) {
					currentMask = MAZE_CHANGE_BITMAP_FIRST_COLUMN_MASK;
					pChangeRow++;
				}
			}
			pChange += 0x10;
			y++;
		} while (y <= yMax);
	}

	p_reached = m_distances[m_endY][m_endX] != MAZE_DISTANCE_UNREACHED;
	p_noChanges = !p_reached || changed ? false : true;
	if (!p_reached && changed && m_radius < 0x14) {
		return false;
	}
	return true;
}

// FUNCTION: LEMBALL 0x00423890
int Direction(int p_x0, int p_y0, int p_x1, int p_y1)
{
	return ((p_y0 - p_y1) * 3 - p_x1) + 4 + p_x0;
}

// FUNCTION: LEMBALL 0x004238b0
void CMaze::BSolution(int& p_count, tSolution* p_solution)
{
	p_count = 0;
	int x = m_endX;
	int y = m_endY;
	int distance = m_distances[y][x];
	p_solution->m_x = (short) x;
	p_solution[p_count].m_y = (short) y;
	p_count++;

	int nextDirection;
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
			nextDirection = Direction(previousX, previousY, x, y);
			if (direction != nextDirection) {
				p_solution[p_count].m_x = (short) previousX;
				p_solution[p_count].m_y = (short) previousY;
				p_count++;
				direction = nextDirection;
			}
		}
	}

	unsigned long elapsed = CurrentMilliTimer() - m_startTime;
	m_startTime = elapsed;
	m_totalTime += elapsed;
	m_solutionCount++;
}
