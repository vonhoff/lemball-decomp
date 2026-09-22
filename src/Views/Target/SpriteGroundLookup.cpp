#include "SpriteGroundLookup.h"

#include "Visos/Foundation/CVsRect.h"

#include <string.h>

// FUNCTION: LEMBALL 0x00441fe0
void SpriteGroundLookup::MarkRect(const CVsRect& p_rect)
{
	short pixelX = p_rect.m_x;
	short pixelY = p_rect.m_y;
	int cellX = (short) (pixelX / 16);
	int cellY = (short) (pixelY / 16);
	int columns = (pixelX + p_rect.m_width - 1) / 16 - cellX + 1;
	int rows = (pixelY + p_rect.m_height - 1) / 16 - cellY + 1;
	int width = m_width;
	int height;
	if (cellX < width && (height = m_height, cellY < height)) {
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
