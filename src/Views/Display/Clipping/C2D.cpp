#include "../C2D.h"

#include "../../Target/ObjectClipGrid.h"
#include "../../Target/SpriteGroundLookup.h"
#include "AI/Base/Coord3d.h"
#include "AI/Navigation/CAI.h"
#include "AI/Objects/LiftEndpointRecord.h"
#include "Control/Game/GameMain.h"
#include "Map/Base/CMap.h"
#include "Visos/Foundation/CObjSq.h"

#include <string.h>

extern const short slinkyOffsets[4][2];
extern const short sheepOffset[2];
extern const unsigned int* g_styleObjectClip;

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

// FUNCTION: LEMBALL 0x0043e220
void C2D::BuildObjectClipData(CViewData& p_viewData, int p_viewIndex)
{
	CVsRect bounds;
	eObjectType objectType = p_viewData.m_objectType;
	int screenX = p_viewData.m_positionX;
	int screenY = p_viewData.m_positionY;
	if (screenX > -40 && screenY > -40 && screenX < m_clipSize.m_x + 40 && screenY < m_clipSize.m_y + 40) {
		int gameX = (unsigned short) p_viewData.m_gameX;
		int gameY = (unsigned short) p_viewData.m_gameY;
		switch (objectType) {
		default:
			AddViewIndexToObjectClipGrid(gameX, gameY, p_viewIndex, p_viewData.m_positionZ, 1);
			break;
		case OBJECT_PLAYER_1:
		case OBJECT_PLAYER_2:
			if (p_viewData.m_action < ACTION_10 || p_viewData.m_action > ACTION_11) {
				AddViewIndexToObjectClipGrid(gameX, gameY, p_viewIndex, p_viewData.m_positionZ, 1);
			}
			else {
				AddViewIndexToObjectClipGrid(gameX - 1, gameY - 1, p_viewIndex, p_viewData.m_positionZ, 0);
			}
			break;
		case OBJECT_CRATE:
			if (p_viewData.m_action == ACTION_24) {
				AddViewIndexToObjectClipGrid(gameX, gameY, p_viewIndex, p_viewData.m_positionZ, 1);
			}
			else if (p_viewData.m_action >= ACTION_25 && p_viewData.m_action <= ACTION_26) {
				AddViewIndexToObjectClipGrid(gameX + 16, gameY + 16, p_viewIndex, p_viewData.m_positionZ, 1);
			}
			break;
		case OBJECT_TRAP_DOOR:
			AddViewIndexToObjectClipGrid(gameX + 32, gameY + 32, p_viewIndex, p_viewData.m_positionZ, 1);
			break;
		case OBJECT_DOOR_1:
		case OBJECT_DOOR_2:
			AddViewIndexToObjectClipGrid(gameX + 16, gameY + 16, p_viewIndex, p_viewData.m_positionZ, 1);
			break;
		case OBJECT_PAINT_GUN:
		case OBJECT_TRAMPOLINE:
			AddViewIndexToObjectClipGrid(gameX, gameY, p_viewIndex, p_viewData.m_positionZ, 0);
			break;
		case OBJECT_MOVER:
			AddViewIndexToObjectClipGrid(gameX - 8, gameY - 8, p_viewIndex, p_viewData.m_positionZ + 8, 1);
			break;
		}
		if (g_nStartupGraphicsDialogRequested != 0) {
			return;
		}

		switch (objectType) {
		case OBJECT_PLAYER_1:
		case OBJECT_PLAYER_2: {
			bounds.m_x = (short) screenX - 24;
			bounds.m_y = (short) screenY - 24;
			bounds.m_width = 48;
			bounds.m_height = 40;
			if (InGroupByObjectNo(p_viewData.m_objectId)) {
				bounds.m_y -= 20;
				bounds.m_height += 20;
			}
			if (p_viewData.m_action != ACTION_4) {
				if (p_viewData.m_action != ACTION_16) {
					break;
				}
				bounds.m_y -= 40;
				bounds.m_height += 40;
				m_clipSearchHeight = 160;
			}
			{
				SpriteGroundLookup* lookup = m_spriteGroundLookup;
				const CVsRect& markedRect = bounds;
				short pixelX = markedRect.m_x;
				short pixelY = markedRect.m_y;
				int cellX = (short) (pixelX / 16);
				int cellY = (short) (pixelY / 16);
				int columns = (pixelX + markedRect.m_width - 1) / 16 - cellX + 1;
				int rows = (pixelY + markedRect.m_height - 1) / 16 - cellY + 1;
				int width = lookup->m_width;
				int height;
				if (cellX < width && (height = lookup->m_height, cellY < height)) {
					if (cellX < 0) {
						columns += cellX;
						cellX = 0;
					}
					if (cellY < 0) {
						rows += cellY;
						cellY = 0;
					}
					if (cellX + columns >= width) {
						columns = width - cellX;
					}
					if (cellY + rows >= height) {
						rows = height - cellY;
					}
					if (columns > 0 && rows > 0) {
						int offset = cellX + cellY * width;
						unsigned char* maskA = lookup->m_maskA + offset;
						unsigned char* maskB = lookup->m_maskB + offset;
						for (; rows != 0; rows--) {
							memset(maskA, 1, columns);
							memset(maskB, 1, columns);
							maskA += lookup->m_width;
							maskB += lookup->m_width;
						}
					}
				}
			}
			int shadowGameX = (unsigned short) p_viewData.m_gameX;
			gameY = (unsigned short) p_viewData.m_gameY;
			CMap* map = m_map;
			int blockX = shadowGameX >> 4;
			int blockY = gameY >> 4;
			unsigned short groundHeight;
			if (shadowGameX < 0 || gameY < 0 || blockX >= map->m_ground.m_width || blockY >= map->m_ground.m_height) {
				groundHeight = 0;
			}
			else {
				int localX = shadowGameX & 15;
				int localY = gameY & 15;
				groundHeight = map->m_ground.m_ground[map->m_ground.m_width * blockY + blockX].GetZ(localX, localY);
			}
			gameX = (shadowGameX << 12) >> 12;
			gameY = (gameY << 12) >> 12;
			m_map->GameToScreen(gameX, gameY);
			bounds.m_x = (short) (((gameX - m_viewOriginX) << 12) >> 12) - 10;
			bounds.m_y = (short) (((gameY - ((groundHeight << 12) >> 12) - m_viewOriginY) << 12) >> 12) - 5;
			bounds.m_width = 20;
			bounds.m_height = 10;
			m_clipSearchHeight = 160;
			break;
		}
		case OBJECT_BULLET:
			bounds.m_x = (short) screenX - 8;
			bounds.m_y = (short) screenY - 8;
			bounds.m_width = 16;
			bounds.m_height = 16;
			break;
		case OBJECT_CATAPULT:
			bounds.m_x = (short) screenX - 48;
			bounds.m_y = (short) screenY - 60;
			bounds.m_width = 64;
			bounds.m_height = 56;
			break;
		case OBJECT_AMMO:
			bounds.m_x = (short) screenX - 8;
			bounds.m_y = (short) screenY - 16;
			bounds.m_width = 16;
			bounds.m_height = 24;
			break;
		case OBJECT_SHEEP:
			bounds.m_x = (short) screenX - sheepOffset[0];
			bounds.m_y = (short) screenY - sheepOffset[1];
			bounds.m_width = sheepOffset[0] * 2;
			bounds.m_height = sheepOffset[1] * 2;
			break;
		case OBJECT_BALL:
			bounds.m_x = (short) screenX - 10;
			bounds.m_y = (short) screenY - 15;
			bounds.m_width = 24;
			bounds.m_height = 24;
			break;
		case OBJECT_FLAG_1:
		case OBJECT_FLAG_2:
			bounds.m_x = (short) screenX - 16;
			bounds.m_y = (short) screenY - 28;
			bounds.m_width = 32;
			bounds.m_height = 32;
			break;
		case OBJECT_TOWER:
			bounds.m_x = (short) screenX - (short) g_styleObjectClip[0];
			bounds.m_y = (short) screenY - (short) g_styleObjectClip[1];
			bounds.m_width = (short) g_styleObjectClip[2];
			bounds.m_height = (short) g_styleObjectClip[3];
			break;
		case OBJECT_CRATE:
			if (p_viewData.m_action == ACTION_24) {
				bounds.m_x = (short) screenX - 12;
				bounds.m_y = (short) screenY - 12;
				bounds.m_width = 24;
				bounds.m_height = 24;
			}
			else {
				bounds.m_x = (short) screenX - 32;
				bounds.m_y = (short) screenY - 64;
				bounds.m_width = 64;
				bounds.m_height = 64;
			}
			break;
		case OBJECT_BONUS:
			bounds.m_x = (short) screenX - 16;
			bounds.m_y = (short) screenY - 16;
			bounds.m_width = 32;
			bounds.m_height = 20;
			break;
		case OBJECT_MINE:
			bounds.m_x = (short) screenX - 28;
			bounds.m_y = (short) screenY - 35;
			bounds.m_width = 56;
			bounds.m_height = 44;
			break;
		case OBJECT_SWITCH:
			bounds.m_x = (short) screenX - 16;
			bounds.m_y = (short) screenY - 16;
			bounds.m_width = 32;
			bounds.m_height = 32;
			break;
		case OBJECT_KEY_1:
		case OBJECT_KEY_2:
		case OBJECT_KEY_3:
			bounds.m_x = (short) screenX - 12;
			bounds.m_y = (short) screenY - 32;
			bounds.m_width = 24;
			bounds.m_height = 32;
			break;
		case OBJECT_TRAP_DOOR:
			bounds.m_x = (short) screenX - 48;
			bounds.m_y = (short) screenY - 40;
			bounds.m_width = 96;
			bounds.m_height = 168;
			m_clipSearchHeight = 160;
			break;
		case OBJECT_DOOR_1:
		case OBJECT_DOOR_2:
			bounds.m_x = (short) screenX - 26;
			bounds.m_y = (short) screenY - 24;
			bounds.m_width = 48;
			bounds.m_height = 40;
			if (p_viewData.m_action == ACTION_0x1c) {
				bounds.m_y -= 12;
				bounds.m_height += 12;
			}
			break;
		case OBJECT_TIME_BONUS:
			bounds.m_x = (short) screenX - 16;
			bounds.m_y = (short) screenY - 12;
			bounds.m_width = 32;
			bounds.m_height = 24;
			break;
		case OBJECT_DUPLICATOR:
			bounds.m_x = (short) screenX - 13;
			bounds.m_y = (short) screenY - 54;
			bounds.m_width = 100;
			bounds.m_height = 60;
			break;
		case OBJECT_LASER_HORIZONTAL:
		case OBJECT_LASER_VERTICAL:
		case OBJECT_LASER_EMITTER_H:
		case OBJECT_LASER_EMITTER_V:
			bounds.m_x = (short) screenX - 20;
			bounds.m_y = (short) screenY - 10;
			bounds.m_width = 40;
			bounds.m_height = 30;
			break;
		case OBJECT_HAND:
			bounds.m_x = (short) screenX - 48;
			bounds.m_y = (short) screenY - 20;
			bounds.m_width = 48;
			bounds.m_height = 48;
			break;
		case OBJECT_ROCKET:
			bounds.m_x = (short) screenX - 13;
			bounds.m_y = (short) screenY - 80;
			bounds.m_width = 26;
			bounds.m_height = 80;
			break;
		case OBJECT_PAINT_GUN:
			bounds.m_x = (short) screenX - 22;
			bounds.m_y = (short) screenY - 35;
			bounds.m_width = 48;
			bounds.m_height = 48;
			break;
		case OBJECT_TRAMPOLINE:
			bounds.m_x = (short) screenX - 22;
			bounds.m_y = (short) screenY - 22;
			bounds.m_width = 48;
			bounds.m_height = 32;
			break;
		case OBJECT_LASER_HORIZONTAL_BEAM:
			bounds.m_x = (short) screenX - 21;
			bounds.m_y = (short) screenY - 14;
			bounds.m_width = 22;
			bounds.m_height = 15;
			break;
		case OBJECT_BALLOON_0:
		case OBJECT_BALLOON_2:
		case OBJECT_BALLOON_4:
		case OBJECT_BALLOON_6:
			bounds.m_x = (short) screenX - 20;
			bounds.m_y = (short) screenY - 68;
			bounds.m_width = 40;
			bounds.m_height = 72;
			break;
		case OBJECT_BALLOON_1:
		case OBJECT_BALLOON_3:
		case OBJECT_BALLOON_5:
		case OBJECT_BALLOON_7:
			bounds.m_x = (short) screenX - 16;
			bounds.m_y = (short) screenY - 48;
			bounds.m_width = 32;
			bounds.m_height = 48;
			break;
		case OBJECT_LASER_VERTICAL_BEAM:
			bounds.m_x = (short) screenX - 21;
			bounds.m_y = (short) screenY - 14;
			bounds.m_width = 22;
			bounds.m_height = 15;
			break;
		case OBJECT_MOVER:
			bounds.m_x = (short) screenX - 24;
			bounds.m_y = (short) screenY - 24;
			bounds.m_width = 48;
			bounds.m_height = 56;
			break;
		case OBJECT_SLINKY: {
			unsigned int direction = (unsigned short) p_viewData.m_actionArgument;
			bounds.m_x = (short) screenX - slinkyOffsets[direction][0];
			bounds.m_y = (short) screenY - slinkyOffsets[direction][1];
			bounds.m_width = 40;
			bounds.m_height = 42;
			break;
		}
		default:
			return;
		}
	}
	{
		SpriteGroundLookup* lookup = m_spriteGroundLookup;
		const CVsRect& markedRect = bounds;
		short pixelX = markedRect.m_x;
		short pixelY = markedRect.m_y;
		int cellX = (short) (pixelX / 16);
		int cellY = (short) (pixelY / 16);
		int columns = (pixelX + markedRect.m_width - 1) / 16 - cellX + 1;
		int rows = (pixelY + markedRect.m_height - 1) / 16 - cellY + 1;
		int width = lookup->m_width;
		int height;
		if (cellX < width && (height = lookup->m_height, cellY < height)) {
			if (cellX < 0) {
				columns += cellX;
				cellX = 0;
			}
			if (cellY < 0) {
				rows += cellY;
				cellY = 0;
			}
			if (cellX + columns >= width) {
				columns = width - cellX;
			}
			if (cellY + rows >= height) {
				rows = height - cellY;
			}
			if (columns > 0 && rows > 0) {
				int offset = cellX + cellY * width;
				unsigned char* maskA = lookup->m_maskA + offset;
				unsigned char* maskB = lookup->m_maskB + offset;
				for (; rows != 0; rows--) {
					memset(maskA, 1, columns);
					memset(maskB, 1, columns);
					maskA += lookup->m_width;
					maskB += lookup->m_width;
				}
			}
		}
	}
}

