#include "GdiDevice.h"
#include "VsGdi.h"

#include <string.h>

#include "../Foundation/ChangeList.h"
#include "../Foundation/VsDebug.h"
#include "../Foundation/VsOStream.h"
#include "../Foundation/VsPoint.h"
#include "../Resources/ResBitmap.h"
#include "../Resources/ResPalette.h"
#include "../Resources/ResZrle.h"
#include "../Target/TargetDibContext.h"
#include "../Target/TargetDrawingContext.h"
#include "../Target/TargetGraphicsDriver.h"
#include "Bitmap.h"
#include "BitmapRes.h"
#include "Circle.h"
#include "ClipRect.h"
#include "CopyColourToBackBuff.h"
#include "CopyToBackBuff.h"
#include "FilledCircle.h"
#include "GdiDevice.h"
#include "Line.h"
#include "Point.h"
#include "ScreenScroll.h"
#include "SolidRect.h"
#include "ZBuffClear.h"
#include "Zrle.h"

#include <new.h>
#include <string.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

extern "C" __declspec(dllimport) int __stdcall GdiFlush();

struct SurfaceListHead {
	void* m_first;
	void* m_last;
	int m_count;
};

// GLOBAL: LEMBALL 0x004a2010
SurfaceListHead* g_pSurfaceList = 0;

// 68K 0x1010830c __ct__8CSurfaceFRC7CVSRectP8CSurface
// FUNCTION: LEMBALL 0x0046c050
Surface::Surface(const VsRect& p_rect, class Surface* p_parentSurface)
{
	void* storage;
	TargetDrawingContext* context;
	SurfaceListNode* node;
	ChangeList* list;
	VsSize viewSize;
	VsSize cellSize;
	int capacity;

	m_platformBitmap = 0;
	m_drawingPort = 0;
	m_presentX = 0;
	m_presentY = 0;
	m_unk0x528 = 0;
	m_unk0x52c = 0;
	m_unk0x530 = 0;
	m_unk0x54c = 1;
	m_changeList = 0;
	m_parentSurface = p_parentSurface;
	m_flag70 = 1;
	m_flag74 = 0;
	m_flag78 = 0;
	InitializeCriticalSection((CRITICAL_SECTION*) m_lock);
	cellSize.m_width = 8;
	cellSize.m_height = 8;
	viewSize.m_width = p_rect.m_width;
	viewSize.m_height = p_rect.m_height;
	if (p_parentSurface == (Surface*) g_pGdiHelperTarget) {
		capacity = 0x1000;
	}
	else {
		capacity = 0;
	}
	storage = operator new(0x4c);
	list = 0;
	if (storage != 0) {
		list = new (storage) ChangeList(capacity, viewSize, cellSize);
	}
	m_changeList = list;
	if (g_pSurfaceList == 0) {
		storage = operator new(0xc);
		if (storage == 0) {
			g_pSurfaceList = 0;
		}
		else {
			g_pSurfaceList = (SurfaceListHead*) storage;
			g_pSurfaceList->m_first = 0;
			g_pSurfaceList->m_last = 0;
			g_pSurfaceList->m_count = 0;
		}
	}
	if (p_parentSurface != 0) {
		storage = operator new(0xc);
		if (storage != 0) {
			node = (SurfaceListNode*) storage;
			node->m_surface = this;
			node->m_next = 0;
			node->m_prev = p_parentSurface->m_unk0x52c;
			if (p_parentSurface->m_unk0x52c != 0) {
				p_parentSurface->m_unk0x52c->m_next = node;
			}
			p_parentSurface->m_unk0x52c = node;
			if (p_parentSurface->m_unk0x528 == 0) {
				p_parentSurface->m_unk0x528 = node;
			}
			p_parentSurface->m_unk0x530 = p_parentSurface->m_unk0x530 + 1;
		}
	}
	if (g_pSurfaceList != 0) {
		storage = operator new(0xc);
		if (storage != 0) {
			node = (SurfaceListNode*) storage;
			node->m_surface = this;
			node->m_next = 0;
			node->m_prev = (SurfaceListNode*) g_pSurfaceList->m_last;
			if (g_pSurfaceList->m_last != 0) {
				((SurfaceListNode*) g_pSurfaceList->m_last)->m_next = node;
			}
			g_pSurfaceList->m_last = node;
			if (g_pSurfaceList->m_first == 0) {
				g_pSurfaceList->m_first = node;
			}
			g_pSurfaceList->m_count = g_pSurfaceList->m_count + 1;
		}
	}
	if (p_parentSurface == (Surface*) g_pGdiHelperTarget && g_pTargetGraphicsDriver != 0) {
		context = g_pTargetGraphicsDriver->CreateDrawingContext();
		m_drawingPort = context;
	}
	NewBitmap(p_rect);
}

static const unsigned char g_anFallbackSystemColors[20][3] = {
	{0x00, 0x00, 0x00}, {0x80, 0x00, 0x00}, {0x00, 0x80, 0x00}, {0x80, 0x80, 0x00}, {0x00, 0x00, 0x80},
	{0x80, 0x00, 0x80}, {0x00, 0x80, 0x80}, {0xc0, 0xc0, 0xc0}, {0xc0, 0xdc, 0xc0}, {0xa6, 0xca, 0xf0},
	{0xff, 0xfb, 0xf0}, {0xa0, 0xa0, 0xa4}, {0x80, 0x80, 0x80}, {0xff, 0x00, 0x00}, {0x00, 0xff, 0x00},
	{0xff, 0xff, 0x00}, {0x00, 0x00, 0xff}, {0xff, 0x00, 0xff}, {0x00, 0xff, 0xff}, {0xff, 0xff, 0xff}};
static const unsigned char g_anReservedOutputColors[2][3] = {{0xff, 0xff, 0xff}, {0x00, 0x00, 0x00}};

// FUNCTION: LEMBALL 0x0046c380
void TargetBuildSurfaceColourTable(unsigned int* p_entries,
								   ResPalette* p_palette,
								   void* p_unused,
								   unsigned int* p_fallbackEntries)
{
	PALETTEENTRY systemEntries[256];
	unsigned char* data;
	HDC hdc;
	int i;
	int colorCount;
	unsigned int color;

	hdc = GetDC(0);
	if ((GetSystemPaletteEntries(hdc, 0, 10, systemEntries) |
		 GetSystemPaletteEntries(hdc, 0xf6, 10, systemEntries + 0xf6)) == 0) {
		i = 0;
		while (i < 10) {
			systemEntries[i].peRed = g_anFallbackSystemColors[i][0];
			systemEntries[i].peGreen = g_anFallbackSystemColors[i][1];
			systemEntries[i].peBlue = g_anFallbackSystemColors[i][2];
			systemEntries[0xf6 + i].peRed = g_anFallbackSystemColors[10 + i][0];
			systemEntries[0xf6 + i].peGreen = g_anFallbackSystemColors[10 + i][1];
			systemEntries[0xf6 + i].peBlue = g_anFallbackSystemColors[10 + i][2];
			i = i + 1;
		}
	}
	if (hdc != 0) {
		ReleaseDC(0, hdc);
	}

	i = 0;
	while (i < 10) {
		p_entries[i] = ((unsigned int) systemEntries[i].peRed << 16) |
					   ((unsigned int) systemEntries[i].peGreen << 8) | systemEntries[i].peBlue;
		p_entries[0xf6 + i] = ((unsigned int) systemEntries[0xf6 + i].peRed << 16) |
							  ((unsigned int) systemEntries[0xf6 + i].peGreen << 8) | systemEntries[0xf6 + i].peBlue;
		i = i + 1;
	}
	p_entries[10] = ((unsigned int) g_anReservedOutputColors[0][0] << 16) |
					((unsigned int) g_anReservedOutputColors[0][1] << 8) | g_anReservedOutputColors[0][2];
	p_entries[11] = ((unsigned int) g_anReservedOutputColors[1][0] << 16) |
					((unsigned int) g_anReservedOutputColors[1][1] << 8) | g_anReservedOutputColors[1][2];

	if (p_palette == 0) {
		if (p_fallbackEntries == 0) {
			i = 12;
			while (i < 0xf6) {
				color = (unsigned char) (0u - (unsigned int) i);
				p_entries[i] = (color << 16) | (color << 8) | color;
				i = i + 1;
			}
		}
		else {
			i = 12;
			while (i < 0xf6) {
				p_entries[i] = p_fallbackEntries[i] & 0xffffff;
				i = i + 1;
			}
		}
	}
	else {
		colorCount = (int) p_palette->m_paletteState - 10;
		if (colorCount > 0xf6) {
			colorCount = 0xec;
		}
		if (colorCount > 12) {
			colorCount = colorCount - 12;
			data = p_palette->GetData();
			if (data != 0) {
				data = data + 12 * 4;
				i = 12;
				while (colorCount != 0) {
					p_entries[i] = ((unsigned int) data[0] << 16) | ((unsigned int) data[1] << 8) | data[2];
					data = data + 4;
					i = i + 1;
					colorCount = colorCount - 1;
				}
			}
		}
	}
}

// 68K 0x10109048 __ct__8CSurfaceFP8GrafPort
// FUNCTION: LEMBALL 0x0046c5d0
Surface::Surface(GrafPort* p_arg0)
{
	m_presentY = 0;
	m_unk0x528 = 0;
	m_presentX = 0;
	m_unk0x52c = 0;
	m_unk0x530 = 0;
	m_drawingPort = new TargetDrawingContext(p_arg0);
	m_platformBitmap = 0;
	m_changeList = 0;
	m_parentSurface = 0;
	InitializeCriticalSection((CRITICAL_SECTION*) m_lock);
	m_unk0x54c = 1;
	m_flag70 = 0;
}

// 68K 0x10109200 __dt__8CSurfaceFv
// FUNCTION: LEMBALL 0x0046c710
Surface::~Surface()
{
	SurfaceListNode* node;
	SurfaceListNode* next;
	SurfaceListNode* prev;
	Surface* parent;
	int locked;

	locked = m_unk0x54c;
	if (locked != 0) {
		EnterCriticalSection((CRITICAL_SECTION*) m_lock);
	}
	if (m_platformBitmap != 0 && g_pTargetGraphicsDriver != 0) {
		g_pTargetGraphicsDriver->RestoreDIBContext((TargetDrawingContext*) m_drawingPort,
												   (TargetDibContext*) m_platformBitmap);
		g_pTargetGraphicsDriver->DestroyDIBContext((TargetDibContext*) m_platformBitmap);
		m_platformBitmap = 0;
	}
	if (m_parentSurface == (Surface*) g_pGdiHelperTarget) {
		FreeBackBuff();
		FreeZBuff();
	}
	if (m_drawingPort != 0 && g_pTargetGraphicsDriver != 0) {
		g_pTargetGraphicsDriver->DestroyDrawingContext((TargetDrawingContext*) m_drawingPort);
		m_drawingPort = 0;
	}
	if (m_changeList != 0) {
		delete m_changeList;
		m_changeList = 0;
	}
	parent = m_parentSurface;
	if (parent != 0) {
		node = parent->m_unk0x528;
		while (node != 0) {
			if (node->m_surface == this) {
				break;
			}
			node = node->m_next;
		}
		if (node != 0) {
			next = node->m_next;
			prev = node->m_prev;
			operator delete(node);
			if (next == 0) {
				parent->m_unk0x52c = prev;
			}
			else {
				next->m_prev = prev;
			}
			if (prev == 0) {
				parent->m_unk0x528 = next;
			}
			else {
				prev->m_next = next;
			}
			parent->m_unk0x530 = parent->m_unk0x530 - 1;
		}
		m_parentSurface = 0;
	}
	if (locked != 0) {
		LeaveCriticalSection((CRITICAL_SECTION*) m_lock);
		DeleteCriticalSection((CRITICAL_SECTION*) m_lock);
		m_unk0x54c = 0;
		if (g_pSurfaceList != 0) {
			node = (SurfaceListNode*) g_pSurfaceList->m_first;
			while (node != 0) {
				if (node->m_surface == this) {
					break;
				}
				node = node->m_next;
			}
			if (node != 0) {
				next = node->m_next;
				prev = node->m_prev;
				operator delete(node);
				if (next == 0) {
					g_pSurfaceList->m_last = prev;
				}
				else {
					next->m_prev = prev;
				}
				if (prev == 0) {
					g_pSurfaceList->m_first = next;
				}
				else {
					prev->m_next = next;
				}
				g_pSurfaceList->m_count = g_pSurfaceList->m_count - 1;
			}
			if (g_pSurfaceList != 0 && g_pSurfaceList->m_count == 0) {
				node = (SurfaceListNode*) g_pSurfaceList->m_first;
				while (node != 0) {
					next = node->m_next;
					operator delete(node);
					node = next;
				}
				operator delete(g_pSurfaceList);
				g_pSurfaceList = 0;
			}
		}
	}
	node = (SurfaceListNode*) m_unk0x528;
	while (node != 0) {
		next = node->m_next;
		operator delete(node);
		node = next;
	}
	m_unk0x528 = 0;
	PvGdiBitmap::Free();
}

// 68K 0x10108bf2 ResetScroll__8CSurfaceFv
// FUNCTION: LEMBALL 0x0046c990
void Surface::ResetScroll()
{
	SurfaceListNode* node;

	PvGdiBitmap::ResetScroll();
	if (HasBackBuff() != 0) {
		PvBackBuffSurface::m_bitmap.ResetScroll();
	}
	if (HasZBuff() != 0) {
		PvZBuffSurface::m_bitmap.ResetScroll();
	}
	for (node = m_unk0x528; node != 0; node = node->m_next) {
		node->m_surface->PvGdiBitmap::ResetLinePtrs();
	}
}

