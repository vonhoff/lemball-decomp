#include "CSurface.h"

#include "CDibContext.h"
#include "CDrawingContext.h"
#include "CGDIDevice.h"
#include "CGdiContext.h"
#include "CGraphicsDriver.h"
#include "CPVBackBuffSurface.h"
#include "CPVGDIBitmap.h"
#include "CPVScrollableSurface.h"
#include "CPVZBuffSurface.h"
#include "Engine/Diagnostics/VsDebug.h"
#include "Engine/Graphics/CChangeList.h"
#include "Engine/Graphics/ChangeListItem.h"
#include "Engine/Graphics/Palettes/CRemap.h"
#include "Engine/Graphics/Primitives/CBigBitmap.h"
#include "Engine/Graphics/Primitives/CBitmap.h"
#include "Engine/Graphics/Primitives/CCircle.h"
#include "Engine/Graphics/Primitives/CClipRect.h"
#include "Engine/Graphics/Primitives/CCopyToBackBuff.h"
#include "Engine/Graphics/Primitives/CFilledCircle.h"
#include "Engine/Graphics/Primitives/CLine.h"
#include "Engine/Graphics/Primitives/CPoint.h"
#include "Engine/Graphics/Primitives/CScreenScroll.h"
#include "Engine/Graphics/Primitives/CSolidRect.h"
#include "Engine/Graphics/Primitives/CZBuffClear.h"
#include "Engine/Graphics/Primitives/CZRLE.h"
#include "Engine/Math/CVSPoint.h"
#include "Engine/Math/CVSRect.h"
#include "Engine/Math/CVSSize.h"
#include "Engine/Resources/Types/CResBITMAP.h"
#include "Engine/Resources/Types/CResPALETTE.h"
#include "Engine/Resources/Types/CResZRLE.h"
#include "Engine/Streams/CVSOStream.h"

#include <stdlib.h>
#include <string.h>
#define WIN32_LEAN_AND_MEAN
#include "Engine/Graphics/Primitives/CCopyColourToBackBuff.h"

#include <windows.h>

extern "C" __declspec(dllimport) int __stdcall GdiFlush();

struct SurfaceListHead {
	void* m_first;
	void* m_last;
	int m_count;
};

#define SURFACE_PALETTE_ENTRY_COUNT 0x100
#define SURFACE_PALETTE_VERSION_WIN3 0x0300
#define SURFACE_PALETTE_SYSTEM_RESERVED_LOW_COUNT 10
#define SURFACE_PALETTE_SYSTEM_RESERVED_HIGH_START                                                                     \
	(SURFACE_PALETTE_ENTRY_COUNT - SURFACE_PALETTE_SYSTEM_RESERVED_LOW_COUNT)
#define SURFACE_PALETTE_SYSTEM_USABLE_ENTRY_COUNT                                                                      \
	(SURFACE_PALETTE_ENTRY_COUNT - 2 * SURFACE_PALETTE_SYSTEM_RESERVED_LOW_COUNT)
#define SURFACE_PALETTE_APP_FIRST_INDEX 12
#define SURFACE_PALETTE_SPECIAL_OUTPUT_ENTRY_COUNT 2
#define SURFACE_PALETTE_RESOURCE_ENTRY_STRIDE_BYTES 4
#define SURFACE_PALETTE_OUTPUT_ENTRY_STRIDE_BYTES 4
#define SURFACE_PALETTE_HIGH_RESERVED_RED_OFFSET_BYTES                                                                 \
	(SURFACE_PALETTE_SYSTEM_RESERVED_HIGH_START * SURFACE_PALETTE_OUTPUT_ENTRY_STRIDE_BYTES)
#define SURFACE_PALETTE_HIGH_RESERVED_GREEN_OFFSET_BYTES (SURFACE_PALETTE_HIGH_RESERVED_RED_OFFSET_BYTES - 1)
#define SURFACE_PALETTE_HIGH_RESERVED_BLUE_OFFSET_BYTES (SURFACE_PALETTE_HIGH_RESERVED_RED_OFFSET_BYTES - 2)
#define SURFACE_PALETTE_HIGH_RESERVED_FLAGS_OFFSET_BYTES (SURFACE_PALETTE_HIGH_RESERVED_RED_OFFSET_BYTES + 1)
#define SURFACE_PALETTE_BUFFER_BYTES (sizeof(LOGPALETTE) + (SURFACE_PALETTE_ENTRY_COUNT - 1) * sizeof(PALETTEENTRY))

// GLOBAL: LEMBALL 0x004a2010
SurfaceListHead* g_pSurfaceList = NULL;

enum {
	SURFACE_CHANGE_TRACKING_CELL_SIZE_PIXELS = 8,
	GDI_HELPER_CHANGE_LIST_CAPACITY = 0x1000
};

// GLOBAL: LEMBALL 0x004a2018
static const unsigned char g_anFallbackSystemColours[20][3] = {
	{0x00, 0x00, 0x00}, {0x80, 0x00, 0x00}, {0x00, 0x80, 0x00}, {0x80, 0x80, 0x00}, {0x00, 0x00, 0x80},
	{0x80, 0x00, 0x80}, {0x00, 0x80, 0x80}, {0xc0, 0xc0, 0xc0}, {0xc0, 0xdc, 0xc0}, {0xa6, 0xca, 0xf0},
	{0xff, 0xfb, 0xf0}, {0xa0, 0xa0, 0xa4}, {0x80, 0x80, 0x80}, {0xff, 0x00, 0x00}, {0x00, 0xff, 0x00},
	{0xff, 0xff, 0x00}, {0x00, 0x00, 0xff}, {0xff, 0x00, 0xff}, {0x00, 0xff, 0xff}, {0xff, 0xff, 0xff}};
// GLOBAL: LEMBALL 0x004a2058
static const unsigned char g_anReservedOutputColours[2][3] = {{0xff, 0xff, 0xff}, {0x00, 0x00, 0x00}};

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

enum eCircleClipResult {
	CIRCLE_OUTSIDE_CLIP = 1,
	CIRCLE_FULLY_INSIDE_CLIP = 2,
	CIRCLE_PARTIALLY_CLIPPED = 3
};

enum eLineClipRegionFlag {
	LINE_CLIP_REGION_INSIDE = 0,
	LINE_CLIP_REGION_LEFT = 0x01,
	LINE_CLIP_REGION_RIGHT = 0x02,
	LINE_CLIP_REGION_TOP = 0x04,
	LINE_CLIP_REGION_BOTTOM = 0x08
};

inline unsigned int CSurface::ClipCode(int p_x, int p_y)
{
	unsigned int code = 0;
	if (p_x < m_clipRect.m_x) {
		code |= 1;
	}
	else if (p_x > m_clipRect.m_x + m_clipRect.m_width - 1) {
		code |= 2;
	}
	if (p_y < m_clipRect.m_y) {
		code |= 4;
	}
	else if (p_y > m_clipRect.m_y + m_clipRect.m_height - 1) {
		code |= 8;
	}
	return code;
}

// FUNCTION: LEMBALL 0x0046c050
CSurface::CSurface(const CVSRect& p_rect, class CSurface* p_parentSurface)
	: m_presentX(m_presentY = 0), m_childSurfaceHead(NULL), m_childSurfaceTail(NULL), m_childSurfaceCount(0)
{
	SurfaceListHead* head;
	SurfaceListNode* node;
	SurfaceListHead* parentList;
	void* storage;

	m_flag70 = 1;
	m_flag78 = 0;
	m_flag74 = 0;
	m_parentSurface = p_parentSurface;
	parentList = (SurfaceListHead*) &m_parentSurface->m_childSurfaceHead;
	storage = operator new(0xc);
	if (storage != NULL) {
		node = (SurfaceListNode*) storage;
		node->m_surface = this;
		node->m_next = NULL;
		node->m_prev = NULL;
	}
	else {
		node = NULL;
	}
	node->m_prev = (SurfaceListNode*) parentList->m_last;
	if (parentList->m_last != NULL) {
		((SurfaceListNode*) parentList->m_last)->m_next = node;
	}
	parentList->m_last = node;
	if (parentList->m_first == NULL) {
		parentList->m_first = node;
	}
	parentList->m_count++;

	if (g_pSurfaceList == NULL) {
		head = (SurfaceListHead*) operator new(sizeof(SurfaceListHead));
		if (head != NULL) {
			head->m_first = NULL;
			head->m_last = NULL;
			head->m_count = 0;
			g_pSurfaceList = head;
		}
		else {
			g_pSurfaceList = NULL;
		}
	}
	head = g_pSurfaceList;
	storage = operator new(0xc);
	if (storage != NULL) {
		node = (SurfaceListNode*) storage;
		node->m_surface = this;
		node->m_next = NULL;
		node->m_prev = NULL;
	}
	else {
		node = NULL;
	}
	node->m_prev = (SurfaceListNode*) head->m_last;
	if (head->m_last != NULL) {
		((SurfaceListNode*) head->m_last)->m_next = node;
	}
	head->m_last = node;
	if (head->m_first == NULL) {
		head->m_first = node;
	}
	head->m_count++;

	m_zoom = 1;
	m_platformBitmap = NULL;
	m_drawingPort = NULL;
	m_reserved40 = 0;
	InitializeCriticalSection((CRITICAL_SECTION*) m_lock);
	m_lockInitialised = 1;
	if (m_parentSurface == (CSurface*) g_pGdiHelperTarget) {
		m_changeList = new CChangeList(
			GDI_HELPER_CHANGE_LIST_CAPACITY,
			p_rect,
			CVSSize(SURFACE_CHANGE_TRACKING_CELL_SIZE_PIXELS, SURFACE_CHANGE_TRACKING_CELL_SIZE_PIXELS));
	}
	else {
		m_changeList = new CChangeList(
			0,
			p_rect,
			CVSSize(SURFACE_CHANGE_TRACKING_CELL_SIZE_PIXELS, SURFACE_CHANGE_TRACKING_CELL_SIZE_PIXELS));
	}
	if (m_parentSurface == (CSurface*) g_pGdiHelperTarget) {
		BuildSurfaceColourTable((unsigned int*) m_colourTable,
								NULL,
								NULL,
								g_pTargetGraphicsDriver->HasPalette() ? g_dwWinGDrawColourTable : NULL);
		CGraphicsDriver* driver = g_pTargetGraphicsDriver;
		m_drawingPort = driver->CreateDrawingContext();
	}
	CVSRect& rect = m_surfaceRect;
	rect.m_width = p_rect.m_width;
	rect.m_height = p_rect.m_height;
	const short* coords;
	if (&p_rect != NULL) {
		coords = &p_rect.m_x;
	}
	else {
		coords = NULL;
	}
	rect.m_x = *coords;
	rect.m_y = coords[1];
	NewBitmap(p_rect);
}

