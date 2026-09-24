#include "../CMaze.h"

#include <string.h>

extern const unsigned char g_aChangeBitMasks[8][4];

// FUNCTION: LEMBALL 0x00423650
bool CMaze::BIteration(unsigned int& p_reached, unsigned int& p_noChanges)
{
	p_reached = 0;
	if (m_endY < 0 || m_endX < 0 || m_height <= m_endY || m_width <= m_endX || m_distances[m_endY][m_endX] == 0xffff) {
		return 1;
	}

	bool changed = false;
	SwapChange();
	int radius = ++m_radius;
	int xMin = m_startX - radius;
	int xMax = m_startX + radius;
	int yMin = m_startY - radius;
	int yMax = m_startY + radius;
	if (xMin < 0) {
		xMin = 0;
	}
	if (m_width - 1 < xMax) {
		xMax = m_width - 1;
	}
	if (yMin < 0) {
		yMin = 0;
	}
	if (m_height - 1 < yMax) {
		yMax = m_height - 1;
	}

	unsigned char* pChange;
	if (m_changeSelect != 0) {
		pChange = m_changeA + ((yMin * 0x80 + xMin) >> 3);
	}
	else {
		pChange = m_changeB + ((yMin * 0x80 + xMin) >> 3);
	}

	if (yMin <= yMax) {
		unsigned int mask;
		memcpy(&mask, &g_aChangeBitMasks[xMin & 7][0], 1);
		int y = yMin;
		do {
			unsigned short* pDistance = m_distances[y] + xMin;
			unsigned char* pChangeRow = pChange;
			int x;
			unsigned char currentMask;
			memcpy(&currentMask, &mask, 1);
			for (x = xMin; x <= xMax; x++, pDistance++) {
				if ((currentMask & *pChangeRow) != 0 && *pDistance != 0xffff && CalcNewDistance(x, y)) {
					changed = true;
					UpdateChangeNext(x, y);
				}
				currentMask >>= 1;
				if (currentMask == 0) {
					currentMask = 0x80;
					pChangeRow++;
				}
			}
			pChange += 0x10;
			y++;
		} while (y <= yMax);
	}

	p_reached = m_distances[m_endY][m_endX] != 0xff00;
	p_noChanges = !p_reached || changed ? 0 : 1;
	if (!p_reached && changed && m_radius < 0x14) {
		return 0;
	}
	return 1;
}
