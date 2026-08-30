#include "GdiDevice.h"
#include "VsGdi.h"

#include "../Foundation/ChangeList.h"
#include "../Foundation/VsDebug.h"
#include "../Foundation/VsPoint.h"
#include "../Resources/ResBitmap.h"
#include "../Resources/ResPalette.h"
#include "../Resources/ResZrle.h"
#include "../Target/TargetDibContext.h"
#include "../Target/TargetDrawingContext.h"
#include "../Target/TargetGraphicsDriver.h"
#include "Bitmap.h"
#include "BitmapRes.h"
#include "ClipRect.h"
#include "CopyColourToBackBuff.h"
#include "CopyToBackBuff.h"
#include "GdiDevice.h"
#include "Line.h"
#include "Point.h"
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
// STUB: LEMBALL 0x0046c5d0
Surface::Surface(GrafPort* p_arg0)
{
	void* storage;
	TargetDrawingContext* context;

	m_platformBitmap = 0;
	m_changeList = 0;
	m_presentX = 0;
	m_presentY = 0;
	m_unk0x528 = 0;
	m_unk0x52c = 0;
	m_unk0x530 = 0;
	m_unk0x54c = 1;
	m_parentSurface = 0;
	m_flag70 = 0;
	m_flag74 = 0;
	m_flag78 = 0;
	storage = operator new(0xc);
	if (storage == 0) {
		m_drawingPort = 0;
	}
	else {
		context = new (storage) TargetDrawingContext();
		context->m_hDC = p_arg0;
		context->m_hBitmap = 0;
		m_drawingPort = context;
	}
	InitializeCriticalSection((CRITICAL_SECTION*) m_lock);
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
// STUB: LEMBALL 0x0046c990
void Surface::ResetScroll()
{
}

// 68K 0x10108c8a SetLinePtrs__8CSurfaceFv
// STUB: LEMBALL 0x0046c9f0
void Surface::SetLinePtrs()
{
	Surface* parent;
	int y;
	int originX;
	int originY;

	parent = m_parentSurface;
	if (parent == (Surface*) g_pGdiHelperTarget) {
		PvGdiBitmap::SetLinePtrs();
		return;
	}
	if (parent == 0) {
		PvGdiBitmap::SetLinePtrs();
		return;
	}
	m_stride = parent->m_stride;
	originX = (int) m_clipRect.m_x;
	originY = (int) m_clipRect.m_y;
	m_bitsBase = parent->m_bitsBase + originY * m_stride + originX;
	m_bits = m_bitsBase;
	m_xOffset = 0;
	m_firstLine = 0;
	if (m_lines == 0 || parent->m_lines == 0) {
		return;
	}
	y = 0;
	while (y < m_height) {
		m_lines[y] = (unsigned char*) parent->m_lines[originY + y] + originX;
		y = y + 1;
	}
}

// 68K 0x10109466 AddToChangeList__8CSurfaceFRC7CVSRect
// STUB: LEMBALL 0x0046cb20
void Surface::AddToChangeList(const VsRect& p_rect)
{
	Surface* parent;
	VsRect translated;
	ChangeList* list;

	parent = m_parentSurface;
	if (parent != (Surface*) g_pGdiHelperTarget && m_flag74 != 0 && m_flag70 != 0) {
		translated.m_width = p_rect.m_width;
		translated.m_height = p_rect.m_height;
		translated.m_x = (short) (p_rect.m_x + m_clipRect.m_x);
		translated.m_y = (short) (p_rect.m_y + m_clipRect.m_y);
		parent->AddToChangeList(translated);
		return;
	}
	list = GetChangeList();
	if (list != 0) {
		list->Add(p_rect);
	}
}

// 68K 0x1010956c GetChangeList__8CSurfaceFv
// STUB: LEMBALL 0x0046cbd0
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
// STUB: LEMBALL 0x0046cda0
void Surface::ToScreen(class Surface* p_destinationSurface)
{
	Surface* parent;
	ChangeList* list;
	int itemCount;
	int index;
	ChangeListItem* item;
	VsRect destRect;
	VsRect sourceRect;
	TargetDrawingContext* destContext;
	short zoom;

	parent = m_parentSurface;
	if (parent != (Surface*) g_pGdiHelperTarget) {
		if (m_flag74 != 0) {
			return;
		}
		if (m_flag78 != 0) {
			parent->AddToChangeList(m_clipRect);
			m_flag78 = 0;
			return;
		}
		if (m_flag70 == 0) {
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
		parent->AddToChangeList(m_clipRect);
		return;
	}

	if (m_platformBitmap == 0) {
		return;
	}
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
															   (TargetDrawingContext*) m_drawingPort,
															   &sourceRect,
															   this);
				}
				else {
					destRect.m_width = (short) (destRect.m_width * zoom);
					destRect.m_height = (short) (destRect.m_height * zoom);
					destRect.m_x = (short) (destRect.m_x * zoom);
					destRect.m_y = (short) (destRect.m_y * zoom);
					g_pTargetGraphicsDriver->BlitWrappedBitmap(destContext,
															   &destRect,
															   (TargetDrawingContext*) m_drawingPort,
															   &sourceRect,
															   this);
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
	AddToChangeList(clip);
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
// STUB: LEMBALL 0x0046d560
void Surface::MoveRel(const VsPoint& p_delta)
{
}

// 68K 0x1010a166 Move__8CSurfaceFRC8CVSPoint
// STUB: LEMBALL 0x0046d5b0
void Surface::Move(const VsPoint& p_position)
{
}

// 68K 0x1010858c SetWindowPtr__8CSurfaceFP8GrafPort
// STUB: LEMBALL 0x0046d7e0
void Surface::SetWindowPtr(void* p_platformPort)
{
	TargetDrawingContext* context;

	context = (TargetDrawingContext*) m_drawingPort;
	context->SetDC(p_platformPort);
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
	AddToChangeList(rect);
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
	Blit((Bitmap*) p_arg0, p_arg1);
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
// STUB: LEMBALL 0x00474c20
void Surface::Blit(ScreenScroll* p_scroll)
{
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
	if (startY <= 0) {
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
	int width = p_arg0->m_field08;
	int height = p_arg0->m_field0a;

	if (width == 0 || height == 0) {
		return;
	}
	int destX = p_arg0->m_field0c;
	int srcX = p_arg0->m_field04;
	int destY = p_arg0->m_field0e;
	int srcY = p_arg0->m_field06;
	do {
		unsigned char* dest = (unsigned char*) PvBackBuffSurface::m_bitmap.m_lines[destY] + destX;
		unsigned char* src = (unsigned char*) m_lines[srcY] + srcX;
		memcpy(dest, src, width);
		srcY++;
		destY++;
		height--;
	} while (height != 0);
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

	if ((int) height * (int) width == 0) {
		return;
	}
	const short* coords = (&p_arg0 != 0) ? &p_arg0.m_x : 0;
	short x = coords[0];
	short y = coords[1];
	if ((int) (short) (x + width) > (int) PvBackBuffSurface::m_allocatedWidth) {
		width = (short) (PvBackBuffSurface::m_allocatedWidth - x);
	}
	if ((int) (short) (height + y) > (int) PvBackBuffSurface::m_allocatedHeight) {
		height = (short) (PvBackBuffSurface::m_allocatedHeight - y);
	}
	int startX = x;
	int startY = y;
	int row = 0;
	if (height <= 0) {
		return;
	}
	do {
		unsigned char* dest = (unsigned char*) m_lines[startY] + startX;
		unsigned char* src = (unsigned char*) PvBackBuffSurface::m_bitmap.m_lines[startY] + startX;
		memcpy(dest, src, width);
		startY++;
		row++;
	} while (height > row);
}

// 68K 0x10111d36 Blit__8CSurfaceFP6CPoint
// STUB: LEMBALL 0x00474fd0
void Surface::Blit(Point* p_point)
{
	short x;
	short y;
	unsigned char color;
	VsRect area;
	short clipX;
	short clipY;

	if (p_point == 0 || m_lines == 0) {
		return;
	}
	x = p_point->m_x;
	y = p_point->m_y;
	clipX = m_clipRect.m_x;
	clipY = m_clipRect.m_y;
	if (clipX <= x && x < (short) (clipX + m_clipRect.m_width) && clipY <= y &&
		y < (short) (clipY + m_clipRect.m_height)) {
		color = (unsigned char) p_point->m_color;
		*((unsigned char*) m_lines[y] + x) = color;
		area.m_width = 1;
		area.m_height = 1;
		area.m_x = x;
		area.m_y = y;
		AddToChangeList(area);
	}
}

// 68K 0x10111e5a Blit__8CSurfaceFP10CSolidRect
// STUB: LEMBALL 0x00475080
void Surface::Blit(SolidRect* p_rect)
{
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
	AddToChangeList(bounds);
}

// 68K 0x10112604 Blit__8CSurfaceFP7CCircle
// STUB: LEMBALL 0x00475290
void Surface::Blit(Circle* p_circle)
{
}

// 68K 0x101132f4 Blit__8CSurfaceFP13CFilledCircle
// STUB: LEMBALL 0x00475490
void Surface::Blit(FilledCircle* p_circle)
{
}

// 68K 0x10111ee8 BlitRect__8CSurfaceF7CVSRecti
// STUB: LEMBALL 0x004756e0
void Surface::BlitRect(VsRect p_rect, int p_colour)
{
}

// 68K 0x101121bc LineClip__8CSurfaceFRiRiRiRi
// FUNCTION: LEMBALL 0x004757a0
int Surface::LineClip(int& p_x1, int& p_y1, int& p_x2, int& p_y2)
{
	int left;
	int top;
	int right;
	int bottom;
	unsigned int code1;
	unsigned int code2;
	int dx;
	int dy;

	if (m_clipRect.m_height < 1 || m_clipRect.m_width < 1) {
		return 1;
	}
	left = m_clipRect.m_x;
	top = m_clipRect.m_y;
	right = m_clipRect.m_x + m_clipRect.m_width - 1;
	bottom = m_clipRect.m_y + m_clipRect.m_height - 1;
	code1 = 0;
	if (p_x1 < left) {
		code1 = 1;
	}
	else if (right < p_x1) {
		code1 = 2;
	}
	if (p_y1 < top) {
		code1 = code1 | 4;
	}
	else if (bottom < p_y1) {
		code1 = code1 | 8;
	}
	code2 = 0;
	if (p_x2 < left) {
		code2 = 1;
	}
	else if (right < p_x2) {
		code2 = 2;
	}
	if (p_y2 < top) {
		code2 = code2 | 4;
	}
	else if (bottom < p_y2) {
		code2 = code2 | 8;
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
							p_x2 = p_x2 + ((top - p_y2) * dx) / dy;
							p_y2 = top;
						}
						else if ((code2 & 8) != 0) {
							p_x2 = p_x2 + ((bottom - p_y2) * dx) / dy;
							p_y2 = bottom;
						}
					}
					else {
						p_y2 = p_y2 + ((right - p_x2) * dy) / dx;
						p_x2 = right;
					}
				}
				else {
					p_y2 = p_y2 + ((left - p_x2) * dy) / dx;
					p_x2 = left;
				}
				code2 = 0;
				if (p_x2 < left) {
					code2 = 1;
				}
				else if (right < p_x2) {
					code2 = 2;
				}
				if (p_y2 < top) {
					code2 = code2 | 4;
				}
				else if (bottom < p_y2) {
					code2 = code2 | 8;
				}
			}
			else {
				if ((code1 & 1) == 0) {
					if ((code1 & 2) == 0) {
						if ((code1 & 4) != 0) {
							p_x1 = p_x1 + ((top - p_y1) * dx) / dy;
							p_y1 = top;
						}
						else if ((code1 & 8) != 0) {
							p_x1 = p_x1 + ((bottom - p_y1) * dx) / dy;
							p_y1 = bottom;
						}
					}
					else {
						p_y1 = ((right - p_x1) * dy) / dx + p_y1;
						p_x1 = right;
					}
				}
				else {
					p_y1 = p_y1 + ((left - p_x1) * dy) / dx;
					p_x1 = left;
				}
				code1 = 0;
				if (p_x1 < left) {
					code1 = 1;
				}
				else if (right < p_x1) {
					code1 = 2;
				}
				if (p_y1 < top) {
					code1 = code1 | 4;
				}
				else if (bottom < p_y1) {
					code1 = code1 | 8;
				}
			}
		} while (code2 != 0 || code1 != 0);
	}
	return 0;
}

