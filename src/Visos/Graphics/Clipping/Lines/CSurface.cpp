#include "Visos/Graphics/CSurface.h"

#include "Visos/Foundation/CVsRect.h"

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
	code1 = 0;
	coordinate = p_x1;
	if (coordinate < m_clipRect.m_x) {
		code1 = 1;
	}
	else if (m_clipRect.m_width + m_clipRect.m_x - 1 < coordinate) {
		code1 = 2;
	}
	coordinate = p_y1;
	if (coordinate < m_clipRect.m_y) {
		code1 |= 4;
	}
	else if (m_clipRect.m_height + m_clipRect.m_y - 1 < coordinate) {
		code1 |= 8;
	}
	code2 = 0;
	coordinate = p_x2;
	if (coordinate < m_clipRect.m_x) {
		code2 = 1;
	}
	else if (m_clipRect.m_width + m_clipRect.m_x - 1 < coordinate) {
		code2 = 2;
	}
	coordinate = p_y2;
	if (coordinate < m_clipRect.m_y) {
		code2 |= 4;
	}
	else if (m_clipRect.m_height + m_clipRect.m_y - 1 < coordinate) {
		code2 |= 8;
	}
	if ((code1 | code2) != 0) {
		do {
			if ((code1 & code2) != 0) {
				return 1;
			}
			x1 = p_x1;
			x2 = p_x2;
			dx = x2 - x1;
			y2 = p_y2;
			y1 = p_y1;
			dy = y2 - y1;
			if (code1 != 0) {
				if ((code1 & 1) == 0) {
					if ((code1 & 2) != 0) {
						p_y1 = y1 + ((m_clipRect.m_x + m_clipRect.m_width - 1 - x1) * dy) / dx;
						p_x1 = m_clipRect.m_x + m_clipRect.m_width - 1;
					}
					else {
						if ((code1 & 4) == 0) {
							if ((code1 & 8) != 0) {
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
				code1 = 0;
				if (p_x1 < m_clipRect.m_x) {
					code1 = 1;
				}
				else if (m_clipRect.m_width + m_clipRect.m_x - 1 < p_x1) {
					code1 = 2;
				}
				if (p_y1 < m_clipRect.m_y) {
					code1 |= 4;
				}
				else if (m_clipRect.m_height + m_clipRect.m_y - 1 < p_y1) {
					code1 |= 8;
				}
			}
			else {
				if ((code2 & 1) == 0) {
					if ((code2 & 2) != 0) {
						p_y2 = y2 + ((m_clipRect.m_x + m_clipRect.m_width - 1 - x2) * dy) / dx;
						p_x2 = m_clipRect.m_x + m_clipRect.m_width - 1;
					}
					else {
						if ((code2 & 4) == 0) {
							if ((code2 & 8) != 0) {
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
				code2 = 0;
				if (p_x2 < m_clipRect.m_x) {
					code2 = 1;
				}
				else if (m_clipRect.m_width + m_clipRect.m_x - 1 < p_x2) {
					code2 = 2;
				}
				if (p_y2 < m_clipRect.m_y) {
					code2 |= 4;
				}
				else if (m_clipRect.m_height + m_clipRect.m_y - 1 < p_y2) {
					code2 |= 8;
				}
			}
		} while ((code1 | code2) != 0);
	}
	return 0;
}
