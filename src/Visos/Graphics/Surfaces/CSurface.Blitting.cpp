#include "CSurface.h"

#include "Visos/Graphics/Primitives/CBigBitmap.h"
#include "Visos/Graphics/Primitives/CBitmap.h"
#include "Visos/Graphics/Primitives/CZRLE.h"
#include "Visos/Resources/Types/CResBITMAP.h"
#include "Visos/Resources/Types/CResZRLE.h"
#include "Visos/Graphics/Palettes/CRemap.h"
#include "Visos/Math/CVSSize.h"
#include "Visos/Streams/CVSOStream.h"
#include <string.h>

// GLOBAL: LEMBALL 0x004a2d50
char g_szClippingHeightTo[] = "Clipping height to ";

// GLOBAL: LEMBALL 0x004a2d64
char g_szClippingDotNewline[] = ".\r\n";

// GLOBAL: LEMBALL 0x004a2d68
char g_szClippingWidthTo[] = "Clipping width to ";

// GLOBAL: LEMBALL 0x004a2d7c
char g_szClippingHighNewline[] = " high.\r\n";

// GLOBAL: LEMBALL 0x004a2d84
char g_szClippingWideAnd[] = " wide and ";

// GLOBAL: LEMBALL 0x004a2d90
char g_szWarningZrleIs[] = "Warning: ZRLE is ";

// FUNCTION: LEMBALL 0x0046dbc0
void CSurface::Blit(CBigBitmap* p_primitive, CResBITMAP* p_bitmap)
{
	Blit(static_cast<CBitmap*>(p_primitive), p_bitmap);
}

// FUNCTION: LEMBALL 0x004766f0
void CSurface::BlitZRLEClip(const CVSRect& p_rect, const CVSRect& p_clip, CResZRLE* p_zrle, unsigned int p_reverse)
{
	int x = p_rect.m_x;
	int step = 1;
	int y = p_rect.m_y;
	unsigned char* src = p_zrle->GetData();
	if (p_reverse != 0) {
		step = SURFACE_STEP_BACKWARD;
		y += p_rect.m_height - 1;
		int skipRows = (p_zrle->m_height - p_clip.m_y) - p_rect.m_height;
		if (skipRows > 0) {
			do {
				unsigned char run;
				do {
					run = *src++;
					if (run > ZRLE_ROW_END_MARKER) {
						run &= ZRLE_RUN_LENGTH_MASK;
						src += run;
					}
				} while (run != ZRLE_ROW_END_MARKER);
				skipRows--;
			} while (skipRows != 0);
		}
	}
	else {
		if (p_clip.m_y > 0) {
			int skipRows = p_clip.m_y;
			do {
				unsigned char run;
				do {
					run = *src++;
					if (run > ZRLE_ROW_END_MARKER) {
						run &= ZRLE_RUN_LENGTH_MASK;
						src += run;
					}
				} while (run != ZRLE_ROW_END_MARKER);
				skipRows--;
			} while (skipRows != 0);
		}
	}
	int row = 0;
	if (p_rect.m_height > 0) {
		do {
			int width = p_rect.m_width;
			int clipX = p_clip.m_x;
			unsigned char* dst = (unsigned char*) m_lines[y] + x;
			unsigned char run;
			do {
				run = *src++;
				if (run < ZRLE_ROW_END_MARKER) {
					clipX -= run;
					if (clipX < 0) {
						width += clipX;
						dst -= clipX;
					}
				}
				else if (run > ZRLE_ROW_END_MARKER) {
					run &= ZRLE_RUN_LENGTH_MASK;
					clipX -= run;
					if (clipX < 0) {
						int copyLen = -clipX;
						if (copyLen < width) {
							memcpy(dst, src + run + clipX, copyLen);
						}
						else {
							memcpy(dst, src + run + clipX, width);
						}
						dst += copyLen;
						width -= copyLen;
					}
					src += run;
				}
				if (run == ZRLE_ROW_END_MARKER) {
					break;
				}
			} while (clipX > 0);
			if (run != ZRLE_ROW_END_MARKER) {
				do {
					if (width <= 0) {
						break;
					}
					run = *src++;
					if (width > 0) {
						if (run < ZRLE_ROW_END_MARKER) {
							dst += run;
							width -= run;
						}
						else if (run > ZRLE_ROW_END_MARKER) {
							run &= ZRLE_RUN_LENGTH_MASK;
							if (run < width) {
								memcpy(dst, src, run);
								width -= run;
								dst += run;
							}
							else {
								memcpy(dst, src, width);
								dst += width;
								width = 0;
							}
							src += run;
						}
					}
				} while (run != ZRLE_ROW_END_MARKER);
			}
			while (run != ZRLE_ROW_END_MARKER) {
				run = *src++;
				if (run > ZRLE_ROW_END_MARKER) {
					run &= ZRLE_RUN_LENGTH_MASK;
					src += run;
				}
			}
			y += step;
			row++;
		} while (row < p_rect.m_height);
	}
}