// 68K 0x10113178 ClipCircle__8CSurfaceFiii
// STUB: LEMBALL 0x00475bc0
int Surface::ClipCircle(int p_centerX, int p_centerY, int p_radius)
{
	return 0;
}

// 68K 0x1011355a DrawClippedFilledCircle__8CSurfaceFiiii
// STUB: LEMBALL 0x00476190
void Surface::DrawClippedFilledCircle(int p_centerX, int p_centerY, int p_radius, int p_colour)
{
}

// 68K 0x1011387e FilledCircleClipPoints__8CSurfaceFiiiii
// STUB: LEMBALL 0x00476470
void Surface::FilledCircleClipPoints(int p_centerX, int p_centerY, int p_xOffset, int p_yOffset, int p_colour)
{
}

// 68K 0x10113bf4 ClipRect__8CSurfaceFR7CVSRectP7CVSRect
// STUB: LEMBALL 0x00476580
bool Surface::ClipRect(VsRect& p_rect, VsRect* p_clipped)
{
	short viewX;
	short viewY;
	short viewW;
	short viewH;
	short clipped;

	viewX = 0;
	viewY = 0;
	viewW = m_width;
	viewH = m_height;
	clipped = 0;
	if ((short) (viewW + viewX) < p_rect.m_x || (short) (viewH + viewY) < p_rect.m_y ||
		(short) (p_rect.m_width + p_rect.m_x) < viewX ||
		(short) (p_rect.m_height + p_rect.m_y) < viewY) {
		return 1;
	}
	if (p_rect.m_x < viewX) {
		p_clipped->m_x = (short) (viewX - p_rect.m_x);
		p_rect.m_x = viewX;
		p_rect.m_width = (short) (p_rect.m_width - p_clipped->m_x);
		clipped = 1;
	}
	if (p_rect.m_y < viewY) {
		p_clipped->m_y = (short) (viewY - p_rect.m_y);
		p_rect.m_y = viewY;
		p_rect.m_height = (short) (p_rect.m_height - p_clipped->m_y);
		clipped = 1;
	}
	if ((short) (viewX + viewW) < (short) (p_rect.m_x + p_rect.m_width)) {
		p_clipped->m_width = (short) ((viewX + viewW) - p_rect.m_x);
		p_rect.m_width = (short) ((viewX - p_rect.m_x) + viewW);
		clipped = 1;
	}
	else {
		p_clipped->m_width = p_rect.m_width;
	}
	if ((short) (viewY + viewH) < (short) (p_rect.m_y + p_rect.m_height)) {
		p_clipped->m_height = (short) ((viewY + viewH) - p_rect.m_y);
		p_rect.m_height = (short) ((viewY - p_rect.m_y) + viewH);
		return 1;
	}
	p_clipped->m_height = p_rect.m_height;
	return clipped != 0;
}

