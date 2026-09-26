#include "../../CSurface.h"

#include "../../../Resources/CResZRLE.h"

// FUNCTION: LEMBALL 0x00477440
void CSurface::BlitZRLENoClipQZBuff(const CVsRect& p_rect, CResZRLE* p_zrle, unsigned short p_depth)
{
	int x = p_rect.m_x;
	int y = p_rect.m_y;
	unsigned char* src = p_zrle->GetData();
	int row = 0;
	if (p_rect.m_height > 0) {
		do {
			unsigned char* dst = (unsigned char*) m_lines[y] + x;
			unsigned short* zlines = (unsigned short*) ((unsigned char*) CPVZBuffSurface::m_bitmap.m_lines[y] + x * 2);
			unsigned char run;
			do {
				run = *src++;
				if (run < 0x80) {
					dst += run;
					zlines += run;
				}
				else if (run > 0x80) {
					run &= 0x7f;
					unsigned short* copyZ = zlines;
					unsigned char count = run;
					unsigned char* copySrc = src;
					unsigned char* copyDst = dst;
					while (count > 0) {
						count--;
						if (*copyZ <= p_depth) {
							*copyDst = *copySrc;
						}
						copyDst++;
						copyZ++;
						copySrc++;
					}
					src += run;
					dst += run;
					zlines += run;
				}
			} while (run != 0x80);
			row++;
			y++;
		} while (row < p_rect.m_height);
	}
}