// 68K 0x10108c8a SetLinePtrs__8CSurfaceFv
// FUNCTION: LEMBALL 0x0046c9f0
void Surface::SetLinePtrs()
{
	Surface* parent;
	int y;
	int originX;
	int originY;
	int parentStride;
	unsigned char* bits;

	parent = (Surface*) PvScrollableSurface::m_parentSurface;
	if (parent == (Surface*) g_pGdiHelperTarget) {
		PvGdiBitmap::SetLinePtrs();
		return;
	}
	parentStride = parent->m_stride;
	m_stride = parentStride;
	originX = (int) PvScrollableSurface::m_windowRect.m_x;
	originY = (int) PvScrollableSurface::m_windowRect.m_y;
	bits = (unsigned char*) parent->m_lines[originY] + originX;
	m_bitsBase = bits;
	m_bits = bits;
	m_xOffset = 0;
	m_firstLine = 0;
	if (parent->PvZBuffSurface::m_bitmap.m_lines != 0) {
		PvZBuffSurface::m_bitmap.m_lines = parent->PvZBuffSurface::m_bitmap.m_lines;
		PvZBuffSurface::m_bitmap.m_bits = (unsigned char*) (originY * parentStride + (int) parent->PvZBuffSurface::m_bitmap.m_bits + originX);
	}
	else {
		PvZBuffSurface::m_bitmap.m_lines = 0;
	}
	if (parent->PvBackBuffSurface::m_bitmap.m_lines != 0) {
		PvBackBuffSurface::m_bitmap.m_lines = parent->PvBackBuffSurface::m_bitmap.m_lines;
		PvBackBuffSurface::m_bitmap.m_bits = (unsigned char*) ((originY * parentStride + originX) * 2 + (int) parent->PvBackBuffSurface::m_bitmap.m_bits);
	}
	else {
		PvBackBuffSurface::m_bitmap.m_lines = 0;
	}
	y = 0;
	if ((short) m_height <= 0) {
		return;
	}
	do {
		m_lines[y] = (void*) ((int) parent->m_lines[originY + y] + originX);
		y = y + 1;
	} while (y < (int) (short) m_height);
}

// 68K 0x10109466 AddToChangeList__8CSurfaceFRC7CVSRect
// FUNCTION: LEMBALL 0x0046cb20
void Surface::AddToChangeList(const VsRect* p_rect)
{
	Surface* parent;
	const short* origin;
	const short* xy;
	VsRect translated;

	parent = (Surface*) PvScrollableSurface::m_parentSurface;
	if (parent != (Surface*) g_pGdiHelperTarget && PvScrollableSurface::m_flag74 != 0 && PvScrollableSurface::m_flag70 != 0) {
		translated.m_width = p_rect->m_width;
		translated.m_height = p_rect->m_height;
		origin = &PvScrollableSurface::m_rect0c.m_x;
		if (p_rect != 0) {
			xy = &p_rect->m_x;
		}
		else {
			xy = 0;
		}
		translated.m_x = xy[0];
		translated.m_y = xy[1];
		translated.m_x += origin[0];
		translated.m_y += origin[1];
		((Surface*) PvScrollableSurface::m_parentSurface)->AddToChangeList(&translated);
		return;
	}
	m_changeList->Add(*p_rect);
}

// 68K 0x1010956c GetChangeList__8CSurfaceFv
// FUNCTION: LEMBALL 0x0046cbd0
ChangeList* Surface::GetChangeList()
{
	return m_changeList;
}

// 68K 0x1010959a Blit__8CSurfaceFP9CClipRect
// FUNCTION: LEMBALL 0x0046cbe0
void Surface::Blit(class ClipRect* p_arg0)
{
	Surface* parent;
	short clipRight;
	short clipBottom;

	if ((p_arg0->m_reserved0c & 0x1000) == 0) {
		m_clipRect.m_width = p_arg0->m_left;
		m_clipRect.m_height = p_arg0->m_top;
		m_clipRect.m_x = p_arg0->m_right;
		m_clipRect.m_y = p_arg0->m_bottom;
	}
	else if ((int) p_arg0->m_left * (int) p_arg0->m_top != 0) {
		if (p_arg0->m_right < m_clipRect.m_x) {
			m_clipRect.m_width = (short) (m_clipRect.m_width + (m_clipRect.m_x - p_arg0->m_right));
			m_clipRect.m_x = p_arg0->m_right;
		}
		if ((short) (m_clipRect.m_width + m_clipRect.m_x) < (short) (p_arg0->m_right + p_arg0->m_left)) {
			m_clipRect.m_width = (short) ((p_arg0->m_left - m_clipRect.m_x) + p_arg0->m_right);
		}
		if (p_arg0->m_bottom < m_clipRect.m_y) {
			m_clipRect.m_height = (short) (m_clipRect.m_height + (m_clipRect.m_y - p_arg0->m_bottom));
			m_clipRect.m_y = p_arg0->m_bottom;
		}
		if ((short) (m_clipRect.m_height + m_clipRect.m_y) < (short) (p_arg0->m_bottom + p_arg0->m_top)) {
			m_clipRect.m_height = (short) ((p_arg0->m_top - m_clipRect.m_y) + p_arg0->m_bottom);
		}
	}
	parent = m_parentSurface;
	if (parent != (Surface*) g_pGdiHelperTarget && (p_arg0->m_reserved0c & 0x10000) == 0 && parent != 0) {
		clipRight = m_clipRect.m_x;
		if (clipRight < parent->m_clipRect.m_x) {
			m_clipRect.m_width = (short) (m_clipRect.m_width + (clipRight - parent->m_clipRect.m_x));
			m_clipRect.m_x = parent->m_clipRect.m_x;
		}
		clipRight = m_clipRect.m_x;
		if ((short) (parent->m_clipRect.m_width + parent->m_clipRect.m_x) <
			(short) (m_clipRect.m_width + clipRight)) {
			m_clipRect.m_width = (short) ((parent->m_clipRect.m_x - clipRight) + parent->m_clipRect.m_width);
		}
		clipBottom = m_clipRect.m_y;
		if (clipBottom < parent->m_clipRect.m_y) {
			m_clipRect.m_height = (short) (m_clipRect.m_height + (clipBottom - parent->m_clipRect.m_y));
			m_clipRect.m_y = parent->m_clipRect.m_y;
		}
		clipBottom = m_clipRect.m_y;
		if ((short) (parent->m_clipRect.m_y + parent->m_clipRect.m_height) <
			(short) (m_clipRect.m_height + clipBottom)) {
			m_clipRect.m_height = (short) ((parent->m_clipRect.m_height - clipBottom) + parent->m_clipRect.m_y);
		}
		if (m_clipRect.m_width < 1 || m_clipRect.m_height < 1) {
			m_clipRect.m_height = 0;
			m_clipRect.m_width = 0;
			m_clipRect.m_y = 0;
			m_clipRect.m_x = 0;
		}
	}
}

// 68K 0x10109e1e ToScreen__8CSurfaceFP8CSurface
// FUNCTION: LEMBALL 0x0046cda0
void Surface::ToScreen(class Surface* p_destinationSurface)
{
	Surface* self;
	Surface* parent;
	ChangeList* list;
	int itemCount;
	int index;
	ChangeListItem* item;
	TargetDrawingContext* destContext;
	short zoom;

	self = this;
	parent = (Surface*) PvScrollableSurface::m_parentSurface;
	if (parent != (Surface*) g_pGdiHelperTarget) {
		if (PvScrollableSurface::m_flag74 != 0) {
			return;
		}
		if (PvScrollableSurface::m_flag78 != 0) {
			parent->AddToChangeList(&m_windowRect);
			PvScrollableSurface::m_flag78 = 0;
			return;
		}
		if (PvScrollableSurface::m_flag70 == 0) {
			return;
		}
		list = GetChangeList();
		if (list == 0) {
			return;
		}
		itemCount = list->GetNumItems() - list->GetDrawMark();
		if (itemCount <= 0) {
			return;
		}
		parent->AddToChangeList(&m_windowRect);
		return;
	}

	if (m_platformBitmap == 0) {
		return;
	}
	{
		VsRect destRect;
		VsRect sourceRect;

		EnterCriticalSection((CRITICAL_SECTION*) m_lock);
		if (p_destinationSurface != 0) {
			EnterCriticalSection((CRITICAL_SECTION*) p_destinationSurface->m_lock);
		}
		destContext = 0;
		if (p_destinationSurface != 0) {
			destContext = (TargetDrawingContext*) p_destinationSurface->m_drawingPort;
		}
		if (destContext != 0 && g_pTargetGraphicsDriver != 0) {
			g_pTargetGraphicsDriver->RealizePalette(destContext);
		}
		if ((int) m_dontUpdateRect.m_width * (int) m_dontUpdateRect.m_height > 0) {
			list = GetChangeList();
			if (list != 0) {
				list->AddWithActiveMark(m_dontUpdateRect, 0);
			}
		}
		list = GetChangeList();
		index = 0;
		if (list != 0) {
			itemCount = list->GetNumItems();
			zoom = 1;
			while (index < itemCount) {
				item = list->GetNItem(index);
				sourceRect.m_width = item->width;
				sourceRect.m_height = item->height;
				sourceRect.m_x = item->x;
				sourceRect.m_y = item->y;
				destRect = sourceRect;
				destRect.m_x = (short) (destRect.m_x + m_presentX);
				destRect.m_y = (short) (destRect.m_y + m_presentY);
				destRect.m_x = (short) (destRect.m_x + m_relOriginX);
				destRect.m_y = (short) (destRect.m_y + m_relOriginY);
				if (destContext != 0 && m_drawingPort != 0 && g_pTargetGraphicsDriver != 0) {
					if (zoom == 1) {
					g_pTargetGraphicsDriver->BlitWrappedBitmap(destContext,
															   &destRect,
															   (TargetDrawingContext*) self->m_drawingPort,
															   &sourceRect,
															   self);
					}
					else {
						destRect.m_width = (short) (destRect.m_width * zoom);
						destRect.m_height = (short) (destRect.m_height * zoom);
						destRect.m_x = (short) (destRect.m_x * zoom);
						destRect.m_y = (short) (destRect.m_y * zoom);
					g_pTargetGraphicsDriver->BlitWrappedBitmap(destContext,
															   &destRect,
															   (TargetDrawingContext*) self->m_drawingPort,
															   &sourceRect,
															   self);
					}
				}
				index = index + 1;
				itemCount = list->GetNumItems();
			}
		}
		GdiFlush();
		if (PvBackBuffSurface::HasBackBuff() != 0 && list != 0) {
			index = 0;
			itemCount = list->GetNumItems();
			while (index < itemCount) {
				item = list->GetNItem(index);
				sourceRect.m_width = item->width;
				sourceRect.m_height = item->height;
				sourceRect.m_x = item->x;
				sourceRect.m_y = item->y;
				CopyBackBuffToScreen(sourceRect);
				index = index + 1;
				itemCount = list->GetNumItems();
			}
		}
		LeaveCriticalSection((CRITICAL_SECTION*) m_lock);
		if (p_destinationSurface != 0) {
			LeaveCriticalSection((CRITICAL_SECTION*) p_destinationSurface->m_lock);
		}
	}
}

// 68K 0x10108fa4 AttachPalette__8CSurfaceFP11CResPALETTE
// FUNCTION: LEMBALL 0x0046d040
void Surface::AttachPalette(ResPalette* p_palette)
{
	TargetBuildSurfaceColourTable(g_dwWinGDrawColourTable, p_palette, 0, 0);
	SetDefaultCtable();
}

// 68K 0x10108642 NewBitmap__8CSurfaceFRC7CVSRect
// ASSERT: _VSRELassert("AllocatedBitmap", "VSGDI.CPP", 736)
// FUNCTION: LEMBALL 0x0046d090
void Surface::NewBitmap(const VsRect& p_rect)
{
	EnterCriticalSection((CRITICAL_SECTION*) m_lock);
	m_clipRect = p_rect;
	m_windowRect = p_rect;
	if (m_parentSurface != (Surface*) g_pGdiHelperTarget) {
		if (m_parentSurface != 0) {
			short parentWidth = m_parentSurface->m_windowRect.m_width;
			short parentHeight = m_parentSurface->m_windowRect.m_height;
			if (m_presentX < 0) {
				m_windowRect.m_width += m_presentX;
				m_presentX = 0;
			}
			if (m_presentX + m_windowRect.m_width > parentWidth) {
				m_windowRect.m_width = parentWidth - m_presentX;
			}
			if (m_presentY < 0) {
				m_windowRect.m_height += m_presentY;
				m_presentY = 0;
			}
			if (m_presentY + m_windowRect.m_height > parentHeight) {
				m_windowRect.m_height = parentHeight - m_presentY;
			}
			if (m_windowRect.m_width <= 0 || m_windowRect.m_height <= 0) {
				m_windowRect.m_height = 0;
				m_windowRect.m_width = 0;
				m_presentY = 0;
				m_presentX = 0;
			}
		}
		VsSize size;
		size.m_width = m_windowRect.m_width;
		size.m_height = m_windowRect.m_height;
		SetSize(size, (int) m_windowRect.m_width);
		m_unk0x524 = 0;
		CreateLinePtrs();
		LeaveCriticalSection((CRITICAL_SECTION*) m_lock);
		return;
	}
	m_windowRect.m_width = (m_windowRect.m_width + 3) & ~3;
	VsSize size;
	size.m_width = m_windowRect.m_width;
	size.m_height = m_windowRect.m_height;
	SetSize(size, m_width);
	TargetDrawingContext* context = (TargetDrawingContext*) m_drawingPort;
	if (m_platformBitmap != 0 && g_pTargetGraphicsDriver != 0 && context != 0) {
		g_pTargetGraphicsDriver->RestoreDIBContext(context, (TargetDibContext*) m_platformBitmap);
		g_pTargetGraphicsDriver->DestroyDIBContext((TargetDibContext*) m_platformBitmap);
		m_platformBitmap = 0;
	}
	if (m_windowRect.m_width == 0 || m_windowRect.m_height == 0) {
		m_unk0x524 = 0;
		LeaveCriticalSection((CRITICAL_SECTION*) m_lock);
		return;
	}
	if (m_platformBitmap == 0 && g_pTargetGraphicsDriver != 0 && context != 0) {
		BITMAPINFO* info = (BITMAPINFO*) m_bitmapInfo;
		g_pTargetGraphicsDriver->InitializeBitmapInfo(info);
		info->bmiHeader.biWidth = size.m_width;
		info->bmiHeader.biHeight = (int) size.m_height * (int) info->bmiHeader.biHeight;
		info->bmiHeader.biPlanes = 1;
		info->bmiHeader.biBitCount = 8;
		info->bmiHeader.biSize = 0x28;
		m_platformBitmap = g_pTargetGraphicsDriver->CreateDIBContext(context, info);
		if (m_platformBitmap != 0) {
			g_pTargetGraphicsDriver->SelectDIBContext(context, (TargetDibContext*) m_platformBitmap);
			m_unk0x524 = (int) m_windowRect.m_width * (int) m_windowRect.m_height;
		}
	}
	if (m_platformBitmap == 0) {
		VsRelAssert("AllocatedBitmap", "VSGDI.CPP", 736);
	}
	TargetDibContext* dib = (TargetDibContext*) m_platformBitmap;
	SetBitsBase(dib->GetBits(), dib->GetStride());
	if (m_changeList != 0) {
		m_changeList->Reset();
	}
	VsRect clip;
	clip.m_x = 0;
	clip.m_y = 0;
	clip.m_width = m_clipRect.m_width;
	clip.m_height = m_clipRect.m_height;
	AddToChangeList(&clip);
	LeaveCriticalSection((CRITICAL_SECTION*) m_lock);
}