// 68K 0x1010519e BlitZRLEClip__8CSurfaceFRC7CVSRectRC7CVSRectP8CResZRLEUc
// STUB: LEMBALL 0x004766f0
void Surface::BlitZrleClip(const VsRect& p_rect, const VsRect& p_clip, ResZrle* p_zrle, unsigned char p_reverse)
{
}

// 68K 0x1010539e BlitZRLEClipZBuff__8CSurfaceFRC7CVSRectRC7CVSRectP8CResZRLEUs
// STUB: LEMBALL 0x00476910
void Surface::BlitZrleClipZBuff(const VsRect& p_rect, const VsRect& p_clip, ResZrle* p_zrle, unsigned short p_depth)
{
}

// 68K 0x10105606 BlitZRLEClipQZBuff__8CSurfaceFRC7CVSRectRC7CVSRectP8CResZRLEUs
// STUB: LEMBALL 0x00476bf0
void Surface::BlitZrleClipQzBuff(const VsRect& p_rect, const VsRect& p_clip, ResZrle* p_zrle, unsigned short p_depth)
{
}

// 68K 0x10113e7a BlitZRLEClipR__8CSurfaceFRC7CVSRectRC7CVSRectP8CResZRLEUc
// STUB: LEMBALL 0x00476ee0
void Surface::BlitZrleClipR(const VsRect& p_rect, const VsRect& p_clip, ResZrle* p_zrle, unsigned char p_reverse)
{
}

