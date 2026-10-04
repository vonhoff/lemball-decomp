#include "CSurface.h"

#include "CChangeList.h"
#include "Visos/Streams/CVSOStream.h"
#include "Visos/Math/CVSPoint.h"
#include "Visos/Diagnostics/VsDebug.h"
#include "Visos/Resources/Types/CResBITMAP.h"
#include "Visos/Resources/Types/CResPALETTE.h"
#include "Visos/Resources/Types/CResZRLE.h"
#include "Platform/Windows/Graphics/CDibContext.h"
#include "Platform/Windows/Graphics/CDrawingContext.h"
#include "Platform/Windows/Graphics/CGdiContext.h"
#include "Platform/Windows/Graphics/CGraphicsDriver.h"
#include "Visos/Graphics/Primitives/CBigBitmap.h"
#include "Visos/Graphics/Primitives/CBitmap.h"
#include "Visos/Graphics/Primitives/CCircle.h"
#include "Visos/Graphics/Primitives/CClipRect.h"
#include "Visos/Graphics/Primitives/CCopyToBackBuff.h"
#include "Visos/Graphics/Primitives/CFilledCircle.h"
#include "CGDIDevice.h"
#include "Visos/Graphics/Primitives/CLine.h"
#include "Visos/Graphics/Primitives/CPoint.h"
#include "Visos/Graphics/Palettes/CRemap.h"
#include "Visos/Graphics/Primitives/CScreenScroll.h"
#include "Visos/Graphics/Primitives/CSolidRect.h"
#include "Visos/Graphics/Primitives/CZBuffClear.h"
#include "Visos/Graphics/Primitives/CZRLE.h"

#include <stdlib.h>
#include <string.h>

#define WIN32_LEAN_AND_MEAN
#include "Visos/Math/CVSRect.h"
#include "Visos/Math/CVSSize.h"
#include "ChangeListItem.h"
#include "CPVBackBuffSurface.h"
#include "CPVGDIBitmap.h"
#include "CPVScrollableSurface.h"
#include "CPVZBuffSurface.h"

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

#include "Visos/Graphics/Primitives/CCopyColourToBackBuff.h"

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
		head = (SurfaceListHead*) operator new(0xc);
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
		int paletteCount = (int) p_palette->m_entryCount - SURFACE_PALETTE_SYSTEM_RESERVED_LOW_COUNT;
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
		if (0 < m_height) {
			do {
				m_lines[y] = (void*) ((int) CPVScrollableSurface::m_parentSurface->m_lines[parentY] +
									  (int) CPVScrollableSurface::m_windowRect.m_x);
				y = y + 1;
				parentY = parentY + 1;
			} while (y < m_height);
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
	CVSRect rect = CVSRect(0, 0, m_windowRect.m_width, m_windowRect.m_height);
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
	int width = p_clear->m_bounds.m_width;
	int startX;
	int height = p_clear->m_bounds.m_height;

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
