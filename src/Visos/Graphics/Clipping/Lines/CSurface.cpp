#include "Visos/Graphics/CSurface.h"

#include "Visos/Foundation/CVSRect.h"

enum eLineClipRegionFlag {
	LINE_CLIP_REGION_INSIDE = 0,
	LINE_CLIP_REGION_LEFT = 0x01,
	LINE_CLIP_REGION_RIGHT = 0x02,
	LINE_CLIP_REGION_TOP = 0x04,
	LINE_CLIP_REGION_BOTTOM = 0x08
};

// FUNCTION: LEMBALL 0x004757a0
int CSurface::LineClip(int& p_x1, int& p_y1, int& p_x2, int& p_y2)
{
	unsigned int code1;
	unsigned int code2;
	int coordinate;
	int dx;
	int dy;
	int x1;
	int y1;
	int x2;
	int y2;

	if (m_clipRect.m_height <= 0 || m_clipRect.m_width <= 0) {
		return 1;
	}
	code1 = LINE_CLIP_REGION_INSIDE;
	coordinate = p_x1;
	if (coordinate < m_clipRect.m_x) {
		code1 = LINE_CLIP_REGION_LEFT;
	}
	else if (m_clipRect.m_width + m_clipRect.m_x - 1 < coordinate) {
		code1 = LINE_CLIP_REGION_RIGHT;
	}
	coordinate = p_y1;
	if (coordinate < m_clipRect.m_y) {
		code1 |= LINE_CLIP_REGION_TOP;
	}
	else if (m_clipRect.m_height + m_clipRect.m_y - 1 < coordinate) {
		code1 |= LINE_CLIP_REGION_BOTTOM;
	}
	code2 = LINE_CLIP_REGION_INSIDE;
	coordinate = p_x2;
	if (coordinate < m_clipRect.m_x) {
		code2 = LINE_CLIP_REGION_LEFT;
	}
	else if (m_clipRect.m_width + m_clipRect.m_x - 1 < coordinate) {
		code2 = LINE_CLIP_REGION_RIGHT;
	}
	coordinate = p_y2;
	if (coordinate < m_clipRect.m_y) {
		code2 |= LINE_CLIP_REGION_TOP;
	}
	else if (m_clipRect.m_height + m_clipRect.m_y - 1 < coordinate) {
		code2 |= LINE_CLIP_REGION_BOTTOM;
	}
	if ((code1 | code2) != LINE_CLIP_REGION_INSIDE) {
		do {
			if ((code1 & code2) != 0) {
				return 1;
			}
			x2 = p_x2;
			x1 = p_x1;
			dx = x2 - x1;
			y2 = p_y2;
			y1 = p_y1;
			dy = y2 - y1;
			if (code1 != LINE_CLIP_REGION_INSIDE) {
				if ((code1 & LINE_CLIP_REGION_LEFT) == 0) {
					if ((code1 & LINE_CLIP_REGION_RIGHT) != 0) {
						p_y1 = y1 + ((m_clipRect.m_x + m_clipRect.m_width - 1 - x1) * dy) / dx;
						p_x1 = m_clipRect.m_x + m_clipRect.m_width - 1;
					}
					else {
						if ((code1 & LINE_CLIP_REGION_TOP) == 0) {
							if ((code1 & LINE_CLIP_REGION_BOTTOM) != 0) {
								p_x1 = x1 + ((m_clipRect.m_y + m_clipRect.m_height - 1 - y1) * dx) / dy;
								p_y1 = m_clipRect.m_y + m_clipRect.m_height - 1;
							}
						}
						else {
							p_x1 = x1 + ((m_clipRect.m_y - y1) * dx) / dy;
							p_y1 = m_clipRect.m_y;
						}
					}
				}
				else {
					p_y1 = y1 + ((m_clipRect.m_x - x1) * dy) / dx;
					p_x1 = m_clipRect.m_x;
				}
				code1 = LINE_CLIP_REGION_INSIDE;
				if (p_x1 < m_clipRect.m_x) {
					code1 = LINE_CLIP_REGION_LEFT;
				}
				else if (m_clipRect.m_width + m_clipRect.m_x - 1 < p_x1) {
					code1 = LINE_CLIP_REGION_RIGHT;
				}
				if (p_y1 < m_clipRect.m_y) {
					code1 |= LINE_CLIP_REGION_TOP;
				}
				else if (m_clipRect.m_height + m_clipRect.m_y - 1 < p_y1) {
					code1 |= LINE_CLIP_REGION_BOTTOM;
				}
			}
			else {
				if ((code2 & LINE_CLIP_REGION_LEFT) == 0) {
					if ((code2 & LINE_CLIP_REGION_RIGHT) != 0) {
						p_y2 = y2 + ((m_clipRect.m_x + m_clipRect.m_width - 1 - x2) * dy) / dx;
						p_x2 = m_clipRect.m_x + m_clipRect.m_width - 1;
					}
					else {
						if ((code2 & LINE_CLIP_REGION_TOP) == 0) {
							if ((code2 & LINE_CLIP_REGION_BOTTOM) != 0) {
								p_x2 = x2 + ((m_clipRect.m_y + m_clipRect.m_height - 1 - y2) * dx) / dy;
								p_y2 = m_clipRect.m_y + m_clipRect.m_height - 1;
							}
						}
						else {
							p_x2 = x2 + ((m_clipRect.m_y - y2) * dx) / dy;
							p_y2 = m_clipRect.m_y;
						}
					}
				}
				else {
					p_y2 = y2 + ((m_clipRect.m_x - x2) * dy) / dx;
					p_x2 = m_clipRect.m_x;
				}
				code2 = LINE_CLIP_REGION_INSIDE;
				if (p_x2 < m_clipRect.m_x) {
					code2 = LINE_CLIP_REGION_LEFT;
				}
				else if (m_clipRect.m_width + m_clipRect.m_x - 1 < p_x2) {
					code2 = LINE_CLIP_REGION_RIGHT;
				}
				if (p_y2 < m_clipRect.m_y) {
					code2 |= LINE_CLIP_REGION_TOP;
				}
				else if (m_clipRect.m_height + m_clipRect.m_y - 1 < p_y2) {
					code2 |= LINE_CLIP_REGION_BOTTOM;
				}
			}
		} while ((code1 | code2) != LINE_CLIP_REGION_INSIDE);
	}
	return 0;
}
