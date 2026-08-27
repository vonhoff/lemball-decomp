#include "ChangeList.h"

#include <new.h>

// STUB: LEMBALL 0x00467040
ChangeListItem::ChangeListItem()
{
	height = 0;
	width = 0;
	y = 0;
	x = 0;
}

// 68K 0x10210cf8 __ct__11CChangeListFi7CVSSize7CVSSize
// STUB: LEMBALL 0x004669a0
ChangeList::ChangeList(int p_arg0, VsSize p_arg1, VsSize p_arg2)
{
	ChangeListItem* items;
	ChangeListItem* item;
	int remaining;

	m_viewSize.m_height = 0;
	m_viewSize.m_width = 0;
	m_cellSize.m_height = 0;
	m_cellSize.m_width = 0;
	m_mapSize.m_height = 0;
	m_mapSize.m_width = 0;
	m_currentArea.m_height = 0;
	m_currentArea.m_width = 0;
	m_currentArea.m_y = 0;
	m_currentArea.m_x = 0;
	items = 0;
	if (p_arg0 != 0) {
		items = (ChangeListItem*) operator new((unsigned int) p_arg0 * sizeof(ChangeListItem));
	}
	if (items == 0) {
		m_items = 0;
	}
	else {
		item = items;
		remaining = p_arg0;
		while (remaining > 0) {
			new (item) ChangeListItem();
			item = item + 1;
			remaining = remaining - 1;
		}
		m_items = items;
	}
	m_capacity = p_arg0;
	m_cellSize.m_width = p_arg2.m_width;
	m_cellSize.m_height = p_arg2.m_height;
	m_viewSize.m_width = p_arg1.m_width;
	m_viewSize.m_height = p_arg1.m_height;
	m_mapSize.m_width = (short) ((p_arg1.m_width + p_arg2.m_width - 1) / p_arg2.m_width);
	m_mapSize.m_height = (short) ((p_arg1.m_height + p_arg2.m_height - 1) / p_arg2.m_height);
	m_activeDepth = 0;
	m_map = 0;
	AllocMap();
	Reset();
}

// 68K 0x10210e4c FreeMap__11CChangeListFv
// FUNCTION: LEMBALL 0x00466a90
void ChangeList::FreeMap()
{
	if (m_map != 0) {
		operator delete(m_map);
		m_map = 0;
	}
	Reset();
}

// 68K 0x10210e98 AllocMap__11CChangeListFv
// FUNCTION: LEMBALL 0x00466ac0
void ChangeList::AllocMap()
{
	unsigned int capacity;

	if (m_map == 0) {
		capacity = (unsigned int) ((int) m_mapSize.m_width * (int) m_mapSize.m_height);
		m_mapCapacity = capacity;
		m_map = (unsigned char*) operator new(capacity);
	}
}

// 68K 0x10210eea Resize__11CChangeListF7CVSSize
// STUB: LEMBALL 0x00466af0
void ChangeList::Resize(VsSize p_size)
{
	short mapWidth;
	short mapHeight;
	int needed;

	if (m_viewSize.m_width != p_size.m_width || m_viewSize.m_height != p_size.m_height) {
		mapWidth = p_size.m_width / m_cellSize.m_width;
		mapHeight = p_size.m_height / m_cellSize.m_height;
		m_viewSize.m_width = p_size.m_width;
		m_viewSize.m_height = p_size.m_height;
		m_mapSize.m_width = mapWidth;
		m_mapSize.m_height = mapHeight;
		needed = (int) mapWidth * (int) mapHeight;
		if (needed - (int) m_mapCapacity != 0 && (int) m_mapCapacity <= needed) {
			FreeMap();
		}
		if (0 < (int) p_size.m_height * (int) p_size.m_width) {
			AllocMap();
		}
	}
}