// 68K 0x10104cd8 BlitZRLENoClip__8CSurfaceFRC7CVSRectP8CResZRLEUc
// STUB: LEMBALL 0x00477130
void Surface::BlitZrleNoClip(const VsRect& p_rect, ResZrle* p_zrle, unsigned char p_reverse)
{
	unsigned char* source;
	unsigned char* dest;
	unsigned char run;
	int y;
	int step;
	int row;
	int count;
	int i;

	if (p_zrle == 0 || m_bits == 0) {
		return;
	}
	source = p_zrle->GetData();
	if (source == 0) {
		return;
	}
	step = 1;
	y = p_rect.m_y;
	if (p_reverse != 0) {
		step = -1;
		y = p_rect.m_y + p_rect.m_height - 1;
	}
	row = 0;
	while (row < p_rect.m_height) {
		if (m_lines != 0) {
			dest = (unsigned char*) m_lines[y] + p_rect.m_x;
		}
		else {
			dest = m_bits + y * m_stride + p_rect.m_x;
		}
		do {
			run = *source;
			source = source + 1;
			if (run < 0x80) {
				dest = dest + run;
			}
			else if (run > 0x80) {
				count = run & 0x7f;
				i = 0;
				while (i < count) {
					dest[i] = source[i];
					i = i + 1;
				}
				dest = dest + count;
				source = source + count;
			}
		} while (run != 0x80);
		y = y + step;
		row = row + 1;
	}
}

