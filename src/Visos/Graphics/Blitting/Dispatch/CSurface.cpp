#include "../../CSurface.h"

#include "../../../Foundation/CVSOStream.h"
#include "../../../Foundation/VsDebug.h"
#include "../../../Resources/CResZRLE.h"
#include "../../CRemap.h"

extern char g_szClippingHeightTo[];
extern char g_szClippingDotNewline[];
extern char g_szClippingWidthTo[];
extern char g_szClippingHighNewline[];
extern char g_szClippingWideAnd[];
extern char g_szWarningZrleIs[];

#pragma inline_depth(255)

// FUNCTION: LEMBALL 0x00478bb0
void CSurface::BlitZRLE(int p_x,
						int p_y,
						CResZRLE* p_zrle,
						unsigned int p_flags,
						CRemap* p_remap,
						unsigned short p_depth)
{
	struct {
		short unused;
		short warningHeight;
		short destination[4];
		short clip[4];
	} frame;
	CResZRLE* resource;
	short zHeight;
	short zWidth;
	unsigned int flags;
	CVsRect* dest;
	CVsRect* clipped;
	int width;

	resource = p_zrle;
	dest = (CVsRect*) frame.destination;
	clipped = (CVsRect*) frame.clip;
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
	if ((flags & 0x400) == 0) {
		dest->m_x = (short) (dest->m_x + resource->m_x);
		dest->m_y = (short) (dest->m_y + resource->m_y);
	}
	clipped->m_height = 0;
	clipped->m_width = 0;
	clipped->m_y = 0;
	clipped->m_x = 0;
	if ((short) dest->m_width > 0xff || (short) dest->m_height > 0xff) {
		CVSOStream& warning = *g_pDebugOutput << g_szWarningZrleIs;
		frame.warningHeight = dest->m_height;
		CVSOStream& heightOutput = warning << width << g_szClippingWideAnd;
		heightOutput << (int) frame.warningHeight << g_szClippingHighNewline;
		if ((short) dest->m_width > 0xff) {
			*g_pDebugOutput << g_szClippingWidthTo << 0xff << g_szClippingDotNewline;
			dest->m_width = 0xff;
		}
		if ((short) dest->m_height > 0xff) {
			*g_pDebugOutput << g_szClippingHeightTo << 0xff << g_szClippingDotNewline;
			dest->m_height = 0xff;
		}
	}
	{
		CRemap* remap;

		remap = p_remap;
		if (ClipRect(*dest, clipped) == 0) {
			AddToChangeList(*dest);
			if ((flags & 0x40000) != 0) {
				if (remap == 0) {
					BlitZRLENoClipZBuff(*dest, resource, p_depth);
					return;
				}
				BlitZRLENoClipZBuffRemap(*dest, resource, p_depth, remap->m_remap);
				return;
			}
			if ((flags & 0x80000) != 0) {
				if (remap == 0) {
					BlitZRLENoClipQZBuff(*dest, resource, p_depth);
					return;
				}
				BlitZRLENoClipQZBuffRemap(*dest, resource, p_depth, remap->m_remap);
				return;
			}
			if (remap == 0) {
				if ((flags & 1) != 0) {
					BlitZRLENoClipR(*dest, resource, (flags & 2) >> 1);
					return;
				}
				BlitZRLENoClip(*dest, resource, (flags & 2) >> 1);
				return;
			}
			if ((flags & 1) != 0) {
				BlitZRLENoClipRemapR(*dest, resource, (flags & 2) >> 1, remap->m_remap);
				return;
			}
			BlitZRLENoClipRemap(*dest, resource, (flags & 2) >> 1, remap->m_remap);
			return;
		}
		if (clipped->m_width <= 0 || clipped->m_height <= 0) {
			return;
		}
		AddToChangeList(*dest);
		if ((flags & 0x40000) != 0) {
			if (remap == 0) {
				BlitZRLEClipZBuff(*dest, *clipped, resource, p_depth);
				return;
			}
			BlitZRLEClipZBuffRemap(*dest, *clipped, resource, p_depth, remap->m_remap);
			return;
		}
		if ((flags & 0x80000) != 0) {
			if (remap == 0) {
				BlitZRLEClipQZBuff(*dest, *clipped, resource, p_depth);
				return;
			}
			BlitZRLEClipQZBuffRemap(*dest, *clipped, resource, p_depth, remap->m_remap);
			return;
		}
		if (remap == 0) {
			if ((flags & 1) != 0) {
				BlitZRLEClipR(*dest, *clipped, resource, (flags & 2) >> 1);
				return;
			}
			BlitZRLEClip(*dest, *clipped, resource, (flags & 2) >> 1);
			return;
		}
		if ((flags & 1) != 0) {
			BlitZRLEClipRemapR(*dest, *clipped, resource, (flags & 2) >> 1, remap->m_remap);
			return;
		}
		BlitZRLEClipRemap(*dest, *clipped, resource, (flags & 2) >> 1, remap->m_remap);
	}
}