// 68K 0x10109fc0 Resize__8CSurfaceFRC7CVSSize
// FUNCTION: LEMBALL 0x0046d420
void Surface::Resize(const VsSize& p_size)
{
	VsRect rect;
	rect.m_x = m_clipRect.m_x;
	rect.m_y = m_clipRect.m_y;
	rect.m_width = p_size.m_width;
	rect.m_height = p_size.m_height;
	if (m_changeList != 0) {
		VsRect clipped;
		ClipRect(rect, &clipped);
		m_changeList->Add(clipped);
	}
	NewBitmap(rect);
	if (m_parentSurface == (Surface*) g_pGdiHelperTarget) {
		if (HasBackBuff()) {
			ResizeBackBuff();
		}
		if (HasZBuff()) {
			ResizeZBuff();
		}
	}
	for (SurfaceListNode* node = m_unk0x528; node != 0; node = node->m_next) {
		Surface* child = node->m_surface;
		if (child != 0) {
			VsSize childSize;
			childSize.m_width = child->m_clipRect.m_width;
			childSize.m_height = child->m_clipRect.m_height;
			child->Resize(childSize);
		}
	}
}

// 68K 0x1010a0fe MoveRel__8CSurfaceFRC8CVSPoint
// FUNCTION: LEMBALL 0x0046d560
void Surface::MoveRel(const VsPoint& p_delta)
{
	m_windowRect.m_x += p_delta.m_x;
	m_windowRect.m_y += p_delta.m_y;
	Move(*(VsPoint*) &m_windowRect.m_x);
}

// 68K 0x1010a166 Move__8CSurfaceFRC8CVSPoint
// FUNCTION: LEMBALL 0x0046d5b0
void Surface::Move(const VsPoint& p_position)
{
	VsPoint delta;
	delta.m_x = p_position.m_x - m_rect0c.m_x;
	delta.m_y = p_position.m_y - m_rect0c.m_y;

	if (m_parentSurface != (Surface*) g_pGdiHelperTarget) {
		EnterCriticalSection((CRITICAL_SECTION*) m_lock);
		m_rect0c.m_x = p_position.m_x;
		m_rect0c.m_y = p_position.m_y;
		short oldWidth = m_windowRect.m_width;
		short oldHeight = m_windowRect.m_height;
		m_windowRect.m_width = m_dontUpdateRect.m_width;
		m_windowRect.m_height = m_dontUpdateRect.m_height;
		m_windowRect.m_x = m_rect0c.m_x;
		m_windowRect.m_y = m_rect0c.m_y;

		short parentWidth = m_parentSurface->m_clipRect.m_width;
		short parentHeight = m_parentSurface->m_clipRect.m_height;

		if (m_windowRect.m_x < 0) {
			m_windowRect.m_width += m_windowRect.m_x;
			m_windowRect.m_x = 0;
		}
		if (parentWidth < (short) (m_windowRect.m_x + m_windowRect.m_width)) {
			m_windowRect.m_width = parentWidth - m_windowRect.m_x;
		}
		if (m_windowRect.m_y < 0) {
			m_windowRect.m_height += m_windowRect.m_y;
			m_windowRect.m_y = 0;
		}
		if (parentHeight < (short) (m_windowRect.m_y + m_windowRect.m_height)) {
			m_windowRect.m_height = parentHeight - m_windowRect.m_y;
		}
		if (m_windowRect.m_width < 1 || m_windowRect.m_height < 1) {
			m_windowRect.m_x = 0;
			m_windowRect.m_width = 0;
			m_windowRect.m_y = 0;
			m_windowRect.m_height = 0;
		}
		m_relOriginX = m_windowRect.m_width;
		m_relOriginY = m_windowRect.m_height;
		if (m_windowRect.m_width != oldWidth || m_windowRect.m_height != oldHeight) {
			Resize((VsSize&) m_windowRect);
		}
		if (m_platformBitmap != 0) {
			g_pTargetGraphicsDriver->DestroyDIBContext((TargetDibContext*) m_platformBitmap);
			m_platformBitmap = 0;
		}
		m_changeList = 0;
		CreateLinePtrs();
		LeaveCriticalSection((CRITICAL_SECTION*) m_lock);
		for (SurfaceListNode* node = m_unk0x528; node != 0; node = node->m_next) {
			node->m_surface->MoveRel(delta);
		}
	}
}

// 68K 0x1010858c SetWindowPtr__8CSurfaceFP8GrafPort
// FUNCTION: LEMBALL 0x0046d7e0
void Surface::SetWindowPtr(void* p_platformPort)
{
	((TargetDrawingContext*) m_drawingPort)->SetDC(p_platformPort);
}

// FUNCTION: LEMBALL 0x0046d800
void Surface::CopyDIBBits(void* p_header, unsigned char* p_bits)
{
	if (m_changeList == 0) {
		return;
	}
	EnterCriticalSection((CRITICAL_SECTION*) m_lock);
	BITMAPINFOHEADER* header = (BITMAPINFOHEADER*) p_header;
	int copyWidth = header->biWidth;
	if (m_width < copyWidth) {
		copyWidth = m_width;
	}
	int copyHeight = header->biHeight;
	if (m_height < copyHeight) {
		copyHeight = m_height;
	}
	int stride = (int) ((header->biBitCount * header->biWidth + 31) & ~31) / 8;
	unsigned char* source = p_bits + (header->biHeight - 1) * stride;
	for (int y = 0; y < copyHeight; y++) {
		memcpy(m_lines[y], source, copyWidth);
		if (m_height < header->biHeight) {
			copyHeight = m_height;
		}
		source -= stride;
	}
	VsRect rect;
	rect.m_width = m_width;
	rect.m_height = m_height;
	rect.m_x = 0;
	rect.m_y = 0;
	m_changeList->Reset();
	AddToChangeList(&rect);
	LeaveCriticalSection((CRITICAL_SECTION*) m_lock);
}

// 68K 0x10108ec2 SetDefaultCtable__8CSurfaceFv
// FUNCTION: LEMBALL 0x0046d930
void Surface::SetDefaultCtable()
{
	unsigned char logPalette[8 + 256 * 4];
	LOGPALETTE* palette;
	PALETTEENTRY* entries;
	unsigned int* source;
	int i;
	SurfaceListNode* node;
	Surface* surface;

	palette = (LOGPALETTE*) logPalette;
	palette->palVersion = 0x300;
	palette->palNumEntries = 0x100;
	entries = palette->palPalEntry;
	source = g_dwWinGDrawColourTable;
	i = 0;
	while (i < 0x100) {
		entries[i].peRed = (unsigned char) (source[i] >> 16);
		entries[i].peGreen = (unsigned char) (source[i] >> 8);
		entries[i].peBlue = (unsigned char) source[i];
		entries[i].peFlags = 4;
		i++;
	}
	if (g_pTargetGraphicsDriver != 0) {
		g_pTargetGraphicsDriver->CreatePalette(palette);
	}
	if (g_pSurfaceList == 0) {
		return;
	}
	node = (SurfaceListNode*) g_pSurfaceList->m_first;
	while (node != 0) {
		surface = node->m_surface;
		if (surface != 0 && (unsigned char*) surface->m_colourTable != 0) {
			memcpy(surface->m_colourTable, g_dwWinGDrawColourTable, 0x400);
		}
		if (surface != 0 && surface->m_drawingPort != 0 && g_pTargetGraphicsDriver != 0) {
			g_pTargetGraphicsDriver->UpdateDIBColourTable((TargetDrawingContext*) surface->m_drawingPort,
														  0,
														  0x100,
														  g_dwWinGDrawColourTable);
		}
		node = node->m_next;
	}
}

// FUNCTION: LEMBALL 0x0046d9f0
bool Surface::BeginRender()
{
	if (m_lines == 0) {
		return 0;
	}
	if (m_parentSurface == (Surface*) g_pGdiHelperTarget) {
		TargetDibContext* dib = (TargetDibContext*) m_platformBitmap;
		if (dib == 0) {
			return 0;
		}
		if (!dib->Lock()) {
			return 0;
		}
		unsigned char* bits = dib->GetBits();
		if (bits != 0 && (unsigned char*) m_lines[0] != bits) {
			m_lines[0] = bits;
			SetLinePtrs();
			return 1;
		}
		return 1;
	}
	if (m_parentSurface == 0) {
		return 0;
	}
	if (!m_parentSurface->BeginRender()) {
		return 0;
	}
	if (m_parentSurface->m_lines == 0) {
		return 0;
	}
	unsigned char* expected = (unsigned char*) m_parentSurface->m_lines[m_windowRect.m_y] + m_windowRect.m_x;
	if (expected != (unsigned char*) m_lines[0]) {
		SetLinePtrs();
		return 1;
	}
	return 1;
}

// FUNCTION: LEMBALL 0x0046daa0
void Surface::EndRender()
{
	Surface* current = this;
	while (current != 0 && current->m_parentSurface != (Surface*) g_pGdiHelperTarget && current->m_parentSurface != 0) {
		current = current->m_parentSurface;
	}
	if (current != 0 && current->m_platformBitmap != 0) {
		TargetDibContext* dib = (TargetDibContext*) current->m_platformBitmap;
		dib->Unlock();
	}
}

// 68K 0x1010171a Blit__8CSurfaceFP10CBigBitmapP10CResBITMAP
// FUNCTION: LEMBALL 0x0046dbc0
void Surface::Blit(BigBitmap* p_arg0, ResBitmap* p_arg1)
{
	((PvSurface*) this)->Blit((Bitmap*) p_arg0, p_arg1);
}

// 68K 0x10105df8 Flush__8CSurfaceFv
// FUNCTION: LEMBALL 0x0046dc50
void Surface::Flush()
{
	g_pGdiDevice->Flush(this);
}

// 68K 0x10105dce GetCurrDB__8CSurfaceFv
// FUNCTION: LEMBALL 0x0046dc80
void* Surface::GetCurrDb()
{
	return &m_currDb;
}

// 68K 0x10111b02 Blit__8CSurfaceFP13CScreenScroll
// FUNCTION: LEMBALL 0x00474c20
void Surface::Blit(ScreenScroll* p_scroll)
{
	VsRect rect = p_scroll->m_rect;
	VsPoint dst = p_scroll->m_destination;

	if (HasBackBuff()) {
		PvBackBuffSurface::m_bitmap.Scroll(&rect, &dst);
	}
	if (HasZBuff()) {
		VsRect zrect(rect.m_x * 2, rect.m_y, rect.m_width * 2, rect.m_height);
		VsPoint zdst;
		zdst.m_x = dst.m_x * 2;
		zdst.m_y = dst.m_y;
		PvZBuffSurface::m_bitmap.Scroll(&zrect, &zdst);
	}
	PvGdiBitmap::Scroll(&rect, &dst);
	AddToChangeList(&rect);
}

// 68K 0x10111c76 Blit__8CSurfaceFP11CZBuffClear
// FUNCTION: LEMBALL 0x00474d40
void Surface::Blit(ZBuffClear* p_arg0)
{
	int width = p_arg0->m_width;
	int height = p_arg0->m_height;

	if (width == 0 || height == 0) {
		return;
	}
	int startX = p_arg0->m_x;
	int startY = p_arg0->m_y;
	unsigned short depth = (unsigned short) p_arg0->m_depth;
	if (height <= 0) {
		return;
	}
	do {
		unsigned short* dest = (unsigned short*) ((unsigned char*) PvZBuffSurface::m_bitmap.m_lines[startY] + startX * 2);
		for (int i = 0; i < width; i++) {
			dest[i] = depth;
		}
		startY++;
		height--;
	} while (height != 0);
}

// 68K 0x10111d0c Blit__8CSurfaceFP12CZBuffScroll
// FUNCTION: LEMBALL 0x00474dc0
void Surface::Blit(ZBuffScroll* p_arg0)
{
}

// 68K 0x10105b48 Blit__8CSurfaceFP15CCopyToBackBuff
// FUNCTION: LEMBALL 0x00474dd0
void Surface::Blit(CopyToBackBuff* p_arg0)
{
	CopyToBackBuff* primitive = p_arg0;
	int width = primitive->m_field08;
	int height = primitive->m_field0a;
	if (width != 0 && height != 0) {
		int dstX = primitive->m_field0c;
		int srcX = primitive->m_field04;
		int dstY = primitive->m_field0e;
		int srcY = primitive->m_field06;
		if (height > 0) {
			int srcRow = srcY * 4;
			int count = height;
			int dstRow = dstY * 4;
			do {
				unsigned char* dst = (unsigned char*) *(int*) ((int) PvBackBuffSurface::m_bitmap.m_lines + dstRow) + dstX;
				unsigned char* src = (unsigned char*) *(int*) ((int) m_lines + srcRow) + srcX;
				memcpy(dst, src, width);
				srcRow += 4;
				dstRow += 4;
				count--;
			} while (count != 0);
		}
	}
}

// 68K 0x10105be0 Blit__8CSurfaceFP21CCopyColourToBackBuff
// FUNCTION: LEMBALL 0x00474e60
void Surface::Blit(CopyColourToBackBuff* p_arg0)
{
	int width = p_arg0->m_width;
	int height = p_arg0->m_height;

	if (width == 0 || height == 0) {
		return;
	}
	int startX = p_arg0->m_x;
	int startY = p_arg0->m_y;
	int color = p_arg0->m_colour;
	if (height <= 0) {
		return;
	}
	do {
		unsigned char* dest = (unsigned char*) PvBackBuffSurface::m_bitmap.m_lines[startY] + startX;
		memset(dest, color, width);
		startY++;
		height--;
	} while (height != 0);
}