// FUNCTION: LEMBALL 0x0046c380
void BuildSurfaceColourTable(unsigned int* p_entries,
							 CResPALETTE* p_palette,
							 void* p_unused,
							 unsigned int* p_fallbackEntries)
{
	unsigned char paletteStorage[SURFACE_PALETTE_BUFFER_BYTES];
	int count;
	PALETTEENTRY* systemEntries = ((LOGPALETTE*) paletteStorage)->palPalEntry;
	unsigned char* output;
	PALETTEENTRY* entry;
	const unsigned char* source;
	HDC hdc = GetDC(NULL);
	unsigned int first = GetSystemPaletteEntries(hdc, 0, SURFACE_PALETTE_SYSTEM_RESERVED_LOW_COUNT, systemEntries);
	unsigned int last = GetSystemPaletteEntries(hdc,
												SURFACE_PALETTE_SYSTEM_RESERVED_HIGH_START,
												SURFACE_PALETTE_SYSTEM_RESERVED_LOW_COUNT,
												systemEntries + SURFACE_PALETTE_SYSTEM_RESERVED_HIGH_START);
	first |= last;
	if (first == 0) {
		source = &g_anFallbackSystemColours[0][0];
		entry = systemEntries;
		do {
			entry->peRed = *source++;
			entry->peGreen = *source++;
			entry->peBlue = *source++;
			entry++;

		} while (entry < systemEntries + SURFACE_PALETTE_SYSTEM_RESERVED_LOW_COUNT);
		entry = systemEntries + SURFACE_PALETTE_SYSTEM_RESERVED_HIGH_START;
		do {
			entry->peRed = *source++;
			entry->peGreen = *source++;
			entry->peBlue = *source++;
			entry++;

		} while (entry < systemEntries + SURFACE_PALETTE_ENTRY_COUNT);
	}
	if (hdc != NULL) {
		ReleaseDC(NULL, hdc);
	}
	((LOGPALETTE*) paletteStorage)->palVersion = SURFACE_PALETTE_VERSION_WIN3;
	((LOGPALETTE*) paletteStorage)->palNumEntries = SURFACE_PALETTE_ENTRY_COUNT;
	output = &((RGBQUAD*) p_entries)[0].rgbRed;
	entry = systemEntries;
	do {
		output[0] = entry->peRed;
		output[-1] = entry->peGreen;
		output[-2] = entry->peBlue;
		entry->peFlags = 0;
		unsigned char red = entry[SURFACE_PALETTE_SYSTEM_RESERVED_HIGH_START].peRed;
		output[1] = 0;
		output[SURFACE_PALETTE_HIGH_RESERVED_RED_OFFSET_BYTES] = red;
		output[SURFACE_PALETTE_HIGH_RESERVED_GREEN_OFFSET_BYTES] =
			entry[SURFACE_PALETTE_SYSTEM_RESERVED_HIGH_START].peGreen;
		output[SURFACE_PALETTE_HIGH_RESERVED_BLUE_OFFSET_BYTES] =
			entry[SURFACE_PALETTE_SYSTEM_RESERVED_HIGH_START].peBlue;
		entry[SURFACE_PALETTE_SYSTEM_RESERVED_HIGH_START].peFlags = 0;
		output[SURFACE_PALETTE_HIGH_RESERVED_FLAGS_OFFSET_BYTES] = 0;
		entry++;
		output += SURFACE_PALETTE_OUTPUT_ENTRY_STRIDE_BYTES;
	} while (entry < systemEntries + SURFACE_PALETTE_SYSTEM_RESERVED_LOW_COUNT);
	source = &g_anReservedOutputColours[0][0];
	output = &((RGBQUAD*) p_entries)[SURFACE_PALETTE_SYSTEM_RESERVED_LOW_COUNT].rgbRed;
	count = SURFACE_PALETTE_SPECIAL_OUTPUT_ENTRY_COUNT;
	do {
		output[0] = *source++;
		output[-1] = *source++;
		output[-2] = *source++;
		output[1] = 0;
		output += SURFACE_PALETTE_OUTPUT_ENTRY_STRIDE_BYTES;
		count--;
	} while (count != 0);
	if (p_palette == NULL) {
		if (p_fallbackEntries == NULL) {
			PALETTEENTRY* entry;
			int index = SURFACE_PALETTE_APP_FIRST_INDEX;
			entry = systemEntries + SURFACE_PALETTE_APP_FIRST_INDEX;
			output = &((RGBQUAD*) p_entries)[SURFACE_PALETTE_APP_FIRST_INDEX].rgbRed;
			do {
				unsigned char colour = -index;
				entry->peRed = colour;
				output[0] = colour;
				entry->peGreen = colour;
				output[-1] = colour;
				entry->peBlue = colour;
				output[-2] = colour;
				output[1] = 0;
				entry->peFlags = PC_RESERVED;
				entry++;
				output += SURFACE_PALETTE_OUTPUT_ENTRY_STRIDE_BYTES;
				index++;
			} while (entry < systemEntries + SURFACE_PALETTE_SYSTEM_RESERVED_HIGH_START);
		}
		else {
			unsigned char* fallback;
			PALETTEENTRY* entry = systemEntries + SURFACE_PALETTE_APP_FIRST_INDEX;
			output = &((RGBQUAD*) p_entries)[SURFACE_PALETTE_APP_FIRST_INDEX].rgbRed;
			fallback = &((RGBQUAD*) p_fallbackEntries)[SURFACE_PALETTE_APP_FIRST_INDEX].rgbRed;
			do {
				unsigned char colour = fallback[0];
				output[0] = colour;
				entry->peRed = colour;
				colour = fallback[-1];
				entry->peGreen = colour;
				output[-1] = colour;
				colour = fallback[-2];
				entry->peBlue = colour;
				output[-2] = colour;
				output[1] = 0;
				entry->peFlags = PC_RESERVED;
				entry++;
				output += SURFACE_PALETTE_OUTPUT_ENTRY_STRIDE_BYTES;
				fallback += SURFACE_PALETTE_OUTPUT_ENTRY_STRIDE_BYTES;
			} while (entry < systemEntries + SURFACE_PALETTE_SYSTEM_RESERVED_HIGH_START);
		}
	}
	else {
		int paletteCount = (int) (p_palette->m_entryCount - SURFACE_PALETTE_SYSTEM_RESERVED_LOW_COUNT);
		if (paletteCount > SURFACE_PALETTE_SYSTEM_RESERVED_HIGH_START) {
			paletteCount = SURFACE_PALETTE_SYSTEM_USABLE_ENTRY_COUNT;
		}
		if (paletteCount > SURFACE_PALETTE_APP_FIRST_INDEX) {
			int i = SURFACE_PALETTE_APP_FIRST_INDEX;
			output = &((RGBQUAD*) p_entries)[SURFACE_PALETTE_APP_FIRST_INDEX].rgbRed;
			paletteCount -= SURFACE_PALETTE_APP_FIRST_INDEX;
			do {
				source = p_palette->m_data + i * SURFACE_PALETTE_RESOURCE_ENTRY_STRIDE_BYTES;
				unsigned char colour = source[0];
				systemEntries[i].peRed = colour;
				output[0] = colour;
				colour = source[1];
				systemEntries[i].peGreen = colour;
				output[-1] = colour;
				colour = source[2];
				systemEntries[i].peBlue = colour;
				output[-2] = colour;
				output[1] = 0;
				systemEntries[i].peFlags = PC_RESERVED;
				output += SURFACE_PALETTE_OUTPUT_ENTRY_STRIDE_BYTES;
				i++;
				paletteCount--;
			} while (paletteCount != 0);
		}
	}
	g_pTargetGraphicsDriver->CreatePalette((LOGPALETTE*) paletteStorage);
}

// FUNCTION: LEMBALL 0x0046c5d0
CSurface::CSurface(GrafPort* p_port)
{
	m_presentY = 0;
	m_childSurfaceHead = NULL;
	m_presentX = 0;
	m_childSurfaceTail = NULL;
	m_childSurfaceCount = 0;
	m_drawingPort = new CGdiContext(p_port);
	m_platformBitmap = NULL;
	m_changeList = NULL;
	m_parentSurface = NULL;
	InitializeCriticalSection((CRITICAL_SECTION*) m_lock);
	m_lockInitialised = 1;
	m_flag70 = 0;
}

// FUNCTION: LEMBALL 0x0046c710
CSurface::~CSurface()
{
	SurfaceListNode* node;
	SurfaceListNode* next;
	SurfaceListNode* prev;
	CSurface* parent;
	int locked;

	locked = 0;
	if (m_lockInitialised != 0) {
		EnterCriticalSection((CRITICAL_SECTION*) m_lock);
		locked = 1;
	}
	if (m_platformBitmap != NULL) {
		g_pTargetGraphicsDriver->RestoreDibContext((CDrawingContext*) m_drawingPort, (CDibContext*) m_platformBitmap);
		g_pTargetGraphicsDriver->DestroyDibContext((CDibContext*) m_platformBitmap);
		m_platformBitmap = NULL;
	}
	if (m_parentSurface == (CSurface*) g_pGdiHelperTarget) {
		FreeBackBuff();
		FreeZBuff();
	}
	if (m_drawingPort != NULL) {
		g_pTargetGraphicsDriver->DestroyDrawingContext((CDrawingContext*) m_drawingPort);
		m_drawingPort = NULL;
	}
	if (m_changeList != NULL) {
		delete m_changeList;
		m_changeList = NULL;
	}
	parent = m_parentSurface;
	if (parent != NULL) {
		node = parent->m_childSurfaceHead;
		while (node != NULL) {
			if (node->m_surface == this) {
				break;
			}
			node = node->m_next;
		}
		if (node != NULL) {
			next = node->m_next;
			prev = node->m_prev;
			operator delete(node);
			if (next == NULL) {
				parent->m_childSurfaceTail = prev;
			}
			else {
				next->m_prev = prev;
			}
			if (prev == NULL) {
				parent->m_childSurfaceHead = next;
			}
			else {
				prev->m_next = next;
			}
			parent->m_childSurfaceCount = parent->m_childSurfaceCount - 1;
		}
		m_parentSurface = NULL;
	}
	if (locked != 0) {
		LeaveCriticalSection((CRITICAL_SECTION*) m_lock);
		DeleteCriticalSection((CRITICAL_SECTION*) m_lock);
		m_lockInitialised = 0;
		if (g_pSurfaceList != NULL) {
			node = (SurfaceListNode*) g_pSurfaceList->m_first;
			while (node != NULL) {
				if (node->m_surface == this) {
					break;
				}
				node = node->m_next;
			}
			if (node != NULL) {
				next = node->m_next;
				prev = node->m_prev;
				operator delete(node);
				if (next == NULL) {
					g_pSurfaceList->m_last = prev;
				}
				else {
					next->m_prev = prev;
				}
				if (prev == NULL) {
					g_pSurfaceList->m_first = next;
				}
				else {
					prev->m_next = next;
				}
				g_pSurfaceList->m_count = g_pSurfaceList->m_count - 1;
			}
			if (g_pSurfaceList != NULL && g_pSurfaceList->m_count == 0) {
				node = (SurfaceListNode*) g_pSurfaceList->m_first;
				while (node != NULL) {
					next = node->m_next;
					operator delete(node);
					node = next;
				}
				operator delete(g_pSurfaceList);
				g_pSurfaceList = NULL;
			}
		}
	}
	node = m_childSurfaceHead;
	while (node != NULL) {
		next = node->m_next;
		operator delete(node);
		node = next;
	}
}

