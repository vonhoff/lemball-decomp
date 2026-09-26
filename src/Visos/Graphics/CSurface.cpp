#include "CSurface.h"

#include "../Foundation/CChangeList.h"
#include "../Foundation/CVSOStream.h"
#include "../Foundation/CVsPoint.h"
#include "../Foundation/VsDebug.h"
#include "../Resources/CResBITMAP.h"
#include "../Resources/CResPALETTE.h"
#include "../Resources/CResZRLE.h"
#include "../Target/Graphics/CDibContext.h"
#include "../Target/Graphics/CDrawingContext.h"
#include "../Target/Graphics/CGdiContext.h"
#include "../Target/Graphics/CGraphicsDriver.h"
#include "CBitmap.h"
#include "CBitmapRes.h"
#include "CCircle.h"
#include "CClipRect.h"
#include "CCopyColourToBackBuff.h"
#include "CCopyToBackBuff.h"
#include "CFilledCircle.h"
#include "CGDIDevice.h"
#include "CLine.h"
#include "CPoint.h"
#include "CRemap.h"
#include "CScreenScroll.h"
#include "CSolidRect.h"
#include "CZBuffClear.h"
#include "CZRLE.h"

#include <new.h>
#include <string.h>

#define WIN32_LEAN_AND_MEAN
#include "Visos/Foundation/CVsRect.h"
#include "Visos/Foundation/CVsSize.h"
#include "Visos/Foundation/ChangeListItem.h"
#include "Visos/Graphics/CPVBackBuffSurface.h"
#include "Visos/Graphics/CPVGDIBitmap.h"
#include "Visos/Graphics/CPVScrollableSurface.h"
#include "Visos/Graphics/CPVZBuffSurface.h"

#include <windows.h>

extern "C" __declspec(dllimport) int __stdcall GdiFlush();

struct SurfaceListHead {
	void* m_first;
	void* m_last;
	int m_count;
};

// GLOBAL: LEMBALL 0x004a2010
SurfaceListHead* g_pSurfaceList = 0;

// FUNCTION: LEMBALL 0x0046c050
CSurface::CSurface(const CVsRect& p_rect, class CSurface* p_parentSurface)
	: m_presentX(m_presentY = 0), m_childSurfaceHead(0), m_childSurfaceTail(0), m_childSurfaceCount(0)
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
	if (storage != 0) {
		node = (SurfaceListNode*) storage;
		node->m_surface = this;
		node->m_next = 0;
		node->m_prev = 0;
	}
	else {
		node = 0;
	}
	node->m_prev = (SurfaceListNode*) parentList->m_last;
	if (parentList->m_last != 0) {
		((SurfaceListNode*) parentList->m_last)->m_next = node;
	}
	parentList->m_last = node;
	if (parentList->m_first == 0) {
		parentList->m_first = node;
	}
	parentList->m_count++;

	if (g_pSurfaceList == 0) {
		head = (SurfaceListHead*) operator new(0xc);
		if (head != 0) {
			head->m_first = 0;
			head->m_last = 0;
			head->m_count = 0;
			g_pSurfaceList = head;
		}
		else {
			g_pSurfaceList = 0;
		}
	}
	head = g_pSurfaceList;
	storage = operator new(0xc);
	if (storage != 0) {
		node = (SurfaceListNode*) storage;
		node->m_surface = this;
		node->m_next = 0;
		node->m_prev = 0;
	}
	else {
		node = 0;
	}
	node->m_prev = (SurfaceListNode*) head->m_last;
	if (head->m_last != 0) {
		((SurfaceListNode*) head->m_last)->m_next = node;
	}
	head->m_last = node;
	if (head->m_first == 0) {
		head->m_first = node;
	}
	head->m_count++;

	m_zoom = 1;
	m_platformBitmap = 0;
	m_drawingPort = 0;
	m_reserved40 = 0;
	InitializeCriticalSection((CRITICAL_SECTION*) m_lock);
	m_lockInitialized = 1;
	if (m_parentSurface == (CSurface*) g_pGdiHelperTarget) {
		m_changeList = new CChangeList(0x1000, p_rect, CVsSize(8, 8));
	}
	else {
		m_changeList = new CChangeList(0, p_rect, CVsSize(8, 8));
	}
	if (m_parentSurface == (CSurface*) g_pGdiHelperTarget) {
		BuildSurfaceColourTable((unsigned int*) m_colourTable,
								0,
								0,
								g_pTargetGraphicsDriver->HasPalette() ? g_dwWinGDrawColourTable : 0);
		CGraphicsDriver* driver = g_pTargetGraphicsDriver;
		m_drawingPort = driver->CreateDrawingContext();
	}
	CVsRect& rect = m_surfaceRect;
	rect.m_width = p_rect.m_width;
	rect.m_height = p_rect.m_height;
	const short* coords;
	if (&p_rect != 0) {
		coords = &p_rect.m_x;
	}
	else {
		coords = 0;
	}
	rect.m_x = *coords;
	rect.m_y = coords[1];
	NewBitmap(p_rect);
}

// GLOBAL: LEMBALL 0x004a2018
static const unsigned char g_anFallbackSystemColors[20][3] = {
	{0x00, 0x00, 0x00}, {0x80, 0x00, 0x00}, {0x00, 0x80, 0x00}, {0x80, 0x80, 0x00}, {0x00, 0x00, 0x80},
	{0x80, 0x00, 0x80}, {0x00, 0x80, 0x80}, {0xc0, 0xc0, 0xc0}, {0xc0, 0xdc, 0xc0}, {0xa6, 0xca, 0xf0},
	{0xff, 0xfb, 0xf0}, {0xa0, 0xa0, 0xa4}, {0x80, 0x80, 0x80}, {0xff, 0x00, 0x00}, {0x00, 0xff, 0x00},
	{0xff, 0xff, 0x00}, {0x00, 0x00, 0xff}, {0xff, 0x00, 0xff}, {0x00, 0xff, 0xff}, {0xff, 0xff, 0xff}};
// GLOBAL: LEMBALL 0x004a2058
static const unsigned char g_anReservedOutputColors[2][3] = {{0xff, 0xff, 0xff}, {0x00, 0x00, 0x00}};