// FUNCTION: LEMBALL 0x00476910
void CSurface::BlitZRLEClipZBuff(const CVSRect& p_rect, const CVSRect& p_clip, CResZRLE* p_zrle, unsigned short p_depth)
{
	unsigned char* src = p_zrle->GetData();
	if (p_clip.m_y > 0) {
		int skipRows = p_clip.m_y;
		do {
			unsigned char run;
			do {
				run = *src++;
				if (run > ZRLE_ROW_END_MARKER) {
					run &= ZRLE_RUN_LENGTH_MASK;
					src += run;
				}
			} while (run != ZRLE_ROW_END_MARKER);
			skipRows--;
		} while (skipRows != 0);
	}
	int row = 0;
	int x = p_rect.m_x;
	int y = p_rect.m_y;
	if (p_rect.m_height > 0) {
		do {
			int width = p_rect.m_width;
			int clipX = p_clip.m_x;
			unsigned short* zlines = (unsigned short*) ((unsigned char*) CPVZBuffSurface::m_bitmap.m_lines[y] + x * 2);
			unsigned char* dst = (unsigned char*) m_lines[y] + x;
			unsigned char run;
			do {
				run = *src++;
				if (run < ZRLE_ROW_END_MARKER) {
					clipX -= run;
					if (clipX < 0) {
						dst -= clipX;
						zlines -= clipX;
						width += clipX;
					}
				}
				else if (run > ZRLE_ROW_END_MARKER) {
					run &= ZRLE_RUN_LENGTH_MASK;
					clipX -= run;
					if (clipX < 0) {
						int copyLen = -clipX;
						if (copyLen < width) {
							memcpy(dst, clipX + src + run, copyLen);
							for (unsigned int i = 0; i < (unsigned int) copyLen; i++) {
								zlines[i] = p_depth;
							}
						}
						else {
							memcpy(dst, clipX + src + run, width);
							for (int i = 0; i < width; i++) {
								zlines[i] = p_depth;
							}
						}
						dst += copyLen;
						width -= copyLen;
						zlines += copyLen;
					}
					src += run;
				}
				if (run == ZRLE_ROW_END_MARKER) {
					break;
				}
			} while (clipX > 0);
			if (run != ZRLE_ROW_END_MARKER) {
				do {
					if (width <= 0) {
						break;
					}
					run = *src++;
					if (width > 0) {
						if (run < ZRLE_ROW_END_MARKER) {
							dst += run;
							zlines += run;
							width -= run;
						}
						else if (run > ZRLE_ROW_END_MARKER) {
							run &= ZRLE_RUN_LENGTH_MASK;
							if (run < width) {
								memcpy(dst, src, run);
								for (unsigned int i = 0; i < (unsigned int) run; i++) {
									zlines[i] = p_depth;
								}
								dst += run;
								width -= run;
								zlines += run;
							}
							else {
								memcpy(dst, src, width);
								for (int i = 0; i < width; i++) {
									zlines[i] = p_depth;
								}
								dst += width;
								zlines += width;
								width = 0;
							}
							src += run;
						}
					}
				} while (run != ZRLE_ROW_END_MARKER);
			}
			while (run != ZRLE_ROW_END_MARKER) {
				run = *src++;
				if (run > ZRLE_ROW_END_MARKER) {
					run &= ZRLE_RUN_LENGTH_MASK;
					src += run;
				}
			}
			y++;
			row++;
		} while (row < p_rect.m_height);
	}
}

// FUNCTION: LEMBALL 0x00476bf0
void CSurface::BlitZRLEClipQZBuff(const CVSRect& p_rect,
								  const CVSRect& p_clip,
								  CResZRLE* p_zrle,
								  unsigned short p_depth)
{
	unsigned char* src = p_zrle->GetData();
	if (p_clip.m_y > 0) {
		int skipRows = p_clip.m_y;
		do {
			unsigned char run;
			do {
				run = *src++;
				if (run > ZRLE_ROW_END_MARKER) {
					run &= ZRLE_RUN_LENGTH_MASK;
					src += run;
				}
			} while (run != ZRLE_ROW_END_MARKER);
			skipRows--;
		} while (skipRows != 0);
	}
	int row = 0;
	int x = p_rect.m_x;
	int y = p_rect.m_y;
	if (p_rect.m_height > 0) {
		do {
			unsigned short* zlines;
			int runCount;
			int width = p_rect.m_width;
			int clipX = p_clip.m_x;
			zlines = (unsigned short*) ((unsigned char*) CPVZBuffSurface::m_bitmap.m_lines[y] + x * 2);
			unsigned char* dst = (unsigned char*) m_lines[y] + x;
			unsigned char run;
			do {
				run = *src++;
				if (run < ZRLE_ROW_END_MARKER) {
					clipX -= run;
					if (clipX < 0) {
						dst -= clipX;
						zlines -= clipX;
						width += clipX;
					}
				}
				else if (run > ZRLE_ROW_END_MARKER) {
					run &= ZRLE_RUN_LENGTH_MASK;
					runCount = run;
					clipX -= runCount;
					if (clipX < 0) {
						int copyLen = -clipX;
						if (copyLen < width) {
							unsigned char count = (unsigned char) copyLen;
							unsigned short* copyZ = zlines;
							unsigned char* copySrc = src + runCount + clipX;
							unsigned char* copyDst = dst;
							while (count != 0) {
								count--;
								if (*copyZ <= p_depth) {
									*copyDst = *copySrc;
								}
								copyDst++;
								copyZ++;
								copySrc++;
							}
						}
						else {
							unsigned char count = (unsigned char) width;
							unsigned short* copyZ = zlines;
							unsigned char* copySrc = src + runCount + clipX;
							unsigned char* copyDst = dst;
							while (count != 0) {
								count--;
								if (*copyZ <= p_depth) {
									*copyDst = *copySrc;
								}
								copyDst++;
								copyZ++;
								copySrc++;
							}
						}
						dst += copyLen;
						width -= copyLen;
						zlines += copyLen;
					}
					src += runCount;
				}
				if (run == ZRLE_ROW_END_MARKER) {
					break;
				}
			} while (clipX > 0);
			if (run != ZRLE_ROW_END_MARKER) {
				do {
					if (width <= 0) {
						break;
					}
					run = *src++;
					if (width > 0) {
						if (run < ZRLE_ROW_END_MARKER) {
							dst += run;
							width -= run;
							zlines += run;
						}
						else if (run > ZRLE_ROW_END_MARKER) {
							run &= ZRLE_RUN_LENGTH_MASK;
							runCount = run;
							if (runCount < width) {
								unsigned char count = (unsigned char) runCount;
								unsigned short* copyZ = zlines;
								unsigned char* copySrc = src;
								unsigned char* copyDst = dst;
								while (count != 0) {
									count--;
									if (*copyZ <= p_depth) {
										*copyDst = *copySrc;
									}
									copyDst++;
									copyZ++;
									copySrc++;
								}
								dst += runCount;
								width -= runCount;
								zlines += runCount;
							}
							else {
								unsigned char count = (unsigned char) width;
								unsigned short* copyZ = zlines;
								unsigned char* copySrc = src;
								unsigned char* copyDst = dst;
								while (count != 0) {
									count--;
									if (*copyZ <= p_depth) {
										*copyDst = *copySrc;
									}
									copyDst++;
									copyZ++;
									copySrc++;
								}
								dst += width;
								zlines += width;
								width = 0;
							}
							src += runCount;
						}
					}
				} while (run != ZRLE_ROW_END_MARKER);
			}
			while (run != ZRLE_ROW_END_MARKER) {
				run = *src++;
				if (run > ZRLE_ROW_END_MARKER) {
					run &= ZRLE_RUN_LENGTH_MASK;
					src += run;
				}
			}
			y++;
			row++;
		} while (row < p_rect.m_height);
	}
}