// FUNCTION: LEMBALL 0x0046c990
void CSurface::ResetScroll()
{
	SurfaceListNode* node;

	CPVGDIBitmap::ResetScroll();
	if (HasBackBuff() != 0) {
		CPVBackBuffSurface::m_bitmap.ResetScroll();
	}
	if (HasZBuff() != 0) {
		CPVZBuffSurface::m_bitmap.ResetScroll();
	}
	for (node = m_childSurfaceHead; node != NULL; node = node->m_next) {
		node->m_surface->CPVGDIBitmap::ResetLinePtrs();
	}
}

// FUNCTION: LEMBALL 0x0046c9f0
void CSurface::SetLinePtrs()
{
	int parentY;
	int y;
	int parentStride;
	unsigned char* bits;

	if (CPVScrollableSurface::m_parentSurface != (CSurface*) g_pGdiHelperTarget) {
		parentStride = CPVScrollableSurface::m_parentSurface->m_stride;
		m_stride = parentStride;
		bits = (unsigned char*)
				   CPVScrollableSurface::m_parentSurface->m_lines[(int) CPVScrollableSurface::m_windowRect.m_y] +
			   (int) CPVScrollableSurface::m_windowRect.m_x;
		m_bitsBase = bits;
		m_bits = bits;
		m_xOffset = 0;
		m_firstLine = 0;
		if (CPVScrollableSurface::m_parentSurface->CPVBackBuffSurface::m_enabled != 0) {
			CPVBackBuffSurface::m_enabled = CPVScrollableSurface::m_parentSurface->CPVBackBuffSurface::m_enabled;
			CPVBackBuffSurface::m_buffer = CPVScrollableSurface::m_parentSurface->CPVBackBuffSurface::m_buffer +
										   (int) CPVScrollableSurface::m_windowRect.m_y * parentStride +
										   (int) CPVScrollableSurface::m_windowRect.m_x;
		}
		else {
			CPVBackBuffSurface::m_enabled = 0;
		}
		if (CPVScrollableSurface::m_parentSurface->CPVZBuffSurface::m_enabled != 0) {
			CPVZBuffSurface::m_enabled = CPVScrollableSurface::m_parentSurface->CPVZBuffSurface::m_enabled;
			CPVZBuffSurface::m_buffer =
				(unsigned short*) ((int) CPVScrollableSurface::m_parentSurface->CPVZBuffSurface::m_buffer +
								   ((int) CPVScrollableSurface::m_windowRect.m_y * parentStride +
									(int) CPVScrollableSurface::m_windowRect.m_x) *
									   2);
		}
		else {
			CPVZBuffSurface::m_enabled = 0;
		}
		parentY = (int) CPVScrollableSurface::m_windowRect.m_y;
		y = 0;
		if (0 < m_size.m_height) {
			do {
				m_lines[y] = (void*) ((int) CPVScrollableSurface::m_parentSurface->m_lines[parentY] +
									  (int) CPVScrollableSurface::m_windowRect.m_x);
				y = y + 1;
				parentY = parentY + 1;
			} while (y < m_size.m_height);
		}
	}
	else {
		CPVGDIBitmap::SetLinePtrs();
	}
}

// FUNCTION: LEMBALL 0x0046cb20
void CSurface::AddToChangeList(const CVSRect& p_rect)
{
	CSurface* parent;
	const CVSPoint* origin;
	short originX;
	short originY;

	parent = (CSurface*) CPVScrollableSurface::m_parentSurface;
	if (parent != (CSurface*) g_pGdiHelperTarget && CPVScrollableSurface::m_flag74 != 0 &&
		CPVScrollableSurface::m_flag70 != 0) {
		origin = &this->CPVScrollableSurface::m_surfaceRect;
		originX = origin->m_x;
		originY = origin->m_y;
		CVSRect translated(p_rect);
		translated.m_x += originX;
		translated.m_y += originY;
		((CSurface*) CPVScrollableSurface::m_parentSurface)->AddToChangeList(translated);
		return;
	}
	m_changeList->Add(p_rect);
}

// FUNCTION: LEMBALL 0x0046cbd0
CChangeList* CSurface::GetChangeList()
{
	return m_changeList;
}

// FUNCTION: LEMBALL 0x0046cbe0
void CSurface::Blit(class CClipRect* p_clipRect)
{
	CVSRect* clip = &m_clipRect;
	short clipRight;

	if ((p_clipRect->m_flags & CClipRect::CLIP_EXPAND_BOUNDS) == 0) {
		const short* coords;

		clip->m_width = p_clipRect->m_bounds.m_width;
		clip->m_height = p_clipRect->m_bounds.m_height;
		if (&p_clipRect->m_bounds.m_width != NULL) {
			coords = &p_clipRect->m_bounds.m_x;
		}
		else {
			coords = NULL;
		}
		clip->m_x = *coords;
		clip->m_y = coords[1];
	}
	else if ((int) p_clipRect->m_bounds.m_width * (int) p_clipRect->m_bounds.m_height != 0) {
		clipRight = clip->m_x;
		if (p_clipRect->m_bounds.m_x < clipRight) {
			clip->m_width = (short) (clip->m_width + (clipRight - p_clipRect->m_bounds.m_x));
			clip->m_x = p_clipRect->m_bounds.m_x;
		}
		short clipX;
		short primitiveWidth = p_clipRect->m_bounds.m_width;
		clipX = clip->m_x;
		short right = clip->m_width;
		right += clipX;
		short primitiveRight = p_clipRect->m_bounds.m_x;
		primitiveRight += primitiveWidth;
		if (right < primitiveRight) {
			primitiveWidth -= clipX;
			primitiveWidth += p_clipRect->m_bounds.m_x;
			clip->m_width = primitiveWidth;
		}
		if (p_clipRect->m_bounds.m_y < clip->m_y) {
			clip->m_height = (short) (clip->m_height + (clip->m_y - p_clipRect->m_bounds.m_y));
			clip->m_y = p_clipRect->m_bounds.m_y;
		}
		short primitiveY;
		short primitiveHeight = p_clipRect->m_bounds.m_height;
		primitiveY = p_clipRect->m_bounds.m_y;
		short clipBottom = (short) (clip->m_height + clip->m_y);
		short primitiveBottom = (short) (primitiveY + primitiveHeight);
		if (clipBottom < primitiveBottom) {
			clip->m_height = (short) ((primitiveHeight - clip->m_y) + primitiveY);
		}
	}
	CSurface* parent = m_parentSurface;
	if ((CSurface*) g_pGdiHelperTarget != parent && (p_clipRect->m_flags & CClipRect::CLIP_IGNORE_PARENT) == 0) {
		CVSRect* parentClip = &parent->m_clipRect;
		short parentX = parentClip->m_x;
		CVSRect* childClip = &m_clipRect;
		clipRight = childClip->m_x;
		if (clipRight < parentX) {
			childClip->m_width = (short) (childClip->m_width + (clipRight - parentX));
			childClip->m_x = parentClip->m_x;
		}
		short parentWidth;
		short childX = childClip->m_x;
		parentX = parentClip->m_x;
		parentWidth = parentClip->m_width;
		if ((short) (parentWidth + parentX) < (short) (childClip->m_width + childX)) {
			parentX -= childX;
			parentX += parentWidth;
			childClip->m_width = parentX;
		}
		if (childClip->m_y < parentClip->m_y) {
			childClip->m_height = (short) (childClip->m_height + (childClip->m_y - parentClip->m_y));
			childClip->m_y = parentClip->m_y;
		}
		if ((short) (parentClip->m_y + parentClip->m_height) < (short) (childClip->m_height + childClip->m_y)) {
			childClip->m_height = (short) ((parentClip->m_height - childClip->m_y) + parentClip->m_y);
		}
		if (childClip->m_width <= 0 || childClip->m_height <= 0) {
			childClip->m_height = 0;
			childClip->m_width = 0;
			childClip->m_y = 0;
			childClip->m_x = 0;
		}
	}
}

// FUNCTION: LEMBALL 0x0046cda0
void CSurface::ToScreen(class CSurface* p_destinationSurface)
{
	if ((void*) m_parentSurface != g_pGdiHelperTarget) {
		if (m_flag74 == 0) {
			if (m_flag78 != 0) {
				m_parentSurface->AddToChangeList(m_windowRect);
				m_flag78 = 0;
			}
			else if (m_flag70 != 0) {
				CChangeList* list = GetChangeList();
				int diff = list->GetNumItems() - list->GetDrawMark();
				if (diff > 0) {
					m_parentSurface->AddToChangeList(m_windowRect);
				}
			}
		}
		return;
	}

	if (m_platformBitmap == NULL) {
		return;
	}

	EnterCriticalSection((CRITICAL_SECTION*) m_lock);
	EnterCriticalSection((CRITICAL_SECTION*) p_destinationSurface->m_lock);
	CDrawingContext* destContext = (CDrawingContext*) p_destinationSurface->m_drawingPort;
	g_pTargetGraphicsDriver->RealizePalette(destContext);
	if ((int) m_dontUpdateRect.m_height * (int) m_dontUpdateRect.m_width > 0) {
		m_changeList->AddWithActiveMark(m_dontUpdateRect, 0);
	}
	int index = 0;
	if (m_changeList->GetNumItems() > 0) {
		do {
			ChangeListItem* item = m_changeList->GetNItem(index);
			CVSRect translated(*(CVSRect*) item);
			if (g_dwFullScreenGdi == 0) {
				translated.m_x += m_presentX;
				translated.m_y += m_presentY;
			}
			const CVSPoint* origin = (const CVSPoint*) &m_relOriginX;
			translated.m_x += origin->m_x;
			translated.m_y += origin->m_y;
			short zoom = m_zoom;
			if (zoom == 1) {
				g_pTargetGraphicsDriver->BlitWrappedBitmap(destContext,
														   &translated,
														   (CDrawingContext*) m_drawingPort,
														   (CVSRect*) item,
														   this);
			}
			else {
				CVSRect destRect(translated.m_x * zoom,
								 translated.m_y * zoom,
								 translated.m_width * zoom,
								 translated.m_height * zoom);
				g_pTargetGraphicsDriver->BlitWrappedBitmap(destContext,
														   &destRect,
														   (CDrawingContext*) m_drawingPort,
														   (CVSRect*) item,
														   this);
			}
			index++;
		} while (index < m_changeList->GetNumItems());
	}
	GdiFlush();
	if (HasBackBuff() != 0) {
		int i = 0;
		if (m_changeList->GetNumItems() > 0) {
			do {
				ChangeListItem* item = m_changeList->GetNItem(i);
				CopyBackBuffToScreen(*(CVSRect*) item);
				i++;
			} while (i < m_changeList->GetNumItems());
		}
	}
	LeaveCriticalSection((CRITICAL_SECTION*) m_lock);
	LeaveCriticalSection((CRITICAL_SECTION*) p_destinationSurface->m_lock);
}