// FUNCTION: LEMBALL 0x0046c380
void BuildSurfaceColourTable(unsigned int* p_entries,
							 CResPALETTE* p_palette,
							 void* p_unused,
							 unsigned int* p_fallbackEntries)
{
	unsigned char paletteStorage[0x404];
	int count;
	PALETTEENTRY* systemEntries = (PALETTEENTRY*) (paletteStorage + 4);
	unsigned char* output;
	PALETTEENTRY* entry;
	const unsigned char* source;
	HDC hdc = GetDC(0);
	unsigned int first = GetSystemPaletteEntries(hdc, 0, 10, systemEntries);
	unsigned int last = GetSystemPaletteEntries(hdc, 0xf6, 10, systemEntries + 0xf6);
	if ((last | first) == 0) {
		source = &g_anFallbackSystemColors[0][0];
		entry = systemEntries;
		do {
			entry->peRed = *source++;
			entry->peGreen = *source++;
			entry->peBlue = *source++;
			entry++;

		} while (entry < systemEntries + 10);
		entry = systemEntries + 0xf6;
		do {
			entry->peRed = *source++;
			entry->peGreen = *source++;
			entry->peBlue = *source++;
			entry++;

		} while (entry < systemEntries + 0x100);
	}
	if (hdc != 0) {
		ReleaseDC(0, hdc);
	}
	((LOGPALETTE*) paletteStorage)->palVersion = 0x300;
	((LOGPALETTE*) paletteStorage)->palNumEntries = 0x100;
	output = &((RGBQUAD*) p_entries)[0].rgbRed;
	entry = systemEntries;
	do {
		output[0] = entry->peRed;
		output[-1] = entry->peGreen;
		output[-2] = entry->peBlue;
		entry->peFlags = 0;
		unsigned char red = entry[0xf6].peRed;
		output[1] = 0;
		output[0x3d8] = red;
		output[0x3d7] = entry[0xf6].peGreen;
		output[0x3d6] = entry[0xf6].peBlue;
		entry[0xf6].peFlags = 0;
		output[0x3d9] = 0;
		entry++;
		output += 4;
	} while (entry < systemEntries + 10);
	source = &g_anReservedOutputColors[0][0];
	output = &((RGBQUAD*) p_entries)[10].rgbRed;
	count = 2;
	do {
		output[0] = *source++;
		output[-1] = *source++;
		output[-2] = *source++;
		output[1] = 0;
		output += 4;
		count--;
	} while (count != 0);
	if (p_palette == 0) {
		if (p_fallbackEntries == 0) {
			PALETTEENTRY* entry;
			int index = 12;
			entry = systemEntries + 12;
			output = &((RGBQUAD*) p_entries)[12].rgbRed;
			do {
				unsigned char color = -index;
				entry->peRed = color;
				output[0] = color;
				entry->peGreen = color;
				output[-1] = color;
				entry->peBlue = color;
				output[-2] = color;
				output[1] = 0;
				entry->peFlags = 1;
				entry++;
				output += 4;
				index++;
			} while (entry < systemEntries + 0xf6);
		}
		else {
			unsigned char* fallback;
			PALETTEENTRY* entry = systemEntries + 12;
			output = &((RGBQUAD*) p_entries)[12].rgbRed;
			fallback = &((RGBQUAD*) p_fallbackEntries)[12].rgbRed;
			do {
				unsigned char color = fallback[0];
				output[0] = color;
				entry->peRed = color;
				color = fallback[-1];
				entry->peGreen = color;
				output[-1] = color;
				color = fallback[-2];
				entry->peBlue = color;
				output[-2] = color;
				output[1] = 0;
				entry->peFlags = 1;
				entry++;
				output += 4;
				fallback += 4;
			} while (entry < systemEntries + 0xf6);
		}
	}
	else {
		int paletteCount = (int) p_palette->m_paletteState - 10;
		if (paletteCount > 0xf6) {
			paletteCount = 0xec;
		}
		if (paletteCount > 12) {
			int i = 12;
			output = &((RGBQUAD*) p_entries)[12].rgbRed;
			paletteCount -= 12;
			do {
				source = p_palette->m_data + i * 4;
				unsigned char color = source[0];
				systemEntries[i].peRed = color;
				output[0] = color;
				color = source[1];
				systemEntries[i].peGreen = color;
				output[-1] = color;
				color = source[2];
				systemEntries[i].peBlue = color;
				output[-2] = color;
				output[1] = 0;
				systemEntries[i].peFlags = 1;
				output += 4;
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
	m_childSurfaceHead = 0;
	m_presentX = 0;
	m_childSurfaceTail = 0;
	m_childSurfaceCount = 0;
	m_drawingPort = new CGdiContext(p_port);
	m_platformBitmap = 0;
	m_changeList = 0;
	m_parentSurface = 0;
	InitializeCriticalSection((CRITICAL_SECTION*) m_lock);
	m_lockInitialized = 1;
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
	if (m_lockInitialized != 0) {
		EnterCriticalSection((CRITICAL_SECTION*) m_lock);
		locked = 1;
	}
	if (m_platformBitmap != 0) {
		g_pTargetGraphicsDriver->RestoreDibContext((CDrawingContext*) m_drawingPort, (CDibContext*) m_platformBitmap);
		g_pTargetGraphicsDriver->DestroyDibContext((CDibContext*) m_platformBitmap);
		m_platformBitmap = 0;
	}
	if (m_parentSurface == (CSurface*) g_pGdiHelperTarget) {
		FreeBackBuff();
		FreeZBuff();
	}
	if (m_drawingPort != 0) {
		g_pTargetGraphicsDriver->DestroyDrawingContext((CDrawingContext*) m_drawingPort);
		m_drawingPort = 0;
	}
	if (m_changeList != 0) {
		delete m_changeList;
		m_changeList = 0;
	}
	parent = m_parentSurface;
	if (parent != 0) {
		node = parent->m_childSurfaceHead;
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
				parent->m_childSurfaceTail = prev;
			}
			else {
				next->m_prev = prev;
			}
			if (prev == 0) {
				parent->m_childSurfaceHead = next;
			}
			else {
				prev->m_next = next;
			}
			parent->m_childSurfaceCount = parent->m_childSurfaceCount - 1;
		}
		m_parentSurface = 0;
	}
	if (locked != 0) {
		LeaveCriticalSection((CRITICAL_SECTION*) m_lock);
		DeleteCriticalSection((CRITICAL_SECTION*) m_lock);
		m_lockInitialized = 0;
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
	node = (SurfaceListNode*) m_childSurfaceHead;
	while (node != 0) {
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
	for (node = m_childSurfaceHead; node != 0; node = node->m_next) {
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
		if (0 < (short) m_height) {
			do {
				m_lines[y] = (void*) ((int) CPVScrollableSurface::m_parentSurface->m_lines[parentY] +
									  (int) CPVScrollableSurface::m_windowRect.m_x);
				y = y + 1;
				parentY = parentY + 1;
			} while (y < (int) (short) m_height);
		}
	}
	else {
		CPVGDIBitmap::SetLinePtrs();
	}
}

// FUNCTION: LEMBALL 0x0046cb20
void CSurface::AddToChangeList(const CVsRect* p_rect)
{
	CSurface* parent;
	const CVsPoint* origin;
	short originX;
	short originY;

	parent = (CSurface*) CPVScrollableSurface::m_parentSurface;
	if (parent != (CSurface*) g_pGdiHelperTarget && CPVScrollableSurface::m_flag74 != 0 &&
		CPVScrollableSurface::m_flag70 != 0) {
		origin = &this->CPVScrollableSurface::m_surfaceRect;
		originX = origin->m_x;
		originY = origin->m_y;
		CVsRect translated(*p_rect);
		translated.m_x += originX;
		translated.m_y += originY;
		((CSurface*) CPVScrollableSurface::m_parentSurface)->AddToChangeList(&translated);
		return;
	}
	m_changeList->Add(*p_rect);
}

// FUNCTION: LEMBALL 0x0046cbd0
CChangeList* CSurface::GetChangeList()
{
	return m_changeList;
}

// FUNCTION: LEMBALL 0x0046cbe0
void CSurface::Blit(class CClipRect* p_clipRect)
{
	CVsRect* clip = &m_clipRect;
	short clipRight;

	if ((p_clipRect->m_reserved0c & 0x1000) == 0) {
		const short* coords;

		clip->m_width = p_clipRect->m_left;
		clip->m_height = p_clipRect->m_top;
		if (&p_clipRect->m_left != 0) {
			coords = &p_clipRect->m_right;
		}
		else {
			coords = 0;
		}
		clip->m_x = *coords;
		clip->m_y = coords[1];
	}
	else if ((int) p_clipRect->m_left * (int) p_clipRect->m_top != 0) {
		clipRight = clip->m_x;
		if (p_clipRect->m_right < clipRight) {
			clip->m_width = (short) (clip->m_width + (clipRight - p_clipRect->m_right));
			clip->m_x = p_clipRect->m_right;
		}
		short clipX;
		short primitiveWidth = p_clipRect->m_left;
		clipX = clip->m_x;
		short right = clip->m_width;
		right += clipX;
		short primitiveRight = p_clipRect->m_right;
		primitiveRight += primitiveWidth;
		if (right < primitiveRight) {
			primitiveWidth -= clipX;
			primitiveWidth += p_clipRect->m_right;
			clip->m_width = primitiveWidth;
		}
		if (p_clipRect->m_bottom < clip->m_y) {
			clip->m_height = (short) (clip->m_height + (clip->m_y - p_clipRect->m_bottom));
			clip->m_y = p_clipRect->m_bottom;
		}
		short primitiveY;
		short primitiveHeight = p_clipRect->m_top;
		primitiveY = p_clipRect->m_bottom;
		short clipBottom = (short) (clip->m_height + clip->m_y);
		short primitiveBottom = (short) (primitiveY + primitiveHeight);
		if (clipBottom < primitiveBottom) {
			clip->m_height = (short) ((primitiveHeight - clip->m_y) + primitiveY);
		}
	}
	CSurface* parent = m_parentSurface;
	if ((CSurface*) g_pGdiHelperTarget != parent && (p_clipRect->m_reserved0c & 0x10000) == 0) {
		CVsRect* parentClip = &parent->m_clipRect;
		short parentX = parentClip->m_x;
		CVsRect* childClip = &m_clipRect;
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
				m_parentSurface->AddToChangeList(&m_windowRect);
				m_flag78 = 0;
			}
			else if (m_flag70 != 0) {
				CChangeList* list = GetChangeList();
				int diff = list->GetNumItems() - list->GetDrawMark();
				if (diff > 0) {
					m_parentSurface->AddToChangeList(&m_windowRect);
				}
			}
		}
		return;
	}

	if (m_platformBitmap == 0) {
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
			CVsRect translated(*(CVsRect*) item);
			if (g_dwFullScreenGdi == 0) {
				translated.m_x += m_presentX;
				translated.m_y += m_presentY;
			}
			const CVsPoint* origin = (const CVsPoint*) &m_relOriginX;
			translated.m_x += origin->m_x;
			translated.m_y += origin->m_y;
			short zoom = m_zoom;
			if (zoom == 1) {
				g_pTargetGraphicsDriver->BlitWrappedBitmap(destContext,
														   &translated,
														   (CDrawingContext*) m_drawingPort,
														   (CVsRect*) item,
														   this);
			}
			else {
				CVsRect destRect(translated.m_x * zoom,
								 translated.m_y * zoom,
								 translated.m_width * zoom,
								 translated.m_height * zoom);
				g_pTargetGraphicsDriver->BlitWrappedBitmap(destContext,
														   &destRect,
														   (CDrawingContext*) m_drawingPort,
														   (CVsRect*) item,
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
				CopyBackBuffToScreen(*(CVsRect*) item);
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
		fallbackEntries = 0;
	}
	BuildSurfaceColourTable(g_dwWinGDrawColourTable, p_palette, 0, fallbackEntries);
	SetDefaultCtable();
}

// FUNCTION: LEMBALL 0x0046d090
void CSurface::NewBitmap(const CVsRect& p_rect)
{
	EnterCriticalSection((CRITICAL_SECTION*) m_lock);
	{
		CVsRect& bounds = m_surfaceRect;
		bounds.m_width = p_rect.m_width;
		bounds.m_height = p_rect.m_height;
		const CVsPoint* position = &p_rect;
		bounds.m_x = position->m_x;
		bounds.m_y = position->m_y;
	}
	{
		CVsRect& bounds = m_windowRect;
		bounds.m_width = p_rect.m_width;
		bounds.m_height = p_rect.m_height;
		const CVsPoint* position = &p_rect;
		bounds.m_x = position->m_x;
		bounds.m_y = position->m_y;
	}
	if ((void*) m_parentSurface != g_pGdiHelperTarget) {
		const CVsSize& parentSize = m_parentSurface->m_windowRect;
		short parentWidth = parentSize.m_width;
		short parentHeight = parentSize.m_height;
		CVsRect& clipped = m_windowRect;
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
			const CVsSize& windowSize = m_windowRect;
			CVsSize& clipSize = m_clipRect;
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
		const CVsSize& windowSize = m_windowRect;
		short height = windowSize.m_height;
		CVsSize& clipSize = m_clipRect;
		clipSize.m_width = windowSize.m_width;
		clipSize.m_height = height;
	}
	short width;
	short height;
	{
		const CVsSize& size = SetSize(m_windowRect, m_reserved40);
		width = size.m_width;
		height = size.m_height;
	}
	if (m_platformBitmap != 0) {
		g_pTargetGraphicsDriver->RestoreDibContext((CDrawingContext*) m_drawingPort, (CDibContext*) m_platformBitmap);
		g_pTargetGraphicsDriver->DestroyDibContext((CDibContext*) m_platformBitmap);
		m_platformBitmap = 0;
	}
	if (m_windowRect.m_width == 0 || m_windowRect.m_height == 0) {
		m_bitmapPixelCount = 0;
	}
	else {
		if (m_platformBitmap == 0) {
			g_pTargetGraphicsDriver->InitializeBitmapInfo((BITMAPINFO*) m_bitmapInfo);
			((BITMAPINFO*) m_bitmapInfo)->bmiHeader.biWidth = width;
			((BITMAPINFO*) m_bitmapInfo)->bmiHeader.biHeight =
				(int) height * (int) ((BITMAPINFO*) m_bitmapInfo)->bmiHeader.biHeight;
			((BITMAPINFO*) m_bitmapInfo)->bmiHeader.biPlanes = 1;
			((BITMAPINFO*) m_bitmapInfo)->bmiHeader.biCompression = 0;
			((BITMAPINFO*) m_bitmapInfo)->bmiHeader.biSizeImage = 0;
			((BITMAPINFO*) m_bitmapInfo)->bmiHeader.biXPelsPerMeter = 0;
			((BITMAPINFO*) m_bitmapInfo)->bmiHeader.biYPelsPerMeter = 0;
			((BITMAPINFO*) m_bitmapInfo)->bmiHeader.biClrUsed = 0;
			((BITMAPINFO*) m_bitmapInfo)->bmiHeader.biClrImportant = 0;
			((BITMAPINFO*) m_bitmapInfo)->bmiHeader.biSize = 0x28;
			((BITMAPINFO*) m_bitmapInfo)->bmiHeader.biBitCount = 8;
			m_platformBitmap =
				g_pTargetGraphicsDriver->CreateDibContext((CDrawingContext*) m_drawingPort, (BITMAPINFO*) m_bitmapInfo);
			if (m_platformBitmap != 0) {
				g_pTargetGraphicsDriver->SelectDibContext((CDrawingContext*) m_drawingPort,
														  (CDibContext*) m_platformBitmap);
				m_bitmapPixelCount = (int) m_windowRect.m_width * (int) m_windowRect.m_height;
			}
		}
		if (m_platformBitmap == 0) {
			_VSRELassert("AllocatedBitmap", "VSGDI.CPP", 736);
		}
		CDibContext* dib = (CDibContext*) m_platformBitmap;
		SetBitsBase(dib->GetBits(), dib->GetStride());
		m_changeList->SetDrawMark();
		CVsRect clip;
		const CVsSize& drawSize = m_surfaceRect;
		short drawHeight = drawSize.m_height;
		clip.m_width = drawSize.m_width;
		clip.m_height = drawHeight;
		AddToChangeList(&clip);
	}
	LeaveCriticalSection((CRITICAL_SECTION*) m_lock);
}

// FUNCTION: LEMBALL 0x0046d420
void CSurface::Resize(const CVsSize& p_size)
{
	CVsRect rect(m_surfaceRect);
	rect.m_width = p_size.m_width;
	rect.m_height = p_size.m_height;
	if (m_changeList != 0) {
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
	for (SurfaceListNode* node = m_childSurfaceHead; node != 0; node = node->m_next) {
		CSurface* child = node->m_surface;
		CVsSize childSize(child->m_surfaceRect);
		child->Resize(childSize);
	}
}

// FUNCTION: LEMBALL 0x0046d560
void CSurface::MoveRel(const CVsPoint& p_delta)
{
	CVsRect* rect = &m_surfaceRect;
	rect->m_x += p_delta.m_x;
	rect->m_y += p_delta.m_y;
	Move(*rect);
}

// FUNCTION: LEMBALL 0x0046d5b0
void CSurface::Move(const CVsPoint& p_position)
{
	CVsPoint delta;
	delta.m_x = p_position.m_x - m_surfaceRect.m_x;
	delta.m_y = p_position.m_y - m_surfaceRect.m_y;

	if (m_parentSurface != (CSurface*) g_pGdiHelperTarget) {
		EnterCriticalSection((CRITICAL_SECTION*) m_lock);
		m_surfaceRect.m_x = p_position.m_x;
		m_surfaceRect.m_y = p_position.m_y;
		short oldWidth = m_windowRect.m_width;
		short oldHeight = m_windowRect.m_height;
		m_windowRect.m_width = m_surfaceRect.m_width;
		m_windowRect.m_height = m_surfaceRect.m_height;
		m_windowRect.m_x = m_surfaceRect.m_x;
		m_windowRect.m_y = m_surfaceRect.m_y;

		CVsRect& clipped = m_windowRect;
		short parentWidth = m_parentSurface->m_windowRect.m_width;
		short parentHeight = m_parentSurface->m_windowRect.m_height;

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
		if (clipped.m_width < 1 || clipped.m_height < 1) {
			clipped.m_x = 0;
			clipped.m_width = 0;
			clipped.m_y = 0;
			clipped.m_height = 0;
		}
		m_clipRect.m_width = m_windowRect.m_width;
		m_clipRect.m_height = m_windowRect.m_height;
		if (m_windowRect.m_width != oldWidth || m_windowRect.m_height != oldHeight) {
			Resize((CVsSize&) m_windowRect);
		}
		if (m_platformBitmap != 0) {
			g_pTargetGraphicsDriver->DestroyDibContext((CDibContext*) m_platformBitmap);
			m_platformBitmap = 0;
		}
		m_bitmapPixelCount = 0;
		CreateLinePtrs();
		LeaveCriticalSection((CRITICAL_SECTION*) m_lock);
		for (SurfaceListNode* node = m_childSurfaceHead; node != 0; node = node->m_next) {
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
	if (m_changeList == 0) {
		return;
	}
	EnterCriticalSection((CRITICAL_SECTION*) m_lock);
	BITMAPINFOHEADER* header = (BITMAPINFOHEADER*) p_header;
	int copyWidth = m_windowRect.m_width <= header->biWidth ? m_windowRect.m_width : header->biWidth;
	int copyHeight = m_windowRect.m_height <= header->biHeight ? m_windowRect.m_height : header->biHeight;
	int stride = (int) ((header->biBitCount * header->biWidth + 31) & ~31) / 8;
	unsigned char* source = p_bits + (header->biHeight - 1) * stride;
	int y = 0;
	while (y < copyHeight) {
		memcpy(m_lines[y], source, copyWidth);
		copyHeight = m_windowRect.m_height <= header->biHeight ? m_windowRect.m_height : header->biHeight;
		source -= stride;
		y++;
	}
	short rectHeight = m_windowRect.m_height;
	short rectWidth = m_windowRect.m_width;
	CVsRect rect;
	rect.m_width = rectWidth;
	rect.m_height = rectHeight;
	m_changeList->Reset();
	AddToChangeList(&rect);
	LeaveCriticalSection((CRITICAL_SECTION*) m_lock);
}

// FUNCTION: LEMBALL 0x0046d930
void CSurface::SetDefaultCtable()
{
	unsigned char logPalette[4 + 256 * 4];
	LOGPALETTE* palette;
	PALETTEENTRY* entries;
	unsigned int* source;
	int i;
	SurfaceListNode* node;
	CSurface* surface;

	palette = (LOGPALETTE*) logPalette;
	palette->palVersion = 0x300;
	palette->palNumEntries = 0x100;
	source = g_dwWinGDrawColourTable;
	i = 0;
	entries = palette->palPalEntry;
	while (i < palette->palNumEntries) {
		entries->peRed = ((unsigned char*) source)[2];
		entries->peGreen = ((unsigned char*) source)[1];
		entries->peBlue = ((unsigned char*) source)[0];
		entries->peFlags = 4;
		entries++;
		i++;
		source++;
	}
	g_pTargetGraphicsDriver->CreatePalette(palette);
	node = (SurfaceListNode*) g_pSurfaceList->m_first;
	while (node != 0) {
		surface = node->m_surface;
		memcpy(m_colourTable, g_dwWinGDrawColourTable, 0x400);
		if (surface->m_drawingPort != 0) {
			g_pTargetGraphicsDriver->UpdateDibColourTable((CDrawingContext*) surface->m_drawingPort,
														  0,
														  0x100,
														  g_dwWinGDrawColourTable);
		}
		node = node->m_next;
	}
}

// FUNCTION: LEMBALL 0x0046d9f0
bool CSurface::BeginRender()
{
	if (m_lines == 0) {
		return 0;
	}
	if (m_parentSurface == (CSurface*) g_pGdiHelperTarget) {
		CDibContext* dib = (CDibContext*) m_platformBitmap;
		if (dib == 0) {
			return 0;
		}
		if (!dib->Lock()) {
			return 0;
		}
		unsigned char* bits = ((CDibContext*) m_platformBitmap)->GetBits();
		if (bits != 0 && m_bitsBase != bits) {
			m_bitsBase = bits;
			CreateLinePtrs();
			return 1;
		}
	}
	else {
		if (m_parentSurface == 0) {
			return 0;
		}
		if (!m_parentSurface->BeginRender()) {
			return 0;
		}
		unsigned char* expected = (unsigned char*) m_parentSurface->m_lines[m_windowRect.m_y] + m_windowRect.m_x;
		if (expected != m_bitsBase) {
			CreateLinePtrs();
			return 1;
		}
	}
	return 1;
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
	Blit((CBitmap*) p_primitive, p_bitmap);
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
	CVsRect rect = p_scroll->m_rect;
	CVsPoint dst = p_scroll->m_destination;

	if (HasBackBuff()) {
		CPVBackBuffSurface::m_bitmap.Scroll(&rect, &dst);
	}
	if (HasZBuff()) {
		CVsRect zrect(rect.m_x * 2, rect.m_y, rect.m_width * 2, rect.m_height);
		CVsPoint zdst;
		zdst.m_x = dst.m_x * 2;
		zdst.m_y = dst.m_y;
		CPVZBuffSurface::m_bitmap.Scroll(&zrect, &zdst);
	}
	CPVGDIBitmap::Scroll(&rect, &dst);
	AddToChangeList(&rect);
}

// FUNCTION: LEMBALL 0x00474d40
void CSurface::Blit(CZBuffClear* p_clear)
{
	int startX;
	int height;
	int width = p_clear->m_width;
	height = p_clear->m_height;

	if (width == 0 || height == 0) {
		return;
	}
	startX = p_clear->m_x;
	int startY = p_clear->m_y;
	unsigned short depth = (unsigned short) p_clear->m_depth;
	if (height <= 0) {
		return;
	}
	do {
		unsigned short* dest =
			(unsigned short*) ((unsigned char*) CPVZBuffSurface::m_bitmap.m_lines[startY] + startX * 2);
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
		int srcX = primitive->m_sourceX;
		int dstY = primitive->m_destination.m_y;
		int srcY = primitive->m_sourceY;
		if (height > 0) {
			int srcRow = srcY * 4;
			int count = height;
			int dstRow = dstY * 4;
			do {
				unsigned char* dst =
					(unsigned char*) *(int*) ((int) CPVBackBuffSurface::m_bitmap.m_lines + dstRow) + dstX;
				unsigned char* src = (unsigned char*) *(int*) ((int) m_lines + srcRow) + srcX;
				memcpy(dst, src, width);
				srcRow += 4;
				dstRow += 4;
				count--;
			} while (count != 0);
		}
	}
}

// FUNCTION: LEMBALL 0x00474ee0
void CSurface::CopyBackBuffToScreen(const CVsRect& p_rect)
{
	short height = p_rect.m_height;
	short width = p_rect.m_width;

	if ((int) height * (int) width != 0) {
		CVsRect rect(p_rect);
		if ((int) (short) (rect.m_x + rect.m_width) > (int) CPVBackBuffSurface::m_allocatedWidth) {
			rect.m_width = (short) (CPVBackBuffSurface::m_allocatedWidth - rect.m_x);
		}
		if ((int) (short) (rect.m_height + rect.m_y) > (int) CPVBackBuffSurface::m_allocatedHeight) {
			rect.m_height = (short) (CPVBackBuffSurface::m_allocatedHeight - rect.m_y);
		}
		const CVsPoint* origin = &rect;
		int x = origin->m_x;
		int y = origin->m_y;
		for (int i = 0; i < rect.m_height; i++) {
			memcpy((unsigned char*) m_lines[y + i] + x,
				   (unsigned char*) CPVBackBuffSurface::m_bitmap.m_lines[y + i] + x,
				   rect.m_width);
		}
	}
}

// FUNCTION: LEMBALL 0x00474fd0
void CSurface::Blit(CPoint* p_point)
{
	CPoint* primitive = p_point;
	if (m_clipRect.m_x <= primitive->m_x && primitive->m_x < (short) (m_clipRect.m_width + m_clipRect.m_x)) {
		if (m_clipRect.m_y <= primitive->m_y && primitive->m_y < (short) (m_clipRect.m_height + m_clipRect.m_y)) {
			*((unsigned char*) m_lines[primitive->m_y] + primitive->m_x) = (unsigned char) primitive->m_color;
			CVsRect rect(p_point->m_x, p_point->m_y, 1, 1);
			AddToChangeList(&rect);
		}
	}
}

// FUNCTION: LEMBALL 0x00475080
void CSurface::Blit(CSolidRect* p_rect)
{
	BlitRect(*p_rect->GetBounds(), p_rect->m_color);
}

// FUNCTION: LEMBALL 0x004750c0
void CSurface::Blit(CLine* p_line)
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

	x1 = p_line->m_bounds.m_width;
	y1 = p_line->m_bounds.m_height;
	x2 = p_line->m_bounds.m_x;
	y2 = p_line->m_bounds.m_y;
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
	CVsRect bounds;
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

// FUNCTION: LEMBALL 0x00475290
void CSurface::Blit(CCircle* p_circle)
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
		CVsRect bounds;
		bounds.m_width = (short) boundW;
		bounds.m_height = (short) boundH;
		bounds.m_x = (short) boundX;
		bounds.m_y = (short) boundY;
		AddToChangeList(&bounds);
	}
}

// FUNCTION: LEMBALL 0x00475490
void CSurface::Blit(CFilledCircle* p_circle)
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
							DrawCircleSpans(x, y, curX, curRadius, colByte);
						}
						if (curX < curRadius) {
							DrawCircleSpans(x, y, curRadius, curX, colByte);
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
		if (minX < (int) m_clipRect.m_x) {
			width += minX - m_clipRect.m_x;
			minX = m_clipRect.m_x;
		}
		if (m_clipRect.m_x + m_clipRect.m_width - 1 < minX + width) {
			width = m_clipRect.m_x + m_clipRect.m_width - minX;
		}
		int height = diameter;
		if (minY < (int) m_clipRect.m_y) {
			height += minY - m_clipRect.m_y;
			minY = m_clipRect.m_y;
		}
		if (m_clipRect.m_y + m_clipRect.m_height - 1 < minY + height) {
			height = m_clipRect.m_y + m_clipRect.m_height - minY;
		}
		CVsRect rect;
		rect.m_width = (short) width;
		rect.m_height = (short) height;
		rect.m_x = (short) minX;
		rect.m_y = (short) minY;
		AddToChangeList(&rect);
	}
}

// FUNCTION: LEMBALL 0x004756e0
void CSurface::BlitRect(CVsRect p_rect, int p_colour)
{
	short storage[4];
	CVsRect& clipped = *(CVsRect*) storage;
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
	AddToChangeList(&p_rect);
}

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

// FUNCTION: LEMBALL 0x00475bc0
int CSurface::ClipCircle(int p_centerX, int p_centerY, int p_radius)
{
	int left = p_centerX - p_radius;
	int top = p_centerY - p_radius;
	int right = p_centerX + p_radius;
	int bottom = p_centerY + p_radius;

	if (m_clipRect.m_height <= 0 || m_clipRect.m_width <= 0) {
		return 1;
	}
	if (right >= m_clipRect.m_x && left <= ((int) m_clipRect.m_x + (int) m_clipRect.m_width - 1) &&
		bottom >= m_clipRect.m_y && top <= ((int) m_clipRect.m_y + (int) m_clipRect.m_height - 1)) {
		if (left >= m_clipRect.m_x && right <= ((int) m_clipRect.m_x + (int) m_clipRect.m_width - 1) &&
			top >= m_clipRect.m_y && bottom <= ((int) m_clipRect.m_y + (int) m_clipRect.m_height - 1)) {
			return 2;
		}
		return 3;
	}
	return 1;
}

// FUNCTION: LEMBALL 0x00475ce0
void CSurface::DrawClippedCircleOutline(int p_centerX, int p_centerY, int p_radius, unsigned char p_colour)
{
	int x = 0;
	int y = p_radius;
	int err = 0;
	int step = 1;
	int errLimit = y * 2 - 1;

	if (m_clipRect.m_x <= p_centerX && m_clipRect.m_x + m_clipRect.m_width - 1 >= p_centerX) {
		if (m_clipRect.m_y <= p_centerY + p_radius &&
			m_clipRect.m_y + m_clipRect.m_height - 1 >= p_centerY + p_radius) {
			*((unsigned char*) m_lines[p_centerY + p_radius] + p_centerX) = p_colour;
		}
	}
	if (m_clipRect.m_x <= p_centerX && m_clipRect.m_x + m_clipRect.m_width - 1 >= p_centerX) {
		if (m_clipRect.m_y <= p_centerY - p_radius &&
			m_clipRect.m_y + m_clipRect.m_height - 1 >= p_centerY - p_radius) {
			*((unsigned char*) m_lines[p_centerY - p_radius] + p_centerX) = p_colour;
		}
	}
	if (m_clipRect.m_x <= p_centerX + p_radius && m_clipRect.m_x + m_clipRect.m_width - 1 >= p_centerX + p_radius) {
		int clipY = m_clipRect.m_y;
		if (clipY <= p_centerY && clipY + m_clipRect.m_height - 1 >= p_centerY) {
			unsigned char* destination = (unsigned char*) m_lines[p_centerY] + p_centerX;
			destination[p_radius] = p_colour;
		}
	}
	if (m_clipRect.m_x <= p_centerX - p_radius && m_clipRect.m_x + m_clipRect.m_width - 1 >= p_centerX - p_radius) {
		int clipY = m_clipRect.m_y;
		if (clipY <= p_centerY && clipY + m_clipRect.m_height - 1 >= p_centerY) {
			*((unsigned char*) m_lines[p_centerY] + p_centerX - p_radius) = p_colour;
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
		if (ClipCirclePoint(p_centerX + x, p_centerY + y) != 0) {
			unsigned char* destination = (unsigned char*) m_lines[p_centerY + y] + p_centerX;
			destination[x] = p_colour;
		}
		if (ClipCirclePoint(p_centerX - x, p_centerY + y) != 0) {
			*((unsigned char*) m_lines[p_centerY + y] + p_centerX - x) = p_colour;
		}
		if (ClipCirclePoint(p_centerX + x, p_centerY - y) != 0) {
			unsigned char* destination = (unsigned char*) m_lines[p_centerY - y] + p_centerX;
			destination[x] = p_colour;
		}
		if (ClipCirclePoint(p_centerX - x, p_centerY - y) != 0) {
			*((unsigned char*) m_lines[p_centerY - y] + p_centerX - x) = p_colour;
		}
		if (y > x) {
			DrawClippedCirclePoint(p_centerX, p_centerY, y, x, p_colour);
		}
	} while (y > x);
}

// FUNCTION: LEMBALL 0x00475fb0
void CSurface::DrawClippedCirclePoint(int p_centerX,
									  int p_centerY,
									  int p_xOffset,
									  int p_yOffset,
									  unsigned char p_colour)
{
	if (m_clipRect.m_x <= (p_centerX + p_xOffset) &&
		(p_centerX + p_xOffset) <= m_clipRect.m_x + m_clipRect.m_width - 1) {
		if (m_clipRect.m_y <= (p_centerY + p_yOffset) &&
			(p_centerY + p_yOffset) <= m_clipRect.m_y + m_clipRect.m_height - 1) {
			*((unsigned char*) m_lines[(p_centerY + p_yOffset)] + (p_centerX + p_xOffset)) = p_colour;
		}
	}
	if (m_clipRect.m_x <= (p_centerX - p_xOffset) &&
		(p_centerX - p_xOffset) <= m_clipRect.m_x + m_clipRect.m_width - 1) {
		if (m_clipRect.m_y <= (p_centerY + p_yOffset) &&
			(p_centerY + p_yOffset) <= m_clipRect.m_y + m_clipRect.m_height - 1) {
			*((unsigned char*) m_lines[(p_centerY + p_yOffset)] + (p_centerX - p_xOffset)) = p_colour;
		}
	}
	if (m_clipRect.m_x <= (p_centerX + p_xOffset) &&
		(p_centerX + p_xOffset) <= m_clipRect.m_x + m_clipRect.m_width - 1) {
		if (m_clipRect.m_y <= (p_centerY - p_yOffset) &&
			(p_centerY - p_yOffset) <= m_clipRect.m_y + m_clipRect.m_height - 1) {
			*((unsigned char*) m_lines[(p_centerY - p_yOffset)] + (p_centerX + p_xOffset)) = p_colour;
		}
	}
	if (m_clipRect.m_x <= (p_centerX - p_xOffset) &&
		(p_centerX - p_xOffset) <= m_clipRect.m_x + m_clipRect.m_width - 1) {
		if (m_clipRect.m_y <= (p_centerY - p_yOffset) &&
			(p_centerY - p_yOffset) <= m_clipRect.m_y + m_clipRect.m_height - 1) {
			*((unsigned char*) m_lines[(p_centerY - p_yOffset)] + (p_centerX - p_xOffset)) = p_colour;
		}
	}
}

// FUNCTION: LEMBALL 0x00476100
void CSurface::DrawCircleSpans(int p_centerX, int p_centerY, int p_halfWidth, int p_yOffset, unsigned char p_colour)
{
	int spanWidth = p_halfWidth * 2 + 1;
	unsigned char* negativeSpan = (unsigned char*) m_lines[p_centerY - p_yOffset] + p_centerX - p_halfWidth;
	memset((unsigned char*) m_lines[p_centerY + p_yOffset] + p_centerX - p_halfWidth,
		   *(unsigned int*) &p_colour,
		   spanWidth);
	memset(negativeSpan, *(unsigned int*) &p_colour, spanWidth);
}

// FUNCTION: LEMBALL 0x00476190
void CSurface::DrawClippedFilledCircle(int p_centerX, int p_centerY, int p_radius, int p_colour)
{
	int x = 0;
	int err = 0;
	int step = 1;
	int errLimit = p_radius * 2 - 1;

	if (p_centerX >= m_clipRect.m_x && p_centerX <= (int) (m_clipRect.m_width + m_clipRect.m_x - 1)) {
		if ((p_centerY + p_radius) >= m_clipRect.m_y &&
			(p_centerY + p_radius) <= (int) (m_clipRect.m_height + m_clipRect.m_y - 1)) {
			*((unsigned char*) m_lines[(p_centerY + p_radius)] + p_centerX) = (unsigned char) p_colour;
		}
	}
	if (p_centerX >= m_clipRect.m_x && p_centerX <= (int) (m_clipRect.m_width + m_clipRect.m_x - 1)) {
		if ((p_centerY - p_radius) >= m_clipRect.m_y &&
			(p_centerY - p_radius) <= (int) (m_clipRect.m_height + m_clipRect.m_y - 1)) {
			*((unsigned char*) m_lines[(p_centerY - p_radius)] + p_centerX) = (unsigned char) p_colour;
		}
	}
	if (p_centerY >= m_clipRect.m_y && p_centerY <= (int) (m_clipRect.m_height + m_clipRect.m_y - 1)) {
		int x1 = p_centerX - p_radius;
		int x2 = p_centerX + p_radius;
		if (x1 < m_clipRect.m_x) {
			x1 = m_clipRect.m_x;
		}
		if ((int) (m_clipRect.m_width + m_clipRect.m_x - 1) < x2) {
			x2 = m_clipRect.m_width + m_clipRect.m_x - 1;
		}
		memset((unsigned char*) m_lines[p_centerY] + x1, p_colour, x2 - x1 + 1);
	}

	if (p_radius > 1) {
		while (x < p_radius) {
			int changed = 0;
			x++;
			err += step;
			step += 2;
			int doubleErr = err * 2;
			if (errLimit < doubleErr) {
				p_radius--;
				changed = 1;
				err -= errLimit;
				errLimit -= 2;
			}
			if (x <= p_radius) {
				if (changed != 0) {
					int yTop = p_centerY - p_radius;
					int yBottom = p_centerY + p_radius;
					int clipY = m_clipRect.m_y;
					if (yTop <= (int) (m_clipRect.m_height + clipY - 1) && yBottom >= clipY) {
						int xLeft = p_centerX - x;
						int xRight = p_centerX + x;
						int clipX = m_clipRect.m_x;
						if (clipX <= xRight && (int) (m_clipRect.m_width + clipX - 1) >= xLeft) {
							if (xRight > (int) (m_clipRect.m_width + clipX - 1)) {
								xRight = m_clipRect.m_width + clipX - 1;
							}
							if (xLeft < clipX) {
								xLeft = clipX;
							}
							if (yTop >= clipY) {
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
		}
	}
}

// FUNCTION: LEMBALL 0x00476470
void CSurface::FilledCircleClipPoints(int p_centerX, int p_centerY, int p_xOffset, int p_yOffset, int p_colour)
{
	int y1 = p_centerY - p_yOffset;
	int y2 = p_centerY + p_yOffset;
	if (y1 <= (int) (m_clipRect.m_height + m_clipRect.m_y - 1) && m_clipRect.m_y <= y2) {
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
			if (m_clipRect.m_y <= y1) {
				memset((unsigned char*) m_lines[y1] + x1, p_colour, x2 - x1 + 1);
			}
			if (y2 <= (int) (m_clipRect.m_height + m_clipRect.m_y - 1)) {
				memset((unsigned char*) m_lines[y2] + x1, p_colour, x2 - x1 + 1);
			}
		}
	}
}

// FUNCTION: LEMBALL 0x00476580
bool CSurface::ClipRect(CVsRect& p_rect, CVsRect* p_clipped)
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
void CSurface::BlitZRLEClip(const CVsRect& p_rect, const CVsRect& p_clip, CResZRLE* p_zrle, unsigned int p_reverse)
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
						run &= 0x7f;
						src += run;
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
						run &= 0x7f;
						src += run;
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
						if (copyLen < width) {
							memcpy(dst, src + run + clipX, copyLen);
						}
						else {
							memcpy(dst, src + run + clipX, width);
						}
						width -= copyLen;
						dst += copyLen;
					}
					src += run;
				}
				if (run == 0x80) {
					break;
				}
			} while (clipX > 0);
			if (run != 0x80) {
				do {
					if (width <= 0) {
						break;
					}
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
				} while (run != 0x80);
			}
			while (run != 0x80) {
				run = *src++;
				if (run > 0x80) {
					run &= 0x7f;
					src += run;
				}
			}
			y += step;
			row++;
		} while (row < p_rect.m_height);
	}
}

// FUNCTION: LEMBALL 0x00476910
void CSurface::BlitZRLEClipZBuff(const CVsRect& p_rect, const CVsRect& p_clip, CResZRLE* p_zrle, unsigned short p_depth)
{
	unsigned char* src = p_zrle->GetData();
	if (p_clip.m_y > 0) {
		int skipRows = p_clip.m_y;
		do {
			unsigned char run;
			do {
				run = *src++;
				if (run > 0x80) {
					run &= 0x7f;
					src += run;
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
			unsigned short* zlines = (unsigned short*) ((unsigned char*) CPVZBuffSurface::m_bitmap.m_lines[y] + x * 2);
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
						if (copyLen < width) {
							memcpy(dst, clipX + src + run, copyLen);
							for (unsigned int i = 0; i < (unsigned int) copyLen; i++) {
								zlines[i] = p_depth;
							}
						}
						else {
							memcpy(dst, clipX + src + run, width);
							for (unsigned int i = 0; i < (unsigned int) width; i++) {
								zlines[i] = p_depth;
							}
						}
						width -= copyLen;
						dst += copyLen;
						zlines += copyLen;
					}
					src += run;
				}
				if (run == 0x80) {
					break;
				}
			} while (clipX > 0);
			if (run != 0x80) {
				do {
					if (width <= 0) {
						break;
					}
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
			}
			while (run != 0x80) {
				run = *src++;
				if (run > 0x80) {
					run &= 0x7f;
					src += run;
				}
			}
			y++;
			row++;
		} while (row < p_rect.m_height);
	}
}

// FUNCTION: LEMBALL 0x00476bf0
void CSurface::BlitZRLEClipQZBuff(const CVsRect& p_rect,
								  const CVsRect& p_clip,
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
				if (run > 0x80) {
					run &= 0x7f;
					src += run;
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
			unsigned short* zlines = (unsigned short*) ((unsigned char*) CPVZBuffSurface::m_bitmap.m_lines[y] + x * 2);
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
				if (run == 0x80) {
					goto row_done_qzbuff;
				}
			} while (clipX > 0);
			if (run != 0x80) {
				do {
					if (width < 1) {
						break;
					}
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
						run &= 0x7f;
						src += run;
					}
				}
			}
			y++;
			row++;
		} while (row < p_rect.m_height);
	}
}