// FUNCTION: LEMBALL 0x00476ee0
void CSurface::BlitZRLEClipR(const CVSRect& p_rect, const CVSRect& p_clip, CResZRLE* p_zrle, unsigned int p_reverse)
{
	unsigned char* src = p_zrle->GetData();
	short sourceWidth = p_zrle->m_width;
	short sourceHeight = p_zrle->m_height;
	int x = p_rect.m_x;
	int step = 1;
	int y = p_rect.m_y;
	if (p_reverse != 0) {
		step = SURFACE_STEP_BACKWARD;
		y += p_rect.m_height - 1;
		int skipRows = sourceHeight - p_clip.m_y - p_rect.m_height;
		if (skipRows > 0) {
			do {
				unsigned char run;
				do {
					run = *src++;
					if (run > ZRLE_ROW_END_MARKER) {
						run &= ZRLE_RUN_LENGTH_MASK;
						src += run;
					}
				} while (run != ZRLE_ROW_END_MARKER);
				skipRows--;
			} while (skipRows != 0);
		}
	}
	else if (p_clip.m_y > 0) {
		int skipRows = p_clip.m_y;
		do {
			unsigned char run;
			do {
				run = *src++;
				if (run > ZRLE_ROW_END_MARKER) {
					run &= ZRLE_RUN_LENGTH_MASK;
					src += run;
				}
			} while (run != ZRLE_ROW_END_MARKER);
			skipRows--;
		} while (skipRows != 0);
	}
	int row = 0;
	if (p_rect.m_height > 0) {
		do {
			int width = p_rect.m_width;
			int skipX = sourceWidth - p_clip.m_x - width;
			unsigned char* dst = (unsigned char*) m_lines[y] + x;
			unsigned char run;
			do {
				run = *src++;
				if (skipX > 0) {
					if (run < ZRLE_ROW_END_MARKER) {
						skipX -= run;
						if (skipX < 0) {
							dst += skipX;
							width += skipX;
						}
					}
					else if (run > ZRLE_ROW_END_MARKER) {
						run &= ZRLE_RUN_LENGTH_MASK;
						int count = run;
						skipX -= count;
						if (skipX < 0) {
							int copyLength = -skipX;
							if (copyLength < width) {
								int remaining = copyLength;
								unsigned char* copySrc = src + count + skipX;
								unsigned char* copyDst = dst;
								while (remaining > 0) {
									*copyDst-- = *copySrc++;
									remaining--;
								}
							}
							else {
								int remaining = width;
								unsigned char* copySrc = src + count + skipX;
								unsigned char* copyDst = dst;
								while (remaining > 0) {
									*copyDst-- = *copySrc++;
									remaining--;
								}
							}
							dst -= copyLength;
							width -= copyLength;
						}
						src += count;
					}
				}
				else if (width > 0) {
					if (run < ZRLE_ROW_END_MARKER) {
						width -= run;
						dst -= run;
					}
					else if (run > ZRLE_ROW_END_MARKER) {
						run &= ZRLE_RUN_LENGTH_MASK;
						int count = run;
						if (count < width) {
							unsigned char* copySrc;
							unsigned char* copyDst;
							int remaining = count;
							copySrc = src;
							copyDst = dst;
							while (remaining > 0) {
								*copyDst-- = *copySrc++;
								remaining--;
							}
							src += count;
							dst -= count;
							width -= count;
						}
						else {
							int remaining = 0;
							unsigned char* copySrc = src;
							unsigned char* copyDst = dst;
							while (remaining < width) {
								*copyDst-- = *copySrc++;
								remaining++;
							}
							src += count;
							dst -= width;
							width = 0;
						}
					}
				}
				else if (run > ZRLE_ROW_END_MARKER) {
					run &= ZRLE_RUN_LENGTH_MASK;
					src += run;
				}
			} while (run != ZRLE_ROW_END_MARKER);
			y += step;
			row++;
		} while (row < p_rect.m_height);
	}
}

// FUNCTION: LEMBALL 0x00477130
void CSurface::BlitZRLENoClip(const CVSRect& p_rect, CResZRLE* p_zrle, unsigned int p_reverse)
{
	int x = p_rect.m_x;
	int step = 1;
	int y = p_rect.m_y;
	if (p_reverse != 0) {
		step = SURFACE_STEP_BACKWARD;
		y += p_rect.m_height - 1;
	}
	unsigned char* src = p_zrle->GetData();
	int row = 0;
	if (p_rect.m_height > 0) {
		do {
			unsigned char* dst = (unsigned char*) m_lines[y] + x;
			unsigned char run;
			do {
				run = *src++;
				if (run < ZRLE_ROW_END_MARKER) {
					dst += run;
				}
				else if (run > ZRLE_ROW_END_MARKER) {
					run &= ZRLE_RUN_LENGTH_MASK;
					memcpy(dst, src, run);
					dst += run;
					src += run;
				}
			} while (run != ZRLE_ROW_END_MARKER);
			y += step;
			row++;
		} while (row < p_rect.m_height);
	}
}

// FUNCTION: LEMBALL 0x00477200
void CSurface::BlitZRLENoClipZBuff(const CVSRect& p_rect, CResZRLE* p_zrle, unsigned short p_depth)
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
				if (run < ZRLE_ROW_END_MARKER) {
					dst += run;
					zlines += run;
				}
				else if (run > ZRLE_ROW_END_MARKER) {
					run &= ZRLE_RUN_LENGTH_MASK;
					memcpy(dst, src, run);
					for (unsigned int i = 0; i < run; i++) {
						zlines[i] = p_depth;
					}
					zlines += run;
					dst += run;
					src += run;
				}
			} while (run != ZRLE_ROW_END_MARKER);
			row++;
			y++;
		} while (row < p_rect.m_height);
	}
}

// FUNCTION: LEMBALL 0x00477310
void CSurface::BlitZRLENoClipZBuffRemap(const CVSRect& p_rect,
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
				if (run < ZRLE_ROW_END_MARKER) {
					dst += run;
					zlines += run;
				}
				else if (run > ZRLE_ROW_END_MARKER) {
					run &= ZRLE_RUN_LENGTH_MASK;
					int count = run;
					unsigned char* copySrc = src;
					unsigned char* copyDst = dst;
					for (; count > 0; count--) {
						*copyDst++ = p_remap[*copySrc];
						copySrc++;
					}
					for (unsigned int i = 0; i < run; i++) {
						zlines[i] = p_depth;
					}
					dst += run;
					zlines += run;
					src += run;
				}
			} while (run != ZRLE_ROW_END_MARKER);
			row++;
			y++;
		} while (row < p_rect.m_height);
	}
}