// FUNCTION: LEMBALL 0x0046d040
void CSurface::AttachPalette(CResPALETTE* p_palette)
{
	unsigned int* fallbackEntries;

	if (g_pTargetGraphicsDriver->HasPalette()) {
		fallbackEntries = g_dwWinGDrawColourTable;
	}
	else {
		fallbackEntries = NULL;
	}
	BuildSurfaceColourTable(g_dwWinGDrawColourTable, p_palette, NULL, fallbackEntries);
	SetDefaultCtable();
}

// FUNCTION: LEMBALL 0x0046d090
void CSurface::NewBitmap(const CVSRect& p_rect)
{
	EnterCriticalSection((CRITICAL_SECTION*) m_lock);
	{
		CVSRect& bounds = m_surfaceRect;
		bounds.m_width = p_rect.m_width;
		bounds.m_height = p_rect.m_height;
		const CVSPoint* position = &p_rect;
		bounds.m_x = position->m_x;
		bounds.m_y = position->m_y;
	}
	{
		CVSRect& bounds = m_windowRect;
		bounds.m_width = p_rect.m_width;
		bounds.m_height = p_rect.m_height;
		const CVSPoint* position = &p_rect;
		bounds.m_x = position->m_x;
		bounds.m_y = position->m_y;
	}
	if ((void*) m_parentSurface != g_pGdiHelperTarget) {
		const CVSSize& parentSize = m_parentSurface->m_windowRect;
		short parentWidth = parentSize.m_width;
		short parentHeight = parentSize.m_height;
		CVSRect& clipped = m_windowRect;
		if (clipped.m_x < 0) {
			clipped.m_width += clipped.m_x;
			clipped.m_x = 0;
		}
		if (parentWidth < (short) (clipped.m_x + clipped.m_width)) {
			clipped.m_width = parentWidth - clipped.m_x;
		}
		if (clipped.m_y < 0) {
			clipped.m_height += clipped.m_y;
			clipped.m_y = 0;
		}
		if (parentHeight < (short) (clipped.m_y + clipped.m_height)) {
			clipped.m_height = parentHeight - clipped.m_y;
		}
		if (clipped.m_width <= 0 || clipped.m_height <= 0) {
			clipped.m_height = 0;
			clipped.m_width = 0;
			clipped.m_y = 0;
			clipped.m_x = 0;
		}
		{
			const CVSSize& windowSize = m_windowRect;
			CVSSize& clipSize = m_clipRect;
			short height = windowSize.m_height;
			clipSize.m_width = windowSize.m_width;
			clipSize.m_height = height;
		}
		SetSize(m_windowRect, (int) m_windowRect.m_width);
		m_bitmapPixelCount = 0;
		CreateLinePtrs();
		return;
	}
	m_windowRect.m_width = (m_windowRect.m_width + 3) & ~3;
	{
		const CVSSize& windowSize = m_windowRect;
		short height = windowSize.m_height;
		CVSSize& clipSize = m_clipRect;
		clipSize.m_width = windowSize.m_width;
		clipSize.m_height = height;
	}
	short width;
	short height;
	{
		const CVSSize& size = SetSize(m_windowRect, m_reserved40);
		width = size.m_width;
		height = size.m_height;
	}
	if (m_platformBitmap != NULL) {
		g_pTargetGraphicsDriver->RestoreDibContext((CDrawingContext*) m_drawingPort, (CDibContext*) m_platformBitmap);
		g_pTargetGraphicsDriver->DestroyDibContext((CDibContext*) m_platformBitmap);
		m_platformBitmap = NULL;
	}
	if (m_windowRect.m_width == 0 || m_windowRect.m_height == 0) {
		m_bitmapPixelCount = 0;
	}
	else {
		if (m_platformBitmap == NULL) {
			g_pTargetGraphicsDriver->InitializeBitmapInfo((BITMAPINFO*) m_bitmapInfo);
			((BITMAPINFO*) m_bitmapInfo)->bmiHeader.biWidth = width;
			((BITMAPINFO*) m_bitmapInfo)->bmiHeader.biHeight =
				(int) height * (int) ((BITMAPINFO*) m_bitmapInfo)->bmiHeader.biHeight;
			((BITMAPINFO*) m_bitmapInfo)->bmiHeader.biPlanes = 1;
			((BITMAPINFO*) m_bitmapInfo)->bmiHeader.biCompression = BI_RGB;
			((BITMAPINFO*) m_bitmapInfo)->bmiHeader.biSizeImage = 0;
			((BITMAPINFO*) m_bitmapInfo)->bmiHeader.biXPelsPerMeter = 0;
			((BITMAPINFO*) m_bitmapInfo)->bmiHeader.biYPelsPerMeter = 0;
			((BITMAPINFO*) m_bitmapInfo)->bmiHeader.biClrUsed = 0;
			((BITMAPINFO*) m_bitmapInfo)->bmiHeader.biClrImportant = 0;
			((BITMAPINFO*) m_bitmapInfo)->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
			((BITMAPINFO*) m_bitmapInfo)->bmiHeader.biBitCount = 8;
			m_platformBitmap =
				g_pTargetGraphicsDriver->CreateDibContext((CDrawingContext*) m_drawingPort, (BITMAPINFO*) m_bitmapInfo);
			if (m_platformBitmap != NULL) {
				g_pTargetGraphicsDriver->SelectDibContext((CDrawingContext*) m_drawingPort,
														  (CDibContext*) m_platformBitmap);
				m_bitmapPixelCount = (int) m_windowRect.m_width * (int) m_windowRect.m_height;
			}
		}
		if (m_platformBitmap == NULL) {
			_VSRELassert("AllocatedBitmap", "VSGDI.CPP", 736);
		}
		CDibContext* dib = (CDibContext*) m_platformBitmap;
		SetBitsBase(dib->GetBits(), dib->GetStride());
		m_changeList->SetDrawMark();
		CVSRect clip;
		const CVSSize& drawSize = m_surfaceRect;
		short drawHeight = drawSize.m_height;
		clip.m_width = drawSize.m_width;
		clip.m_height = drawHeight;
		AddToChangeList(clip);
	}
	LeaveCriticalSection((CRITICAL_SECTION*) m_lock);
}

// FUNCTION: LEMBALL 0x0046d420
void CSurface::Resize(const CVSSize& p_size)
{
	CVSRect rect(m_surfaceRect);
	rect.m_width = p_size.m_width;
	rect.m_height = p_size.m_height;
	if (m_changeList != NULL) {
		m_changeList->Resize(rect);
	}
	NewBitmap(rect);
	if (m_parentSurface == (CSurface*) g_pGdiHelperTarget) {
		if (HasBackBuff()) {
			ResizeBackBuff();
		}
	}
	if (m_parentSurface == (CSurface*) g_pGdiHelperTarget) {
		if (HasZBuff()) {
			ResizeZBuff();
		}
	}
	for (SurfaceListNode* node = m_childSurfaceHead; node != NULL; node = node->m_next) {
		CSurface* child = node->m_surface;
		CVSSize childSize(child->m_surfaceRect);
		child->Resize(childSize);
	}
}

// FUNCTION: LEMBALL 0x0046d560
void CSurface::MoveRel(const CVSPoint& p_delta)
{
	CVSRect* rect = &m_surfaceRect;
	rect->m_x += p_delta.m_x;
	rect->m_y += p_delta.m_y;
	Move(*rect);
}

// FUNCTION: LEMBALL 0x0046d5b0
void CSurface::Move(const CVSPoint& p_position)
{
	const CVSPoint& position = m_surfaceRect;
	CVSPoint delta(p_position.m_x - position.m_x, p_position.m_y - position.m_y);

	if (m_parentSurface != (CSurface*) g_pGdiHelperTarget) {
		CRITICAL_SECTION* lock = (CRITICAL_SECTION*) m_lock;
		EnterCriticalSection(lock);
		{
			CVSRect& surface = m_surfaceRect;
			surface.m_x = p_position.m_x;
			surface.m_y = p_position.m_y;
		}
		CVSRect oldRect(m_windowRect);
		{
			CVSRect& window = m_windowRect;
			const CVSRect& surface = m_surfaceRect;
			window.m_width = surface.m_width;
			window.m_height = surface.m_height;
			const CVSPoint& origin = surface;
			window.m_x = origin.m_x;
			window.m_y = origin.m_y;
		}

		const CVSSize& parentSize = m_parentSurface->m_windowRect;
		CVSRect& clipped = m_windowRect;
		short parentWidth = parentSize.m_width;
		short parentHeight = parentSize.m_height;

		short left = clipped.m_x;
		if (left < 0) {
			clipped.m_width += left;
			clipped.m_x = 0;
		}
		left = clipped.m_x;
		if (parentWidth < (short) (left + clipped.m_width)) {
			clipped.m_width = parentWidth - left;
		}
		short top = clipped.m_y;
		if (top < 0) {
			clipped.m_height += top;
			clipped.m_y = 0;
		}
		top = clipped.m_y;
		if (parentHeight < (short) (top + clipped.m_height)) {
			clipped.m_height = parentHeight - top;
		}
		if (clipped.m_width <= 0 || clipped.m_height <= 0) {
			clipped.m_height = 0;
			clipped.m_width = 0;
			clipped.m_y = 0;
			clipped.m_x = 0;
		}
		{
			const CVSSize& windowSize = m_windowRect;
			CVSSize& clipSize = m_clipRect;
			short height = windowSize.m_height;
			clipSize.m_width = windowSize.m_width;
			clipSize.m_height = height;
		}
		const CVSSize& newSize = m_windowRect;
		short height = newSize.m_height;
		if (newSize.m_width != oldRect.m_width || oldRect.m_height != height) {
			Resize(newSize);
		}
		if (m_platformBitmap != NULL) {
			g_pTargetGraphicsDriver->DestroyDibContext((CDibContext*) m_platformBitmap);
			m_platformBitmap = NULL;
		}
		m_bitmapPixelCount = 0;
		CreateLinePtrs();
		LeaveCriticalSection(lock);
		for (SurfaceListNode* node = m_childSurfaceHead; node != NULL; node = node->m_next) {
			node->m_surface->MoveRel(delta);
		}
	}
}

