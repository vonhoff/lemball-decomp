#include "Visos/Graphics/CSurface.h"

#include <string.h>

// FUNCTION: LEMBALL 0x00476470
void CSurface::FilledCircleClipPoints(int p_centerX, int p_centerY, int p_xOffset, int p_yOffset, int p_colour)
{
	int y1 = p_centerY - p_yOffset;
	int y2 = p_centerY + p_yOffset;
	if (y1 <= (int) (m_clipRect.m_height + m_clipRect.m_y - 1) && m_clipRect.m_y <= y2) {
		int x1 = p_centerX - p_xOffset;
		int x2 = p_centerX + p_xOffset;
		int clipX = m_clipRect.m_x;
		if (clipX <= x2 && x1 <= (int) (m_clipRect.m_width + clipX - 1)) {
			if ((int) (m_clipRect.m_width + clipX - 1) < x2) {
				x2 = m_clipRect.m_width + clipX - 1;
			}
			if (x1 < clipX) {
				x1 = clipX;
			}
			if (m_clipRect.m_y <= y1) {
				memset((unsigned char*) m_lines[y1] + x1, p_colour, x2 - x1 + 1);
			}
			if (y2 <= (int) (m_clipRect.m_height + m_clipRect.m_y - 1)) {
				memset((unsigned char*) m_lines[y2] + x1, p_colour, x2 - x1 + 1);
			}
		}
	}
}