// 68K 0x10105c6c CopyBackBuffToScreen__8CSurfaceFRC7CVSRect
// FUNCTION: LEMBALL 0x00474ee0
void Surface::CopyBackBuffToScreen(const VsRect& p_arg0)
{
	short height = p_arg0.m_height;
	short width = p_arg0.m_width;

	if ((int) height * (int) width != 0) {
		const short* coords = &p_arg0.m_x;
		short x = coords[0];
		short y = coords[1];
		if ((int) (short) (x + width) > (int) PvBackBuffSurface::m_allocatedWidth) {
			width = (short) (PvBackBuffSurface::m_allocatedWidth - x);
		}
		if ((int) (short) (height + y) > (int) PvBackBuffSurface::m_allocatedHeight) {
			height = (short) (PvBackBuffSurface::m_allocatedHeight - y);
		}
		for (int i = 0; i < height; i++) {
			memcpy((unsigned char*) m_lines[y + i] + x, (unsigned char*) PvBackBuffSurface::m_bitmap.m_lines[y + i] + x, width);
		}
	}
}

// 68K 0x10111d36 Blit__8CSurfaceFP6CPoint
// FUNCTION: LEMBALL 0x00474fd0
void Surface::Blit(Point* p_point)
{
	Point* primitive = p_point;
	if (m_clipRect.m_x <= primitive->m_x && primitive->m_x < (short) (m_clipRect.m_width + m_clipRect.m_x)) {
		if (m_clipRect.m_y <= primitive->m_y && primitive->m_y < (short) (m_clipRect.m_height + m_clipRect.m_y)) {
			*((unsigned char*) m_lines[primitive->m_y] + primitive->m_x) = (unsigned char) primitive->m_color;
			VsRect rect;
			rect.m_x = p_point->m_x;
			rect.m_y = p_point->m_y;
			rect.m_width = 1;
			rect.m_height = 1;
			AddToChangeList(&rect);
		}
	}
}

// 68K 0x10111e5a Blit__8CSurfaceFP10CSolidRect
// FUNCTION: LEMBALL 0x00475080
void Surface::Blit(SolidRect* p_rect)
{
	BlitRect(*p_rect->GetBounds(), p_rect->m_color);
}

// 68K 0x10111fca Blit__8CSurfaceFP5CLine
// FUNCTION: LEMBALL 0x004750c0
void Surface::Blit(Line* p_line)
{
	int x1;
	int y1;
	int x2;
	int y2;
	int dx;
	int dy;
	int absDy;
	int stepY;
	int err;
	int x;
	int y;
	int remaining;
	unsigned char color;
	VsRect bounds;

	if (p_line == 0 || m_lines == 0) {
		return;
	}
	x1 = p_line->m_x1;
	y1 = p_line->m_y1;
	x2 = p_line->m_x2;
	y2 = p_line->m_y2;
	color = (unsigned char) p_line->m_color;
	if (x2 < x1) {
		x = x1;
		x1 = x2;
		x2 = x;
		y = y1;
		y1 = y2;
		y2 = y;
	}
	if (LineClip(x1, y1, x2, y2) != 0) {
		return;
	}
	dx = x2 - x1;
	dy = y2 - y1;
	stepY = 1;
	absDy = dy;
	if (dy < 0) {
		stepY = -1;
		absDy = -dy;
	}
	if (absDy < dx) {
		err = 0;
		if (0 < dx) {
			x = x1;
			y = y1;
			remaining = dx;
			do {
				remaining = remaining - 1;
				x = x + 1;
				err = err + absDy * 2;
				*((unsigned char*) m_lines[y] + (x - 1)) = color;
				if (dx < err) {
					y = y + stepY;
					err = err + dx * -2;
				}
			} while (remaining != 0);
		}
	}
	else {
		remaining = dy;
		if (stepY != 1) {
			remaining = y1 - y2;
		}
		if (0 < remaining) {
			x = x1;
			y = y1;
			err = 0;
			do {
				remaining = remaining - 1;
				err = err + dx * 2;
				*((unsigned char*) m_lines[y] + x) = color;
				y = y + stepY;
				if (absDy < err) {
					x = x + 1;
					err = err + absDy * -2;
				}
			} while (remaining != 0);
		}
	}
	bounds.m_width = (short) (dx + 1);
	if (dy < 0) {
		bounds.m_height = (short) (-dy + 1);
	}
	else {
		bounds.m_height = (short) (dy + 1);
	}
	bounds.m_x = (short) x1;
	if (y2 <= y1) {
		bounds.m_y = (short) y2;
	}
	else {
		bounds.m_y = (short) y1;
	}
	AddToChangeList(&bounds);
}

// 68K 0x10112604 Blit__8CSurfaceFP7CCircle
// FUNCTION: LEMBALL 0x00475290
void Surface::Blit(Circle* p_circle)
{
	int color = p_circle->m_color;
	int centerY = p_circle->m_y;
	int centerX = p_circle->m_x;
	int radius = (int) p_circle->m_radius;
	if (radius < 0) {
		radius = -radius;
	}
	int clipResult = ClipCircle(centerX, centerY, radius);
	if (clipResult != 1) {
		if (clipResult == 2) {
			int curX = 0;
			int err = 0;
			int errLimit = radius * 2 - 1;
			int step = 1;
			*((unsigned char*) m_lines[centerY + radius] + centerX) = (unsigned char) color;
			*((unsigned char*) m_lines[centerY - radius] + centerX) = (unsigned char) color;
			*((unsigned char*) m_lines[centerY] + centerX + radius) = (unsigned char) color;
			*((unsigned char*) m_lines[centerY] + centerX - radius) = (unsigned char) color;
			int curY = radius;
			if (radius > 0) {
				do {
					curX++;
					int stepNext = step + 2;
					err += step;
					if (errLimit < err * 2) {
						curY--;
						err -= errLimit;
						errLimit -= 2;
					}
					step = stepNext;
					if (curX <= curY) {
						DrawCircleSymmetricPoints(centerX, centerY, curX, curY, (unsigned char) color);
						if (curX < curY) {
							DrawCircleSymmetricPoints(centerX, centerY, curY, curX, (unsigned char) color);
						}
					}
				} while (curX < curY);
			}
		}
		else if (clipResult == 3) {
			DrawClippedCircleOutline(centerX, centerY, radius, (unsigned char) color);
		}
		int boundX = centerX - radius;
		int boundY = centerY - radius;
		int boundW = radius * 2 + 1;
		int boundH = boundW;
		if (boundX < (int) m_clipRect.m_x) {
			boundW += boundX - m_clipRect.m_x;
			boundX = m_clipRect.m_x;
		}
		if ((int) m_clipRect.m_x + (int) m_clipRect.m_width - 1 < boundX + boundW) {
			boundW = (m_clipRect.m_x + m_clipRect.m_width) - boundX;
		}
		if (boundY < (int) m_clipRect.m_y) {
			boundH += boundY - m_clipRect.m_y;
			boundY = m_clipRect.m_y;
		}
		if ((int) m_clipRect.m_y + (int) m_clipRect.m_height - 1 < boundY + boundH) {
			boundH = (m_clipRect.m_y + m_clipRect.m_height) - boundY;
		}
		VsRect bounds;
		bounds.m_width = (short) boundW;
		bounds.m_height = (short) boundH;
		bounds.m_x = (short) boundX;
		bounds.m_y = (short) boundY;
		AddToChangeList(&bounds);
	}
}

// 68K 0x101132f4 Blit__8CSurfaceFP13CFilledCircle
// FUNCTION: LEMBALL 0x00475490
void Surface::Blit(FilledCircle* p_circle)
{
	int colour = p_circle->m_color;
	int y = p_circle->m_y;
	int x = p_circle->m_x;
	int radius = (int) p_circle->m_radius;
	if (radius < 0) {
		radius = -radius;
	}
	int clipResult = ClipCircle(x, y, radius);
	if (clipResult != 1) {
		if (clipResult == 2) {
			int curX = 0;
			int err = 0;
			int errLimit = radius * 2 - 1;
			unsigned char colByte = (unsigned char) colour;
			int step = 1;
			*((unsigned char*) m_lines[y + radius] + x) = colByte;
			*((unsigned char*) m_lines[y - radius] + x) = colByte;
			memset((unsigned char*) m_lines[y] + x - radius, colByte, radius * 2 + 1);
			if (radius > 1) {
				int curRadius = radius;
				do {
					int oldErrLimit = errLimit;
					curX++;
					int stepNext = step + 2;
					err += step;
					int doubleErr = err * 2;
					if (errLimit < doubleErr) {
						curRadius--;
						err -= errLimit;
						errLimit -= 2;
					}
					step = stepNext;
					if (curX <= curRadius) {
						if (oldErrLimit < doubleErr) {
							DrawFilledCircleSymmetricSpans(x, y, curX, curRadius, colByte);
						}
						if (curX < curRadius) {
							DrawFilledCircleSymmetricSpans(x, y, curRadius, curX, colByte);
						}
					}
				} while (curX < curRadius);
			}
		}
		else if (clipResult == 3) {
			DrawClippedFilledCircle(x, y, radius, colour);
		}
		int minX = x - radius;
		int minY = y - radius;
		int diameter = radius * 2 + 1;
		int width = diameter;
		if (minX < (int) m_windowRect.m_x) {
			width += minX - m_windowRect.m_x;
			minX = m_windowRect.m_x;
		}
		if (m_windowRect.m_x + m_windowRect.m_width - 1 < minX + width) {
			width = m_windowRect.m_x + m_windowRect.m_width - minX;
		}
		int height = diameter;
		if (minY < (int) m_windowRect.m_y) {
			height += minY - m_windowRect.m_y;
			minY = m_windowRect.m_y;
		}
		if (m_windowRect.m_y + m_windowRect.m_height - 1 < minY + height) {
			height = m_windowRect.m_y + m_windowRect.m_height - minY;
		}
		VsRect rect;
		rect.m_width = (short) width;
		rect.m_height = (short) height;
		rect.m_x = (short) minX;
		rect.m_y = (short) minY;
		AddToChangeList(&rect);
	}
}

// 68K 0x10111ee8 BlitRect__8CSurfaceF7CVSRecti
// FUNCTION: LEMBALL 0x004756e0
void Surface::BlitRect(VsRect p_rect, int p_colour)
{
	VsRect clipped;
	if (ClipRect(p_rect, &clipped)) {
		if (clipped.m_width <= 0 || clipped.m_height <= 0) {
			return;
		}
		p_rect.m_width = clipped.m_width;
		p_rect.m_height = clipped.m_height;
	}
	for (int y = 0; y < p_rect.m_height; y++) {
		memset((unsigned char*) m_lines[p_rect.m_y + y] + p_rect.m_x, p_colour, p_rect.m_width);
	}
	AddToChangeList(&p_rect);
}

// 68K 0x101121bc LineClip__8CSurfaceFRiRiRiRi
// FUNCTION: LEMBALL 0x004757a0
int Surface::LineClip(int& p_x1, int& p_y1, int& p_x2, int& p_y2)
{
	unsigned int code1;
	unsigned int code2;
	int dx;
	int dy;

	if (m_clipRect.m_height < 1 || m_clipRect.m_width < 1) {
		return 1;
	}
	code1 = 0;
	if (p_x1 < (int) m_clipRect.m_x) {
		code1 = 1;
	}
	else if (m_clipRect.m_x + m_clipRect.m_width - 1 < p_x1) {
		code1 = 2;
	}
	if (p_y1 < (int) m_clipRect.m_y) {
		code1 |= 4;
	}
	else if (m_clipRect.m_y + m_clipRect.m_height - 1 < p_y1) {
		code1 |= 8;
	}
	code2 = 0;
	if (p_x2 < (int) m_clipRect.m_x) {
		code2 = 1;
	}
	else if (m_clipRect.m_x + m_clipRect.m_width - 1 < p_x2) {
		code2 = 2;
	}
	if (p_y2 < (int) m_clipRect.m_y) {
		code2 |= 4;
	}
	else if (m_clipRect.m_y + m_clipRect.m_height - 1 < p_y2) {
		code2 |= 8;
	}
	if (code2 != 0 || code1 != 0) {
		do {
			if ((code1 & code2) != 0) {
				return 1;
			}
			dx = p_x2 - p_x1;
			dy = p_y2 - p_y1;
			if (code1 == 0) {
				if ((code2 & 1) == 0) {
					if ((code2 & 2) == 0) {
						if ((code2 & 4) != 0) {
							p_x2 = p_x2 + ((m_clipRect.m_y - p_y2) * dx) / dy;
							p_y2 = m_clipRect.m_y;
						}
						else if ((code2 & 8) != 0) {
							p_x2 = p_x2 + ((m_clipRect.m_y + m_clipRect.m_height - 1 - p_y2) * dx) / dy;
							p_y2 = m_clipRect.m_y + m_clipRect.m_height - 1;
						}
					}
					else {
						p_y2 = p_y2 + ((m_clipRect.m_x + m_clipRect.m_width - 1 - p_x2) * dy) / dx;
						p_x2 = m_clipRect.m_x + m_clipRect.m_width - 1;
					}
				}
				else {
					p_y2 = p_y2 + ((m_clipRect.m_x - p_x2) * dy) / dx;
					p_x2 = m_clipRect.m_x;
				}
				code2 = 0;
				if (p_x2 < (int) m_clipRect.m_x) {
					code2 = 1;
				}
				else if (m_clipRect.m_x + m_clipRect.m_width - 1 < p_x2) {
					code2 = 2;
				}
				if (p_y2 < (int) m_clipRect.m_y) {
					code2 |= 4;
				}
				else if (m_clipRect.m_y + m_clipRect.m_height - 1 < p_y2) {
					code2 |= 8;
				}
			}
			else {
				if ((code1 & 1) == 0) {
					if ((code1 & 2) == 0) {
						if ((code1 & 4) != 0) {
							p_x1 = p_x1 + ((m_clipRect.m_y - p_y1) * dx) / dy;
							p_y1 = m_clipRect.m_y;
						}
						else if ((code1 & 8) != 0) {
							p_x1 = p_x1 + ((m_clipRect.m_y + m_clipRect.m_height - 1 - p_y1) * dx) / dy;
							p_y1 = m_clipRect.m_y + m_clipRect.m_height - 1;
						}
					}
					else {
						p_y1 = p_y1 + ((m_clipRect.m_x + m_clipRect.m_width - 1 - p_x1) * dy) / dx;
						p_x1 = m_clipRect.m_x + m_clipRect.m_width - 1;
					}
				}
				else {
					p_y1 = p_y1 + ((m_clipRect.m_x - p_x1) * dy) / dx;
					p_x1 = m_clipRect.m_x;
				}
				code1 = 0;
				if (p_x1 < (int) m_clipRect.m_x) {
					code1 = 1;
				}
				else if (m_clipRect.m_x + m_clipRect.m_width - 1 < p_x1) {
					code1 = 2;
				}
				if (p_y1 < (int) m_clipRect.m_y) {
					code1 |= 4;
				}
				else if (m_clipRect.m_y + m_clipRect.m_height - 1 < p_y1) {
					code1 |= 8;
				}
			}
		} while (code2 != 0 || code1 != 0);
	}
	return 0;
}