// 68K 0x10210f9a Reset__11CChangeListFv
// FUNCTION: LEMBALL 0x00466b60
void ChangeList::Reset()
{
	unsigned int zero;

	zero = 0;
	m_currentArea.m_width = (short) zero;
	m_area = zero;
	m_activeMark = 0x50;
	m_currentArea.m_height = (short) zero;
	m_currentArea.m_x = (short) zero;
	m_currentArea.m_y = (short) zero;
	m_itemCount = -1;
	m_drawMarkCount = -1;
}

// 68K 0x10211012 __dt__11CChangeListFv
// STUB: LEMBALL 0x00466b90
ChangeList::~ChangeList()
{
	if (m_items != 0) {
		operator delete(m_items);
		m_items = 0;
	}
	FreeMap();
}

// 68K 0x10211070 PushActive__11CChangeListFUc
// FUNCTION: LEMBALL 0x00466bc0
void ChangeList::PushActive(unsigned char p_mark)
{
	m_activeStack[m_activeDepth] = m_activeMark;
	m_activeDepth = m_activeDepth + 1;
}

// 68K 0x102110ce PopActive__11CChangeListFv
// FUNCTION: LEMBALL 0x00466bd0
void ChangeList::PopActive()
{
	m_activeDepth = m_activeDepth - 1;
	m_activeMark = m_activeStack[m_activeDepth];
}

// 68K 0x10211106 Add__11CChangeListFRC7CVSRect
// STUB: LEMBALL 0x00466be0
void ChangeList::Add(const VsRect& p_area)
{
	unsigned char mark;
	short cellWidth;
	short cellHeight;
	int cellX;
	int cellY;
	unsigned int spanX;
	int spanY;
	int mapHeight;
	int mapWidth;
	unsigned char* row;
	unsigned char* cell;
	unsigned int dwordCount;
	unsigned int tail;
	unsigned int splat;

	if (m_map == 0) {
		return;
	}
	cellWidth = m_cellSize.m_width;
	cellHeight = m_cellSize.m_height;
	cellX = (int) (p_area.m_x / cellWidth);
	cellY = (int) (p_area.m_y / cellHeight);
	spanX = ((int) p_area.m_width + (int) p_area.m_x - 1 + (int) cellWidth) / (int) cellWidth - cellX;
	spanY = ((int) p_area.m_height + (int) p_area.m_y - 1 + (int) cellHeight) / (int) cellHeight - cellY;
	mapHeight = (int) m_mapSize.m_height;
	if (mapHeight < cellY + spanY) {
		spanY = mapHeight - cellY;
	}
	mapWidth = (int) m_mapSize.m_width;
	if (mapWidth < (int) (cellX + spanX)) {
		spanX = (unsigned int) (mapWidth - cellX);
	}
	if ((int) spanX > 0 && spanY > 0) {
		row = m_map + cellX + mapWidth * cellY;
		do {
			mark = m_activeMark;
			splat = (unsigned int) mark | ((unsigned int) mark << 8) | ((unsigned int) mark << 16) |
					((unsigned int) mark << 24);
			cell = row;
			dwordCount = spanX >> 2;
			while (dwordCount != 0) {
				*(unsigned int*) cell = splat;
				cell = cell + 4;
				dwordCount = dwordCount - 1;
			}
			tail = spanX & 3;
			while (tail != 0) {
				*cell = mark;
				cell = cell + 1;
				tail = tail - 1;
			}
			row = row + m_mapSize.m_width;
			spanY = spanY - 1;
		} while (spanY != 0);
		m_area = m_area + (int) p_area.m_width * (int) p_area.m_height;
	}
}

// FUNCTION: LEMBALL 0x00466ce0
void ChangeList::AddWithActiveMark(const VsRect& p_area, unsigned char p_mark)
{
	unsigned char prior;

	prior = m_activeMark;
	m_activeMark = p_mark;
	Add(p_area);
	m_activeMark = prior;
}