// FUNCTION: LEMBALL 0x0046d7e0
void CSurface::SetWindowPtr(void* p_platformPort)
{
	((CDrawingContext*) m_drawingPort)->SetDc(p_platformPort);
}

// FUNCTION: LEMBALL 0x0046d800
void CSurface::CopyDibBits(void* p_header, unsigned char* p_bits)
{
	if (m_changeList == NULL) {
		return;
	}
	EnterCriticalSection((CRITICAL_SECTION*) m_lock);
	BITMAPINFOHEADER* header = (BITMAPINFOHEADER*) p_header;
	int copyWidth = m_windowRect.m_width <= header->biWidth ? m_windowRect.m_width : header->biWidth;
	int copyHeight = m_windowRect.m_height <= header->biHeight ? m_windowRect.m_height : header->biHeight;
	int stride = (int) ((header->biBitCount * header->biWidth + 31) & ~31) / 8;
	unsigned char* source = p_bits + (header->biHeight - 1) * stride;
	int y = 0;
copyRow:
	if (y >= copyHeight) {
		goto copiedRows;
	}
	{
		memcpy(m_lines[y], source, copyWidth);
		copyHeight = m_windowRect.m_height <= header->biHeight ? m_windowRect.m_height : header->biHeight;
		source -= stride;
		y++;
		goto copyRow;
	}
copiedRows:
	CVSSize size(m_windowRect.m_width, m_windowRect.m_height);
	CVSRect rect(0, 0, &size);
	rect.m_x = rect.m_y = 0;
	m_changeList->Reset();
	AddToChangeList(rect);
	LeaveCriticalSection((CRITICAL_SECTION*) m_lock);
}

// FUNCTION: LEMBALL 0x0046d930
void CSurface::SetDefaultCtable()
{
	unsigned char logPalette[SURFACE_PALETTE_BUFFER_BYTES];
	LOGPALETTE* palette;
	PALETTEENTRY* entries;
	unsigned int* source;
	int i;
	SurfaceListNode* node;
	CSurface* surface;

	palette = (LOGPALETTE*) logPalette;
	palette->palVersion = SURFACE_PALETTE_VERSION_WIN3;
	palette->palNumEntries = SURFACE_PALETTE_ENTRY_COUNT;
	source = g_dwWinGDrawColourTable;
	i = 0;
	entries = palette->palPalEntry;
	while (i < palette->palNumEntries) {
		entries->peRed = ((unsigned char*) source)[2];
		entries->peGreen = ((unsigned char*) source)[1];
		entries->peBlue = ((unsigned char*) source)[0];
		entries->peFlags = PC_NOCOLLAPSE;
		entries++;
		i++;
		source++;
	}
	g_pTargetGraphicsDriver->CreatePalette(palette);
	node = (SurfaceListNode*) g_pSurfaceList->m_first;
	while (node != NULL) {
		surface = node->m_surface;
		memcpy(m_colourTable, g_dwWinGDrawColourTable, sizeof(m_colourTable));
		if (surface->m_drawingPort != NULL) {
			g_pTargetGraphicsDriver->UpdateDibColourTable((CDrawingContext*) surface->m_drawingPort,
														  0,
														  SURFACE_PALETTE_ENTRY_COUNT,
														  g_dwWinGDrawColourTable);
		}
		node = node->m_next;
	}
}

// FUNCTION: LEMBALL 0x0046d9f0
bool CSurface::BeginRender()
{
	if (m_lines == NULL) {
		return false;
	}
	if (m_parentSurface == (CSurface*) g_pGdiHelperTarget) {
		CDibContext* dib = (CDibContext*) m_platformBitmap;
		if (dib == NULL) {
			return false;
		}
		if (!dib->Lock()) {
			return false;
		}
		unsigned char* bits = ((CDibContext*) m_platformBitmap)->GetBits();
		if (bits != NULL && m_bitsBase != bits) {
			m_bitsBase = bits;
			CreateLinePtrs();
			return true;
		}
	}
	else {
		if (m_parentSurface == NULL) {
			return false;
		}
		if (!m_parentSurface->BeginRender()) {
			return false;
		}
		unsigned char* expected = (unsigned char*) m_parentSurface->m_lines[m_windowRect.m_y] + m_windowRect.m_x;
		if (expected != m_bitsBase) {
			CreateLinePtrs();
			return true;
		}
	}
	return true;
}

// FUNCTION: LEMBALL 0x0046daa0
void CSurface::EndRender()
{
	CSurface* current = this;
	for (;;) {
		if (current->m_parentSurface == (CSurface*) g_pGdiHelperTarget) {
			CDibContext* dib = (CDibContext*) current->m_platformBitmap;
			dib->Unlock();
			return;
		}
		current = current->m_parentSurface;
	}
}

// FUNCTION: LEMBALL 0x0046dbc0
void CSurface::Blit(CBigBitmap* p_primitive, CResBITMAP* p_bitmap)
{
	Blit(static_cast<CBitmap*>(p_primitive), p_bitmap);
}

// FUNCTION: LEMBALL 0x0046dc50
void CSurface::Flush()
{
	g_pGdiDevice->Flush(this);
}

// FUNCTION: LEMBALL 0x0046dc80
void* CSurface::GetCurrDB()
{
	return &m_currDb;
}

// FUNCTION: LEMBALL 0x00474c20
void CSurface::Blit(CScreenScroll* p_scroll)
{
	CVSRect rect = p_scroll->m_rect;
	CVSPoint dst = p_scroll->m_destination;

	if (HasBackBuff()) {
		CPVBackBuffSurface::m_bitmap.Scroll(&rect, &dst);
	}
	if (HasZBuff()) {
		CVSRect zrect(rect.m_x * 2, rect.m_y, rect.m_width * 2, rect.m_height);
		CVSPoint zdst;
		zdst.m_x = dst.m_x * 2;
		zdst.m_y = dst.m_y;
		CPVZBuffSurface::m_bitmap.Scroll(&zrect, &zdst);
	}
	CPVGDIBitmap::Scroll(&rect, &dst);
	AddToChangeList(rect);
}

// FUNCTION: LEMBALL 0x00474d40
void CSurface::Blit(CZBuffClear* p_clear)
{
	int startX;
	int height;
	int width = p_clear->m_bounds.m_width;
	height = p_clear->m_bounds.m_height;

	if (width == 0 || height == 0) {
		return;
	}
	startX = p_clear->m_bounds.m_x;
	int startY = p_clear->m_bounds.m_y;
	unsigned short depth = (unsigned short) p_clear->m_depth;
	if (height <= 0) {
		return;
	}
	do {
		unsigned short* dest = (unsigned short*) CPVZBuffSurface::m_bitmap.m_lines[startY] + startX;
		for (int i = 0; i < width; i++) {
			dest[i] = depth;
		}
		startY++;
		height--;
	} while (height != 0);
}

// FUNCTION: LEMBALL 0x00474dc0
void CSurface::Blit(CZBuffScroll* p_scroll)
{
}

// FUNCTION: LEMBALL 0x00474dd0
void CSurface::Blit(CCopyToBackBuff* p_copy)
{
	CCopyToBackBuff* primitive = p_copy;
	int width = primitive->m_destination.m_width;
	int height = primitive->m_destination.m_height;
	if (width != 0 && height != 0) {
		int dstX = primitive->m_destination.m_x;
		int srcX = primitive->m_x;
		int dstY = primitive->m_destination.m_y;
		int srcY = primitive->m_y;
		if (height > 0) {
			int count = height;
			do {
				unsigned char* dst = (unsigned char*) CPVBackBuffSurface::m_bitmap.m_lines[dstY] + dstX;
				unsigned char* src = (unsigned char*) m_lines[srcY] + srcX;
				memcpy(dst, src, width);
				srcY++;
				dstY++;
				count--;
			} while (count != 0);
		}
	}
}

// FUNCTION: LEMBALL 0x00474e60
void CSurface::Blit(CCopyColourToBackBuff* p_fill)
{
	int startX;
	int startY;
	int width = p_fill->m_bounds.m_width;
	int height = p_fill->m_bounds.m_height;

	if (width == 0 || height == 0) {
		return;
	}
	startX = p_fill->m_bounds.m_x;
	startY = p_fill->m_bounds.m_y;
	int colour = p_fill->m_colour;
	if (height <= 0) {
		return;
	}
	do {
		unsigned char* dest = (unsigned char*) CPVBackBuffSurface::m_bitmap.m_lines[startY] + startX;
		memset(dest, colour, width);
		startY++;
		height--;
	} while (height != 0);
}

// FUNCTION: LEMBALL 0x00474ee0
void CSurface::CopyBackBuffToScreen(const CVSRect& p_rect)
{
	short height = p_rect.m_height;
	short width = p_rect.m_width;

	if ((int) height * (int) width != 0) {
		CVSRect rect(p_rect);
		if ((int) (short) (rect.m_x + rect.m_width) > (int) CPVBackBuffSurface::m_allocatedWidth) {
			rect.m_width = (short) (CPVBackBuffSurface::m_allocatedWidth - rect.m_x);
		}
		if ((int) (short) (rect.m_height + rect.m_y) > (int) CPVBackBuffSurface::m_allocatedHeight) {
			rect.m_height = (short) (CPVBackBuffSurface::m_allocatedHeight - rect.m_y);
		}
		const CVSPoint* origin = &rect;
		int x = origin->m_x;
		int y = origin->m_y;
		for (int i = 0; i < rect.m_height; i++) {
			memcpy((unsigned char*) m_lines[y + i] + x,
				   (unsigned char*) CPVBackBuffSurface::m_bitmap.m_lines[y + i] + x,
				   rect.m_width);
		}
	}
}

