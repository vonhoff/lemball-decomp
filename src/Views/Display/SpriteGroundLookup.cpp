#include "SpriteGroundLookup.h"

#include "Map/CGround.h"
#include "Visos/Math/CVSRect.h"

#include <string.h>

// FUNCTION: LEMBALL 0x00441fe0
void SpriteGroundLookup::MarkRect(const CVSRect& p_rect)
{
	short pixelX = p_rect.m_x;
	const short& pixelY = p_rect.m_y;
	int cellX = (short) (pixelX / GROUND_BLOCK_PIXEL_SIZE);
	int cellY = (short) (pixelY / GROUND_BLOCK_PIXEL_SIZE);
	int columns = (pixelX + p_rect.m_width - 1) / GROUND_BLOCK_PIXEL_SIZE - cellX + 1;
	int rows = (pixelY + p_rect.m_height - 1) / GROUND_BLOCK_PIXEL_SIZE - cellY + 1;
	short width = m_width;
	short height;
	if (cellX < width && ((height = m_height), cellY < height)) {
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
			unsigned char* maskA = m_maskA + offset;
			unsigned char* maskB = m_maskB + offset;
			for (; rows != 0; rows--) {
				memset(maskA, 1, columns);
				memset(maskB, 1, columns);
				maskA += m_width;
				maskB += m_width;
			}
		}
	}
}