// FUNCTION: LEMBALL 0x00477440
void CSurface::BlitZRLENoClipQZBuff(const CVSRect& p_rect, CResZRLE* p_zrle, unsigned short p_depth)
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
				if (run < ZRLE_ROW_END_MARKER) {
					dst += run;
					zlines += run;
				}
				else if (run > ZRLE_ROW_END_MARKER) {
					run &= ZRLE_RUN_LENGTH_MASK;
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
			} while (run != ZRLE_ROW_END_MARKER);
			row++;
			y++;
		} while (row < p_rect.m_height);
	}
}

// FUNCTION: LEMBALL 0x00477540
void CSurface::BlitZRLENoClipQZBuffRemap(const CVSRect& p_rect,
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
				if (run < ZRLE_ROW_END_MARKER) {
					dst += run;
					zlines += run;
				}
				else if (run > ZRLE_ROW_END_MARKER) {
					run &= ZRLE_RUN_LENGTH_MASK;
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
			} while (run != ZRLE_ROW_END_MARKER);
			row++;
			y++;
		} while (row < p_rect.m_height);
	}
}

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

// FUNCTION: LEMBALL 0x00477740
void CSurface::BlitZRLEClipRemap(const CVSRect& p_rect,
								 const CVSRect& p_clip,
								 CResZRLE* p_zrle,
								 unsigned int p_reverse,
								 unsigned char* p_remap)
{
	unsigned char* src = p_zrle->GetData();
	int step = 1;
	int x = p_rect.m_x;
	int y = p_rect.m_y;
	if (p_reverse != 0) {
		step = SURFACE_STEP_BACKWARD;
		y += p_rect.m_height - 1;
		int skipRows = (p_zrle->m_height - p_clip.m_y) - p_rect.m_height;
		if (skipRows > 0) {
			do {
				unsigned char run;
				do {
					run = *src++;
					if (run > ZRLE_ROW_END_MARKER) {
						run &= ZRLE_RUN_LENGTH_MASK;
						src += run;
					}
				} while (run != ZRLE_ROW_END_MARKER);
				skipRows--;
			} while (skipRows != 0);
		}
	}
	else {
		if (p_clip.m_y > 0) {
			int skipRows = p_clip.m_y;
			do {
				unsigned char run;
				do {
					run = *src++;
					if (run > ZRLE_ROW_END_MARKER) {
						run &= ZRLE_RUN_LENGTH_MASK;
						src += run;
					}
				} while (run != ZRLE_ROW_END_MARKER);
				skipRows--;
			} while (skipRows != 0);
		}
	}
	int row = 0;
	if (p_rect.m_height > 0) {
		do {
			int width = p_rect.m_width;
			int clipX = p_clip.m_x;
			unsigned char* dst = (unsigned char*) m_lines[y] + x;
			unsigned char run;
			do {
				run = *src++;
				if (clipX > 0) {
					if (run < ZRLE_ROW_END_MARKER) {
						clipX -= run;
						if (clipX < 0) {
							dst -= clipX;
							width += clipX;
						}
					}
					else if (run > ZRLE_ROW_END_MARKER) {
						run &= ZRLE_RUN_LENGTH_MASK;
						int count = run;
						clipX -= count;
						if (clipX < 0) {
							int copyLen = -clipX;
							if (width > copyLen) {
								int i = copyLen;
								unsigned char* copySrc = src + count + clipX;
								unsigned char* copyDst = dst;
								for (; i > 0; i--) {
									*copyDst++ = p_remap[*copySrc++];
								}
							}
							else {
								int i = copyLen;
								unsigned char* copySrc = src + count + clipX;
								unsigned char* copyDst = dst;
								for (; i > 0; i--) {
									*copyDst++ = p_remap[*copySrc++];
								}
							}
							dst += copyLen;
							width -= copyLen;
						}
						src += count;
					}
				}
				else if (width > 0) {
					if (run < ZRLE_ROW_END_MARKER) {
						dst += run;
						width -= run;
					}
					else if (run > ZRLE_ROW_END_MARKER) {
						run &= ZRLE_RUN_LENGTH_MASK;
						int count = run;
						if (width > count) {
							int i = count;
							unsigned char* copySrc = src;
							unsigned char* copyDst = dst;
							for (; i > 0; i--) {
								*copyDst++ = p_remap[*copySrc++];
							}
							src += count;
							dst += count;
							width -= count;
						}
						else {
							int i = width;
							unsigned char* copySrc = src;
							unsigned char* copyDst = dst;
							for (; i > 0; i--) {
								*copyDst++ = p_remap[*copySrc++];
							}
							dst += width;
							src += count;
							width = 0;
						}
					}
				}
				else {
					if (run > ZRLE_ROW_END_MARKER) {
						run &= ZRLE_RUN_LENGTH_MASK;
						src += run;
					}
				}
			} while (run != ZRLE_ROW_END_MARKER);
			y += step;
			row++;
		} while (row < p_rect.m_height);
	}
}

// FUNCTION: LEMBALL 0x004779d0
void CSurface::BlitZRLEClipZBuffRemap(const CVSRect& p_rect,
									  const CVSRect& p_clip,
									  CResZRLE* p_zrle,
									  unsigned short p_depth,
									  unsigned char* p_remap)
{
	unsigned char* src = p_zrle->GetData();
	int x = p_rect.m_x;
	int y = p_rect.m_y;
	if (p_clip.m_y > 0) {
		int skipRows = p_clip.m_y;
		do {
			unsigned char run;
			do {
				run = *src++;
				if (run > ZRLE_ROW_END_MARKER) {
					run &= ZRLE_RUN_LENGTH_MASK;
					src += run;
				}
			} while (run != ZRLE_ROW_END_MARKER);
			skipRows--;
		} while (skipRows != 0);
	}
	int row = 0;
	if (p_rect.m_height > 0) {
		do {
			unsigned short* zlines = (unsigned short*) CPVZBuffSurface::m_bitmap.m_lines[y] + x;
			int width = p_rect.m_width;
			int clipX = p_clip.m_x;
			unsigned char* dst = (unsigned char*) m_lines[y] + x;
			unsigned char run;
			do {
				run = *src++;
				if (clipX > 0) {
					if (run < ZRLE_ROW_END_MARKER) {
						clipX -= run;
						if (clipX < 0) {
							dst -= clipX;
							zlines -= clipX;
							width += clipX;
						}
					}
					else if (run > ZRLE_ROW_END_MARKER) {
						run &= ZRLE_RUN_LENGTH_MASK;
						int count = run;
						clipX -= count;
						if (clipX < 0) {
							int copyLen = -clipX;
							if (copyLen < width) {
								int i = copyLen;
								unsigned char* copySrc = src + count + clipX;
								unsigned char* copyDst = dst;
								for (; i > 0; i--) {
									*copyDst++ = p_remap[*copySrc];
									copySrc++;
								}
							}
							else {
								int i = copyLen;
								unsigned char* copySrc = src + count + clipX;
								unsigned char* copyDst = dst;
								for (; i > 0; i--) {
									*copyDst++ = p_remap[*copySrc];
									copySrc++;
								}
							}
							dst += copyLen;
							width -= copyLen;
							zlines += copyLen;
						}
						src += count;
					}
				}
				else if (width > 0) {
					if (run < ZRLE_ROW_END_MARKER) {
						dst += run;
						width -= run;
						zlines += run;
					}
					else if (run > ZRLE_ROW_END_MARKER) {
						run &= ZRLE_RUN_LENGTH_MASK;
						int count = run;
						if (count < width) {
							int i = count;
							unsigned char* copySrc = src;
							unsigned char* copyDst = dst;
							for (; i > 0; i--) {
								*copyDst++ = p_remap[*copySrc];
								copySrc++;
							}
							dst += count;
							zlines += count;
							src += count;
							width -= count;
						}
						else {
							int i = width;
							unsigned char* copySrc = src;
							unsigned char* copyDst = dst;
							for (; i > 0; i--) {
								*copyDst++ = p_remap[*copySrc];
								copySrc++;
							}
							dst += width;
							zlines += width;
							src += count;
							width = 0;
						}
					}
				}
				else {
					if (run > ZRLE_ROW_END_MARKER) {
						run &= ZRLE_RUN_LENGTH_MASK;
						src += run;
					}
				}
			} while (run != ZRLE_ROW_END_MARKER);
			y++;
			row++;
		} while (row < p_rect.m_height);
	}
}