// 68K 0x10104e4c BlitZRLENoClipZBuff__8CSurfaceFRC7CVSRectP8CResZRLEUs
// STUB: LEMBALL 0x00477200
void Surface::BlitZrleNoClipZBuff(const VsRect& p_rect, ResZrle* p_zrle, unsigned short p_depth)
{
}

// 68K 0x10114162 BlitZRLENoClipZBuffRemap__8CSurfaceFRC7CVSRectP8CResZRLEUsPUc
// STUB: LEMBALL 0x00477310
void Surface::BlitZrleNoClipZBuffRemap(const VsRect& p_rect,
									   ResZrle* p_zrle,
									   unsigned short p_depth,
									   unsigned char* p_remap)
{
}

// 68K 0x10104fa6 BlitZRLENoClipQZBuff__8CSurfaceFRC7CVSRectP8CResZRLEUs
// STUB: LEMBALL 0x00477440
void Surface::BlitZrleNoClipQzBuff(const VsRect& p_rect, ResZrle* p_zrle, unsigned short p_depth)
{
}

// 68K 0x101142ac BlitZRLENoClipQZBuffRemap__8CSurfaceFRC7CVSRectP8CResZRLEUsPUc
// STUB: LEMBALL 0x00477540
void Surface::BlitZrleNoClipQzBuffRemap(const VsRect& p_rect,
										ResZrle* p_zrle,
										unsigned short p_depth,
										unsigned char* p_remap)
{
}

// 68K 0x101143ee BlitZRLENoClipR__8CSurfaceFRC7CVSRectP8CResZRLEUc
// STUB: LEMBALL 0x00477660
void Surface::BlitZrleNoClipR(const VsRect& p_rect, ResZrle* p_zrle, unsigned char p_reverse)
{
}

