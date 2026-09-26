#include "Visos/Graphics/CSurface.h"

#include "Visos/Resources/CResZRLE.h"

// FUNCTION: LEMBALL 0x00477660
void CSurface::BlitZRLENoClipR(const CVsRect& p_rect, CResZRLE* p_zrle, unsigned int p_reverse)
{
	int startX = p_rect.m_x + p_rect.m_width - 1;
	int y = p_rect.m_y;
	int step = 1;
	if (p_reverse != 0) {
		step = -1;
		y += p_rect.m_height - 1;
	}
	unsigned char* src = p_zrle->GetData();
	int row = 0;
	if (p_rect.m_height > 0) {
		do {
			unsigned char* dst = (unsigned char*) m_lines[y] + startX;
			unsigned char run;
			do {
				run = *src++;
				if (run < 0x80) {
					dst -= run;
				}
				else if (run > 0x80) {
					run &= 0x7f;
					int i = run;
					unsigned char* copySrc = src;
					unsigned char* copyDst = dst;
					for (; i > 0; i--) {
						*copyDst-- = *copySrc++;
					}
					dst -= run;
					src += run;
				}
			} while (run != 0x80);
			y += step;
			row++;
		} while (row < p_rect.m_height);
	}
}