// FUNCTION: LEMBALL 0x0043f060
void C2D::MarkGroundAnimAndLiftBounds()
{
	char* scratch = m_groundClipScratch;
	int count = m_ai->ExportGroundAnimRecords((Coord3d*) scratch);
	if (count > 0) {
		int remaining = count;
		Coord3d* coordinate = (Coord3d*) scratch;
		do {
			int screenX = coordinate->m_x;
			int screenY = coordinate->m_y;
			int groundHeight = coordinate->m_z;
			m_map->GameToScreen(screenX, screenY);
			screenX -= m_viewOriginX;
			screenY -= m_viewOriginY;
			screenY -= groundHeight;
			short pixelX = (short) screenX - 16;
			short pixelY = (short) screenY - 24;
			short rectHeight = (short) groundHeight + 48;
			SpriteGroundLookup* lookup = m_spriteGroundLookup;
			int cellX = (short) (pixelX / 16);
			int cellY = (short) (pixelY / 16);
			int columns = (pixelX + 32 - 1) / 16 - cellX + 1;
			int rows = (pixelY + rectHeight - 1) / 16 - cellY + 1;
			int width = lookup->m_width;
			int height;
			if (width > cellX && (height = lookup->m_height, height > cellY)) {
				if (cellX < 0) {
					columns += cellX;
					cellX = 0;
				}
				if (cellY < 0) {
					rows += cellY;
					cellY = 0;
				}
				if (cellX + columns >= width) {
					columns = width - cellX;
				}
				if (cellY + rows >= height) {
					rows = height - cellY;
				}
				if (columns > 0 && rows > 0) {
					int offset = cellX + width * cellY;
					unsigned char* maskA = lookup->m_maskA + offset;
					unsigned char* maskB = lookup->m_maskB + offset;
					for (; rows != 0; rows--) {
						memset(maskA, 1, columns);
						memset(maskB, 1, columns);
						maskA += lookup->m_width;
						maskB += lookup->m_width;
					}
				}
			}
			coordinate++;
		} while (--remaining != 0);
	}

	count = m_ai->ExportLiftEndpointRecords((LiftEndpointRecord*) scratch);
	if (count > 0) {
		int remaining = count;
		LiftEndpointRecord* endpoints = (LiftEndpointRecord*) scratch;
		do {
			int groundHeight = endpoints->m_start.m_z;
			int startX = endpoints->m_start.m_x;
			int startY = endpoints->m_start.m_y;
			m_map->GameToScreen(startX, startY);
			startX -= m_viewOriginX;
			startY -= m_viewOriginY;
			startY -= groundHeight;

			int rightX = endpoints->m_end.m_x;
			int rightY = endpoints->m_start.m_y;
			m_map->GameToScreen(rightX, rightY);
			rightX -= m_viewOriginX;
			rightY -= m_viewOriginY;
			rightY -= groundHeight;

			int endX = endpoints->m_end.m_x;
			int endY = endpoints->m_end.m_y;
			m_map->GameToScreen(endX, endY);
			endX -= m_viewOriginX;
			endY -= m_viewOriginY;
			endY -= groundHeight;

			int leftX = endpoints->m_start.m_x;
			int leftY = endpoints->m_end.m_y;
			m_map->GameToScreen(leftX, leftY);
			leftX -= m_viewOriginX;
			leftY -= m_viewOriginY;
			leftY -= groundHeight;

			short pixelX = (short) leftX - 16;
			short pixelY = (short) startY - 24;
			short rectWidth = (short) rightX - (short) leftX + 32;
			short rectHeight = (short) groundHeight - (short) startY + (short) endY + 32;
			SpriteGroundLookup* lookup = m_spriteGroundLookup;
			int cellX = (short) (pixelX / 16);
			int cellY = (short) (pixelY / 16);
			int columns = (pixelX + rectWidth - 1) / 16 - cellX + 1;
			int rows = (pixelY + rectHeight - 1) / 16 - cellY + 1;
			int width = lookup->m_width;
			int height;
			if (width > cellX && (height = lookup->m_height, height > cellY)) {
				if (cellX < 0) {
					columns += cellX;
					cellX = 0;
				}
				if (cellY < 0) {
					rows += cellY;
					cellY = 0;
				}
				if (cellX + columns >= width) {
					columns = width - cellX;
				}
				if (cellY + rows >= height) {
					rows = height - cellY;
				}
				if (columns > 0 && rows > 0) {
					int offset = cellX + width * cellY;
					unsigned char* maskA = lookup->m_maskA + offset;
					unsigned char* maskB = lookup->m_maskB + offset;
					for (; rows != 0; rows--) {
						memset(maskA, 1, columns);
						memset(maskB, 1, columns);
						maskA += lookup->m_width;
						maskB += lookup->m_width;
					}
				}
			}
			endpoints++;
		} while (--remaining != 0);
	}
}