// 68K 0x10113178 ClipCircle__8CSurfaceFiii
// FUNCTION: LEMBALL 0x00475bc0
int Surface::ClipCircle(int p_centerX, int p_centerY, int p_radius)
{
	int left = p_centerX - p_radius;
	int top = p_centerY - p_radius;
	int right = p_centerX + p_radius;
	int bottom = p_centerY + p_radius;

	if (m_clipRect.m_height <= 0 || m_clipRect.m_width <= 0) {
		return 1;
	}
	int clipRight = (int) m_clipRect.m_x + (int) m_clipRect.m_width - 1;
	int clipBottom = (int) m_clipRect.m_y + (int) m_clipRect.m_height - 1;
	if (right >= m_clipRect.m_x && left <= clipRight && bottom >= m_clipRect.m_y && top <= clipBottom) {
		if (left >= m_clipRect.m_x && right <= clipRight && top >= m_clipRect.m_y && bottom <= clipBottom) {
			return 2;
		}
		return 3;
	}
	return 1;
}

// FUNCTION: LEMBALL 0x00475ce0
void Surface::DrawClippedCircleOutline(int p_centerX, int p_centerY, int p_radius, unsigned char p_colour)
{
	int x = 0;
	int err = 0;
	int step = 1;
	int errLimit = p_radius * 2 - 1;

	if (m_clipRect.m_x <= p_centerX && p_centerX <= m_clipRect.m_x + m_clipRect.m_width - 1) {
		int rowY = p_centerY + p_radius;
		if (m_clipRect.m_y <= rowY && rowY <= m_clipRect.m_y + m_clipRect.m_height - 1) {
			*((unsigned char*) m_lines[rowY] + p_centerX) = p_colour;
		}
	}
	if (m_clipRect.m_x <= p_centerX && p_centerX <= m_clipRect.m_x + m_clipRect.m_width - 1) {
		int rowY = p_centerY - p_radius;
		if (m_clipRect.m_y <= rowY && rowY <= m_clipRect.m_y + m_clipRect.m_height - 1) {
			*((unsigned char*) m_lines[rowY] + p_centerX) = p_colour;
		}
	}
	if (m_clipRect.m_x <= p_centerX + p_radius && p_centerX + p_radius <= m_clipRect.m_x + m_clipRect.m_width - 1 &&
		m_clipRect.m_y <= p_centerY && p_centerY <= m_clipRect.m_y + m_clipRect.m_height - 1) {
		*((unsigned char*) m_lines[p_centerY] + p_centerX + p_radius) = p_colour;
	}
	if (m_clipRect.m_x <= p_centerX - p_radius && p_centerX - p_radius <= m_clipRect.m_x + m_clipRect.m_width - 1 &&
		m_clipRect.m_y <= p_centerY && p_centerY <= m_clipRect.m_y + m_clipRect.m_height - 1) {
		*((unsigned char*) m_lines[p_centerY] + p_centerX - p_radius) = p_colour;
	}
	if (p_radius <= 0) {
		return;
	}
	int lowerRow = p_centerY - p_radius;
	int upperRow = p_centerY + p_radius;
	do {
		x = x + 1;
		err = err + step;
		if (errLimit < err * 2) {
			p_radius = p_radius - 1;
			lowerRow = lowerRow + 1;
			upperRow = upperRow - 1;
			err = err - errLimit;
			errLimit = errLimit - 2;
		}
		if (x <= p_radius) {
			if (ClipCirclePoint(p_centerX + x, p_centerY + p_radius) != 0) {
				*((unsigned char*) m_lines[upperRow] + p_centerX + x) = p_colour;
			}
			if (ClipCirclePoint(p_centerX - x, p_centerY + p_radius) != 0) {
				*((unsigned char*) m_lines[upperRow] + p_centerX - x) = p_colour;
			}
			if (ClipCirclePoint(p_centerX + x, p_centerY - p_radius) != 0) {
				*((unsigned char*) m_lines[lowerRow] + p_centerX + x) = p_colour;
			}
			if (ClipCirclePoint(p_centerX - x, p_centerY - p_radius) != 0) {
				*((unsigned char*) m_lines[lowerRow] + p_centerX - x) = p_colour;
			}
			if (x < p_radius) {
				DrawClippedCirclePoint(p_centerX, p_centerY, p_radius, x, p_colour);
			}
		}
		step = step + 2;
	} while (x < p_radius);
}

// FUNCTION: LEMBALL 0x00475f60
int Surface::ClipCirclePoint(int p_x, int p_y)
{
	short clipX;
	short clipRight;
	short clipY;
	short clipBottom;

	clipX = PvScrollableSurface::m_clipRect.m_x;
	if (p_x < clipX) {
		return 0;
	}
	clipRight = (short) (PvScrollableSurface::m_clipRect.m_width + clipX - 1);
	if (clipRight < p_x) {
		return 0;
	}
	clipY = PvScrollableSurface::m_clipRect.m_y;
	if (p_y < clipY) {
		return 0;
	}
	clipBottom = (short) (PvScrollableSurface::m_clipRect.m_height + clipY - 1);
	if (clipBottom < p_y) {
		return 0;
	}
	return 1;
}

// FUNCTION: LEMBALL 0x00475fb0
void Surface::DrawClippedCirclePoint(int p_centerX, int p_centerY, int p_xOffset, int p_yOffset, unsigned char p_colour)
{
	int px1 = p_centerX + p_xOffset;
	if (m_clipRect.m_x <= px1 && px1 <= m_clipRect.m_x + m_clipRect.m_width - 1) {
		int py1 = p_centerY + p_yOffset;
		if (m_clipRect.m_y <= py1 && py1 <= m_clipRect.m_y + m_clipRect.m_height - 1) {
			*((unsigned char*) m_lines[py1] + px1) = p_colour;
		}
	}
	int px2 = p_centerX - p_xOffset;
	if (m_clipRect.m_x <= px2 && px2 <= m_clipRect.m_x + m_clipRect.m_width - 1) {
		int py1 = p_centerY + p_yOffset;
		if (m_clipRect.m_y <= py1 && py1 <= m_clipRect.m_y + m_clipRect.m_height - 1) {
			*((unsigned char*) m_lines[py1] + px2) = p_colour;
		}
	}
	if (m_clipRect.m_x <= px1 && px1 <= m_clipRect.m_x + m_clipRect.m_width - 1) {
		int py2 = p_centerY - p_yOffset;
		if (m_clipRect.m_y <= py2 && py2 <= m_clipRect.m_y + m_clipRect.m_height - 1) {
			*((unsigned char*) m_lines[py2] + px1) = p_colour;
		}
	}
	if (m_clipRect.m_x <= px2 && px2 <= m_clipRect.m_x + m_clipRect.m_width - 1) {
		int py2 = p_centerY - p_yOffset;
		if (m_clipRect.m_y <= py2 && py2 <= m_clipRect.m_y + m_clipRect.m_height - 1) {
			*((unsigned char*) m_lines[py2] + px2) = p_colour;
		}
	}
}

// FUNCTION: LEMBALL 0x00476100
void Surface::DrawFilledCircleSymmetricSpans(int p_centerX, int p_centerY, int p_halfWidth, int p_yOffset, unsigned char p_colour)
{
	int spanWidth = p_halfWidth * 2 + 1;
	memset((unsigned char*) m_lines[p_centerY + p_yOffset] + p_centerX - p_halfWidth, p_colour, spanWidth);
	memset((unsigned char*) m_lines[p_centerY - p_yOffset] + p_centerX - p_halfWidth, p_colour, spanWidth);
}

// 68K 0x1011355a DrawClippedFilledCircle__8CSurfaceFiiii
// FUNCTION: LEMBALL 0x00476190
void Surface::DrawClippedFilledCircle(int p_centerX, int p_centerY, int p_radius, int p_colour)
{
	int x = 0;
	int err = 0;
	int step = 1;
	int errLimit = p_radius * 2 - 1;

	if (m_clipRect.m_x <= p_centerX && p_centerX <= (int) (m_clipRect.m_width + m_clipRect.m_x - 1)) {
		int y1 = p_centerY + p_radius;
		if (m_clipRect.m_y <= y1 && y1 <= (int) (m_clipRect.m_height + m_clipRect.m_y - 1)) {
			*((unsigned char*) m_lines[y1] + p_centerX) = (unsigned char) p_colour;
		}
	}
	if (m_clipRect.m_x <= p_centerX && p_centerX <= (int) (m_clipRect.m_width + m_clipRect.m_x - 1)) {
		int y2 = p_centerY - p_radius;
		if (m_clipRect.m_y <= y2 && y2 <= (int) (m_clipRect.m_height + m_clipRect.m_y - 1)) {
			*((unsigned char*) m_lines[y2] + p_centerX) = (unsigned char) p_colour;
		}
	}
	if (m_clipRect.m_y <= p_centerY && p_centerY <= (int) (m_clipRect.m_height + m_clipRect.m_y - 1)) {
		int x1 = p_centerX - p_radius;
		if (x1 < m_clipRect.m_x) {
			x1 = m_clipRect.m_x;
		}
		int x2 = p_centerX + p_radius;
		if ((int) (m_clipRect.m_width + m_clipRect.m_x - 1) < x2) {
			x2 = m_clipRect.m_width + m_clipRect.m_x - 1;
		}
		memset((unsigned char*) m_lines[p_centerY] + x1, p_colour, x2 - x1 + 1);
	}

	if (p_radius > 1) {
		do {
			int oldErrLimit = errLimit;
			x++;
			err += step;
			step += 2;
			int doubleErr = err * 2;
			if (errLimit < doubleErr) {
				p_radius--;
				err -= errLimit;
				errLimit -= 2;
			}
			if (x <= p_radius) {
				if (oldErrLimit < doubleErr) {
					int yTop = p_centerY - p_radius;
					int yBottom = p_centerY + p_radius;
					int clipY = m_clipRect.m_y;
					if (yTop <= (int) (m_clipRect.m_height + clipY - 1) && clipY <= yBottom) {
						int xLeft = p_centerX - x;
						int xRight = p_centerX + x;
						int clipX = m_clipRect.m_x;
						if (clipX <= xRight && xLeft <= (int) (m_clipRect.m_width + clipX - 1)) {
							if ((int) (m_clipRect.m_width + clipX - 1) < xRight) {
								xRight = m_clipRect.m_width + clipX - 1;
							}
							if (xLeft < clipX) {
								xLeft = clipX;
							}
							if (clipY <= yTop) {
								memset((unsigned char*) m_lines[yTop] + xLeft, p_colour, xRight - xLeft + 1);
							}
							if (yBottom <= (int) (m_clipRect.m_height + m_clipRect.m_y - 1)) {
								memset((unsigned char*) m_lines[yBottom] + xLeft, p_colour, xRight - xLeft + 1);
							}
						}
					}
				}
				if (x < p_radius) {
					FilledCircleClipPoints(p_centerX, p_centerY, p_radius, x, p_colour);
				}
			}
		} while (x < p_radius);
	}
}

// 68K 0x1011387e FilledCircleClipPoints__8CSurfaceFiiiii
// FUNCTION: LEMBALL 0x00476470
void Surface::FilledCircleClipPoints(int p_centerX, int p_centerY, int p_xOffset, int p_yOffset, int p_colour)
{
	int y1 = p_centerY - p_yOffset;
	int y2 = p_centerY + p_yOffset;
	int clipY = m_clipRect.m_y;
	if (y1 <= (int) (m_clipRect.m_height + clipY - 1) && clipY <= y2) {
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
			if (clipY <= y1) {
				memset((unsigned char*) m_lines[y1] + x1, p_colour, x2 - x1 + 1);
			}
			if (y2 <= (int) (m_clipRect.m_height + m_clipRect.m_y - 1)) {
				memset((unsigned char*) m_lines[y2] + x1, p_colour, x2 - x1 + 1);
			}
		}
	}
}

// 68K 0x10113bf4 ClipRect__8CSurfaceFR7CVSRectP7CVSRect
// FUNCTION: LEMBALL 0x00476580
bool Surface::ClipRect(VsRect& p_rect, VsRect* p_clipped)
{
	bool clipped = false;

	if ((short) (m_clipRect.m_width + m_clipRect.m_x) < p_rect.m_x ||
		(short) (m_clipRect.m_height + m_clipRect.m_y) < p_rect.m_y ||
		(short) (p_rect.m_width + p_rect.m_x) < m_clipRect.m_x ||
		(short) (p_rect.m_height + p_rect.m_y) < m_clipRect.m_y) {
		return true;
	}

	if (m_clipRect.m_x > p_rect.m_x) {
		p_clipped->m_x = m_clipRect.m_x - p_rect.m_x;
		clipped = true;
		p_rect.m_x = m_clipRect.m_x;
		p_rect.m_width -= p_clipped->m_x;
	}

	if (m_clipRect.m_y > p_rect.m_y) {
		p_clipped->m_y = m_clipRect.m_y - p_rect.m_y;
		clipped = true;
		p_rect.m_y = m_clipRect.m_y;
		p_rect.m_height -= p_clipped->m_y;
	}

	if ((short) (p_rect.m_x + p_rect.m_width) > (short) (m_clipRect.m_x + m_clipRect.m_width)) {
		p_clipped->m_width = (m_clipRect.m_x + m_clipRect.m_width) - p_rect.m_x;
		p_rect.m_width = (m_clipRect.m_x - p_rect.m_x) + m_clipRect.m_width;
		clipped = true;
	}
	else {
		p_clipped->m_width = p_rect.m_width;
	}

	if ((short) (p_rect.m_y + p_rect.m_height) > (short) (m_clipRect.m_y + m_clipRect.m_height)) {
		p_clipped->m_height = (m_clipRect.m_y + m_clipRect.m_height) - p_rect.m_y;
		p_rect.m_height = (m_clipRect.m_y - p_rect.m_y) + m_clipRect.m_height;
		return true;
	}

	p_clipped->m_height = p_rect.m_height;
	return clipped;
}

