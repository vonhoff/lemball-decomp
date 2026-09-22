#include "../C2D.h"

#include "../../Target/ObjectClipGrid.h"
#include "Map/Base/CMap.h"
#include "Visos/Foundation/CObjSq.h"

// FUNCTION: LEMBALL 0x004368f0
CVsRect* C2D::GetClipRectangle()
{
	static CVsRect clipRectangle;
	CVsRect rectangle((short) m_clipOffsetX, (short) m_clipOffsetY, m_clipSize.m_x, m_clipSize.m_y);
	clipRectangle = rectangle;
	return &clipRectangle;
}

// FUNCTION: LEMBALL 0x0043df30
void C2D::AddViewIndexToObjectClipGrid(int p_x, int p_y, int p_viewIndex, int p_groundHeight, int p_adjustForGround)
{
	if (p_adjustForGround != 0) {
		CMap* map = m_map;
		p_groundHeight += 4;
		int nextY = p_y + 0x10;
		int blockX = p_x >> 4;
		int nextBlockY = nextY >> 4;
		int yHeight;
		int xHeight;
		int diagonalHeight;
		unsigned short sampledHeight;
		if (p_x < 0 || nextY < 0 || map->m_ground.m_width <= blockX || map->m_ground.m_height <= nextBlockY) {
			sampledHeight = 0;
		}
		else {
			int localX = p_x & 0xf;
			int localY = p_y & 0xf;
			sampledHeight = map->m_ground.GetGroundCell(blockX, nextBlockY)->GetZ(localX, localY);
		}
		yHeight = sampledHeight;

		map = m_map;
		int nextBlockX = (p_x + 0x10) >> 4;
		int blockY = p_y >> 4;
		if (p_x + 0x10 < 0 || p_y < 0 || map->m_ground.m_width <= nextBlockX || map->m_ground.m_height <= blockY) {
			sampledHeight = 0;
		}
		else {
			int localY = p_y & 0xf;
			int localX = p_x & 0xf;
			sampledHeight = map->m_ground.GetGroundCell(nextBlockX, blockY)->GetZ(localX, localY);
		}
		xHeight = sampledHeight;

		map = m_map;
		if (p_x + 0x10 < 0 || nextY < 0 || map->m_ground.m_width <= nextBlockX ||
			map->m_ground.m_height <= nextBlockY) {
			sampledHeight = 0;
		}
		else {
			sampledHeight =
				map->m_ground.m_ground[map->m_ground.m_width * nextBlockY + nextBlockX].GetZ(p_x & 0xf, p_y & 0xf);
		}
		diagonalHeight = sampledHeight;

		blockX = p_x / 16;
		blockY = p_y / 16;
		unsigned short yCollision;
		unsigned short xCollision;
		unsigned short diagonalCollision;
		if (blockX < 0 || blockY + 1 < 0 || m_map->m_ground.m_width <= blockX ||
			m_map->m_ground.m_height <= blockY + 1) {
			yCollision = 3;
		}
		else {
			yCollision = m_map->m_ground.m_ground[m_map->m_ground.m_width * (blockY + 1) + blockX].m_collision;
		}
		yCollision &= 1;
		if (blockX + 1 < 0 || blockY < 0 || m_map->m_ground.m_width <= blockX + 1 ||
			m_map->m_ground.m_height <= blockY) {
			xCollision = 3;
		}
		else {
			xCollision = m_map->m_ground.m_ground[m_map->m_ground.m_width * blockY + blockX + 1].m_collision;
		}
		xCollision &= 1;
		if (blockX + 1 < 0 || blockY + 1 < 0 || m_map->m_ground.m_width <= blockX + 1 ||
			m_map->m_ground.m_height <= blockY + 1) {
			diagonalCollision = 3;
		}
		else {
			diagonalCollision =
				m_map->m_ground.m_ground[m_map->m_ground.m_width * (blockY + 1) + blockX + 1].m_collision;
		}
		diagonalCollision &= 1;

		int shiftY = yHeight <= p_groundHeight && yCollision == 0;
		int shiftX = xHeight <= p_groundHeight && xCollision == 0;
		int shiftDiagonal = diagonalHeight <= p_groundHeight && diagonalCollision == 0;
		if (shiftDiagonal && shiftX) {
			if (shiftY) {
				p_x += 0x10;
				p_y = nextY;
			}
		}
		else if (shiftY) {
			p_y = nextY;
		}
	}

	p_x >>= 4;
	p_y >>= 4;
	ObjectClipGrid* grid = m_objectClipGrid;
	if (p_x >= 0 && p_y >= 0 && p_x < grid->m_width && p_y < grid->m_height) {
		CObjSq* cell = &grid->m_cells[grid->m_width * p_y + p_x];
		if (cell->m_objectCount < 4) {
			cell->m_viewIndices[cell->m_objectCount] = (short) p_viewIndex;
			cell->m_objectCount++;
		}
		if ((int) grid->m_touchedCount < 100) {
			grid->m_touchedCells[grid->m_touchedCount] = cell;
			grid->m_touchedCount++;
		}
	}
}
