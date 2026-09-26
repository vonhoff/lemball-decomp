#include "Visos/Graphics/CSurface.h"

#include "Visos/Foundation/CVsRect.h"
#include "Visos/Graphics/CPVZBuffSurface.h"
#include "Visos/Resources/CResZRLE.h"

// FUNCTION: LEMBALL 0x00477540
void CSurface::BlitZRLENoClipQZBuffRemap(const CVsRect& p_rect,
										 CResZRLE* p_zrle,
										 unsigned short p_depth,
										 unsigned char* p_remap)
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
					int i = run;
					unsigned char* copySrc = src;
					unsigned char* copyDst = dst;
					for (; i > 0; i--) {
						if (*copyZ <= p_depth) {
							*copyDst = p_remap[*copySrc];
						}
						copyZ++;
						copyDst++;
						copySrc++;
					}
					dst += run;
					zlines += run;
					src += run;
				}
			} while (run != 0x80);
			row++;
			y++;
		} while (row < p_rect.m_height);
	}
}