#undef SURFACE_PALETTE_ENTRY_COUNT
#undef SURFACE_PALETTE_VERSION_WIN3
#undef SURFACE_PALETTE_SYSTEM_RESERVED_LOW_COUNT
#undef SURFACE_PALETTE_SYSTEM_RESERVED_HIGH_START
#undef SURFACE_PALETTE_SYSTEM_USABLE_ENTRY_COUNT
#undef SURFACE_PALETTE_APP_FIRST_INDEX
#undef SURFACE_PALETTE_SPECIAL_OUTPUT_ENTRY_COUNT
#undef SURFACE_PALETTE_RESOURCE_ENTRY_STRIDE_BYTES
#undef SURFACE_PALETTE_OUTPUT_ENTRY_STRIDE_BYTES
#undef SURFACE_PALETTE_HIGH_RESERVED_RED_OFFSET_BYTES
#undef SURFACE_PALETTE_HIGH_RESERVED_GREEN_OFFSET_BYTES
#undef SURFACE_PALETTE_HIGH_RESERVED_BLUE_OFFSET_BYTES
#undef SURFACE_PALETTE_HIGH_RESERVED_FLAGS_OFFSET_BYTES
#undef SURFACE_PALETTE_BUFFER_BYTES

// FUNCTION: LEMBALL 0x00474fd0
void CSurface::Blit(CPoint* p_point)
{
	const short& x = p_point->m_x;
	int colour = p_point->m_colour;
	if (m_clipRect.m_x <= x && x < (short) (m_clipRect.m_width + m_clipRect.m_x)) {
		if (m_clipRect.m_y <= p_point->m_y && p_point->m_y < (short) (m_clipRect.m_height + m_clipRect.m_y)) {
			*((unsigned char*) m_lines[p_point->m_y] + x) = (unsigned char) colour;
			CVSRect rect(p_point->m_x, p_point->m_y, 1, 1);
			AddToChangeList(rect);
		}
	}
}

// FUNCTION: LEMBALL 0x00475080
void CSurface::Blit(CSolidRect* p_rect)
{
	BlitRect(*p_rect->GetBounds(), p_rect->m_colour);
}

// FUNCTION: LEMBALL 0x004750c0
void CSurface::Blit(CLine* p_line)
{
	int y2 = p_line->m_end.m_y;
	int x2 = p_line->m_end.m_x;
	int colour = p_line->m_colour;
	int y1 = p_line->m_start.m_y;
	int x1 = p_line->m_start.m_x;
	if (x2 < x1) {
		int x = x1;
		x1 = x2;
		x2 = x;
		int y = y1;
		y1 = y2;
		y2 = y;
	}
	CSurface* surface = this;
	if (surface->LineClip(x1, y1, x2, y2) != 0) {
		return;
	}
	int endY = y2;
	int y = y1;
	int x = x1;
	int dx = x2 - x;
	int remaining = endY - y;
	int stepY = 1;
	int absDy;
	if (remaining < 0) {
		stepY = SURFACE_STEP_BACKWARD;
		absDy = -remaining;
	}
	else {
		absDy = remaining;
	}
	if (absDy < dx) {
		int doubleDx = dx * 2;
		int doubleDy = absDy * 2;
		remaining = dx;
		int err = 0;
		if (0 < dx) {
			do {
				remaining = remaining - 1;
				x = x + 1;
				err = err + doubleDy;
				*((unsigned char*) m_lines[y] + (x - 1)) = (unsigned char) colour;
				if (dx < err) {
					y = y + stepY;
					err = err - doubleDx;
				}
			} while (remaining != 0);
		}
	}
	else {
		int doubleDy = absDy * 2;
		int doubleDx = dx * 2;
		int err = 0;
		if (stepY != 1) {
			remaining = y - endY;
		}
		if (0 < remaining) {
			do {
				remaining = remaining - 1;
				err = err + doubleDx;
				*((unsigned char*) m_lines[y] + x) = (unsigned char) colour;
				y = y + stepY;
				if (absDy < err) {
					x = x + 1;
					err = err - doubleDy;
				}
			} while (remaining != 0);
		}
	}
	surface->AddToChangeList(
		CVSRect((short) x1, (short) min(y1, y2), (short) (x2 - x1 + 1), (short) (abs(y2 - y1) + 1)));
}

// FUNCTION: LEMBALL 0x00475290
void CSurface::Blit(CCircle* p_circle)
{
	int colour = p_circle->m_colour;
	int centreY = p_circle->m_y;
	int centreX = p_circle->m_x;
	int radius = abs((int) p_circle->m_radius);
	int clipResult = ClipCircle(centreX, centreY, radius);
	if (clipResult != CIRCLE_OUTSIDE_CLIP) {
		switch (clipResult) {
		case CIRCLE_FULLY_INSIDE_CLIP: {
			int curX = 0;
			int curY = radius;
			int err = 0;
			int step = 1;
			int errLimit = radius * 2 - 1;
			*((unsigned char*) m_lines[centreY + radius] + centreX) = (unsigned char) colour;
			*((unsigned char*) m_lines[centreY - radius] + centreX) = (unsigned char) colour;
			unsigned char* centrePixel = (unsigned char*) m_lines[centreY] + centreX;
			centrePixel[radius] = (unsigned char) colour;
			*((unsigned char*) m_lines[centreY] - radius + centreX) = (unsigned char) colour;
			while (curX < curY) {
				curX++;
				err += step;
				step += 2;
				if (err * 2 > errLimit) {
					curY--;
					err -= errLimit;
					errLimit -= 2;
				}
				if (curX <= curY) {
					DrawCircleSymmetricPoints(centreX, centreY, curX, curY, colour);
					if (curX < curY) {
						DrawCircleSymmetricPoints(centreX, centreY, curY, curX, colour);
					}
				}
			}
			break;
		}
		case CIRCLE_PARTIALLY_CLIPPED:
			DrawClippedCircleOutline(centreX, centreY, radius, colour);
			break;
		}
		centreX -= radius;
		int boundY = centreY - radius;
		int boundW = radius * 2 + 1;
		int boundH = boundW;
		if (centreX < (int) m_clipRect.m_x) {
			boundW += centreX - m_clipRect.m_x;
			centreX = m_clipRect.m_x;
		}
		if ((int) m_clipRect.m_x + (int) m_clipRect.m_width - 1 < centreX + boundW) {
			boundW = (m_clipRect.m_x + m_clipRect.m_width) - centreX;
		}
		if (boundY < (int) m_clipRect.m_y) {
			boundH += boundY - m_clipRect.m_y;
			boundY = m_clipRect.m_y;
		}
		if ((int) m_clipRect.m_y + (int) m_clipRect.m_height - 1 < boundY + boundH) {
			boundH = (m_clipRect.m_y + m_clipRect.m_height) - boundY;
		}
		AddToChangeList(CVSRect((short) centreX, (short) boundY, (short) boundW, (short) boundH));
	}
}

// FUNCTION: LEMBALL 0x00475490
void CSurface::Blit(CFilledCircle* p_circle)
{
	int colour = p_circle->m_colour;
	int y = p_circle->m_y;
	int x = p_circle->m_x;
	int radius = abs((int) p_circle->m_radius);
	int clipResult = ClipCircle(x, y, radius);
	if (clipResult != CIRCLE_OUTSIDE_CLIP) {
		switch (clipResult) {
		case CIRCLE_FULLY_INSIDE_CLIP: {
			int curRadius;
			int curX = 0;
			curRadius = radius;
			int err = 0;
			int step = 1;
			int errLimit = radius * 2 - 1;
			*((unsigned char*) m_lines[y + radius] + x) = (unsigned char) colour;
			*((unsigned char*) m_lines[y - radius] + x) = (unsigned char) colour;
			memset((unsigned char*) m_lines[y] - radius + x, colour, radius * 2 + 1);
			if (radius > 1) {
				while (curX < curRadius) {
					int changed = 0;
					curX++;
					err += step;
					step += 2;
					if (err * 2 > errLimit) {
						changed = 1;
						curRadius--;
						err -= errLimit;
						errLimit -= 2;
					}
					if (curX <= curRadius) {
						if (changed) {
							DrawCircleSpans(x, y, curX, curRadius, colour);
						}
						if (curX < curRadius) {
							DrawCircleSpans(x, y, curRadius, curX, colour);
						}
					}
				}
			}
			break;
		}
		case CIRCLE_PARTIALLY_CLIPPED:
			DrawClippedFilledCircle(x, y, radius, colour);
			break;
		}
		int minX = x - radius;
		int minY = y - radius;
		int width = radius * 2 + 1;
		int height = width;
		if (minX < (int) m_clipRect.m_x) {
			width += minX - m_clipRect.m_x;
			minX = m_clipRect.m_x;
		}
		if (m_clipRect.m_x + m_clipRect.m_width - 1 < minX + width) {
			width = m_clipRect.m_x + m_clipRect.m_width - minX;
		}
		if (minY < (int) m_clipRect.m_y) {
			height += minY - m_clipRect.m_y;
			minY = m_clipRect.m_y;
		}
		if (m_clipRect.m_y + m_clipRect.m_height - 1 < minY + height) {
			height = m_clipRect.m_y + m_clipRect.m_height - minY;
		}
		AddToChangeList(CVSRect((short) minX, (short) minY, (short) width, (short) height));
	}
}

// FUNCTION: LEMBALL 0x004756e0
void CSurface::BlitRect(CVSRect p_rect, int p_colour)
{
	short storage[4];
	CVSRect& clipped = *(CVSRect*) storage;
	clipped.m_width = clipped.m_height = 0;
	clipped.m_x = clipped.m_y = 0;
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
	AddToChangeList(p_rect);
}