// FUNCTION: LEMBALL 0x00476ee0
void CSurface::BlitZRLEClipR(const CVsRect& p_rect, const CVsRect& p_clip, CResZRLE* p_zrle, unsigned int p_reverse)
{
	unsigned char* src = p_zrle->GetData();
	short sourceWidth = p_zrle->m_width;
	short sourceHeight = p_zrle->m_height;
	int step = 1;
	int x = p_rect.m_x;
	int y = p_rect.m_y;
	if (p_reverse != 0) {
		step = -1;
		y += p_rect.m_height - 1;
		int skipRows = sourceHeight - p_clip.m_y - p_rect.m_height;
		if (skipRows > 0) {
			do {
				unsigned char run;
				do {
					run = *src++;
					if (run > 0x80) {
						run &= 0x7f;
						src += run;
					}
				} while (run != 0x80);
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
				if (run > 0x80) {
					run &= 0x7f;
					src += run;
				}
			} while (run != 0x80);
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
					if (run < 0x80) {
						skipX -= run;
						if (skipX < 0) {
							width += skipX;
							dst += skipX;
						}
					}
					else if (run > 0x80) {
						run &= 0x7f;
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
							width -= copyLength;
							dst -= copyLength;
						}
						src += count;
					}
				}
				else if (width > 0) {
					if (run < 0x80) {
						width -= run;
						dst -= run;
					}
					else if (run > 0x80) {
						run &= 0x7f;
						int count = run;
						if (count < width) {
							int remaining = count;
							unsigned char* copySrc = src;
							unsigned char* copyDst = dst;
							while (remaining > 0) {
								*copyDst-- = *copySrc++;
								remaining--;
							}
							src += count;
							width -= count;
							dst -= count;
						}
						else {
							int remaining = width;
							unsigned char* copySrc = src;
							unsigned char* copyDst = dst;
							while (remaining > 0) {
								*copyDst-- = *copySrc++;
								remaining--;
							}
							src += count;
							dst -= width;
							width = 0;
						}
					}
				}
				else if (run > 0x80) {
					run &= 0x7f;
					src += run;
				}
			} while (run != 0x80);
			y += step;
			row++;
		} while (row < p_rect.m_height);
	}
}