// FUNCTION: LEMBALL 0x00477c60
void CSurface::BlitZRLEClipQZBuffRemap(const CVSRect& p_rect,
									   const CVSRect& p_clip,
									   CResZRLE* p_zrle,
									   unsigned short p_depth,
									   unsigned char* p_remap)
{
	unsigned char* src = p_zrle->GetData();
	int x = p_rect.m_x;
	int y = p_rect.m_y;
	unsigned char run;
	if (p_clip.m_y > 0) {
		int skipRows = p_clip.m_y;
		do {
			do {
				run = *src++;
				if (run > ZRLE_ROW_END_MARKER) {
					run &= ZRLE_RUN_LENGTH_MASK;
					src += run;
				}
			} while (run != ZRLE_ROW_END_MARKER);
			skipRows--;
		} while (skipRows != 0);
	}
	int row = 0;
	if (p_rect.m_height > 0) {
		int lineIndex = y;
		do {
			int count;
			unsigned short* zlines = (unsigned short*) CPVZBuffSurface::m_bitmap.m_lines[lineIndex] + x;
			int copyLen;
			int width = p_rect.m_width;
			int clipX = p_clip.m_x;
			unsigned char* dst = (unsigned char*) m_lines[lineIndex] + x;
			do {
				run = *src++;
				if (clipX > 0) {
					if (run < ZRLE_ROW_END_MARKER) {
						clipX -= run;
						if (clipX < 0) {
							dst -= clipX;
							zlines -= clipX;
							width += clipX;
						}
					}
					else if (run > ZRLE_ROW_END_MARKER) {
						run &= ZRLE_RUN_LENGTH_MASK;
						count = run;
						clipX -= count;
						if (clipX < 0) {
							copyLen = -clipX;
							if (copyLen < width) {
								int i;
								unsigned char* copySrc;
								unsigned char* copyDst;
								unsigned short* copyZ;
								copyZ = zlines;
								copyDst = dst;
								i = copyLen;
								copySrc = src + count + clipX;
								for (; i > 0; i--) {
									if (*copyZ <= p_depth) {
										*copyDst = p_remap[*copySrc];
									}
									copyZ++;
									copyDst++;
									copySrc++;
								}
							}
							else {
								unsigned short* copyZ = zlines;
								int i = copyLen;
								unsigned char* copySrc = src + count + clipX;
								unsigned char* copyDst = dst;
								for (; i > 0; i--) {
									if (*copyZ <= p_depth) {
										*copyDst = p_remap[*copySrc];
									}
									copyZ++;
									copyDst++;
									copySrc++;
								}
							}
							dst += copyLen;
							width -= copyLen;
							zlines += copyLen;
						}
						src += count;
					}
				}
				else if (width > 0) {
					if (run < ZRLE_ROW_END_MARKER) {
						dst += run;
						width -= run;
						zlines += run;
					}
					else if (run > ZRLE_ROW_END_MARKER) {
						run &= ZRLE_RUN_LENGTH_MASK;
						count = run;
						if (width > count) {
							unsigned short* copyZ = zlines;
							int i = count;
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
							dst += count;
							zlines += count;
							src += count;
							width -= count;
						}
						else {
							unsigned short* copyZ = zlines;
							int i = width;
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
							dst += width;
							zlines += width;
							src += count;
							width = 0;
						}
					}
				}
				else {
					if (run > ZRLE_ROW_END_MARKER) {
						run &= ZRLE_RUN_LENGTH_MASK;
						src += run;
					}
				}
			} while (run != ZRLE_ROW_END_MARKER);
			lineIndex++;
			row++;
		} while (row < p_rect.m_height);
	}
}