// FUNCTION: LEMBALL 0x004757a0
int CSurface::LineClip(int& p_x1, int& p_y1, int& p_x2, int& p_y2)
{
	unsigned int code1;
	unsigned int code2;
	int coordinate;

	if (m_clipRect.m_height <= 0 || m_clipRect.m_width <= 0) {
		return 1;
	}
	code1 = LINE_CLIP_REGION_INSIDE;
	coordinate = p_x1;
	if (coordinate < m_clipRect.m_x) {
		code1 = LINE_CLIP_REGION_LEFT;
	}
	else if (m_clipRect.m_width + m_clipRect.m_x - 1 < coordinate) {
		code1 = LINE_CLIP_REGION_RIGHT;
	}
	coordinate = p_y1;
	if (coordinate < m_clipRect.m_y) {
		code1 |= LINE_CLIP_REGION_TOP;
	}
	else if (m_clipRect.m_height + m_clipRect.m_y - 1 < coordinate) {
		code1 |= LINE_CLIP_REGION_BOTTOM;
	}
	code2 = LINE_CLIP_REGION_INSIDE;
	coordinate = p_x2;
	if (coordinate < m_clipRect.m_x) {
		code2 = LINE_CLIP_REGION_LEFT;
	}
	else if (m_clipRect.m_width + m_clipRect.m_x - 1 < coordinate) {
		code2 = LINE_CLIP_REGION_RIGHT;
	}
	coordinate = p_y2;
	if (coordinate < m_clipRect.m_y) {
		code2 |= LINE_CLIP_REGION_TOP;
	}
	else if (m_clipRect.m_height + m_clipRect.m_y - 1 < coordinate) {
		code2 |= LINE_CLIP_REGION_BOTTOM;
	}
	if ((code1 | code2) != LINE_CLIP_REGION_INSIDE) {
		do {
			if ((code1 & code2) != 0) {
				return 1;
			}
			const int x2 = p_x2;
			const int x1 = p_x1;
			const int dx = x2 - x1;
			const int y2 = p_y2;
			const int y1 = p_y1;
			const int dy = y2 - y1;
			if (code1 != LINE_CLIP_REGION_INSIDE) {
				if ((code1 & LINE_CLIP_REGION_LEFT) == 0) {
					if ((code1 & LINE_CLIP_REGION_RIGHT) != 0) {
						p_y1 = y1 + ((m_clipRect.m_x + m_clipRect.m_width - 1 - x1) * dy) / dx;
						p_x1 = m_clipRect.m_x + m_clipRect.m_width - 1;
					}
					else {
						if ((code1 & LINE_CLIP_REGION_TOP) == 0) {
							if ((code1 & LINE_CLIP_REGION_BOTTOM) != 0) {
								p_x1 = x1 + ((m_clipRect.m_y + m_clipRect.m_height - 1 - y1) * dx) / dy;
								p_y1 = m_clipRect.m_y + m_clipRect.m_height - 1;
							}
						}
						else {
							p_x1 = x1 + ((m_clipRect.m_y - y1) * dx) / dy;
							p_y1 = m_clipRect.m_y;
						}
					}
				}
				else {
					p_y1 = y1 + ((m_clipRect.m_x - x1) * dy) / dx;
					p_x1 = m_clipRect.m_x;
				}
				code1 = LINE_CLIP_REGION_INSIDE;
				if (p_x1 < m_clipRect.m_x) {
					code1 = LINE_CLIP_REGION_LEFT;
				}
				else if (m_clipRect.m_width + m_clipRect.m_x - 1 < p_x1) {
					code1 = LINE_CLIP_REGION_RIGHT;
				}
				if (p_y1 < m_clipRect.m_y) {
					code1 |= LINE_CLIP_REGION_TOP;
				}
				else if (m_clipRect.m_height + m_clipRect.m_y - 1 < p_y1) {
					code1 |= LINE_CLIP_REGION_BOTTOM;
				}
			}
			else {
				if ((code2 & LINE_CLIP_REGION_LEFT) == 0) {
					if ((code2 & LINE_CLIP_REGION_RIGHT) != 0) {
						p_y2 = y2 + ((m_clipRect.m_x + m_clipRect.m_width - 1 - x2) * dy) / dx;
						p_x2 = m_clipRect.m_x + m_clipRect.m_width - 1;
					}
					else {
						if ((code2 & LINE_CLIP_REGION_TOP) == 0) {
							if ((code2 & LINE_CLIP_REGION_BOTTOM) != 0) {
								p_x2 = x2 + ((m_clipRect.m_y + m_clipRect.m_height - 1 - y2) * dx) / dy;
								p_y2 = m_clipRect.m_y + m_clipRect.m_height - 1;
							}
						}
						else {
							p_x2 = x2 + ((m_clipRect.m_y - y2) * dx) / dy;
							p_y2 = m_clipRect.m_y;
						}
					}
				}
				else {
					p_y2 = y2 + ((m_clipRect.m_x - x2) * dy) / dx;
					p_x2 = m_clipRect.m_x;
				}
				code2 = LINE_CLIP_REGION_INSIDE;
				if (p_x2 < m_clipRect.m_x) {
					code2 = LINE_CLIP_REGION_LEFT;
				}
				else if (m_clipRect.m_width + m_clipRect.m_x - 1 < p_x2) {
					code2 = LINE_CLIP_REGION_RIGHT;
				}
				if (p_y2 < m_clipRect.m_y) {
					code2 |= LINE_CLIP_REGION_TOP;
				}
				else if (m_clipRect.m_height + m_clipRect.m_y - 1 < p_y2) {
					code2 |= LINE_CLIP_REGION_BOTTOM;
				}
			}
		} while ((code1 | code2) != LINE_CLIP_REGION_INSIDE);
	}
	return 0;
}

// FUNCTION: LEMBALL 0x00475bc0
int CSurface::ClipCircle(int p_centreX, int p_centreY, int p_radius)
{
	int left = p_centreX - p_radius;
	int top = p_centreY - p_radius;
	int right = p_centreX + p_radius;
	int bottom = p_centreY + p_radius;

	if (m_clipRect.m_height <= 0 || m_clipRect.m_width <= 0) {
		return CIRCLE_OUTSIDE_CLIP;
	}
	if (right >= m_clipRect.m_x && left <= ((int) m_clipRect.m_x + (int) m_clipRect.m_width - 1) &&
		bottom >= m_clipRect.m_y && top <= ((int) m_clipRect.m_y + (int) m_clipRect.m_height - 1)) {
		if (left >= m_clipRect.m_x && right <= ((int) m_clipRect.m_x + (int) m_clipRect.m_width - 1) &&
			top >= m_clipRect.m_y && bottom <= ((int) m_clipRect.m_y + (int) m_clipRect.m_height - 1)) {
			return CIRCLE_FULLY_INSIDE_CLIP;
		}
		return CIRCLE_PARTIALLY_CLIPPED;
	}
	return CIRCLE_OUTSIDE_CLIP;
}

// FUNCTION: LEMBALL 0x00475ce0
void CSurface::DrawClippedCircleOutline(int p_centreX, int p_centreY, int p_radius, unsigned char p_colour)
{
	int x = 0;
	int y = p_radius;
	int err = 0;
	int step = 1;
	int errLimit = y * 2 - 1;

	if (m_clipRect.m_x <= p_centreX && m_clipRect.m_x + m_clipRect.m_width - 1 >= p_centreX) {
		if (m_clipRect.m_y <= p_centreY + p_radius &&
			m_clipRect.m_y + m_clipRect.m_height - 1 >= p_centreY + p_radius) {
			*((unsigned char*) m_lines[p_centreY + p_radius] + p_centreX) = p_colour;
		}
	}
	if (m_clipRect.m_x <= p_centreX && m_clipRect.m_x + m_clipRect.m_width - 1 >= p_centreX) {
		if (m_clipRect.m_y <= p_centreY - p_radius &&
			m_clipRect.m_y + m_clipRect.m_height - 1 >= p_centreY - p_radius) {
			*((unsigned char*) m_lines[p_centreY - p_radius] + p_centreX) = p_colour;
		}
	}
	if (m_clipRect.m_x <= p_centreX + p_radius && m_clipRect.m_x + m_clipRect.m_width - 1 >= p_centreX + p_radius) {
		int clipY = m_clipRect.m_y;
		if (clipY <= p_centreY && clipY + m_clipRect.m_height - 1 >= p_centreY) {
			unsigned char* destination = (unsigned char*) m_lines[p_centreY] + p_centreX;
			destination[p_radius] = p_colour;
		}
	}
	if (m_clipRect.m_x <= p_centreX - p_radius && m_clipRect.m_x + m_clipRect.m_width - 1 >= p_centreX - p_radius) {
		int clipY = m_clipRect.m_y;
		if (clipY <= p_centreY && clipY + m_clipRect.m_height - 1 >= p_centreY) {
			*((unsigned char*) m_lines[p_centreY] + p_centreX - p_radius) = p_colour;
		}
	}
	if (p_radius <= 0) {
		return;
	}
	do {
		x = x + 1;
		err = err + step;
		step = step + 2;
		if (errLimit < err * 2) {
			y = y - 1;
			err = err - errLimit;
			errLimit = errLimit - 2;
		}
		if (y < x) {
			continue;
		}
		if (ClipCirclePoint(p_centreX + x, p_centreY + y) != 0) {
			unsigned char* destination = (unsigned char*) m_lines[p_centreY + y] + p_centreX;
			destination[x] = p_colour;
		}
		if (ClipCirclePoint(p_centreX - x, p_centreY + y) != 0) {
			*((unsigned char*) m_lines[p_centreY + y] + p_centreX - x) = p_colour;
		}
		if (ClipCirclePoint(p_centreX + x, p_centreY - y) != 0) {
			unsigned char* destination = (unsigned char*) m_lines[p_centreY - y] + p_centreX;
			destination[x] = p_colour;
		}
		if (ClipCirclePoint(p_centreX - x, p_centreY - y) != 0) {
			*((unsigned char*) m_lines[p_centreY - y] + p_centreX - x) = p_colour;
		}
		if (y > x) {
			DrawClippedCirclePoint(p_centreX, p_centreY, y, x, p_colour);
		}
	} while (y > x);
}

// FUNCTION: LEMBALL 0x00475f60
int CSurface::ClipCirclePoint(int p_x, int p_y)
{
	int clipX;
	int clipRight;
	int clipY;
	int clipBottom;

	clipX = CPVScrollableSurface::m_clipRect.m_x;
	if (clipX <= p_x) {
		clipRight = CPVScrollableSurface::m_clipRect.m_width + clipX - 1;
		if (p_x <= clipRight) {
			clipY = CPVScrollableSurface::m_clipRect.m_y;
			if (clipY <= p_y) {
				clipBottom = CPVScrollableSurface::m_clipRect.m_height + clipY - 1;
				if (p_y <= clipBottom) {
					return 1;
				}
			}
		}
	}
	return 0;
}