// 68K 0x1010519e BlitZRLEClip__8CSurfaceFRC7CVSRectRC7CVSRectP8CResZRLEUc
// FUNCTION: LEMBALL 0x004766f0
void Surface::BlitZrleClip(const VsRect& p_rect, const VsRect& p_clip, ResZrle* p_zrle, unsigned int p_reverse)
{
	int x = p_rect.m_x;
	int step = 1;
	int y = p_rect.m_y;
	unsigned char* src = p_zrle->GetData();
	if (p_reverse != 0) {
		step = -1;
		y += p_rect.m_height - 1;
		int skipRows = (p_zrle->m_height - p_clip.m_y) - p_rect.m_height;
		if (skipRows > 0) {
			do {
				unsigned char run;
				do {
					run = *src++;
					if (run > 0x80) {
						src += run & 0x7f;
					}
				} while (run != 0x80);
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
					if (run > 0x80) {
						src += run & 0x7f;
					}
				} while (run != 0x80);
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
				if (run < 0x80) {
					clipX -= run;
					if (clipX < 0) {
						width += clipX;
						dst -= clipX;
					}
				}
				else if (run > 0x80) {
					run &= 0x7f;
					clipX -= run;
					if (clipX < 0) {
						int copyLen = -clipX;
						unsigned char* copySrc = src + run + clipX;
						if (copyLen < width) {
							memcpy(dst, copySrc, copyLen);
						}
						else {
							memcpy(dst, copySrc, width);
						}
						width += clipX;
						dst += copyLen;
					}
					src += run;
				}
				if (run == 0x80) goto row_done_clip;
			} while (clipX > 0);
			if (run != 0x80) {
				do {
					if (width < 1) break;
					run = *src++;
					if (width > 0) {
						if (run < 0x80) {
							dst += run;
							width -= run;
						}
						else if (run > 0x80) {
							run &= 0x7f;
							if (run < width) {
								memcpy(dst, src, run);
								width -= run;
							}
							else {
								memcpy(dst, src, width);
								width = 0;
							}
							dst += run;
							src += run;
						}
					}
				} while (run != 0x80);
row_done_clip:
				while (run != 0x80) {
					run = *src++;
					if (run > 0x80) {
						src += run & 0x7f;
					}
				}
			}
			y += step;
			row++;
		} while (row < p_rect.m_height);
	}
}

// 68K 0x1010539e BlitZRLEClipZBuff__8CSurfaceFRC7CVSRectRC7CVSRectP8CResZRLEUs
// FUNCTION: LEMBALL 0x00476910
void Surface::BlitZrleClipZBuff(const VsRect& p_rect, const VsRect& p_clip, ResZrle* p_zrle, unsigned short p_depth)
{
	unsigned char* src = p_zrle->GetData();
	if (p_clip.m_y > 0) {
		int skipRows = p_clip.m_y;
		do {
			unsigned char run;
			do {
				run = *src++;
				if (run > 0x80) {
					src += run & 0x7f;
				}
			} while (run != 0x80);
			skipRows--;
		} while (skipRows != 0);
	}
	int row = 0;
	int x = p_rect.m_x;
	if (p_rect.m_height > 0) {
		int y = p_rect.m_y;
		do {
			int width = p_rect.m_width;
			int clipX = p_clip.m_x;
			unsigned short* zlines = (unsigned short*) ((unsigned char*) PvZBuffSurface::m_bitmap.m_lines[y] + x * 2);
			unsigned char* dst = (unsigned char*) m_lines[y] + x;
			unsigned char run;
			do {
				run = *src++;
				if (run < 0x80) {
					clipX -= run;
					if (clipX < 0) {
						width += clipX;
						dst -= clipX;
						zlines -= clipX;
					}
				}
				else if (run > 0x80) {
					run &= 0x7f;
					clipX -= run;
					if (clipX < 0) {
						int copyLen = -clipX;
						unsigned char* copySrc = src + run + clipX;
						if (copyLen < width) {
							memcpy(dst, copySrc, copyLen);
							for (unsigned int i = 0; i < (unsigned int) copyLen; i++) {
								zlines[i] = p_depth;
							}
						}
						else {
							memcpy(dst, copySrc, width);
							for (unsigned int i = 0; i < (unsigned int) width; i++) {
								zlines[i] = p_depth;
							}
						}
						width += clipX;
						dst += copyLen;
						zlines -= clipX;
					}
					src += run;
				}
				if (run == 0x80) goto row_done_zbuff;
			} while (clipX > 0);
			if (run != 0x80) {
				do {
					if (width < 1) break;
					run = *src++;
					if (width > 0) {
						if (run < 0x80) {
							dst += run;
							zlines += run;
							width -= run;
						}
						else if (run > 0x80) {
							run &= 0x7f;
							if (run < width) {
								memcpy(dst, src, run);
								for (unsigned int i = 0; i < (unsigned int) run; i++) {
									zlines[i] = p_depth;
								}
								width -= run;
								dst += run;
								zlines += run;
							}
							else {
								memcpy(dst, src, width);
								for (unsigned int i = 0; i < (unsigned int) width; i++) {
									zlines[i] = p_depth;
								}
								dst += width;
								zlines += width;
								width = 0;
							}
							src += run;
						}
					}
				} while (run != 0x80);
row_done_zbuff:
				while (run != 0x80) {
					run = *src++;
					if (run > 0x80) {
						src += run & 0x7f;
					}
				}
			}
			y++;
			row++;
		} while (row < p_rect.m_height);
	}
}

// 68K 0x10105606 BlitZRLEClipQZBuff__8CSurfaceFRC7CVSRectRC7CVSRectP8CResZRLEUs
// FUNCTION: LEMBALL 0x00476bf0
void Surface::BlitZrleClipQzBuff(const VsRect& p_rect, const VsRect& p_clip, ResZrle* p_zrle, unsigned short p_depth)
{
	unsigned char* src = p_zrle->GetData();
	if (p_clip.m_y > 0) {
		int skipRows = p_clip.m_y;
		do {
			unsigned char run;
			do {
				run = *src++;
				if (run > 0x80) {
					src += run & 0x7f;
				}
			} while (run != 0x80);
			skipRows--;
		} while (skipRows != 0);
	}
	int row = 0;
	int x = p_rect.m_x;
	if (p_rect.m_height > 0) {
		int y = p_rect.m_y;
		do {
			int width = p_rect.m_width;
			int clipX = p_clip.m_x;
			unsigned short* zlines = (unsigned short*) ((unsigned char*) PvZBuffSurface::m_bitmap.m_lines[y] + x * 2);
			unsigned char* dst = (unsigned char*) m_lines[y] + x;
			unsigned char run;
			do {
				run = *src++;
				if (run < 0x80) {
					clipX -= run;
					if (clipX < 0) {
						dst -= clipX;
						width += clipX;
						zlines -= clipX;
					}
				}
				else if (run > 0x80) {
					run &= 0x7f;
					clipX -= run;
					if (clipX < 0) {
						int copyLen = -clipX;
						unsigned char* copySrc = src + run + clipX;
						if (copyLen < width) {
							unsigned char count = (unsigned char) copyLen;
							unsigned short* copyZ = zlines;
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
							char count = (char) width;
							unsigned short* copyZ = zlines;
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
						width += clipX;
						zlines -= clipX;
					}
					src += run;
				}
				if (run == 0x80) goto row_done_qzbuff;
			} while (clipX > 0);
			if (run != 0x80) {
				do {
					if (width < 1) break;
					run = *src++;
					if (width > 0) {
						if (run < 0x80) {
							dst += run;
							width -= run;
							zlines += run;
						}
						else if (run > 0x80) {
							run &= 0x7f;
							unsigned char* copySrc = src;
							unsigned short* copyZ = zlines;
							unsigned char* copyDst = dst;
							unsigned char r = run;
							if (run < width) {
								while (r != 0) {
									r--;
									if (*copyZ <= p_depth) {
										*copyDst = *copySrc;
									}
									copySrc++;
									copyZ++;
									copyDst++;
								}
								width -= run;
							}
							else {
								while ((char) width != 0) {
									width--;
									if (*copyZ <= p_depth) {
										*copyDst = *copySrc;
									}
									copyDst++;
									copyZ++;
									copySrc++;
								}
								width = 0;
							}
							dst += run;
							zlines += run;
							src += run;
						}
					}
				} while (run != 0x80);
row_done_qzbuff:
				while (run != 0x80) {
					run = *src++;
					if (run > 0x80) {
						src += run & 0x7f;
					}
				}
			}
			y++;
			row++;
		} while (row < p_rect.m_height);
	}
}

// 68K 0x10113e7a BlitZRLEClipR__8CSurfaceFRC7CVSRectRC7CVSRectP8CResZRLEUc
// FUNCTION: LEMBALL 0x00476ee0
void Surface::BlitZrleClipR(const VsRect& p_rect, const VsRect& p_clip, ResZrle* p_zrle, unsigned int p_reverse)
{
	int step = 1;
	int y = p_rect.m_y;
	unsigned char* src = p_zrle->GetData();
	if (p_reverse == 0) {
		if (p_clip.m_y > 0) {
			int skipRows = p_clip.m_y;
			do {
				unsigned char run;
				do {
					run = *src++;
					if (run > 0x80) {
						src += run & 0x7f;
					}
				} while (run != 0x80);
				skipRows--;
			} while (skipRows != 0);
		}
	}
	else {
		step = -1;
		y += p_rect.m_height - 1;
		int skipRows = (p_zrle->m_height - p_clip.m_y) - p_rect.m_height;
		if (skipRows > 0) {
			do {
				unsigned char run;
				do {
					run = *src++;
					if (run > 0x80) {
						src += run & 0x7f;
					}
				} while (run != 0x80);
				skipRows--;
			} while (skipRows != 0);
		}
	}
	int row = 0;
	if (p_rect.m_height > 0) {
		int rowIndex = y << 2;
		do {
			int width = p_rect.m_width;
			int skipX = (p_zrle->m_width - p_clip.m_x) - width;
			unsigned char* dst = (unsigned char*) *(int*) ((int) m_lines + rowIndex) + p_rect.m_x;
			unsigned char run;
			do {
				run = *src++;
				if (skipX < 1) {
					if (width < 1) {
						if (run > 0x80) {
							src += run & 0x7f;
						}
					}
					else if (run < 0x80) {
						width -= run;
						dst -= run;
					}
					else if (run > 0x80) {
						int count = run & 0x7f;
						if (count < width) {
							for (int i = 0; i < count; i++) {
								*dst-- = *src++;
							}
							width -= count;
						}
						else {
							for (int i = 0; i < width; i++) {
								*dst-- = *src++;
							}
							src += count - width;
							width = 0;
						}
					}
				}
				else if (run < 0x80) {
					skipX -= run;
					if (skipX < 0) {
						width += skipX;
						dst += skipX;
					}
				}
				else if (run > 0x80) {
					int count = run & 0x7f;
					skipX -= count;
					if (skipX < 0) {
						int copyLen = -skipX;
						int actualLen = (copyLen < width) ? copyLen : width;
						unsigned char* copySrc = src + count + skipX;
						for (int i = 0; i < actualLen; i++) {
							*dst-- = *copySrc++;
						}
						width += skipX;
						dst += skipX;
					}
					src += count;
				}
			} while (run != 0x80);
			rowIndex += step * 4;
			row++;
		} while (row < p_rect.m_height);
	}
}

// 68K 0x10104cd8 BlitZRLENoClip__8CSurfaceFRC7CVSRectP8CResZRLEUc
// FUNCTION: LEMBALL 0x00477130
void Surface::BlitZrleNoClip(const VsRect& p_rect, ResZrle* p_zrle, unsigned int p_reverse)
{
	int x = p_rect.m_x;
	int step = 1;
	int y = p_rect.m_y;
	if (p_reverse != 0) {
		step = -1;
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
				if (run < 0x80) {
					dst += run;
				}
				else if (run > 0x80) {
					run &= 0x7f;
					memcpy(dst, src, run);
					dst += run;
					src += run;
				}
			} while (run != 0x80);
			y += step;
			row++;
		} while (row < p_rect.m_height);
	}
}

// 68K 0x10104e4c BlitZRLENoClipZBuff__8CSurfaceFRC7CVSRectP8CResZRLEUs
// FUNCTION: LEMBALL 0x00477200
void Surface::BlitZrleNoClipZBuff(const VsRect& p_rect, ResZrle* p_zrle, unsigned short p_depth)
{
	int x = p_rect.m_x;
	int y = p_rect.m_y;
	unsigned char* src = p_zrle->GetData();
	int row = 0;
	if (p_rect.m_height > 0) {
		do {
			unsigned char* dst = (unsigned char*) m_lines[y] + x;
			unsigned short* zlines = (unsigned short*) ((unsigned char*) PvZBuffSurface::m_bitmap.m_lines[y] + x * 2);
			unsigned char run;
			do {
				run = *src++;
				if (run < 0x80) {
					dst += run;
					zlines += run;
				}
				else if (run > 0x80) {
					run &= 0x7f;
					memcpy(dst, src, run);
					if (run) {
						for (unsigned int i = 0; i < run; i++) {
							zlines[i] = p_depth;
						}
					}
					zlines += run;
					dst += run;
					src += run;
				}
			} while (run != 0x80);
			row++;
			y++;
		} while (row < p_rect.m_height);
	}
}