// FUNCTION: LEMBALL 0x00477f50
void CSurface::BlitZRLEClipRemapR(const CVSRect& p_rect,
								  const CVSRect& p_clip,
								  CResZRLE* p_zrle,
								  unsigned int p_reverse,
								  unsigned char* p_remap)
{
	int startX = p_rect.m_x + p_rect.m_width - 1;
	int y = p_rect.m_y;
	int step = 1;
	unsigned char* src = p_zrle->GetData();
	short zrleWidth = p_zrle->m_width;
	short zrleHeight = p_zrle->m_height;
	if (p_reverse != 0) {
		step = SURFACE_STEP_BACKWARD;
		y += p_rect.m_height - 1;
		int skipRows = (zrleHeight - p_clip.m_y) - p_rect.m_height;
		if (skipRows > 0) {
			do {
				unsigned char run;
				do {
					run = *src++;
					if (run > ZRLE_ROW_END_MARKER) {
						run &= ZRLE_RUN_LENGTH_MASK;
						src += run;
					}
				} while (run != ZRLE_ROW_END_MARKER);
				skipRows--;
			} while (skipRows != 0);
		}
	}
	else {
		if (p_clip.m_y > 0) {
			int skipRows = p_clip.m_y;
			do {
				unsigned char run;
				do {
					run = *src++;
					if (run > ZRLE_ROW_END_MARKER) {
						run &= ZRLE_RUN_LENGTH_MASK;
						src += run;
					}
				} while (run != ZRLE_ROW_END_MARKER);
				skipRows--;
			} while (skipRows != 0);
		}
	}
	int row = 0;
	if (p_rect.m_height > 0) {
		int lineOffset = y * 4;
		int stepOffset = step * 4;
		do {
			int width = p_rect.m_width;
			int skipX = (zrleWidth - p_clip.m_x) - width;
			unsigned char* dst = *(unsigned char**) ((unsigned char*) m_lines + lineOffset) + startX;
			unsigned char run;
			do {
				run = *src++;
				if (skipX > 0) {
					if (run < ZRLE_ROW_END_MARKER) {
						skipX -= run;
						if (skipX < 0) {
							int overshoot = skipX;
							dst += overshoot;
							width += overshoot;
						}
					}
					else if (run > ZRLE_ROW_END_MARKER) {
						run &= ZRLE_RUN_LENGTH_MASK;
						int count = run;
						skipX -= count;
						if (skipX < 0) {
							int copyLen = -skipX;
							if (copyLen < width) {
								int i = copyLen;
								unsigned char* copySrc = src + count + skipX;
								unsigned char* copyDst = dst;
								for (; i > 0; i--) {
									*copyDst-- = p_remap[*copySrc++];
								}
							}
							else {
								int i = width;
								unsigned char* copySrc = src + count + skipX;
								unsigned char* copyDst = dst;
								for (; i > 0; i--) {
									*copyDst-- = p_remap[*copySrc++];
								}
							}
							dst -= copyLen;
							width -= copyLen;
						}
						src += count;
					}
				}
				else if (width > 0) {
					if (run < ZRLE_ROW_END_MARKER) {
						width -= run;
						dst -= run;
					}
					else if (run > ZRLE_ROW_END_MARKER) {
						run &= ZRLE_RUN_LENGTH_MASK;
						int count = run;
						if (count < width) {
							int i = count;
							unsigned char* copySrc = src;
							unsigned char* copyDst = dst;
							for (; i > 0; i--) {
								*copyDst-- = p_remap[*copySrc++];
							}
							src += count;
							dst -= count;
							width -= count;
						}
						else {
							int i = width;
							unsigned char* copySrc = src;
							unsigned char* copyDst = dst;
							for (; i > 0; i--) {
								*copyDst-- = p_remap[*copySrc++];
							}
							src += count;
							dst -= width;
							width = 0;
						}
					}
				}
				else {
					if (run > ZRLE_ROW_END_MARKER) {
						run &= ZRLE_RUN_LENGTH_MASK;
						src += run;
					}
				}
			} while (run != ZRLE_ROW_END_MARKER);
			lineOffset += stepOffset;
			row++;
		} while (row < p_rect.m_height);
	}
}

// FUNCTION: LEMBALL 0x004781e0
void CSurface::BlitZRLENoClipRemap(const CVSRect& p_rect,
								   CResZRLE* p_zrle,
								   unsigned int p_reverse,
								   unsigned char* p_remap)
{
	int x = p_rect.m_x;
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
			unsigned char* dst = (unsigned char*) m_lines[y] + x;
			unsigned char run;
			do {
				run = *src++;
				if (run < ZRLE_ROW_END_MARKER) {
					dst += run;
				}
				else if (run > ZRLE_ROW_END_MARKER) {
					run &= ZRLE_RUN_LENGTH_MASK;
					int count = (int) run;
					int i = count;
					unsigned char* copySrc = src;
					unsigned char* copyDst = dst;
					for (; i > 0; i--) {
						*copyDst++ = p_remap[*copySrc++];
					}
					dst += count;
					src += count;
				}
			} while (run != ZRLE_ROW_END_MARKER);
			y += step;
			row++;
		} while (row < p_rect.m_height);
	}
}

// FUNCTION: LEMBALL 0x004782d0
void CSurface::BlitZRLENoClipRemapR(const CVSRect& p_rect,
									CResZRLE* p_zrle,
									unsigned int p_reverse,
									unsigned char* p_remap)
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
					int count = (int) run;
					int i = count;
					unsigned char* copySrc = src;
					unsigned char* copyDst = dst;
					for (; i > 0; i--) {
						*copyDst-- = p_remap[*copySrc++];
					}
					dst -= count;
					src += count;
				}
			} while (run != ZRLE_ROW_END_MARKER);
			y += step;
			row++;
		} while (row < p_rect.m_height);
	}
}

