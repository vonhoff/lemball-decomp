#include "Visos/Graphics/CSurface.h"

#include "Visos/Resources/CResZRLE.h"

// FUNCTION: LEMBALL 0x00477660
void CSurface::BlitZRLENoClipR(const CVSRect& p_rect, CResZRLE* p_zrle, unsigned int p_reverse)
{
	int startX = p_rect.m_x + p_rect.m_width - 1;
	int y = p_rect.m_y;
	int step = 1;
	if (p_reverse != 0) {
		step = SURFACE_STEP_BACKWARD;
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
				if (run < ZRLE_ROW_END_MARKER) {
					dst -= run;
				}
				else if (run > ZRLE_ROW_END_MARKER) {
					run &= ZRLE_RUN_LENGTH_MASK;
					int i = run;
					unsigned char* copySrc = src;
					unsigned char* copyDst = dst;
					for (; i > 0; i--) {
						*copyDst-- = *copySrc++;
					}
					dst -= run;
					src += run;
				}
			} while (run != ZRLE_ROW_END_MARKER);
			y += step;
			row++;
		} while (row < p_rect.m_height);
	}
}