// 68K 0x101144ee BlitZRLEClipRemap__8CSurfaceFRC7CVSRectRC7CVSRectP8CResZRLEUcPUc
// STUB: LEMBALL 0x00477740
void Surface::BlitZrleClipRemap(const VsRect& p_rect,
								const VsRect& p_clip,
								ResZrle* p_zrle,
								unsigned char p_reverse,
								unsigned char* p_remap)
{
}

// 68K 0x101147c6 BlitZRLEClipZBuffRemap__8CSurfaceFRC7CVSRectRC7CVSRectP8CResZRLEUsPUc
// STUB: LEMBALL 0x004779d0
void Surface::BlitZrleClipZBuffRemap(const VsRect& p_rect,
									 const VsRect& p_clip,
									 ResZrle* p_zrle,
									 unsigned short p_depth,
									 unsigned char* p_remap)
{
}

// 68K 0x10114ab8 BlitZRLEClipQZBuffRemap__8CSurfaceFRC7CVSRectRC7CVSRectP8CResZRLEUsPUc
// STUB: LEMBALL 0x00477c60
void Surface::BlitZrleClipQzBuffRemap(const VsRect& p_rect,
									  const VsRect& p_clip,
									  ResZrle* p_zrle,
									  unsigned short p_depth,
									  unsigned char* p_remap)
{
}

// 68K 0x10114e22 BlitZRLEClipRemapR__8CSurfaceFRC7CVSRectRC7CVSRectP8CResZRLEUcPUc
// STUB: LEMBALL 0x00477f50
void Surface::BlitZrleClipRemapR(const VsRect& p_rect,
								 const VsRect& p_clip,
								 ResZrle* p_zrle,
								 unsigned char p_reverse,
								 unsigned char* p_remap)
{
}

// 68K 0x10115124 BlitZRLENoClipRemap__8CSurfaceFRC7CVSRectP8CResZRLEUcPUc
// STUB: LEMBALL 0x004781e0
void Surface::BlitZrleNoClipRemap(const VsRect& p_rect,
								  ResZrle* p_zrle,
								  unsigned char p_reverse,
								  unsigned char* p_remap)
{
}

// 68K 0x10115230 BlitZRLENoClipRemapR__8CSurfaceFRC7CVSRectP8CResZRLEUcPUc
// STUB: LEMBALL 0x004782d0
void Surface::BlitZrleNoClipRemapR(const VsRect& p_rect,
								   ResZrle* p_zrle,
								   unsigned char p_reverse,
								   unsigned char* p_remap)
{
}

// 68K 0x1011534a Blit__8CSurfaceFP5CZRLEP8CResZRLE
// STUB: LEMBALL 0x004783c0
void Surface::Blit(Zrle* p_primitive, ResZrle* p_zrle)
{
	if (p_primitive == 0 || p_zrle == 0) {
		return;
	}
	BlitZrle(p_primitive->m_x, p_primitive->m_y, p_zrle, p_primitive->m_flags, p_primitive->m_remap, 0);
}