#pragma inline_depth(0)
// FUNCTION: LEMBALL 0x004783c0
void CSurface::Blit(CZRLE* p_primitive, CResZRLE* p_zrle)
{
	unsigned int flags = p_primitive->m_flags;
	if ((flags & (ZRLE_DRAW_FLAG_Z_BUFFER | ZRLE_DRAW_FLAG_QUICK_Z_BUFFER)) == 0) {
		BlitZRLE((int) p_primitive->m_x, (int) p_primitive->m_y, p_zrle, flags, p_primitive->m_remap, 0);
		return;
	}
	{
		unsigned short stateDepth = (unsigned short) p_primitive->m_state;
		CRemap* remap = p_primitive->m_remap;
		int primitiveY = (int) p_primitive->m_y;
		int primitiveX = (int) p_primitive->m_x;

		if ((int) p_zrle->m_height * (int) p_zrle->m_width == 0) {
			return;
		}
		{
			CVSRect dest((short) primitiveX, (short) primitiveY, (CVSSize*) &p_zrle->m_width);
			if ((flags & ZRLE_DRAW_FLAG_ABSOLUTE_POSITION) == 0) {
				((CVSPoint*) &dest.m_x)->AddInPlace((CVSPoint*) &p_zrle->m_x);
			}
			{
				CVSRect clipped;

				if (dest.m_width > ZRLE_CLIPPED_DIMENSION_MAX || dest.m_height > ZRLE_CLIPPED_DIMENSION_MAX) {
					short warningWidth = dest.m_width;
					CVSOStream& warningStream = *g_pDebugOutput << g_szWarningZrleIs;
					short warningHeight = dest.m_height;
					CVSOStream& widthStream = warningStream << (int) warningWidth << g_szClippingWideAnd;
					widthStream << (int) warningHeight << g_szClippingHighNewline;
					if (dest.m_width > ZRLE_CLIPPED_DIMENSION_MAX) {
						*g_pDebugOutput << g_szClippingWidthTo << (int) ZRLE_CLIPPED_DIMENSION_MAX
										<< g_szClippingDotNewline;
						dest.m_width = ZRLE_CLIPPED_DIMENSION_MAX;
					}
					if (dest.m_height > ZRLE_CLIPPED_DIMENSION_MAX) {
						*g_pDebugOutput << g_szClippingHeightTo << (int) ZRLE_CLIPPED_DIMENSION_MAX
										<< g_szClippingDotNewline;
						dest.m_height = ZRLE_CLIPPED_DIMENSION_MAX;
					}
				}
				if (ClipRect(dest, &clipped) == 0) {
					AddToChangeList(dest);
					if ((flags & ZRLE_DRAW_FLAG_Z_BUFFER) != 0) {
						if (remap == NULL) {
							BlitZRLENoClipZBuff(dest, p_zrle, stateDepth);
							return;
						}
						BlitZRLENoClipZBuffRemap(dest, p_zrle, stateDepth, remap->m_remap);
						return;
					}
					if ((flags & ZRLE_DRAW_FLAG_QUICK_Z_BUFFER) != 0) {
						if (remap == NULL) {
							BlitZRLENoClipQZBuff(dest, p_zrle, stateDepth);
							return;
						}
						BlitZRLENoClipQZBuffRemap(dest, p_zrle, stateDepth, remap->m_remap);
						return;
					}
					if (remap == NULL) {
						if ((flags & ZRLE_DRAW_FLAG_MIRROR_HORIZONTAL) != 0) {
							BlitZRLENoClipR(dest, p_zrle, ((flags & ZRLE_DRAW_FLAG_REVERSE_VERTICAL) != 0));
							return;
						}
						BlitZRLENoClip(dest, p_zrle, ((flags & ZRLE_DRAW_FLAG_REVERSE_VERTICAL) != 0));
						return;
					}
					if ((flags & ZRLE_DRAW_FLAG_MIRROR_HORIZONTAL) != 0) {
						BlitZRLENoClipRemapR(dest,
											 p_zrle,
											 ((flags & ZRLE_DRAW_FLAG_REVERSE_VERTICAL) != 0),
											 remap->m_remap);
						return;
					}
					BlitZRLENoClipRemap(dest, p_zrle, ((flags & ZRLE_DRAW_FLAG_REVERSE_VERTICAL) != 0), remap->m_remap);
					return;
				}
				if (clipped.m_width <= 0 || clipped.m_height <= 0) {
					return;
				}
				AddToChangeList(dest);
				if ((flags & ZRLE_DRAW_FLAG_Z_BUFFER) != 0) {
					if (remap == NULL) {
						BlitZRLEClipZBuff(dest, clipped, p_zrle, stateDepth);
						return;
					}
					BlitZRLEClipZBuffRemap(dest, clipped, p_zrle, stateDepth, remap->m_remap);
					return;
				}
				if ((flags & ZRLE_DRAW_FLAG_QUICK_Z_BUFFER) != 0) {
					if (remap == NULL) {
						BlitZRLEClipQZBuff(dest, clipped, p_zrle, stateDepth);
						return;
					}
					BlitZRLEClipQZBuffRemap(dest, clipped, p_zrle, stateDepth, remap->m_remap);
					return;
				}
				if (remap == NULL) {
					if ((flags & ZRLE_DRAW_FLAG_MIRROR_HORIZONTAL) != 0) {
						BlitZRLEClipR(dest, clipped, p_zrle, ((flags & ZRLE_DRAW_FLAG_REVERSE_VERTICAL) != 0));
						return;
					}
					BlitZRLEClip(dest, clipped, p_zrle, ((flags & ZRLE_DRAW_FLAG_REVERSE_VERTICAL) != 0));
					return;
				}
				if ((flags & ZRLE_DRAW_FLAG_MIRROR_HORIZONTAL) != 0) {
					BlitZRLEClipRemapR(dest,
									   clipped,
									   p_zrle,
									   ((flags & ZRLE_DRAW_FLAG_REVERSE_VERTICAL) != 0),
									   remap->m_remap);
					return;
				}
				BlitZRLEClipRemap(dest,
								  clipped,
								  p_zrle,
								  ((flags & ZRLE_DRAW_FLAG_REVERSE_VERTICAL) != 0),
								  remap->m_remap);
			}
		}
	}
}

#pragma inline_depth(255)

// FUNCTION: LEMBALL 0x004787f0
void CSurface::Blit(CBitmap* p_primitive, CResBITMAP* p_bitmap)
{
	short x = p_primitive->m_x;
	short y = p_primitive->m_y;
	CVSRect sourceRect(p_primitive->m_sourceRect);
	if (sourceRect.m_height == 0 && sourceRect.m_width == 0) {
		sourceRect.m_width = p_bitmap->m_x;
		sourceRect.m_height = p_bitmap->m_y;
	}
	unsigned int flags = p_primitive->m_flags;
	if ((int) p_bitmap->m_y * (int) p_bitmap->m_x != 0) {
		CVSRect dest(sourceRect);
		dest.m_x = x;
		dest.m_y = y;
		CVSRect clip;
		if (ClipRect(dest, &clip) != 0) {
			if (clip.m_width <= 0 || clip.m_height <= 0) {
				return;
			}
			CVSSize clippedSize;
			clippedSize = clip;
			(CVSSize&) dest = clippedSize;
		}
		AddToChangeList(dest);
		int destX = dest.m_x;
		int destY = dest.m_y;
		int yStep = 1;
		if ((flags & CBitmap::BITMAP_REVERSE_ROWS) != 0) {
			yStep = SURFACE_STEP_BACKWARD;
			destY += dest.m_height - 1;
		}
		int bitmapWidth = (int) p_bitmap->m_x;
		unsigned char* source = p_bitmap->GetData() + ((int) sourceRect.m_y + (int) clip.m_y) * bitmapWidth +
								(int) sourceRect.m_x + (int) clip.m_x;
		if ((flags & CBitmap::BITMAP_TRANSPARENT_ZERO) != 0) {
			int sourceSkip = bitmapWidth - dest.m_width;
			int i = 0;
			if (dest.m_height > 0) {
				do {
					unsigned char* dst = (unsigned char*) m_lines[destY] + destX;
					int j = 0;
					if (dest.m_width > 0) {
						do {
							unsigned char pixel = *source;
							if (pixel != 0) {
								*dst = pixel;
							}
							j++;
							dst++;
							source++;
						} while (j < dest.m_width);
					}
					destY += yStep;
					source += sourceSkip;
					i++;
				} while (i < dest.m_height);
			}
		}
		else {
			int i = 0;
			if (dest.m_height > 0) {
				do {
					memcpy((unsigned char*) m_lines[destY] + destX, source, dest.m_width);
					destY += yStep;
					source += bitmapWidth;
					i++;
				} while (i < dest.m_height);
			}
		}
	}
}