// FUNCTION: LEMBALL 0x00477130
void CSurface::BlitZRLENoClip(const CVsRect& p_rect, CResZRLE* p_zrle, unsigned int p_reverse)
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

// FUNCTION: LEMBALL 0x00477200
void CSurface::BlitZRLENoClipZBuff(const CVsRect& p_rect, CResZRLE* p_zrle, unsigned short p_depth)
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
					memcpy(dst, src, run);
					for (unsigned int i = 0; i < run; i++) {
						zlines[i] = p_depth;
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

// FUNCTION: LEMBALL 0x00477310
void CSurface::BlitZRLENoClipZBuffRemap(const CVsRect& p_rect,
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
			} while (run != 0x80);
			row++;
			y++;
		} while (row < p_rect.m_height);
	}
}

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

// FUNCTION: LEMBALL 0x00477660
void CSurface::BlitZRLENoClipR(const CVsRect& p_rect, CResZRLE* p_zrle, unsigned int p_reverse)
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
					int i = run;
					unsigned char* copySrc = src;
					unsigned char* copyDst = dst;
					for (; i > 0; i--) {
						*copyDst-- = *copySrc++;
					}
					dst -= run;
					src += run;
				}
			} while (run != 0x80);
			y += step;
			row++;
		} while (row < p_rect.m_height);
	}
}