// FUNCTION: LEMBALL 0x00475fb0
void CSurface::DrawClippedCirclePoint(int p_centreX,
									  int p_centreY,
									  int p_xOffset,
									  int p_yOffset,
									  unsigned char p_colour)
{
	if (m_clipRect.m_x <= (p_centreX + p_xOffset) &&
		(p_centreX + p_xOffset) <= m_clipRect.m_x + m_clipRect.m_width - 1) {
		if (m_clipRect.m_y <= (p_centreY + p_yOffset) &&
			(p_centreY + p_yOffset) <= m_clipRect.m_y + m_clipRect.m_height - 1) {
			*((unsigned char*) m_lines[(p_centreY + p_yOffset)] + (p_centreX + p_xOffset)) = p_colour;
		}
	}
	if (m_clipRect.m_x <= (p_centreX - p_xOffset) &&
		(p_centreX - p_xOffset) <= m_clipRect.m_x + m_clipRect.m_width - 1) {
		if (m_clipRect.m_y <= (p_centreY + p_yOffset) &&
			(p_centreY + p_yOffset) <= m_clipRect.m_y + m_clipRect.m_height - 1) {
			*((unsigned char*) m_lines[(p_centreY + p_yOffset)] + (p_centreX - p_xOffset)) = p_colour;
		}
	}
	if (m_clipRect.m_x <= (p_centreX + p_xOffset) &&
		(p_centreX + p_xOffset) <= m_clipRect.m_x + m_clipRect.m_width - 1) {
		if (m_clipRect.m_y <= (p_centreY - p_yOffset) &&
			(p_centreY - p_yOffset) <= m_clipRect.m_y + m_clipRect.m_height - 1) {
			*((unsigned char*) m_lines[(p_centreY - p_yOffset)] + (p_centreX + p_xOffset)) = p_colour;
		}
	}
	if (m_clipRect.m_x <= (p_centreX - p_xOffset) &&
		(p_centreX - p_xOffset) <= m_clipRect.m_x + m_clipRect.m_width - 1) {
		if (m_clipRect.m_y <= (p_centreY - p_yOffset) &&
			(p_centreY - p_yOffset) <= m_clipRect.m_y + m_clipRect.m_height - 1) {
			*((unsigned char*) m_lines[(p_centreY - p_yOffset)] + (p_centreX - p_xOffset)) = p_colour;
		}
	}
}

// FUNCTION: LEMBALL 0x00476100
void CSurface::DrawCircleSpans(int p_centreX, int p_centreY, int p_halfWidth, int p_yOffset, int p_colour)
{
	int spanWidth = p_halfWidth * 2 + 1;
	unsigned char* negativeSpan = (unsigned char*) m_lines[p_centreY - p_yOffset] + p_centreX - p_halfWidth;
	memset((unsigned char*) m_lines[p_centreY + p_yOffset] + p_centreX - p_halfWidth, p_colour, spanWidth);
	memset(negativeSpan, p_colour, spanWidth);
}

// FUNCTION: LEMBALL 0x00476190
void CSurface::DrawClippedFilledCircle(int p_centreX, int p_centreY, int p_radius, int p_colour)
{
	int x;
	int err;
	int step;
	int errLimit;
	int poleY;
	int x1;
	int x2;
	int changed;
	int doubleErr;
	int yTop;
	int yBottom;
	int clipY;
	int xLeft;
	int xRight;
	int clipX;
	x = 0;
	err = 0;
	step = 1;
	errLimit = p_radius * 2 - 1;

	if (p_centreX >= m_clipRect.m_x && p_centreX <= (m_clipRect.m_width + m_clipRect.m_x - 1)) {
		clipY = m_clipRect.m_y;
		poleY = p_centreY + p_radius;
		if (poleY >= clipY && poleY <= (m_clipRect.m_height + clipY - 1)) {
			*((unsigned char*) m_lines[poleY] + p_centreX) = (unsigned char) p_colour;
		}
	}
	if (p_centreX >= m_clipRect.m_x && p_centreX <= (m_clipRect.m_width + m_clipRect.m_x - 1)) {
		if ((p_centreY - p_radius) >= m_clipRect.m_y &&
			(p_centreY - p_radius) <= (m_clipRect.m_height + m_clipRect.m_y - 1)) {
			*((unsigned char*) m_lines[(p_centreY - p_radius)] + p_centreX) = (unsigned char) p_colour;
		}
	}
	if (p_centreY >= m_clipRect.m_y && p_centreY <= (m_clipRect.m_height + m_clipRect.m_y - 1)) {
		x1 = p_centreX - p_radius;
		x2 = p_centreX + p_radius;
		if (x1 < m_clipRect.m_x) {
			x1 = m_clipRect.m_x;
		}
		if ((m_clipRect.m_width + m_clipRect.m_x - 1) < x2) {
			x2 = m_clipRect.m_width + m_clipRect.m_x - 1;
		}
		memset((unsigned char*) m_lines[p_centreY] + x1, p_colour, x2 - x1 + 1);
	}

	if (p_radius > 1) {
		while (x < p_radius) {
			changed = 0;
			x++;
			err += step;
			step += 2;
			doubleErr = err * 2;
			if (errLimit < doubleErr) {
				p_radius--;
				changed = 1;
				err -= errLimit;
				errLimit -= 2;
			}
			if (x <= p_radius) {
				if (changed != 0) {
					yTop = p_centreY - p_radius;
					yBottom = p_centreY + p_radius;
					clipY = m_clipRect.m_y;
					if (yTop <= (m_clipRect.m_height + clipY - 1) && yBottom >= clipY) {
						xLeft = p_centreX - x;
						xRight = p_centreX + x;
						clipX = m_clipRect.m_x;
						if (clipX <= xRight && (m_clipRect.m_width + clipX - 1) >= xLeft) {
							if (xRight > (m_clipRect.m_width + clipX - 1)) {
								xRight = m_clipRect.m_width + clipX - 1;
							}
							if (xLeft < clipX) {
								xLeft = clipX;
							}
							if (yTop >= clipY) {
								memset((unsigned char*) m_lines[yTop] + xLeft, p_colour, xRight - xLeft + 1);
							}
							if (yBottom <= (m_clipRect.m_height + m_clipRect.m_y - 1)) {
								memset((unsigned char*) m_lines[yBottom] + xLeft, p_colour, xRight - xLeft + 1);
							}
						}
					}
				}
				if (x < p_radius) {
					FilledCircleClipPoints(p_centreX, p_centreY, p_radius, x, p_colour);
				}
			}
		}
	}
}

// FUNCTION: LEMBALL 0x00476470
void CSurface::FilledCircleClipPoints(int p_centreX, int p_centreY, int p_xOffset, int p_yOffset, int p_colour)
{
	int y1 = p_centreY - p_yOffset;
	int y2 = p_centreY + p_yOffset;
	if (y1 <= (m_clipRect.m_height + m_clipRect.m_y - 1) && m_clipRect.m_y <= y2) {
		int x1 = p_centreX - p_xOffset;
		int x2 = p_centreX + p_xOffset;
		int clipX = m_clipRect.m_x;
		if (clipX <= x2 && x1 <= (m_clipRect.m_width + clipX - 1)) {
			if ((m_clipRect.m_width + clipX - 1) < x2) {
				x2 = m_clipRect.m_width + clipX - 1;
			}
			if (x1 < clipX) {
				x1 = clipX;
			}
			if (m_clipRect.m_y <= y1) {
				memset((unsigned char*) m_lines[y1] + x1, p_colour, x2 - x1 + 1);
			}
			if (y2 <= (m_clipRect.m_height + m_clipRect.m_y - 1)) {
				memset((unsigned char*) m_lines[y2] + x1, p_colour, x2 - x1 + 1);
			}
		}
	}
}

// FUNCTION: LEMBALL 0x00476580
bool CSurface::ClipRect(CVSRect& p_rect, CVSRect* p_clipped)
{
	bool clipped = false;
	short clipX = m_clipRect.m_x;
	short rectX = p_rect.m_x;

	if ((short) (m_clipRect.m_width + clipX) < rectX || (short) (m_clipRect.m_height + m_clipRect.m_y) < p_rect.m_y ||
		(short) (p_rect.m_width + rectX) < clipX || (short) (p_rect.m_height + p_rect.m_y) < m_clipRect.m_y) {
		return true;
	}

	if (m_clipRect.m_x > p_rect.m_x) {
		p_clipped->m_x = m_clipRect.m_x - p_rect.m_x;
		short clippedX = m_clipRect.m_x;
		p_rect.m_x = clippedX;
		clipped = true;
		p_rect.m_width -= p_clipped->m_x;
	}

	if (m_clipRect.m_y > p_rect.m_y) {
		p_clipped->m_y = m_clipRect.m_y - p_rect.m_y;
		short clippedY = m_clipRect.m_y;
		p_rect.m_y = clippedY;
		clipped = true;
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
		int skipRows = (p_zrle->m_size.m_height - p_clip.m_y) - p_rect.m_height;
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
	short sourceWidth = p_zrle->m_size.m_width;
	short sourceHeight = p_zrle->m_size.m_height;
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
		int skipRows = (p_zrle->m_size.m_height - p_clip.m_y) - p_rect.m_height;
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
	short zrleWidth = p_zrle->m_size.m_width;
	short zrleHeight = p_zrle->m_size.m_height;
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

		if ((int) p_zrle->m_size.m_height * (int) p_zrle->m_size.m_width == 0) {
			return;
		}
		{
			CVSRect dest((short) primitiveX, (short) primitiveY, &p_zrle->m_size);
			if ((flags & ZRLE_DRAW_FLAG_ABSOLUTE_POSITION) == 0) {
				static_cast<CVSPoint&>(dest).AddInPlace(&p_zrle->m_rasterPoint);
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
		sourceRect.m_width = p_bitmap->m_rasterPoint.m_x;
		sourceRect.m_height = p_bitmap->m_rasterPoint.m_y;
	}
	unsigned int flags = p_primitive->m_flags;
	if ((int) p_bitmap->m_rasterPoint.m_y * (int) p_bitmap->m_rasterPoint.m_x != 0) {
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
		int bitmapWidth = (int) p_bitmap->m_rasterPoint.m_x;
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
	short warningHeight;
	CResZRLE* resource;
	short zHeight;
	short zWidth;
	unsigned int flags;
	int width;

	resource = p_zrle;
	zWidth = resource->m_size.m_width;
	zHeight = resource->m_size.m_height;
	width = (int) zWidth;
	if ((int) zHeight * width == 0) {
		return;
	}
	CVSRect destination((short) p_x, (short) p_y, zWidth, zHeight);
	flags = p_flags;
	CVSRect* dest = &destination;
	if ((flags & ZRLE_DRAW_FLAG_ABSOLUTE_POSITION) == 0) {
		dest->m_x = (short) (dest->m_x + resource->m_rasterPoint.m_x);
		dest->m_y = (short) (dest->m_y + resource->m_rasterPoint.m_y);
	}
	CVSRect clip;
	CVSRect* clipped = &clip;
	if (dest->m_width > ZRLE_CLIPPED_DIMENSION_MAX || dest->m_height > ZRLE_CLIPPED_DIMENSION_MAX) {
		CVSOStream& warning = *g_pDebugOutput << g_szWarningZrleIs;
		warningHeight = dest->m_height;
		CVSOStream& heightOutput = warning << width << g_szClippingWideAnd;
		heightOutput << (int) warningHeight << g_szClippingHighNewline;
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