// 68K 0x10114162 BlitZRLENoClipZBuffRemap__8CSurfaceFRC7CVSRectP8CResZRLEUsPUc
// FUNCTION: LEMBALL 0x00477310
void Surface::BlitZrleNoClipZBuffRemap(const VsRect& p_rect,
									   ResZrle* p_zrle,
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
			unsigned short* zlines = (unsigned short*) ((unsigned char*) PvZBuffSurface::m_bitmap.m_lines[y] + x * 2);
			unsigned char run;
			do {
				run = *src++;
				if (run < 0x80) {
					dst += run;
					zlines += run;
				}
				else if (run > 0x80) {
					run &= 0x7f;
					for (int i = 0; i < (int) run; i++) {
						dst[i] = p_remap[src[i]];
					}
					if (run) {
						for (unsigned int i = 0; i < run; i++) {
							zlines[i] = p_depth;
						}
					}
					dst += run;
					src += run;
					zlines += run;
				}
			} while (run != 0x80);
			row++;
		} while (row < p_rect.m_height);
	}
}

// 68K 0x10104fa6 BlitZRLENoClipQZBuff__8CSurfaceFRC7CVSRectP8CResZRLEUs
// FUNCTION: LEMBALL 0x00477440
void Surface::BlitZrleNoClipQzBuff(const VsRect& p_rect, ResZrle* p_zrle, unsigned short p_depth)
{
	int x = p_rect.m_x;
	int y = p_rect.m_y;
	unsigned char* src = p_zrle->GetData();
	int row = 0;
	if (p_rect.m_height > 0) {
		do {
			unsigned char* dst = (unsigned char*) m_lines[y] + x;
			unsigned short* zlines = (unsigned short*) ((unsigned char*) PvZBuffSurface::m_bitmap.m_lines[y] + x * 2);
			unsigned char run;
			do {
				run = *src++;
				if (run < 0x80) {
					dst += run;
					zlines += run;
				}
				else if (run > 0x80) {
					run &= 0x7f;
					unsigned char count = run;
					unsigned char* copySrc = src;
					while (count != 0) {
						count--;
						if (*zlines <= p_depth) {
							*dst = *copySrc;
						}
						dst++;
						zlines++;
						copySrc++;
					}
					src += run;
				}
			} while (run != 0x80);
			row++;
			y++;
		} while (row < p_rect.m_height);
	}
}

// 68K 0x101142ac BlitZRLENoClipQZBuffRemap__8CSurfaceFRC7CVSRectP8CResZRLEUsPUc
// FUNCTION: LEMBALL 0x00477540
void Surface::BlitZrleNoClipQzBuffRemap(const VsRect& p_rect,
										ResZrle* p_zrle,
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
			unsigned short* zlines = (unsigned short*) ((unsigned char*) PvZBuffSurface::m_bitmap.m_lines[y] + x * 2);
			unsigned char run;
			do {
				run = *src++;
				if (run < 0x80) {
					dst += run;
					zlines += run;
				}
				else if (run > 0x80) {
					run &= 0x7f;
					unsigned char* copySrc = src;
					for (unsigned char i = run; i != 0; i--) {
						if (*zlines <= p_depth) {
							*dst = p_remap[*copySrc];
						}
						zlines++;
						dst++;
						copySrc++;
					}
					src += run;
				}
			} while (run != 0x80);
			row++;
			y++;
		} while (row < p_rect.m_height);
	}
}

// 68K 0x101143ee BlitZRLENoClipR__8CSurfaceFRC7CVSRectP8CResZRLEUc
// FUNCTION: LEMBALL 0x00477660
void Surface::BlitZrleNoClipR(const VsRect& p_rect, ResZrle* p_zrle, unsigned int p_reverse)
{
	int startX = p_rect.m_x + p_rect.m_width - 1;
	int step = 1;
	int y = p_rect.m_y;
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
					unsigned char* copySrc = src;
					unsigned char* copyDst = dst;
					for (int i = run; i != 0; i--) {
						*copyDst-- = *copySrc++;
					}
					src += run;
					dst -= run;
				}
			} while (run != 0x80);
			y += step;
			row++;
		} while (row < p_rect.m_height);
	}
}

// 68K 0x101144ee BlitZRLEClipRemap__8CSurfaceFRC7CVSRectRC7CVSRectP8CResZRLEUcPUc
// FUNCTION: LEMBALL 0x00477740
void Surface::BlitZrleClipRemap(const VsRect& p_rect,
								const VsRect& p_clip,
								ResZrle* p_zrle,
								unsigned int p_reverse,
								unsigned char* p_remap)
{
	int step = 1;
	int y = p_rect.m_y;
	unsigned char* src = p_zrle->GetData();
	if (p_reverse == 0) {
		if (p_clip.m_y > 0) {
			int skipRows = p_clip.m_y;
			do {
				unsigned char run;
				do {
					run = *src++;
					if (run > 0x80) {
						src += run & 0x7f;
					}
				} while (run != 0x80);
				skipRows--;
			} while (skipRows != 0);
		}
	}
	else {
		step = -1;
		y += p_rect.m_height - 1;
		int skipRows = (p_zrle->m_height - p_clip.m_y) - p_rect.m_height;
		if (skipRows > 0) {
			do {
				unsigned char run;
				do {
					run = *src++;
					if (run > 0x80) {
						src += run & 0x7f;
					}
				} while (run != 0x80);
				skipRows--;
			} while (skipRows != 0);
		}
	}
	int row = 0;
	if (p_rect.m_height > 0) {
		do {
			int width = p_rect.m_width;
			int clipX = p_clip.m_x;
			unsigned char* dst = (unsigned char*) m_lines[y] + p_rect.m_x;
			unsigned char run;
			do {
				run = *src++;
				if (run < 0x80) {
					clipX -= run;
					if (clipX < 0) {
						width += clipX;
						dst -= clipX;
					}
				}
				else if (run > 0x80) {
					int count = run & 0x7f;
					clipX -= count;
					if (clipX < 0) {
						int copyLen = -clipX;
						int actualLen = (copyLen < width) ? copyLen : width;
						unsigned char* copySrc = src + count + clipX;
						for (int i = 0; i < actualLen; i++) {
							*dst++ = p_remap[*copySrc++];
						}
						width += clipX;
						dst += copyLen - actualLen;
					}
					src += count;
				}
				if (run == 0x80) goto row_done_remap;
			} while (clipX > 0);
			if (run != 0x80) {
				do {
					if (width < 1) break;
					run = *src++;
					if (width > 0) {
						if (run < 0x80) {
							dst += run;
							width -= run;
						}
						else if (run > 0x80) {
							int count = run & 0x7f;
							int actualLen = (count < width) ? count : width;
							for (int i = 0; i < actualLen; i++) {
								*dst++ = p_remap[*src++];
							}
							src += count - actualLen;
							width -= actualLen;
						}
					}
				} while (run != 0x80);
row_done_remap:
				while (run != 0x80) {
					run = *src++;
					if (run > 0x80) {
						src += run & 0x7f;
					}
				}
			}
			y += step;
			row++;
		} while (row < p_rect.m_height);
	}
}

// 68K 0x101147c6 BlitZRLEClipZBuffRemap__8CSurfaceFRC7CVSRectRC7CVSRectP8CResZRLEUsPUc
// FUNCTION: LEMBALL 0x004779d0
void Surface::BlitZrleClipZBuffRemap(const VsRect& p_rect,
									 const VsRect& p_clip,
									 ResZrle* p_zrle,
									 unsigned short p_depth,
									 unsigned char* p_remap)
{
	unsigned char* src = p_zrle->GetData();
	if (p_clip.m_y > 0) {
		int skipRows = p_clip.m_y;
		do {
			unsigned char run;
			do {
				run = *src++;
				if (run > 0x80) {
					src += run & 0x7f;
				}
			} while (run != 0x80);
			skipRows--;
		} while (skipRows != 0);
	}
	int row = 0;
	if (p_rect.m_height > 0) {
		do {
			int y = p_rect.m_y + row;
			int width = p_rect.m_width;
			int clipX = p_clip.m_x;
			unsigned short* zlines = (unsigned short*) PvZBuffSurface::m_bitmap.m_lines[y] + p_rect.m_x;
			unsigned char* dst = (unsigned char*) m_lines[y] + p_rect.m_x;
			unsigned char run;
			do {
				run = *src++;
				if (run < 0x80) {
					clipX -= run;
					if (clipX < 0) {
						width += clipX;
						dst -= clipX;
						zlines -= clipX;
					}
				}
				else if (run > 0x80) {
					int count = run & 0x7f;
					clipX -= count;
					if (clipX < 0) {
						int copyLen = -clipX;
						int actualLen = (copyLen < width) ? copyLen : width;
						unsigned char* copySrc = src + count + clipX;
						for (int i = 0; i < actualLen; i++) {
							dst[i] = p_remap[copySrc[i]];
							zlines[i] = p_depth;
						}
						width += clipX;
						dst += copyLen;
						zlines -= clipX;
					}
					src += count;
				}
				if (run == 0x80) goto row_done_zbuff_remap;
			} while (clipX > 0);
			if (run != 0x80) {
				do {
					if (width < 1) break;
					run = *src++;
					if (width > 0) {
						if (run < 0x80) {
							dst += run;
							zlines += run;
							width -= run;
						}
						else if (run > 0x80) {
							int count = run & 0x7f;
							int actualLen = (count < width) ? count : width;
							for (int i = 0; i < actualLen; i++) {
								dst[i] = p_remap[src[i]];
								zlines[i] = p_depth;
							}
							width = (count < width) ? width - count : 0;
							dst += actualLen;
							zlines += actualLen;
							src += count;
						}
					}
				} while (run != 0x80);
row_done_zbuff_remap:
				while (run != 0x80) {
					run = *src++;
					if (run > 0x80) {
						src += run & 0x7f;
					}
				}
			}
			row++;
		} while (row < p_rect.m_height);
	}
}

// 68K 0x10114ab8 BlitZRLEClipQZBuffRemap__8CSurfaceFRC7CVSRectRC7CVSRectP8CResZRLEUsPUc
// FUNCTION: LEMBALL 0x00477c60
void Surface::BlitZrleClipQzBuffRemap(const VsRect& p_rect,
									  const VsRect& p_clip,
									  ResZrle* p_zrle,
									  unsigned short p_depth,
									  unsigned char* p_remap)
{
	unsigned char* src = p_zrle->GetData();
	if (p_clip.m_y > 0) {
		int skipRows = p_clip.m_y;
		do {
			unsigned char run;
			do {
				run = *src++;
				if (run > 0x80) {
					src += run & 0x7f;
				}
			} while (run != 0x80);
			skipRows--;
		} while (skipRows != 0);
	}
	int row = 0;
	if (p_rect.m_height > 0) {
		do {
			int y = p_rect.m_y + row;
			int width = p_rect.m_width;
			int clipX = p_clip.m_x;
			unsigned short* zlines = (unsigned short*) PvZBuffSurface::m_bitmap.m_lines[y] + p_rect.m_x;
			unsigned char* dst = (unsigned char*) m_lines[y] + p_rect.m_x;
			unsigned char run;
			do {
				run = *src++;
				if (run < 0x80) {
					clipX -= run;
					if (clipX < 0) {
						width += clipX;
						dst -= clipX;
						zlines -= clipX;
					}
				}
				else if (run > 0x80) {
					int count = run & 0x7f;
					clipX -= count;
					if (clipX < 0) {
						int copyLen = -clipX;
						int actualLen = (copyLen < width) ? copyLen : width;
						unsigned char* copySrc = src + count + clipX;
						for (int i = 0; i < actualLen; i++) {
							if (zlines[i] <= p_depth) {
								dst[i] = p_remap[copySrc[i]];
							}
						}
						width += clipX;
						dst += copyLen;
						zlines -= clipX;
					}
					src += count;
				}
				if (run == 0x80) goto row_done_qzbuff_remap;
			} while (clipX > 0);
			if (run != 0x80) {
				do {
					if (width < 1) break;
					run = *src++;
					if (width > 0) {
						if (run < 0x80) {
							dst += run;
							zlines += run;
							width -= run;
						}
						else if (run > 0x80) {
							int count = run & 0x7f;
							int actualLen = (count < width) ? count : width;
							for (int i = 0; i < actualLen; i++) {
								if (zlines[i] <= p_depth) {
									dst[i] = p_remap[src[i]];
								}
							}
							width = (count < width) ? width - count : 0;
							dst += actualLen;
							zlines += actualLen;
							src += count;
						}
					}
				} while (run != 0x80);
row_done_qzbuff_remap:
				while (run != 0x80) {
					run = *src++;
					if (run > 0x80) {
						src += run & 0x7f;
					}
				}
			}
			row++;
		} while (row < p_rect.m_height);
	}
}

// 68K 0x10114e22 BlitZRLEClipRemapR__8CSurfaceFRC7CVSRectRC7CVSRectP8CResZRLEUcPUc
// FUNCTION: LEMBALL 0x00477f50
void Surface::BlitZrleClipRemapR(const VsRect& p_rect,
								 const VsRect& p_clip,
								 ResZrle* p_zrle,
								 unsigned int p_reverse,
								 unsigned char* p_remap)
{
	int startX = p_rect.m_x + p_rect.m_width - 1;
	int y = p_rect.m_y;
	int step = 1;
	unsigned char* src = p_zrle->GetData();
	int zrleWidth = p_zrle->m_width;
	if (p_reverse != 0) {
		step = -1;
		y += p_rect.m_height - 1;
		int skipRows = (p_zrle->m_height - p_clip.m_y) - p_rect.m_height;
		if (skipRows > 0) {
			do {
				unsigned char run;
				do {
					run = *src++;
					if (run > 0x80) {
						src += run & 0x7f;
					}
				} while (run != 0x80);
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
					if (run > 0x80) {
						src += run & 0x7f;
					}
				} while (run != 0x80);
				skipRows--;
			} while (skipRows != 0);
		}
	}
	int row = 0;
	if (p_rect.m_height > 0) {
		do {
			int width = p_rect.m_width;
			int skipX = (zrleWidth - p_clip.m_x) - width;
			unsigned char* dst = (unsigned char*) m_lines[y] + startX;
			unsigned char run;
			do {
				run = *src++;
				if (skipX < 1) {
					if (width < 1) {
						if (run > 0x80) {
							src += run & 0x7f;
						}
					}
					else if (run < 0x80) {
						width -= run;
						dst -= run;
					}
					else if (run > 0x80) {
						run &= 0x7f;
						if (run < width) {
							for (int i = run; i != 0; i--) {
								*dst-- = p_remap[*src++];
							}
							width -= run;
						}
						else {
							int i = width;
							if (width > 0) {
								do {
									*dst-- = p_remap[*src++];
								} while (--i != 0);
							}
							src += run;
							dst -= width;
							width = 0;
						}
					}
				}
				else {
					if (run < 0x80) {
						skipX -= run;
						if (skipX < 0) {
							width += skipX;
							dst += skipX;
						}
					}
					else if (run > 0x80) {
						run &= 0x7f;
						skipX -= run;
						if (skipX < 0) {
							int copyLen = -skipX;
							int actualLen = (copyLen < width) ? copyLen : width;
							unsigned char* copySrc = src + run + skipX;
							for (int i = actualLen; i != 0; i--) {
								*dst-- = p_remap[*copySrc++];
							}
							width += skipX;
							dst += skipX;
						}
						src += run;
					}
				}
			} while (run != 0x80);
			y += step;
			row++;
		} while (row < p_rect.m_height);
	}
}