// FUNCTION: LEMBALL 0x00477740
void CSurface::BlitZRLEClipRemap(const CVsRect& p_rect,
								 const CVsRect& p_clip,
								 CResZRLE* p_zrle,
								 unsigned int p_reverse,
								 unsigned char* p_remap)
{
	unsigned char* src = p_zrle->GetData();
	int step = 1;
	int x = p_rect.m_x;
	int y = p_rect.m_y;
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
						run &= 0x7f;
						src += run;
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
						run &= 0x7f;
						src += run;
					}
				} while (run != 0x80);
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
			int clipX = p_clip.m_x;
			unsigned char* dst = *(unsigned char**) ((unsigned char*) m_lines + lineOffset) + x;
			unsigned char run;
			do {
				run = *src++;
				if (clipX > 0) {
					if (run < 0x80) {
						clipX -= run;
						if (clipX < 0) {
							dst -= clipX;
							width += clipX;
						}
					}
					else if (run > 0x80) {
						run &= 0x7f;
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
					if (run < 0x80) {
						dst += run;
						width -= run;
					}
					else if (run > 0x80) {
						run &= 0x7f;
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
					if (run > 0x80) {
						run &= 0x7f;
						src += run;
					}
				}
			} while (run != 0x80);
			lineOffset += stepOffset;
			row++;
		} while (row < p_rect.m_height);
	}
}