// 68K 0x1021122a SetDrawMark__11CChangeListFv
// STUB: LEMBALL 0x00466d10
void ChangeList::SetDrawMark()
{
	unsigned int mark;

	mark = m_activeMark;
	if (mark == 0x50) {
		m_activeMark = 0x41;
		return;
	}
	if (mark == 0x70) {
		m_activeMark = 0x61;
	}
}

// 68K 0x1021128c GetArea__11CChangeListFv
// FUNCTION: LEMBALL 0x00466d30
unsigned int ChangeList::GetArea()
{
	return m_area;
}

// 68K 0x102112b8 GetNextArea__11CChangeListFUcUcUc
// STUB: LEMBALL 0x00466d40
bool ChangeList::GetNextArea(unsigned char p_findMark, unsigned char p_itemMark, unsigned char p_replacementMark)
{
	int mapWidth;
	int mapHeight;
	int scanY;
	int scanX;
	unsigned char* row;
	int startX;
	int widthPixels;
	int heightCells;
	int widthCells;
	int probeX;
	int cell;
	ChangeListItem* item;

	mapWidth = (int) m_mapSize.m_width;
	scanY = m_scanY;
	scanX = m_scanX;
	row = m_map + scanY * mapWidth;
	mapHeight = (int) m_mapSize.m_height;
	if (scanY >= mapHeight) {
		return 0;
	}
	while (1) {
		if (scanX < mapWidth) {
			while (scanX < mapWidth) {
				if (row[scanX] == p_findMark) {
					break;
				}
				scanX = scanX + 1;
			}
			if (scanX < mapWidth) {
				break;
			}
		}
		row = row + mapWidth;
		scanX = 0;
		scanY = scanY + 1;
		if (scanY >= mapHeight) {
			return 0;
		}
	}

	startX = scanX;
	widthPixels = 0;
	if (scanX < mapWidth) {
		while (scanX < mapWidth && row[scanX] == p_findMark) {
			row[scanX] = p_replacementMark;
			scanX = scanX + 1;
			widthPixels = widthPixels + (int) m_cellSize.m_width;
		}
	}

	heightCells = 1;
	widthCells = 0;
	if ((int) m_cellSize.m_width != 0) {
		widthCells = widthPixels / (int) m_cellSize.m_width;
	}
	row = row + mapWidth;
	if (scanY + 1 < mapHeight) {
		while (scanY + heightCells < mapHeight) {
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
			cell = 0;
			probeX = startX;
			while (cell < widthCells) {
				row[probeX] = p_replacementMark;
				probeX = probeX + 1;
				cell = cell + 1;
			}
			row = row + mapWidth;
		}
	}

	item = m_items + m_itemCount;
	item->width = (short) widthPixels;
	item->height = (short) (heightCells * (int) m_cellSize.m_height);
	item->x = (short) ((int) m_cellSize.m_width * startX);
	item->y = (short) (scanY * (int) m_cellSize.m_height);
	item->drawMark = p_itemMark;
	m_scanX = 0;
	m_itemCount = m_itemCount + 1;
	if (mapWidth > scanX) {
		m_scanX = scanX;
	}
	m_scanY = scanY;
	return 1;
}

// 68K 0x10211504 GetNumItems__11CChangeListFv
// STUB: LEMBALL 0x00466ef0
int ChangeList::GetNumItems()
{
	if (m_map == 0) {
		return 0;
	}
	if (m_items == 0) {
		return 0;
	}
	if (m_itemCount == -1) {
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

// 68K 0x10211630 GetNItem__11CChangeListFi
// FUNCTION: LEMBALL 0x00467000
ChangeListItem* ChangeList::GetNItem(int p_index)
{
	if (m_itemCount == -1) {
		GetNumItems();
	}
	return m_items + p_index;
}

// 68K 0x1021167a GetDrawMark__11CChangeListFv
// FUNCTION: LEMBALL 0x00467020
int ChangeList::GetDrawMark()
{
	if (m_map == 0) {
		return 0;
	}
	if (m_itemCount == -1) {
		GetNumItems();
	}
	return m_drawMarkCount;
}