// FUNCTION: LEMBALL 0x00478bb0
void CSurface::BlitZRLE(int p_x,
						int p_y,
						CResZRLE* p_zrle,
						unsigned int p_flags,
						CRemap* p_remap,
						unsigned short p_depth)
{
	struct {
		short m_unused;
		short m_warningHeight;
		short m_destination[4];
		short m_clip[4];
	} frame;
	CResZRLE* resource;
	short zHeight;
	short zWidth;
	unsigned int flags;
	CVSRect* dest;
	CVSRect* clipped;
	int width;

	resource = p_zrle;
	dest = (CVSRect*) frame.m_destination;
	clipped = (CVSRect*) frame.m_clip;
	zWidth = resource->m_width;
	zHeight = resource->m_height;
	width = (int) zWidth;
	if ((int) zHeight * width == 0) {
		return;
	}
	dest->m_width = zWidth;
	flags = p_flags;
	dest->m_height = zHeight;
	dest->m_x = (short) p_x;
	dest->m_y = (short) p_y;
	if ((flags & ZRLE_DRAW_FLAG_ABSOLUTE_POSITION) == 0) {
		dest->m_x = (short) (dest->m_x + resource->m_x);
		dest->m_y = (short) (dest->m_y + resource->m_y);
	}
	clipped->m_height = 0;
	clipped->m_width = 0;
	clipped->m_y = 0;
	clipped->m_x = 0;
	if (dest->m_width > ZRLE_CLIPPED_DIMENSION_MAX || dest->m_height > ZRLE_CLIPPED_DIMENSION_MAX) {
		CVSOStream& warning = *g_pDebugOutput << g_szWarningZrleIs;
		frame.m_warningHeight = dest->m_height;
		CVSOStream& heightOutput = warning << width << g_szClippingWideAnd;
		heightOutput << (int) frame.m_warningHeight << g_szClippingHighNewline;
		if (dest->m_width > ZRLE_CLIPPED_DIMENSION_MAX) {
			*g_pDebugOutput << g_szClippingWidthTo << (int) ZRLE_CLIPPED_DIMENSION_MAX << g_szClippingDotNewline;
			dest->m_width = ZRLE_CLIPPED_DIMENSION_MAX;
		}
		if (dest->m_height > ZRLE_CLIPPED_DIMENSION_MAX) {
			*g_pDebugOutput << g_szClippingHeightTo << (int) ZRLE_CLIPPED_DIMENSION_MAX << g_szClippingDotNewline;
			dest->m_height = ZRLE_CLIPPED_DIMENSION_MAX;
		}
	}
	{
		CRemap* remap;

		remap = p_remap;
		if (ClipRect(*dest, clipped) == 0) {
			AddToChangeList(*dest);
			if ((flags & ZRLE_DRAW_FLAG_Z_BUFFER) != 0) {
				if (remap == NULL) {
					BlitZRLENoClipZBuff(*dest, resource, p_depth);
					return;
				}
				BlitZRLENoClipZBuffRemap(*dest, resource, p_depth, remap->m_remap);
				return;
			}
			if ((flags & ZRLE_DRAW_FLAG_QUICK_Z_BUFFER) != 0) {
				if (remap == NULL) {
					BlitZRLENoClipQZBuff(*dest, resource, p_depth);
					return;
				}
				BlitZRLENoClipQZBuffRemap(*dest, resource, p_depth, remap->m_remap);
				return;
			}
			if (remap == NULL) {
				if ((flags & ZRLE_DRAW_FLAG_MIRROR_HORIZONTAL) != 0) {
					BlitZRLENoClipR(*dest, resource, ((flags & ZRLE_DRAW_FLAG_REVERSE_VERTICAL) != 0));
					return;
				}
				BlitZRLENoClip(*dest, resource, ((flags & ZRLE_DRAW_FLAG_REVERSE_VERTICAL) != 0));
				return;
			}
			if ((flags & ZRLE_DRAW_FLAG_MIRROR_HORIZONTAL) != 0) {
				BlitZRLENoClipRemapR(*dest, resource, ((flags & ZRLE_DRAW_FLAG_REVERSE_VERTICAL) != 0), remap->m_remap);
				return;
			}
			BlitZRLENoClipRemap(*dest, resource, ((flags & ZRLE_DRAW_FLAG_REVERSE_VERTICAL) != 0), remap->m_remap);
			return;
		}
		if (clipped->m_width <= 0 || clipped->m_height <= 0) {
			return;
		}
		AddToChangeList(*dest);
		if ((flags & ZRLE_DRAW_FLAG_Z_BUFFER) != 0) {
			if (remap == NULL) {
				BlitZRLEClipZBuff(*dest, *clipped, resource, p_depth);
				return;
			}
			BlitZRLEClipZBuffRemap(*dest, *clipped, resource, p_depth, remap->m_remap);
			return;
		}
		if ((flags & ZRLE_DRAW_FLAG_QUICK_Z_BUFFER) != 0) {
			if (remap == NULL) {
				BlitZRLEClipQZBuff(*dest, *clipped, resource, p_depth);
				return;
			}
			BlitZRLEClipQZBuffRemap(*dest, *clipped, resource, p_depth, remap->m_remap);
			return;
		}
		if (remap == NULL) {
			if ((flags & ZRLE_DRAW_FLAG_MIRROR_HORIZONTAL) != 0) {
				BlitZRLEClipR(*dest, *clipped, resource, ((flags & ZRLE_DRAW_FLAG_REVERSE_VERTICAL) != 0));
				return;
			}
			BlitZRLEClip(*dest, *clipped, resource, ((flags & ZRLE_DRAW_FLAG_REVERSE_VERTICAL) != 0));
			return;
		}
		if ((flags & ZRLE_DRAW_FLAG_MIRROR_HORIZONTAL) != 0) {
			BlitZRLEClipRemapR(*dest,
							   *clipped,
							   resource,
							   ((flags & ZRLE_DRAW_FLAG_REVERSE_VERTICAL) != 0),
							   remap->m_remap);
			return;
		}
		BlitZRLEClipRemap(*dest, *clipped, resource, ((flags & ZRLE_DRAW_FLAG_REVERSE_VERTICAL) != 0), remap->m_remap);
	}
}