// FUNCTION: LEMBALL 0x004779d0
void CSurface::BlitZRLEClipZBuffRemap(const CVsRect& p_rect,
									  const CVsRect& p_clip,
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
				if (run > 0x80) {
					run &= 0x7f;
					src += run;
				}
			} while (run != 0x80);
			skipRows--;
		} while (skipRows != 0);
	}
	int row = 0;
	if (p_rect.m_height > 0) {
		int lineOffset = y * 4;
		do {
			unsigned short* zlines =
				*(unsigned short**) ((unsigned char*) CPVZBuffSurface::m_bitmap.m_lines + lineOffset) + x;
			int width = p_rect.m_width;
			int clipX = p_clip.m_x;
			unsigned char* dst = *(unsigned char**) ((unsigned char*) m_lines + lineOffset) + x;
			unsigned char run;
			do {
				run = *src++;
				if (clipX > 0) {
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
						int count = run;
						clipX -= count;
						if (clipX < 0) {
							int copyLen = -clipX;
							if (copyLen < width) {
								unsigned char* copySrc = src + count + clipX;
								unsigned char* copyDst = dst;
								for (int i = copyLen; i > 0; i--) {
									*copyDst++ = p_remap[*copySrc++];
								}
							}
							else {
								unsigned char* copySrc = src + count + clipX;
								unsigned char* copyDst = dst;
								for (int i = copyLen; i > 0; i--) {
									*copyDst++ = p_remap[*copySrc++];
								}
							}
							dst += copyLen;
							width += clipX;
							zlines += copyLen;
						}
						src += count;
					}
				}
				else if (width > 0) {
					if (run < 0x80) {
						dst += run;
						width -= run;
						zlines += run;
					}
					else if (run > 0x80) {
						run &= 0x7f;
						int count = run;
						if (count < width) {
							unsigned char* copySrc = src;
							unsigned char* copyDst = dst;
							for (unsigned int i = count; i != 0; i--) {
								*copyDst++ = p_remap[*copySrc++];
							}
							src += count;
							dst += count;
							zlines += count;
							width -= count;
						}
						else {
							unsigned char* copySrc = src;
							unsigned char* copyDst = dst;
							for (int i = width; i > 0; i--) {
								*copyDst++ = p_remap[*copySrc++];
							}
							src += count;
							dst += width;
							width = 0;
							zlines += width;
						}
					}
				}
				else {
					if (run > 0x80) {
						run &= 0x7f;
						src += run;
					}
				}
			} while (run != 0x80);
			lineOffset += 4;
			row++;
		} while (row < p_rect.m_height);
	}
}

