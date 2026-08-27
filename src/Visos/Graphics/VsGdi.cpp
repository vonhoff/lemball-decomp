#include "VsGdi.h"

#include "../Foundation/ChangeList.h"
#include "../Foundation/VsPoint.h"
#include "../Resources/ResBitmap.h"
#include "../Resources/ResPalette.h"
#include "../Resources/ResZrle.h"
#include "../Target/TargetDibContext.h"
#include "../Target/TargetDrawingContext.h"
#include "../Target/TargetGraphicsDriver.h"
#include "Bitmap.h"
#include "ClipRect.h"
#include "CopyToBackBuff.h"
#include "GdiDevice.h"
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

struct SurfaceListNode {
	Surface* m_surface;
	SurfaceListNode* m_next;
	SurfaceListNode* m_prev;
};

// GLOBAL: LEMBALL 0x004a2010
static SurfaceListHead* g_pSurfaceList = 0;

// 68K 0x1010830c __ct__8CSurfaceFRC7CVSRectP8CSurface
// STUB: LEMBALL 0x0046c050
Surface::Surface(const VsRect& p_arg0, class Surface* p_arg1)
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
	m_parentSurface = p_arg1;
	m_flag70 = 1;
	m_flag74 = 0;
	m_flag78 = 0;
	InitializeCriticalSection((CRITICAL_SECTION*) m_lock);
	cellSize.m_width = 8;
	cellSize.m_height = 8;
	viewSize.m_width = p_arg0.m_width;
	viewSize.m_height = p_arg0.m_height;
	if (p_arg1 == (Surface*) g_pGdiHelperTarget) {
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
	if (p_arg1 == (Surface*) g_pGdiHelperTarget && g_pTargetGraphicsDriver != 0) {
		context = g_pTargetGraphicsDriver->CreateDrawingContext();
		m_drawingPort = context;
	}
	else {
		storage = operator new(0xc);
		if (storage == 0) {
			m_drawingPort = 0;
		}
		else {
			context = new (storage) TargetDrawingContext();
			context->m_hDC = 0;
			context->m_hBitmap = 0;
			m_drawingPort = context;
		}
	}
	NewBitmap(p_arg0);
}

