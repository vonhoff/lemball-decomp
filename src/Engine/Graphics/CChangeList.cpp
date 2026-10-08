#include "Engine/Graphics/CChangeList.h"

#include "Engine/Graphics/ChangeListItem.h"
#include "Engine/Math/CVSRect.h"
#include "Engine/Math/CVSSize.h"

#include <string.h>

enum {
	CHANGE_LIST_ITEMS_NEED_REBUILD = -1
};

// FUNCTION: LEMBALL 0x004669a0
CChangeList::CChangeList(int p_capacity, CVSSize p_viewSize, CVSSize p_cellSize)
{
	if (p_capacity != 0) {
		m_items = new ChangeListItem[p_capacity];
	}
	else {
		m_items = NULL;
	}
	m_capacity = p_capacity;
	m_cellSize.m_width = p_cellSize.m_width;
	m_cellSize.m_height = p_cellSize.m_height;
	m_viewSize.m_width = p_viewSize.m_width;
	m_viewSize.m_height = p_viewSize.m_height;
	m_mapSize.m_width = (short) (p_viewSize.m_width + p_cellSize.m_width - 1) / p_cellSize.m_width;
	m_mapSize.m_height = (short) (p_viewSize.m_height + p_cellSize.m_height - 1) / p_cellSize.m_height;
	m_activeDepth = 0;
	m_map = NULL;
	AllocMap();
	Reset();
}

// FUNCTION: LEMBALL 0x00466a90
void CChangeList::FreeMap()
{
	if (m_map != NULL) {
		operator delete(m_map);
		m_map = NULL;
	}
	Reset();
}

// FUNCTION: LEMBALL 0x00466ac0
void CChangeList::AllocMap()
{
	unsigned int capacity;

	if (m_map == NULL) {
		capacity = (unsigned int) ((int) m_mapSize.m_width * (int) m_mapSize.m_height);
		m_mapCapacity = capacity;
		m_map = (unsigned char*) operator new(capacity);
	}
}

// FUNCTION: LEMBALL 0x00466af0
void CChangeList::Resize(CVSSize p_size)
{
	short mapWidth;
	int mapHeight;
	int needed;

	if (m_viewSize.m_width != p_size.m_width || m_viewSize.m_height != p_size.m_height) {
		mapWidth = p_size.m_width / m_cellSize.m_width;
		mapHeight = (short) (p_size.m_height / m_cellSize.m_height);
		m_viewSize.m_width = p_size.m_width;
		m_viewSize.m_height = p_size.m_height;
		m_mapSize.m_width = mapWidth;
		needed = (int) mapWidth * mapHeight;
		m_mapSize.m_height = mapHeight;
		if (needed > (int) m_mapCapacity) {
			FreeMap();
		}
		if (0 < (int) p_size.m_height * (int) p_size.m_width) {
			AllocMap();
		}
	}
}

// FUNCTION: LEMBALL 0x00466b60
void CChangeList::Reset()
{
	unsigned int zero;

	zero = 0;
	m_currentArea.m_width = (short) zero;
	m_area = zero;
	m_activeMark = 'P';
	m_currentArea.m_height = (short) zero;
	m_currentArea.m_x = (short) zero;
	m_currentArea.m_y = (short) zero;
	m_itemCount = CHANGE_LIST_ITEMS_NEED_REBUILD;
	m_drawMarkCount = CHANGE_LIST_ITEMS_NEED_REBUILD;
}

// FUNCTION: LEMBALL 0x00466b90
CChangeList::~CChangeList()
{
	if (m_items != NULL) {
		operator delete(m_items);
		m_items = NULL;
	}
	FreeMap();
}

// FUNCTION: LEMBALL 0x00466bc0
void CChangeList::PushActive(unsigned char p_mark)
{
	m_activeStack[m_activeDepth] = m_activeMark;
	m_activeDepth = m_activeDepth + 1;
}

// FUNCTION: LEMBALL 0x00466bd0
void CChangeList::PopActive()
{
	m_activeDepth = m_activeDepth - 1;
	m_activeMark = m_activeStack[m_activeDepth];
}

// FUNCTION: LEMBALL 0x00466be0
void CChangeList::Add(const CVSRect& p_area)
{
	short cellWidth;
	int cellX;
	int cellY;
	int spanX;
	int spanY;
	int mapHeight;
	int mapWidth;
	unsigned char* row;

	if (m_map != NULL) {
		cellWidth = m_cellSize.m_width;
		const short& cellHeight = m_cellSize.m_height;
		cellX = (int) (p_area.m_x / cellWidth);
		cellY = (int) (p_area.m_y / cellHeight);
		spanX = ((int) p_area.m_width + (int) p_area.m_x - 1 + (int) cellWidth) / (int) cellWidth - cellX;
		spanY = ((int) p_area.m_height + (int) p_area.m_y - 1 + (int) cellHeight) / (int) cellHeight - cellY;
		mapHeight = (int) m_mapSize.m_height;
		if (mapHeight < cellY + spanY) {
			spanY = mapHeight - cellY;
		}
		mapWidth = (int) m_mapSize.m_width;
		if (mapWidth < cellX + spanX) {
			spanX = mapWidth - cellX;
		}
		if (spanX > 0 && spanY > 0) {
			row = m_map + cellX + mapWidth * cellY;
			while (spanY > 0) {
				memset(row, m_activeMark, spanX);
				row = row + m_mapSize.m_width;
				spanY = spanY - 1;
			}
			m_area = m_area + (int) p_area.m_width * (int) p_area.m_height;
		}
	}
}

