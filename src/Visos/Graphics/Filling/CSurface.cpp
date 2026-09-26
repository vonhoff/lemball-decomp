#include "Visos/Graphics/CSurface.h"

#include "Visos/Graphics/CCopyColourToBackBuff.h"

#include <string.h>

// FUNCTION: LEMBALL 0x00474e60
void CSurface::Blit(CCopyColourToBackBuff* p_fill)
{
	int startX;
	int startY;
	int width = p_fill->m_width;
	int height = p_fill->m_height;

	if (width == 0 || height == 0) {
		return;
	}
	startX = p_fill->m_x;
	startY = p_fill->m_y;
	int color = reinterpret_cast<int&>(p_fill->m_colour);
	if (height <= 0) {
		return;
	}
	do {
		unsigned char* dest = (unsigned char*) CPVBackBuffSurface::m_bitmap.m_lines[startY] + startX;
		memset(dest, color, width);
		startY++;
		height--;
	} while (height != 0);
}