// STUB: LEMBALL 0x0046c380
void TargetBuildSurfaceColourTable(unsigned int* p_entries,
								   ResPalette* p_palette,
								   void* p_unused,
								   unsigned int* p_fallbackEntries)
{
	RGBQUAD* table;
	PALETTEENTRY systemEntries[256];
	unsigned char* data;
	HDC hdc;
	int i;
	int copied;

	table = (RGBQUAD*) p_entries;
	hdc = GetDC(0);
	copied = 0;
	if (hdc != 0) {
		copied = GetSystemPaletteEntries(hdc, 0, 256, systemEntries);
		ReleaseDC(0, hdc);
	}
	i = 0;
	while (i < 256) {
		if (copied != 0) {
			table[i].rgbBlue = systemEntries[i].peBlue;
			table[i].rgbGreen = systemEntries[i].peGreen;
			table[i].rgbRed = systemEntries[i].peRed;
		}
		else {
			table[i].rgbBlue = (unsigned char) i;
			table[i].rgbGreen = (unsigned char) i;
			table[i].rgbRed = (unsigned char) i;
		}
		table[i].rgbReserved = 0;
		i = i + 1;
	}
	if (p_palette != 0) {
		data = p_palette->GetData();
		if (data != 0) {
			i = 10;
			while (i < 246) {
				table[i].rgbRed = data[0x30 + i * 4];
				table[i].rgbGreen = data[0x30 + i * 4 + 1];
				table[i].rgbBlue = data[0x30 + i * 4 + 2];
				table[i].rgbReserved = 0;
				i = i + 1;
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
// STUB: LEMBALL 0x0046c710
Surface::~Surface()
{
	if (m_platformBitmap == 0 && m_bits != 0) {
		operator delete(m_bits);
		m_bits = 0;
		m_bitsBase = 0;
	}
	m_platformBitmap = 0;
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
// STUB: LEMBALL 0x0046cbe0
void Surface::Blit(class ClipRect* p_arg0)
{
	VsRect* clip;
	Surface* parent;
	VsRect* parentClip;
	short clipRight;
	short clipBottom;

	clip = &m_clipRect;
	if ((p_arg0->m_reserved0c & 0x1000) == 0) {
		clip->m_width = p_arg0->m_left;
		clip->m_height = p_arg0->m_top;
		clip->m_x = p_arg0->m_right;
		clip->m_y = p_arg0->m_bottom;
	}
	else if ((int) p_arg0->m_left * (int) p_arg0->m_top != 0) {
		if (p_arg0->m_right < clip->m_x) {
			clip->m_width = (short) (clip->m_width + (clip->m_x - p_arg0->m_right));
			clip->m_x = p_arg0->m_right;
		}
		if ((short) (clip->m_width + clip->m_x) < (short) (p_arg0->m_right + p_arg0->m_left)) {
			clip->m_width = (short) ((p_arg0->m_left - clip->m_x) + p_arg0->m_right);
		}
		if (p_arg0->m_bottom < clip->m_y) {
			clip->m_height = (short) (clip->m_height + (clip->m_y - p_arg0->m_bottom));
			clip->m_y = p_arg0->m_bottom;
		}
		if ((short) (clip->m_height + clip->m_y) < (short) (p_arg0->m_bottom + p_arg0->m_top)) {
			clip->m_height = (short) ((p_arg0->m_top - clip->m_y) + p_arg0->m_bottom);
		}
	}
	parent = m_parentSurface;
	if (parent != (Surface*) g_pGdiHelperTarget && (p_arg0->m_reserved0c & 0x10000) == 0 && parent != 0) {
		parentClip = &parent->m_clipRect;
		clipRight = clip->m_x;
		if (clipRight < parentClip->m_x) {
			clip->m_width = (short) (clip->m_width + (clipRight - parentClip->m_x));
			clip->m_x = parentClip->m_x;
		}
		clipRight = clip->m_x;
		if ((short) (parentClip->m_width + parentClip->m_x) < (short) (clip->m_width + clipRight)) {
			clip->m_width = (short) ((parentClip->m_x - clipRight) + parentClip->m_width);
		}
		clipBottom = clip->m_y;
		if (clipBottom < parentClip->m_y) {
			clip->m_height = (short) (clip->m_height + (clipBottom - parentClip->m_y));
			clip->m_y = parentClip->m_y;
		}
		clipBottom = clip->m_y;
		if ((short) (parentClip->m_y + parentClip->m_height) < (short) (clip->m_height + clipBottom)) {
			clip->m_height = (short) ((parentClip->m_height - clipBottom) + parentClip->m_y);
		}
		if (clip->m_width < 1 || clip->m_height < 1) {
			clip->m_height = 0;
			clip->m_width = 0;
			clip->m_y = 0;
			clip->m_x = 0;
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
			destRect.m_x = (short) (destRect.m_x + m_clipRect.m_x);
			destRect.m_y = (short) (destRect.m_y + m_clipRect.m_y);
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
// STUB: LEMBALL 0x0046d040
void Surface::AttachPalette(ResPalette* p_palette)
{
	TargetBuildSurfaceColourTable(g_dwWinGDrawColourTable, p_palette, 0, 0);
	SetDefaultCtable();
}

// 68K 0x10108642 NewBitmap__8CSurfaceFRC7CVSRect
// ASSERT: _VSRELassert("AllocatedBitmap", "VSGDI.CPP", 736)
// STUB: LEMBALL 0x0046d090
void Surface::NewBitmap(const VsRect& p_rect)
{
	unsigned char* bits;
	int stride;
	int height;
	int width;
	VsSize size;
	Surface* parent;
	BITMAPINFO* info;
	TargetDibContext* dib;
	TargetDrawingContext* context;
	ChangeList* list;
	VsRect* clip;

	EnterCriticalSection((CRITICAL_SECTION*) m_lock);
	width = p_rect.m_width;
	height = p_rect.m_height;
	clip = &m_clipRect;
	clip->m_width = p_rect.m_width;
	clip->m_height = p_rect.m_height;
	clip->m_x = p_rect.m_x;
	clip->m_y = p_rect.m_y;
	m_windowRect = p_rect;
	parent = m_parentSurface;
	if (parent != (Surface*) g_pGdiHelperTarget) {
		size.m_width = p_rect.m_width;
		size.m_height = p_rect.m_height;
		SetSize(size, (int) p_rect.m_width);
		CreateLinePtrs();
		SetLinePtrs();
		LeaveCriticalSection((CRITICAL_SECTION*) m_lock);
		return;
	}

	width = (width + 3) & ~3;
	size.m_width = (short) width;
	size.m_height = (short) height;
	SetSize(size, width);
	context = (TargetDrawingContext*) m_drawingPort;
	if (m_platformBitmap != 0 && g_pTargetGraphicsDriver != 0 && context != 0) {
		g_pTargetGraphicsDriver->RestoreDIBContext(context, (TargetDibContext*) m_platformBitmap);
		g_pTargetGraphicsDriver->DestroyDIBContext((TargetDibContext*) m_platformBitmap);
		m_platformBitmap = 0;
	}
	if (width == 0 || height == 0) {
		LeaveCriticalSection((CRITICAL_SECTION*) m_lock);
		return;
	}
	if (m_platformBitmap == 0 && g_pTargetGraphicsDriver != 0 && context != 0) {
		info = (BITMAPINFO*) m_bitmapInfo;
		g_pTargetGraphicsDriver->InitializeBitmapInfo(info);
		info->bmiHeader.biWidth = width;
		info->bmiHeader.biHeight = -height;
		info->bmiHeader.biBitCount = 8;
		info->bmiHeader.biSize = 0x28;
		dib = g_pTargetGraphicsDriver->CreateDIBContext(context, info);
		m_platformBitmap = dib;
		if (dib != 0) {
			g_pTargetGraphicsDriver->SelectDIBContext(context, dib);
		}
	}
	if (m_platformBitmap != 0) {
		dib = (TargetDibContext*) m_platformBitmap;
		stride = dib->GetStride();
		bits = dib->GetBits();
		SetBitsBase(bits, stride);
		list = GetChangeList();
		if (list != 0) {
			list->SetDrawMark();
		}
	}
	else {
		stride = width;
		bits = (unsigned char*) operator new((unsigned int) (stride * height));
		if (bits != 0) {
			memset(bits, 0, (unsigned int) (stride * height));
			SetBitsBase(bits, stride);
		}
	}
	LeaveCriticalSection((CRITICAL_SECTION*) m_lock);
}

// 68K 0x10109fc0 Resize__8CSurfaceFRC7CVSSize
// STUB: LEMBALL 0x0046d420
void Surface::Resize(const VsSize& p_size)
{
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

// STUB: LEMBALL 0x0046d800
void Surface::CopyDIBBits(void* p_header, unsigned char* p_bits)
{
	BITMAPINFOHEADER* header;
	ChangeList* list;
	int copyWidth;
	int copyHeight;
	int stride;
	int y;
	unsigned char* source;
	unsigned char* dest;
	VsRect area;

	list = GetChangeList();
	if (list == 0) {
		return;
	}
	EnterCriticalSection((CRITICAL_SECTION*) m_lock);
	header = (BITMAPINFOHEADER*) p_header;
	copyWidth = header->biWidth;
	if ((int) m_width <= copyWidth) {
		copyWidth = (int) m_width;
	}
	copyHeight = header->biHeight;
	if ((int) m_height <= copyHeight) {
		copyHeight = (int) m_height;
	}
	stride = (int) ((header->biBitCount * header->biWidth + 31) & ~31) / 8;
	source = p_bits + (header->biHeight - 1) * stride;
	y = 0;
	while (y < copyHeight) {
		dest = (unsigned char*) m_lines[y];
		memcpy(dest, source, (unsigned int) copyWidth);
		source = source - stride;
		y = y + 1;
	}
	area.m_width = m_width;
	area.m_height = m_height;
	area.m_x = 0;
	area.m_y = 0;
	list->Reset();
	AddToChangeList(area);
	LeaveCriticalSection((CRITICAL_SECTION*) m_lock);
}

// 68K 0x10108e58 SetDefaultCtable__8CSurfaceFv
// STUB: LEMBALL 0x0046d930
void Surface::SetDefaultCtable()
{
	unsigned char logPalette[8 + 256 * 4];
	LOGPALETTE* palette;
	PALETTEENTRY* entries;
	unsigned int* source;
	int i;
	SurfaceListNode* node;
	Surface* surface;
	unsigned int* dest;

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
		i = i + 1;
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
		dest = (unsigned int*) m_colourTable;
		source = g_dwWinGDrawColourTable;
		i = 0;
		while (i < 0x100) {
			dest[i] = source[i];
			i = i + 1;
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

// STUB: LEMBALL 0x0046d9f0
bool Surface::BeginRender()
{
	return 1;
}

// STUB: LEMBALL 0x0046daa0
void Surface::EndRender()
{
}

// 68K 0x1010171a Blit__8CSurfaceFP10CBigBitmapP10CResBITMAP
// STUB: LEMBALL 0x0046dbc0
void Surface::Blit(BigBitmap* p_arg0, ResBitmap* p_arg1)
{
}

// 68K 0x10105df8 Flush__8CSurfaceFv
// STUB: LEMBALL 0x0046dc50
void Surface::Flush()
{
	if (g_pGdiDevice != 0) {
		g_pGdiDevice->Flush(this);
	}
}

// 68K 0x10105dce GetCurrDB__8CSurfaceFv
// STUB: LEMBALL 0x0046dc80
void* Surface::GetCurrDb()
{
	return static_cast<PvSurface*>(this);
}

// 68K 0x10111b02 Blit__8CSurfaceFP13CScreenScroll
// STUB: LEMBALL 0x00474c20
void Surface::Blit(ScreenScroll* p_scroll)
{
}

// 68K 0x10111c76 Blit__8CSurfaceFP11CZBuffClear
// STUB: LEMBALL 0x00474d40
void Surface::Blit(ZBuffClear* p_arg0)
{
}

// 68K 0x10111d0c Blit__8CSurfaceFP12CZBuffScroll
// FUNCTION: LEMBALL 0x00474dc0
void Surface::Blit(ZBuffScroll* p_arg0)
{
}

// 68K 0x10105b48 Blit__8CSurfaceFP15CCopyToBackBuff
// STUB: LEMBALL 0x00474dd0
void Surface::Blit(CopyToBackBuff* p_arg0)
{
	unsigned int width;
	int height;
	int srcY;
	int destY;
	unsigned int dwordCount;
	unsigned int tail;
	unsigned char* src;
	unsigned char* dest;

	width = (unsigned int) p_arg0->m_field08;
	height = (int) p_arg0->m_field0a;
	if (width == 0 || height == 0 || m_lines == 0 || PvBackBuffSurface::m_bitmap.m_lines == 0) {
		return;
	}
	srcY = (int) p_arg0->m_field06;
	destY = (int) p_arg0->m_field0e;
	while (height != 0) {
		src = (unsigned char*) m_lines[srcY] + (int) p_arg0->m_field04;
		dest = (unsigned char*) PvBackBuffSurface::m_bitmap.m_lines[destY] + (int) p_arg0->m_field0c;
		dwordCount = width >> 2;
		while (dwordCount != 0) {
			*(unsigned int*) dest = *(unsigned int*) src;
			src = src + 4;
			dest = dest + 4;
			dwordCount = dwordCount - 1;
		}
		tail = width & 3;
		while (tail != 0) {
			*dest = *src;
			src = src + 1;
			dest = dest + 1;
			tail = tail - 1;
		}
		srcY = srcY + 1;
		destY = destY + 1;
		height = height - 1;
	}
}

// 68K 0x10105be0 Blit__8CSurfaceFP21CCopyColourToBackBuff
// STUB: LEMBALL 0x00474e60
void Surface::Blit(CopyColourToBackBuff* p_arg0)
{
}

// 68K 0x10105c6c CopyBackBuffToScreen__8CSurfaceFRC7CVSRect
// STUB: LEMBALL 0x00474ee0
void Surface::CopyBackBuffToScreen(const VsRect& p_arg0)
{
	short width;
	short height;
	short x;
	short y;
	int row;
	unsigned int dwordCount;
	unsigned int tail;
	unsigned char* src;
	unsigned char* dest;

	width = p_arg0.m_width;
	height = p_arg0.m_height;
	if ((int) width * (int) height == 0 || m_lines == 0 || PvBackBuffSurface::m_bitmap.m_lines == 0) {
		return;
	}
	x = p_arg0.m_x;
	y = p_arg0.m_y;
	if ((int) PvBackBuffSurface::m_allocatedWidth < (int) (short) (x + width)) {
		width = (short) (PvBackBuffSurface::m_allocatedWidth - x);
	}
	if ((int) PvBackBuffSurface::m_allocatedHeight < (int) (short) (height + y)) {
		height = (short) (PvBackBuffSurface::m_allocatedHeight - y);
	}
	row = 0;
	while (row < height) {
		src = (unsigned char*) PvBackBuffSurface::m_bitmap.m_lines[y + row] + (int) x;
		dest = (unsigned char*) m_lines[y + row] + (int) x;
		dwordCount = (unsigned int) (int) width >> 2;
		while (dwordCount != 0) {
			*(unsigned int*) dest = *(unsigned int*) src;
			src = src + 4;
			dest = dest + 4;
			dwordCount = dwordCount - 1;
		}
		tail = (int) width & 3;
		while (tail != 0) {
			*dest = *src;
			src = src + 1;
			dest = dest + 1;
			tail = tail - 1;
		}
		row = row + 1;
	}
}

// 68K 0x10111d36 Blit__8CSurfaceFP6CPoint
// STUB: LEMBALL 0x00474fd0
void Surface::Blit(Point* p_point)
{
}

// 68K 0x10111e5a Blit__8CSurfaceFP10CSolidRect
// STUB: LEMBALL 0x00475080
void Surface::Blit(SolidRect* p_rect)
{
}

// 68K 0x10111fca Blit__8CSurfaceFP5CLine
// STUB: LEMBALL 0x004750c0
void Surface::Blit(Line* p_line)
{
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
// STUB: LEMBALL 0x004757a0
int Surface::LineClip(int& p_x1, int& p_y1, int& p_x2, int& p_y2)
{
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
// STUB: LEMBALL 0x004787f0
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
	flags = p_bitmap->m_flags;
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
// STUB: LEMBALL 0x00478bb0
void Surface::BlitZrle(int p_x, int p_y, ResZrle* p_zrle, unsigned int p_flags, Remap* p_remap, unsigned short p_depth)
{
	VsRect dest;

	if (p_zrle == 0) {
		return;
	}
	dest.m_x = (short) p_x;
	dest.m_y = (short) p_y;
	dest.m_width = p_zrle->m_width;
	dest.m_height = p_zrle->m_height;
	if ((p_flags & 0x400) == 0) {
		dest.m_x = (short) (dest.m_x + p_zrle->m_x);
		dest.m_y = (short) (dest.m_y + p_zrle->m_y);
	}
	BlitZrleNoClip(dest, p_zrle, (unsigned char) ((p_flags & 2) >> 1));
}

void Surface::Blit(Bitmap* p_primitive)
{
}