// 68K 0x10115868 Blit__8CSurfaceFP7CBitmapP10CResBITMAP
// FUNCTION: LEMBALL 0x004787f0
void Surface::Blit(Bitmap* p_primitive, ResBitmap* p_bitmap)
{
	VsRect dest;
	VsRect clip;
	unsigned int flags;
	unsigned char* source;
	unsigned char* destBits;
	int row;
	int destY;
	int yStep;
	int bitmapWidth;
	int pixel;
	int copyWidth;

	dest.m_x = p_primitive->m_x;
	dest.m_y = p_primitive->m_y;
	dest.m_width = p_primitive->m_width;
	dest.m_height = p_primitive->m_height;
	if (dest.m_height == 0 && dest.m_width == 0) {
		dest.m_width = p_bitmap->m_x;
		dest.m_height = p_bitmap->m_y;
	}
	flags = ((BitmapRes*) p_primitive)->m_flags;
	if ((int) p_bitmap->m_y * (int) p_bitmap->m_x == 0) {
		return;
	}
	clip.m_width = 0;
	clip.m_height = 0;
	clip.m_x = 0;
	clip.m_y = 0;
	if (ClipRect(dest, &clip) != 0) {
		if (clip.m_width < 1) {
			return;
		}
		if (clip.m_height < 1) {
			return;
		}
		dest.m_width = clip.m_width;
		dest.m_height = clip.m_height;
	}
	AddToChangeList(dest);
	yStep = 1;
	destY = dest.m_y;
	if ((flags & 2) != 0) {
		yStep = -1;
		destY = destY + dest.m_height - 1;
	}
	bitmapWidth = (int) p_bitmap->m_x;
	source = p_bitmap->GetData();
	source = source + ((int) p_primitive->m_sourceY + (int) clip.m_y) * bitmapWidth +
			 (int) p_primitive->m_sourceX + (int) clip.m_x;
	if ((flags & 0x800) != 0) {
		row = 0;
		if (0 < dest.m_height) {
			do {
				destBits = (unsigned char*) m_lines[destY] + dest.m_x;
				pixel = 0;
				if (0 < dest.m_width) {
					do {
						if (*source != 0) {
							*destBits = *source;
						}
						pixel = pixel + 1;
						destBits = destBits + 1;
						source = source + 1;
					} while (pixel < dest.m_width);
				}
				destY = destY + yStep;
				source = source + (bitmapWidth - dest.m_width);
				row = row + 1;
			} while (row < dest.m_height);
		}
		return;
	}
	row = 0;
	if (0 < dest.m_height) {
		do {
			copyWidth = dest.m_width;
			destBits = (unsigned char*) m_lines[destY] + dest.m_x;
			memcpy(destBits, source, (unsigned int) copyWidth);
			destY = destY + yStep;
			source = source + bitmapWidth;
			row = row + 1;
		} while (row < dest.m_height);
	}
}

// 68K 0x101153f4 BlitZRLE__8CSurfaceFiiP8CResZRLEUlP6CRemapUs
// FUNCTION: LEMBALL 0x00478bb0
void Surface::BlitZrle(int p_x, int p_y, ResZrle* p_zrle, unsigned int p_flags, Remap* p_remap, unsigned short p_depth)
{
	VsRect dest;
	VsRect clipped;
	unsigned char reverse;

	if (p_zrle == 0) {
		return;
	}
	dest.m_width = p_zrle->m_width;
	dest.m_height = p_zrle->m_height;
	if ((int) dest.m_height * (int) dest.m_width == 0) {
		return;
	}
	dest.m_x = (short) p_x;
	dest.m_y = (short) p_y;
	if ((p_flags & 0x400) == 0) {
		dest.m_x = (short) (dest.m_x + p_zrle->m_x);
		dest.m_y = (short) (dest.m_y + p_zrle->m_y);
	}
	clipped.m_width = 0;
	clipped.m_height = 0;
	clipped.m_x = 0;
	clipped.m_y = 0;
	reverse = (unsigned char) ((p_flags & 2) >> 1);
	if (ClipRect(dest, &clipped) == 0) {
		AddToChangeList(dest);
		if ((p_flags & 0x40000) == 0 && (p_flags & 0x80000) == 0) {
			if (p_remap == 0) {
				BlitZrleNoClip(dest, p_zrle, reverse);
				return;
			}
			if ((p_flags & 1) == 0) {
				BlitZrleNoClipRemap(dest, p_zrle, reverse, (unsigned char*) p_remap);
				return;
			}
			BlitZrleNoClipRemapR(dest, p_zrle, reverse, (unsigned char*) p_remap);
			return;
		}
		return;
	}
	if (clipped.m_width < 1 || clipped.m_height < 1) {
		return;
	}
	AddToChangeList(dest);
	if ((p_flags & 0x40000) == 0 && (p_flags & 0x80000) == 0) {
		if (p_remap == 0) {
			BlitZrleClip(dest, clipped, p_zrle, reverse);
			return;
		}
		if ((p_flags & 1) != 0) {
			BlitZrleClipRemapR(dest, clipped, p_zrle, reverse, (unsigned char*) p_remap);
			return;
		}
		BlitZrleClipRemap(dest, clipped, p_zrle, reverse, (unsigned char*) p_remap);
	}
}