// 68K 0x10115124 BlitZRLENoClipRemap__8CSurfaceFRC7CVSRectP8CResZRLEUcPUc
// FUNCTION: LEMBALL 0x004781e0
void Surface::BlitZrleNoClipRemap(const VsRect& p_rect,
								  ResZrle* p_zrle,
								  unsigned int p_reverse,
								  unsigned char* p_remap)
{
	int x = p_rect.m_x;
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
			unsigned char* dst = (unsigned char*) m_lines[y] + x;
			unsigned char run;
			do {
				run = *src++;
				if (run < 0x80) {
					dst += run;
				}
				else if (run > 0x80) {
					run &= 0x7f;
					unsigned char* copySrc = src;
					unsigned char* copyDst = dst;
					for (int i = (int) run; i > 0; i--) {
						*copyDst++ = p_remap[*copySrc++];
					}
					src += run;
					dst += run;
				}
			} while (run != 0x80);
			y += step;
			row++;
		} while (row < p_rect.m_height);
	}
}

// 68K 0x10115230 BlitZRLENoClipRemapR__8CSurfaceFRC7CVSRectP8CResZRLEUcPUc
// FUNCTION: LEMBALL 0x004782d0
void Surface::BlitZrleNoClipRemapR(const VsRect& p_rect,
								   ResZrle* p_zrle,
								   unsigned int p_reverse,
								   unsigned char* p_remap)
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
					unsigned char* copySrc = src;
					unsigned char* copyDst = dst;
					for (int i = run; i != 0; i--) {
						*copyDst-- = p_remap[*copySrc++];
					}
					src += run;
					dst -= run;
				}
			} while (run != 0x80);
			y += step;
			row++;
		} while (row < p_rect.m_height);
	}
}

// GLOBAL: LEMBALL 0x004a2d50
char g_szClippingHeightTo[] = "Clipping height to ";

// GLOBAL: LEMBALL 0x004a2d64
char g_szClippingDotNewline[] = ".\n";

// GLOBAL: LEMBALL 0x004a2d68
char g_szClippingWidthTo[] = "Clipping width to ";

// GLOBAL: LEMBALL 0x004a2d7c
char g_szClippingHighNewline[] = " high.\n";

// GLOBAL: LEMBALL 0x004a2d84
char g_szClippingWideAnd[] = " wide and ";

// GLOBAL: LEMBALL 0x004a2d90
char g_szWarningZrleIs[] = "Warning: ZRLE is ";

// 68K 0x1011534a Blit__8CSurfaceFP5CZRLEP8CResZRLE
// FUNCTION: LEMBALL 0x004783c0
void Surface::Blit(Zrle* p_primitive, ResZrle* p_zrle)
{
	Zrle* primitive;
	unsigned int flags;

	primitive = p_primitive;
	flags = primitive->m_flags;
	if ((flags & 0xc0000) == 0) {
		BlitZrle((int) primitive->m_x,
				 (int) primitive->m_y,
				 p_zrle,
				 flags,
				 primitive->m_remap,
				 0);
		return;
	}
	{
		unsigned short stateDepth;
		Remap* remap;
		ResZrle* resource;
		int posX;
		int posY;

		stateDepth = (unsigned short) primitive->m_state;
		remap = primitive->m_remap;
		resource = p_zrle;
		posX = (int) primitive->m_x;
		posY = (int) primitive->m_y;
		if ((int) resource->m_height * (int) resource->m_width == 0) {
			return;
		}
		{
			VsRect dest;
			unsigned int reverse;
			unsigned char* remapData;

			dest.InitFromSizeAndPosition((short) posX, (short) posY, (VsSize*) &resource->m_width);
			if ((flags & 0x400) == 0) {
				((VsPoint*) &dest.m_x)->AddInPlace((VsPoint*) &resource->m_x);
			}
			{
				VsRect clipped;

				if ((short) dest.m_width > 0xff || (short) dest.m_height > 0xff) {
					*g_pDebugOutput << g_szWarningZrleIs << (int) (short) dest.m_width << g_szClippingWideAnd
									<< (int) (short) dest.m_height << g_szClippingHighNewline;
					if ((short) dest.m_width > 0xff) {
						*g_pDebugOutput << g_szClippingWidthTo << 0xff << g_szClippingDotNewline;
						dest.m_width = 0xff;
					}
					if ((short) dest.m_height > 0xff) {
						*g_pDebugOutput << g_szClippingHeightTo << 0xff << g_szClippingDotNewline;
						dest.m_height = 0xff;
					}
				}
				reverse = (unsigned char) ((flags & 2) >> 1);
				remapData = (unsigned char*) remap;
				if (ClipRect(dest, &clipped) == 0) {
					AddToChangeList(&dest);
					if ((flags & 0x40000) != 0) {
						if (remap == 0) {
							BlitZrleNoClipZBuff(dest, p_zrle, stateDepth);
							return;
						}
						BlitZrleNoClipZBuffRemap(dest, p_zrle, stateDepth, remapData);
						return;
					}
					if ((flags & 0x80000) != 0) {
						if (remap == 0) {
							BlitZrleNoClipQzBuff(dest, p_zrle, stateDepth);
							return;
						}
						BlitZrleNoClipQzBuffRemap(dest, p_zrle, stateDepth, remapData);
						return;
					}
					if (remap == 0) {
						if ((flags & 1) != 0) {
							BlitZrleNoClipR(dest, p_zrle, reverse);
							return;
						}
						BlitZrleNoClip(dest, p_zrle, reverse);
						return;
					}
					if ((flags & 1) != 0) {
						BlitZrleNoClipRemapR(dest, p_zrle, reverse, remapData);
						return;
					}
					BlitZrleNoClipRemap(dest, p_zrle, reverse, remapData);
					return;
				}
				if (clipped.m_width <= 0 || clipped.m_height <= 0) {
					return;
				}
				AddToChangeList(&dest);
				if ((flags & 0x40000) != 0) {
					if (remap == 0) {
						BlitZrleClipZBuff(dest, clipped, p_zrle, stateDepth);
						return;
					}
					BlitZrleClipZBuffRemap(dest, clipped, p_zrle, stateDepth, remapData);
					return;
				}
				if ((flags & 0x80000) != 0) {
					if (remap == 0) {
						BlitZrleClipQzBuff(dest, clipped, p_zrle, stateDepth);
						return;
					}
					BlitZrleClipQzBuffRemap(dest, clipped, p_zrle, stateDepth, remapData);
					return;
				}
				if (remap == 0) {
					if ((flags & 1) != 0) {
						BlitZrleClipR(dest, clipped, p_zrle, reverse);
						return;
					}
					BlitZrleClip(dest, clipped, p_zrle, reverse);
					return;
				}
				if ((flags & 1) != 0) {
					BlitZrleClipRemapR(dest, clipped, p_zrle, reverse, remapData);
					return;
				}
				BlitZrleClipRemap(dest, clipped, p_zrle, reverse, remapData);
			}
		}
	}
}

// 68K 0x10115868 Blit__8CSurfaceFP7CBitmapP10CResBITMAP
// FUNCTION: LEMBALL 0x004787f0
void Surface::Blit(Bitmap* p_primitive, ResBitmap* p_bitmap)
{
	VsRect dest;
	short width;
	short height;
	short sourceX;
	short sourceY;
	VsRect clip;

	dest.m_x = p_primitive->m_x;
	dest.m_y = p_primitive->m_y;
	width = p_primitive->m_width;
	height = p_primitive->m_height;
	sourceX = p_primitive->m_sourceX;
	sourceY = p_primitive->m_sourceY;
	if (height == 0 && width == 0) {
		width = p_bitmap->m_x;
		height = p_bitmap->m_y;
	}
	unsigned int flags = ((BitmapRes*) p_primitive)->m_flags;
	if ((int) p_bitmap->m_y * (int) p_bitmap->m_x != 0) {
		dest.m_width = width;
		dest.m_height = height;
		clip.m_height = 0;
		clip.m_width = 0;
		clip.m_x = 0;
		clip.m_y = 0;
		if (ClipRect(dest, &clip) != 0) {
			if (clip.m_width < 1 || clip.m_height < 1) {
				return;
			}
			dest = clip;
		}
		AddToChangeList(&dest);
		int destX = dest.m_x;
		int destY = dest.m_y;
		int yStep = 1;
		if ((flags & 2) != 0) {
			yStep = -1;
			destY += dest.m_height - 1;
		}
		int bitmapWidth = (int) p_bitmap->m_x;
		unsigned char* source = p_bitmap->GetData() + ((int) sourceY + (int) clip.m_y) * bitmapWidth +
								(int) sourceX + (int) clip.m_x;
		if ((flags & 0x800) == 0) {
			if (dest.m_height > 0) {
				for (int i = 0; i < dest.m_height; i++) {
					memcpy((unsigned char*) m_lines[destY] + destX, source, dest.m_width);
					destY += yStep;
					source += bitmapWidth;
				}
			}
		}
		else {
			if (dest.m_height > 0) {
				for (int i = 0; i < dest.m_height; i++) {
					unsigned char* dst = (unsigned char*) m_lines[destY] + destX;
					for (int j = 0; j < dest.m_width; j++) {
						if (source[j] != 0) {
							dst[j] = source[j];
						}
					}
					destY += yStep;
					source += bitmapWidth;
				}
			}
		}
	}
}

// 68K 0x101153f4 BlitZRLE__8CSurfaceFiiP8CResZRLEUlP6CRemapUs
// FUNCTION: LEMBALL 0x00478bb0
void Surface::BlitZrle(int p_x, int p_y, ResZrle* p_zrle, unsigned int p_flags, Remap* p_remap, unsigned short p_depth)
{
	unsigned char frameSpace[0x14];
	Surface* self;
	ResZrle* resource;
	unsigned int ebpFlags;
	short zWidth;
	short zHeight;
	VsRect* dest;
	VsRect* clipped;
	int area;

	self = this;
	resource = p_zrle;
	dest = (VsRect*) (frameSpace + 4);
	clipped = (VsRect*) (frameSpace + 12);
	zHeight = resource->m_height;
	zWidth = resource->m_width;
	area = (int) zWidth;
	area = area * (int) zHeight;
	if (area == 0) {
		return;
	}
	dest->m_width = zWidth;
	ebpFlags = p_flags;
	dest->m_height = zHeight;
	dest->m_x = (short) p_x;
	dest->m_y = (short) p_y;
	if ((ebpFlags & 0x400) == 0) {
		dest->m_x = (short) (dest->m_x + resource->m_x);
		dest->m_y = (short) (dest->m_y + resource->m_y);
	}
	clipped->m_height = 0;
	clipped->m_width = 0;
	clipped->m_y = 0;
	clipped->m_x = 0;
	if ((short) dest->m_width > 0xff || (short) dest->m_height > 0xff) {
		*g_pDebugOutput << g_szWarningZrleIs << (int) (short) dest->m_width << g_szClippingWideAnd
						<< (int) (short) dest->m_height << g_szClippingHighNewline;
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
		unsigned char* remapData;
		Remap* remap;
		unsigned short depth;

		remap = p_remap;
		depth = p_depth;
		remapData = (unsigned char*) remap;
		if (ClipRect(*dest, clipped) == 0) {
			AddToChangeList(dest);
			if ((ebpFlags & 0x40000) != 0) {
				if (remap == 0) {
					BlitZrleNoClipZBuff(*dest, resource, depth);
					return;
				}
				BlitZrleNoClipZBuffRemap(*dest, resource, depth, remapData);
				return;
			}
			if ((ebpFlags & 0x80000) != 0) {
				if (remap == 0) {
					BlitZrleNoClipQzBuff(*dest, resource, depth);
					return;
				}
				BlitZrleNoClipQzBuffRemap(*dest, resource, depth, remapData);
				return;
			}
			if (remap == 0) {
				if ((ebpFlags & 1) != 0) {
					BlitZrleNoClipR(*dest, resource, (ebpFlags & 2) >> 1);
					return;
				}
				BlitZrleNoClip(*dest, resource, (ebpFlags & 2) >> 1);
				return;
			}
			if ((ebpFlags & 1) != 0) {
				BlitZrleNoClipRemapR(*dest, resource, (ebpFlags & 2) >> 1, remapData);
				return;
			}
			BlitZrleNoClipRemap(*dest, resource, (ebpFlags & 2) >> 1, remapData);
			return;
		}
		if (clipped->m_width <= 0 || clipped->m_height <= 0) {
			return;
		}
		AddToChangeList(dest);
		if ((ebpFlags & 0x40000) != 0) {
			if (remap == 0) {
				BlitZrleClipZBuff(*dest, *clipped, resource, depth);
				return;
			}
			BlitZrleClipZBuffRemap(*dest, *clipped, resource, depth, remapData);
			return;
		}
		if ((ebpFlags & 0x80000) != 0) {
			if (remap == 0) {
				BlitZrleClipQzBuff(*dest, *clipped, resource, depth);
				return;
			}
			BlitZrleClipQzBuffRemap(*dest, *clipped, resource, depth, remapData);
			return;
		}
		if (remap == 0) {
			if ((ebpFlags & 1) != 0) {
				BlitZrleClipR(*dest, *clipped, resource, (ebpFlags & 2) >> 1);
				return;
			}
			BlitZrleClip(*dest, *clipped, resource, (ebpFlags & 2) >> 1);
			return;
		}
		if ((ebpFlags & 1) != 0) {
			BlitZrleClipRemapR(*dest, *clipped, resource, (ebpFlags & 2) >> 1, remapData);
			return;
		}
		BlitZrleClipRemap(*dest, *clipped, resource, (ebpFlags & 2) >> 1, remapData);
	}
}