// FUNCTION: LEMBALL 0x00477c60
void CSurface::BlitZRLEClipQZBuffRemap(const CVsRect& p_rect,
									   const CVsRect& p_clip,
									   CResZRLE* p_zrle,
									   unsigned short p_depth,
									   unsigned char* p_remap)
{
	unsigned char* src = p_zrle->GetData();
	int x = p_rect.m_x;
	int y = p_rect.m_y;
	unsigned char run;
	int count;
	if (p_clip.m_y > 0) {
		int skipRows = p_clip.m_y;
		do {
			do {
				run = *src++;
				if (run > 0x80) {
					run &= 0x7f;
					src += run;
				}
			} while (run != 0x80);
			skipRows--;
		} while (skipRows != 0);
	}
	int row = 0;
	if (p_rect.m_height > 0) {
		int lineOffset = y * 4;
		do {
			unsigned short* zlines =
				*(unsigned short**) ((unsigned char*) CPVZBuffSurface::m_bitmap.m_lines + lineOffset) + x;
			int width = p_rect.m_width;
			int clipX = p_clip.m_x;
			unsigned char* dst = *(unsigned char**) ((unsigned char*) m_lines + lineOffset) + x;
			do {
				run = *src++;
				if (clipX > 0) {
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
						count = run;
						clipX -= count;
						if (clipX < 0) {
							int copyLen = -clipX;
							if (copyLen < width) {
								unsigned char* copySrc = src + count + clipX;
								unsigned char* copyDst = dst;
								unsigned short* copyZ = zlines;
								for (int i = copyLen; i > 0; i--) {
									if (*copyZ <= p_depth) {
										*copyDst = p_remap[*copySrc];
									}
									copyZ++;
									copyDst++;
									copySrc++;
								}
							}
							else {
								unsigned char* copySrc = src + count + clipX;
								unsigned char* copyDst = dst;
								unsigned short* copyZ = zlines;
								for (int i = copyLen; i > 0; i--) {
									if (*copyZ <= p_depth) {
										*copyDst = p_remap[*copySrc];
									}
									copyZ++;
									copyDst++;
									copySrc++;
								}
							}
							dst += copyLen;
							width += clipX;
							zlines += copyLen;
						}
						src += count;
					}
				}
				else if (width > 0) {
					if (run < 0x80) {
						dst += run;
						width -= run;
						zlines += run;
					}
					else if (run > 0x80) {
						run &= 0x7f;
						count = run;
						if (count < width) {
							unsigned char* copySrc = src;
							unsigned char* copyDst = dst;
							unsigned short* copyZ = zlines;
							for (int i = count; i > 0; i--) {
								if (*copyZ <= p_depth) {
									*copyDst = p_remap[*copySrc];
								}
								copyZ++;
								copyDst++;
								copySrc++;
							}
							src += count;
							dst += count;
							zlines += count;
							width -= count;
						}
						else {
							unsigned char* copySrc = src;
							unsigned char* copyDst = dst;
							unsigned short* copyZ = zlines;
							for (int i = width; i > 0; i--) {
								if (*copyZ <= p_depth) {
									*copyDst = p_remap[*copySrc];
								}
								copyZ++;
								copyDst++;
								copySrc++;
							}
							src += count;
							dst += width;
							zlines += width;
							width = 0;
						}
					}
				}
				else {
					if (run > 0x80) {
						run &= 0x7f;
						src += run;
					}
				}
			} while (run != 0x80);
			lineOffset += 4;
			row++;
		} while (row < p_rect.m_height);
	}
}