// FUNCTION: LEMBALL 0x0043f480
void C2D::UpdateSpriteGroundLookupRegions()
{
	short translatedX = m_spriteGroundTranslationPoint.m_x - (short) m_clipOffsetX - 3;
	short translatedY = m_spriteGroundTranslationPoint.m_y - (short) m_clipOffsetY - 3;
	SpriteGroundLookup* lookup = m_spriteGroundLookup;
	m_spriteGroundTranslatedPointRect.m_width = 16;
	m_spriteGroundTranslatedPointRect.m_height = 16;
	m_spriteGroundTranslatedPointRect.m_x = translatedX;
	m_spriteGroundTranslatedPointRect.m_y = translatedY;

	short pixelX = m_spriteGroundLookupRectA.m_x;
	int cellX = (short) (pixelX / 16);
	int cellY = (short) (m_spriteGroundLookupRectA.m_y / 16);
	int columns = (pixelX + m_spriteGroundLookupRectA.m_width - 1) / 16 - cellX + 1;
	int rows = (m_spriteGroundLookupRectA.m_y + m_spriteGroundLookupRectA.m_height - 1) / 16 - cellY + 1;
	int width = lookup->m_width;
	int height;
	if (cellX < width && (height = lookup->m_height, cellY < height)) {
		if (cellX < 0) {
			columns += cellX;
			cellX = 0;
		}
		if (cellY < 0) {
			rows += cellY;
			cellY = 0;
		}
		if (cellX + columns >= width) {
			columns = width - cellX;
		}
		if (cellY + rows >= height) {
			rows = height - cellY;
		}
		if (columns > 0 && rows > 0) {
			int offset = cellX + cellY * width;
			unsigned char* maskA = lookup->m_maskA + offset;
			unsigned char* maskB = lookup->m_maskB + offset;
			for (; rows != 0; rows--) {
				memset(maskA, 1, columns);
				memset(maskB, 1, columns);
				maskA += lookup->m_width;
				maskB += lookup->m_width;
			}
		}
	}
	m_spriteGroundLookup->MarkRect(m_spriteGroundLookupRectB);
}