// FUNCTION: LEMBALL 0x00466ce0
void CChangeList::AddWithActiveMark(const CVSRect& p_area, unsigned char p_mark)
{
	unsigned char prior;

	prior = m_activeMark;
	m_activeMark = p_mark;
	Add(p_area);
	m_activeMark = prior;
}

// FUNCTION: LEMBALL 0x00466d10
void CChangeList::SetDrawMark()
{
	unsigned int mark;

	mark = m_activeMark;
	switch (mark) {
	case 'P':
		m_activeMark = 'A';
		break;
	case 'p':
		m_activeMark = 'a';
		break;
	}
}

// FUNCTION: LEMBALL 0x00466d30
unsigned int CChangeList::GetArea()
{
	return m_area;
}

// FUNCTION: LEMBALL 0x00466d40
bool CChangeList::GetNextArea(unsigned char p_findMark, unsigned int p_itemMark, unsigned char p_replacementMark)
{
	int mapWidth;
	int scanY;
	int scanX;
	unsigned char* row;
	int startX;
	int widthPixels;
	int heightCells;
	int widthCells;
	int probeX;
	int cell;
	unsigned char* pixel;
	ChangeListItem* item;

	scanY = m_scanY;
	mapWidth = (int) m_mapSize.m_width;
	scanX = m_scanX;
	row = scanY * mapWidth + m_map;
	while (scanY < (int) m_mapSize.m_height) {
		if (scanX < (int) m_mapSize.m_width) {
			do {
				if (row[scanX] == p_findMark) {
					break;
				}
				scanX = scanX + 1;
			} while (scanX < (int) m_mapSize.m_width);
			if (scanX < (int) m_mapSize.m_width) {
				goto found;
			}
		}
		row = row + (int) m_mapSize.m_width;
		scanX = 0;
		scanY = scanY + 1;
	}
	return false;

found:
	startX = scanX;
	widthPixels = 0;
	while (scanX < (int) m_mapSize.m_width) {
		pixel = row + scanX;
		if (*pixel != p_findMark) {
			break;
		}
		widthPixels = widthPixels + (int) m_cellSize.m_width;
		*pixel = p_replacementMark;
		scanX = scanX + 1;
	}

	heightCells = 1;
	widthCells = widthPixels / (int) m_cellSize.m_width;
	row = row + (int) m_mapSize.m_width;
	if (scanY + 1 < (int) m_mapSize.m_height) {
		while (scanY + heightCells < (int) m_mapSize.m_height) {
			mapWidth = (int) m_mapSize.m_width;
			probeX = startX;
			if (probeX < mapWidth) {
				while (probeX < mapWidth && row[probeX] == p_findMark) {
					probeX = probeX + 1;
				}
			}
			if (probeX - widthCells != startX) {
				break;
			}
			heightCells = heightCells + 1;
			probeX = startX;
			if (widthCells > 0) {
				cell = 0;
				do {
					row[probeX] = p_replacementMark;
					probeX = probeX + 1;
					cell = cell + 1;
				} while (probeX - startX < widthCells);
			}
			row = row + (int) m_mapSize.m_width;
		}
	}

	CVSRect area;
	area.m_width = (short) widthPixels;
	area.m_height = (short) heightCells * m_cellSize.m_height;
	area.m_x = m_cellSize.m_width * (short) startX;
	area.m_y = (short) scanY * m_cellSize.m_height;
	item = m_items + m_itemCount;
	memcpy(&item->m_width, &area.m_width, sizeof(area.m_width));
	memcpy(&item->m_height, &area.m_height, sizeof(area.m_height));
	memcpy(&item->m_x, &area.m_x, sizeof(area.m_x));
	memcpy(&item->m_y, &area.m_y, sizeof(area.m_y));
	m_items[m_itemCount].m_drawMark = p_itemMark;
	m_scanX = 0;
	m_itemCount = m_itemCount + 1;
	if ((int) m_mapSize.m_width > scanX) {
		m_scanX = scanX;
	}
	m_scanY = scanY;
	return true;
}

// FUNCTION: LEMBALL 0x00466ef0
int CChangeList::GetNumItems()
{
	if (m_map == NULL) {
		return 0;
	}
	if (m_items == NULL) {
		return 0;
	}
	if (m_itemCount == CHANGE_LIST_ITEMS_NEED_REBUILD) {
		m_itemCount = 0;
		m_scanY = 0;
		m_scanX = 0;
		while (GetNextArea(1, 1, 0) != 0) {
		}
		m_scanY = 0;
		m_scanX = 0;
		while (GetNextArea('P', 1, 1) != 0) {
		}
		m_scanY = 0;
		m_scanX = 0;
		while (GetNextArea('p', 0, 1) != 0) {
		}
		m_drawMarkCount = m_itemCount;
		if (m_activeMark == 'A' || m_activeMark == 'a') {
			m_scanY = 0;
			m_scanX = 0;
			while (GetNextArea('A', 1, 1) != 0) {
			}
			m_scanY = 0;
			m_scanX = 0;
			while (GetNextArea('a', 0, 1) != 0) {
			}
		}
	}
	return m_itemCount;
}

// FUNCTION: LEMBALL 0x00467000
ChangeListItem* CChangeList::GetNItem(int p_index)
{
	if (m_itemCount == CHANGE_LIST_ITEMS_NEED_REBUILD) {
		GetNumItems();
	}
	return m_items + p_index;
}

// FUNCTION: LEMBALL 0x00467020
int CChangeList::GetDrawMark()
{
	if (m_map == NULL) {
		return 0;
	}
	if (m_itemCount == CHANGE_LIST_ITEMS_NEED_REBUILD) {
		GetNumItems();
	}
	return m_drawMarkCount;
}