// FUNCTION: LEMBALL 0x00477f50
void CSurface::BlitZRLEClipRemapR(const CVsRect& p_rect,
								  const CVsRect& p_clip,
								  CResZRLE* p_zrle,
								  unsigned int p_reverse,
								  unsigned char* p_remap)
{
	int startX = p_rect.m_x + p_rect.m_width - 1;
	int y = p_rect.m_y;
	unsigned char* src = p_zrle->GetData();
	int zrleWidth = p_zrle->m_width;
	int step = 1;
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
						run &= 0x7f;
						src += run;
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
						run &= 0x7f;
						src += run;
					}
				} while (run != 0x80);
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
					if (run < 0x80) {
						skipX -= run;
						if (skipX < 0) {
							width += skipX;
							dst += skipX;
						}
					}
					else if (run > 0x80) {
						run &= 0x7f;
						int count = run;
						skipX -= count;
						if (skipX < 0) {
							int copyLen = -skipX;
							if (copyLen < width) {
								unsigned char* copySrc = src + count + skipX;
								unsigned char* copyDst = dst;
								for (int i = copyLen; i > 0; i--) {
									*copyDst-- = p_remap[*copySrc++];
								}
							}
							else {
								unsigned char* copySrc = src + count + skipX;
								unsigned char* copyDst = dst;
								for (int i = width; i > 0; i--) {
									*copyDst-- = p_remap[*copySrc++];
								}
							}
							width -= copyLen;
							dst -= copyLen;
						}
						src += count;
					}
				}
				else if (width > 0) {
					if (run < 0x80) {
						width -= run;
						dst -= run;
					}
					else if (run > 0x80) {
						run &= 0x7f;
						int count = run;
						if (count < width) {
							unsigned char* copySrc = src;
							unsigned char* copyDst = dst;
							for (unsigned int i = count; i != 0; i--) {
								*copyDst-- = p_remap[*copySrc++];
							}
							src += count;
							width -= count;
							dst -= count;
						}
						else {
							unsigned char* copySrc = src;
							unsigned char* copyDst = dst;
							for (int i = width; i > 0; i--) {
								*copyDst-- = p_remap[*copySrc++];
							}
							src += count;
							dst -= width;
							width = 0;
						}
					}
				}
				else {
					if (run > 0x80) {
						run &= 0x7f;
						src += run;
					}
				}
			} while (run != 0x80);
			lineOffset += stepOffset;
			row++;
		} while (row < p_rect.m_height);
	}
}

// FUNCTION: LEMBALL 0x004781e0
void CSurface::BlitZRLENoClipRemap(const CVsRect& p_rect,
								   CResZRLE* p_zrle,
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
			} while (run != 0x80);
			y += step;
			row++;
		} while (row < p_rect.m_height);
	}
}

// FUNCTION: LEMBALL 0x004782d0
void CSurface::BlitZRLENoClipRemapR(const CVsRect& p_rect,
									CResZRLE* p_zrle,
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

#pragma inline_depth(0)
// FUNCTION: LEMBALL 0x004783c0
void CSurface::Blit(CZRLE* p_primitive, CResZRLE* p_zrle)
{
	unsigned int flags = p_primitive->m_flags;
	if ((flags & 0xc0000) == 0) {
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
			CVsRect dest((short) primitiveX, (short) primitiveY, (CVsSize*) &p_zrle->m_width);
			if ((flags & 0x400) == 0) {
				((CVsPoint*) &dest.m_x)->AddInPlace((CVsPoint*) &p_zrle->m_x);
			}
			{
				CVsRect clipped;

				if ((short) dest.m_width > 0xff || (short) dest.m_height > 0xff) {
					short warningWidth = dest.m_width;
					CVSOStream& warningStream = *g_pDebugOutput << g_szWarningZrleIs;
					short warningHeight = dest.m_height;
					CVSOStream& widthStream = warningStream << (int) warningWidth << g_szClippingWideAnd;
					widthStream << (int) warningHeight << g_szClippingHighNewline;
					if ((short) dest.m_width > 0xff) {
						*g_pDebugOutput << g_szClippingWidthTo << 0xff << g_szClippingDotNewline;
						dest.m_width = 0xff;
					}
					if ((short) dest.m_height > 0xff) {
						*g_pDebugOutput << g_szClippingHeightTo << 0xff << g_szClippingDotNewline;
						dest.m_height = 0xff;
					}
				}
				if (ClipRect(dest, &clipped) == 0) {
					AddToChangeList(&dest);
					if ((flags & 0x40000) != 0) {
						if (remap == 0) {
							BlitZRLENoClipZBuff(dest, p_zrle, stateDepth);
							return;
						}
						BlitZRLENoClipZBuffRemap(dest, p_zrle, stateDepth, remap->m_remap);
						return;
					}
					if ((flags & 0x80000) != 0) {
						if (remap == 0) {
							BlitZRLENoClipQZBuff(dest, p_zrle, stateDepth);
							return;
						}
						BlitZRLENoClipQZBuffRemap(dest, p_zrle, stateDepth, remap->m_remap);
						return;
					}
					if (remap == 0) {
						if ((flags & 1) != 0) {
							BlitZRLENoClipR(dest, p_zrle, (flags & 2) >> 1);
							return;
						}
						BlitZRLENoClip(dest, p_zrle, (flags & 2) >> 1);
						return;
					}
					if ((flags & 1) != 0) {
						BlitZRLENoClipRemapR(dest, p_zrle, (flags & 2) >> 1, remap->m_remap);
						return;
					}
					BlitZRLENoClipRemap(dest, p_zrle, (flags & 2) >> 1, remap->m_remap);
					return;
				}
				if (clipped.m_width <= 0 || clipped.m_height <= 0) {
					return;
				}
				AddToChangeList(&dest);
				if ((flags & 0x40000) != 0) {
					if (remap == 0) {
						BlitZRLEClipZBuff(dest, clipped, p_zrle, stateDepth);
						return;
					}
					BlitZRLEClipZBuffRemap(dest, clipped, p_zrle, stateDepth, remap->m_remap);
					return;
				}
				if ((flags & 0x80000) != 0) {
					if (remap == 0) {
						BlitZRLEClipQZBuff(dest, clipped, p_zrle, stateDepth);
						return;
					}
					BlitZRLEClipQZBuffRemap(dest, clipped, p_zrle, stateDepth, remap->m_remap);
					return;
				}
				if (remap == 0) {
					if ((flags & 1) != 0) {
						BlitZRLEClipR(dest, clipped, p_zrle, (flags & 2) >> 1);
						return;
					}
					BlitZRLEClip(dest, clipped, p_zrle, (flags & 2) >> 1);
					return;
				}
				if ((flags & 1) != 0) {
					BlitZRLEClipRemapR(dest, clipped, p_zrle, (flags & 2) >> 1, remap->m_remap);
					return;
				}
				BlitZRLEClipRemap(dest, clipped, p_zrle, (flags & 2) >> 1, remap->m_remap);
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
	CVsRect sourceRect(p_primitive->m_sourceX, p_primitive->m_sourceY, p_primitive->m_width, p_primitive->m_height);
	if (sourceRect.m_height == 0 && sourceRect.m_width == 0) {
		sourceRect.m_width = p_bitmap->m_x;
		sourceRect.m_height = p_bitmap->m_y;
	}
	unsigned int flags = ((CBitmapRes*) p_primitive)->m_flags;
	if ((int) p_bitmap->m_y * (int) p_bitmap->m_x != 0) {
		CVsRect dest(sourceRect);
		dest.m_x = x;
		dest.m_y = y;
		CVsRect clip;
		if (ClipRect(dest, &clip) != 0) {
			if (clip.m_width <= 0 || clip.m_height <= 0) {
				return;
			}
			CVsSize clippedSize;
			clippedSize = clip;
			(CVsSize&) dest = clippedSize;
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
		unsigned char* source = p_bitmap->GetData() + ((int) sourceRect.m_y + (int) clip.m_y) * bitmapWidth +
								(int) sourceRect.m_x + (int) clip.m_x;
		if ((flags & 0x800) != 0) {
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
			AddToChangeList(dest);
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
		AddToChangeList(dest);
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
