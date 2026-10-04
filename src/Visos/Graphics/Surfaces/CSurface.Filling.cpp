#include "CSurface.h"

#include "Visos/Graphics/Primitives/CCopyColourToBackBuff.h"

#include <string.h>

// FUNCTION: LEMBALL 0x00474e60
void CSurface::Blit(CCopyColourToBackBuff* p_fill)
{
	int startX;
	int startY;
	int width = p_fill->m_bounds.m_width;
	int height = p_fill->m_bounds.m_height;

	if (width == 0 || height == 0) {
		return;
	}
	startX = p_fill->m_bounds.m_x;
	startY = p_fill->m_bounds.m_y;
	int colour = p_fill->m_colour;
	if (height <= 0) {
		return;
	}
	do {
		unsigned char* dest = (unsigned char*) CPVBackBuffSurface::m_bitmap.m_lines[startY] + startX;
		memset(dest, colour, width);
		startY++;
		height--;
	} while (height != 0);
}
