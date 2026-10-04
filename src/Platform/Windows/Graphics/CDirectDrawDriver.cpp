#include "CDirectDrawDriver.h"

#include "Platform/Windows/Entry.h"
#include "Engine/Streams/CVSOStream.h"
#include "Platform/Windows/Windowing/CWnd.h"
#include "CDirectDrawContext.h"
#include "CDirectDrawSurface.h"
#include "DirectDrawError.h"
#include "Platform/DirectX/IDirectDraw.h"

#define WIN32_LEAN_AND_MEAN
#include "Platform/DirectX/DDBLTFX.h"
#include "Platform/DirectX/DDSURFACEDESC.h"
#include "Platform/DirectX/IDirectDrawPalette.h"
#include "Platform/DirectX/IDirectDrawSurface.h"
#include "Engine/Math/CVSPoint.h"
#include "Engine/Math/CVSRect.h"
#include "Engine/Math/CVSSize.h"
#include "CDibContext.h"
#include "CDrawingContext.h"

enum {
	DIRECTDRAW_DISPLAY_COLOR_DEPTH_BITS = 8
};

#include <windows.h>

// FUNCTION: LEMBALL 0x00457410
CDirectDrawDriver::CDirectDrawDriver(CVSSize* p_size, int p_fullScreen)
{
	WNDCLASSA windowClass;
	DDSURFACEDESC description;
	unsigned long cooperativeFlags;
	long result;
	m_screenSize.m_width = p_size->m_width;
	m_screenSize.m_height = p_size->m_height;
	m_directDraw = NULL;
	m_primarySurface = NULL;
	m_surface24 = NULL;
	m_surface28 = NULL;
	m_surface2c = NULL;
	m_nextContextIndex = 1;
	m_paletteInterface = NULL;
	IDirectDraw** directDraw = &m_directDraw;
	m_driverModule = LoadLibraryA("DDRAW.DLL");
	if (m_driverModule == NULL) {
		return;
	}
	typedef long(__stdcall * CreateFunction)(void*, IDirectDraw**, void*);
	m_contextSurfaces[0] = (void*) GetProcAddress((HMODULE) m_driverModule, "DirectDrawCreate");
	if (m_contextSurfaces[0] == NULL) {
		return;
	}
	result = ((CreateFunction) m_contextSurfaces[0])(NULL, directDraw, NULL);
	if (result != 0) {
		OnDirectDrawCreateFailure(0, result);
		return;
	}
	if (p_fullScreen != 0) {
		windowClass.style = 0xb;
		windowClass.lpfnWndProc = (WNDPROC) CWnd::ProcessMessage;
		windowClass.cbClsExtra = 0;
		windowClass.cbWndExtra = 0;
		windowClass.hInstance = (HINSTANCE) g_pApplicationInstance;
		windowClass.hIcon = LoadIconA(NULL, IDI_APPLICATION);
		windowClass.hCursor = NULL;
		windowClass.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH);
		windowClass.lpszMenuName = NULL;
		windowClass.lpszClassName = "DirectDrawClass";
		LoadCursorA(NULL, IDC_ARROW);
		ATOM registered = RegisterClassA(&windowClass);
		ShowCursor(0);
		if (registered == 0) {
			*g_pErrorOutput << "Unable to register DD base window class\n";
			return;
		}
		m_window = CreateWindowExA(WS_EX_TOPMOST,
								   "DirectDrawClass",
								   "DirectDraw",
								   WS_POPUP,
								   0,
								   0,
								   m_screenSize.m_width,
								   m_screenSize.m_height,
								   NULL,
								   NULL,
								   (HINSTANCE) g_pApplicationInstance,
								   NULL);
		if (m_window == NULL) {
			return;
		}
		ShowWindow((HWND) m_window, SW_SHOW);
		UpdateWindow((HWND) m_window);
		SetForegroundWindow((HWND) m_window);
		cooperativeFlags = DDSCL_FULLSCREEN | DDSCL_EXCLUSIVE;
	}
	else {
		cooperativeFlags = DDSCL_NORMAL;
	}
	result = (*directDraw)->SetCooperativeLevel(m_window, cooperativeFlags);
	if (result != 0) {
		*g_pErrorOutput << "Direct Draw Set Coorperative Level (DD object) failed : "
						<< FormatUnknownDirectDrawError(result & DIRECT_DRAW_ERROR_CODE_MASK) << "\n";
		return;
	}
	if ((cooperativeFlags & DDSCL_FULLSCREEN) != 0) {
		result = (*directDraw)
					 ->SetDisplayMode(m_screenSize.m_width, m_screenSize.m_height, DIRECTDRAW_DISPLAY_COLOR_DEPTH_BITS);
		if (result != 0) {
			*g_pErrorOutput << "Direct Draw Set Display Mode failed : "
							<< FormatUnknownDirectDrawError(result & DIRECT_DRAW_ERROR_CODE_MASK) << "\n";
			return;
		}
	}
	IDirectDraw* interface = *directDraw;
	description.dwSize = sizeof(DDSURFACEDESC);
	description.dwFlags = 0;
	description.ddsCaps = DDSCAPS_PRIMARYSURFACE;
	result = interface->CreateSurface(&description, &m_primarySurface, NULL);
	if (result != 0) {
		*g_pErrorOutput << "Direct Draw Create Primary Surface failed : "
						<< FormatUnknownDirectDrawError(result & DIRECT_DRAW_ERROR_CODE_MASK) << "\n";
		return;
	}
	m_ready = 1;
}

// FUNCTION: LEMBALL 0x004576e0
CDirectDrawDriver::~CDirectDrawDriver()
{
	if (m_directDraw != NULL) {
		if (m_primarySurface != NULL) {
			m_primarySurface->Release();
			m_primarySurface = NULL;
		}
		if (m_surface28 != NULL) {
			m_surface28 = NULL;
		}
		if (m_surface2c != NULL) {
			m_surface2c = NULL;
		}
		if (m_paletteInterface != NULL) {
			m_paletteInterface->Release();
			m_paletteInterface = NULL;
		}
		m_directDraw->Release();
		m_directDraw = NULL;
		ShowCursor(1);
	}
	if (m_window != NULL) {
		m_window = NULL;
	}
}

// FUNCTION: LEMBALL 0x00457760
CDrawingContext* CDirectDrawDriver::CreateDrawingContext()
{
	int index = m_nextContextIndex;
	m_contextSurfaces[index] = NULL;
	m_nextContextIndex++;
	return new CDirectDrawContext(index);
}

// FUNCTION: LEMBALL 0x004577a0
int CDirectDrawDriver::DestroyDrawingContext(CDrawingContext* p_drawingContext)
{
	delete p_drawingContext;
	return 1;
}

// FUNCTION: LEMBALL 0x004577c0
bool CDirectDrawDriver::InitializeBitmapInfo(void* p_bitmapInfo)
{
	BITMAPINFOHEADER* header = (BITMAPINFOHEADER*) p_bitmapInfo;
	header->biPlanes = 1;
	header->biSize = 0x28;
	header->biCompression = 0;
	header->biSizeImage = 0;
	header->biXPelsPerMeter = 0;
	header->biYPelsPerMeter = 0;
	header->biClrUsed = 0;
	header->biHeight = DIB_INITIAL_TOP_DOWN_HEIGHT;
	header->biBitCount = 8;
	header->biClrImportant = 0;
	return true;
}

// FUNCTION: LEMBALL 0x00457800
CDibContext* CDirectDrawDriver::CreateDibContext(CDrawingContext* p_drawingContext, void* p_bitmapInfo)
{
	DDSURFACEDESC description;
	IDirectDrawSurface* surface;
	CDirectDrawSurface* context;
	BITMAPINFOHEADER* info = (BITMAPINFOHEADER*) p_bitmapInfo;
	long height = info->biHeight;
	description.dwSize = sizeof(DDSURFACEDESC);
	description.dwFlags = 6;
	description.ddsCaps = 0x40;
	if (height < 0) {
		height = -height;
	}
	long width = info->biWidth;
	description.dwHeight = height;
	description.dwWidth = width;
	IDirectDraw* directDraw = (IDirectDraw*) m_directDraw;
	if (directDraw->CreateSurface(&description, &surface, NULL) != 0) {
		return NULL;
	}
	context = new CDirectDrawSurface(surface);
	context->RefreshDescription();
	return context;
}

// FUNCTION: LEMBALL 0x004578a0
int CDirectDrawDriver::DestroyDibContext(CDibContext* p_dibContext)
{
	if (p_dibContext != NULL) {
		delete p_dibContext;
		return 1;
	}
	return 1;
}

#define DIRECTDRAW_PALETTE_ENTRY_COUNT 256
// FUNCTION: LEMBALL 0x004578c0
unsigned int CDirectDrawDriver::UpdateDibColourTable(CDrawingContext* p_drawingContext,
													 unsigned int p_startIndex,
													 unsigned int p_entryCount,
													 void* p_colours)
{
	return DIRECTDRAW_PALETTE_ENTRY_COUNT;
}
#undef DIRECTDRAW_PALETTE_ENTRY_COUNT

// FUNCTION: LEMBALL 0x004578d0
int CDirectDrawDriver::BitBltContexts(CDrawingContext* p_destination,
									  CVSRect* p_destinationRect,
									  CDrawingContext* p_source,
									  CVSPoint* p_sourcePosition)
{
	RECT source;
	CVSRect clipped;
	clipped.m_height = p_destinationRect->m_height;
	clipped.m_width = p_destinationRect->m_width;
	CVSPoint* point = p_destinationRect;
	clipped.m_x = point->m_x;
	clipped.m_y = point->m_y;
	CVSSize limits;
	limits.m_width = m_screenSize.m_width;
	limits.m_height = m_screenSize.m_height;
	if (clipped.m_x < 0) {
		clipped.m_width += clipped.m_x;
		clipped.m_x = 0;
	}
	if ((short) (clipped.m_width + clipped.m_x) > limits.m_width) {
		clipped.m_width = limits.m_width - clipped.m_x;
	}
	if (clipped.m_y < 0) {
		clipped.m_height += clipped.m_y;
		clipped.m_y = 0;
	}
	if ((short) (clipped.m_y + clipped.m_height) > limits.m_height) {
		clipped.m_height = limits.m_height - clipped.m_y;
	}
	if (clipped.m_width <= 0 || clipped.m_height <= 0) {
		clipped.m_width = 0;
		clipped.m_height = 0;
		clipped.m_y = 0;
		clipped.m_x = 0;
	}
	int width = clipped.m_width;
	int height = clipped.m_height;
	if (height * width == 0) {
		return 1;
	}
	long left = p_sourcePosition->m_x;
	long top = p_sourcePosition->m_y;
	width += left;
	height += top;
	source.left = left;
	source.top = top;
	source.right = width;
	source.bottom = height;
	long result = m_primarySurface->BltFast(
		clipped.m_x,
		clipped.m_y,
		((CDirectDrawSurface*) m_contextSurfaces[((CDirectDrawContext*) p_source)->m_surfaceIndex])->m_surface,
		&source,
		0);
	if (result != 0) {
		*g_pErrorOutput << "Blit failed: " << source.left << ", " << source.top << ", " << source.right << ", "
						<< source.bottom << " - " << FormatUnknownDirectDrawError(result & DIRECT_DRAW_ERROR_CODE_MASK)
						<< "\n";
	}
	return 1;
}

// FUNCTION: LEMBALL 0x00457a60
int CDirectDrawDriver::StretchBltContexts(CDrawingContext* p_destination,
										  CVSRect* p_destinationRect,
										  CDrawingContext* p_source,
										  CVSRect* p_sourceRect)
{
	CVSRect clipped;
	clipped.m_width = p_destinationRect->m_width;
	clipped.m_height = p_destinationRect->m_height;
	CVSPoint* point = p_destinationRect;
	clipped.m_x = point->m_x;
	clipped.m_y = point->m_y;
	CVSSize limits;
	limits.m_width = m_screenSize.m_width;
	limits.m_height = m_screenSize.m_height;
	if (clipped.m_x < 0) {
		clipped.m_width += clipped.m_x;
		clipped.m_x = 0;
	}
	if ((short) (clipped.m_width + clipped.m_x) > limits.m_width) {
		clipped.m_width = limits.m_width - clipped.m_x;
	}
	if (clipped.m_y < 0) {
		clipped.m_height += clipped.m_y;
		clipped.m_y = 0;
	}
	if ((short) (clipped.m_y + clipped.m_height) > limits.m_height) {
		clipped.m_height = limits.m_height - clipped.m_y;
	}
	if (clipped.m_width <= 0 || clipped.m_height <= 0) {
		clipped.m_y = 0;
		clipped.m_height = 0;
		clipped.m_x = 0;
		clipped.m_width = 0;
	}
	if ((int) clipped.m_height * (int) clipped.m_width == 0) {
		return 1;
	}
	if ((int) p_sourceRect->m_height * (int) p_sourceRect->m_width == 0) {
		return 1;
	}
	RECT source;
	RECT destination;
	source.left = p_sourceRect->m_x;
	source.top = p_sourceRect->m_y;
	source.right = (short) (p_sourceRect->m_x + p_sourceRect->m_width);
	source.bottom = (short) (p_sourceRect->m_height + p_sourceRect->m_y);
	destination.left = clipped.m_x;
	destination.top = clipped.m_y;
	destination.right = (short) (clipped.m_x + clipped.m_width);
	destination.bottom = (short) (clipped.m_height + clipped.m_y);
	long result = m_primarySurface->Blt(
		&destination,
		((CDirectDrawSurface*) m_contextSurfaces[((CDirectDrawContext*) p_source)->m_surfaceIndex])->m_surface,
		&source,
		0,
		NULL);
	if (result != 0) {
		*g_pErrorOutput << "Blit failed: " << destination.left << ", " << destination.top << ", " << destination.right
						<< ", " << destination.bottom << " - "
						<< FormatUnknownDirectDrawError(result & DIRECT_DRAW_ERROR_CODE_MASK) << "\n";
	}
	return 1;
}

// FUNCTION: LEMBALL 0x00457c50
CDibContext* CDirectDrawDriver::SelectDibContext(CDrawingContext* p_drawingContext, CDibContext* p_dibContext)
{
	CDirectDrawContext* context = (CDirectDrawContext*) p_drawingContext;
	m_contextSurfaces[context->m_surfaceIndex] = p_dibContext;
	return p_dibContext;
}

// FUNCTION: LEMBALL 0x00457c70
CDibContext* CDirectDrawDriver::RestoreDibContext(CDrawingContext* p_drawingContext, CDibContext* p_dibContext)
{
	return p_dibContext;
}

// FUNCTION: LEMBALL 0x00457c80
bool CDirectDrawDriver::CreatePalette(void* p_paletteDescription)
{
	enum {
		DDBLT_COLORFILL = 0x400
	};
	DDBLTFX effects;
	long result;
	LOGPALETTE* palette = (LOGPALETTE*) p_paletteDescription;
	effects.dwSize = sizeof(DDBLTFX);
	effects.dwFillColor = 0;
	IDirectDrawSurface* primary = (IDirectDrawSurface*) m_primarySurface;
	result = primary->Blt(NULL, NULL, NULL, DDBLT_COLORFILL, &effects);
	if (result != 0) {
		*g_pErrorOutput << "Direct Draw Initial rectangle blit failed : "
						<< FormatUnknownDirectDrawError(result & DIRECT_DRAW_ERROR_CODE_MASK) << "\n";
		return false;
	}
	if (m_paletteInterface != NULL) {
		result = m_paletteInterface->SetEntries(0, 0, palette->palNumEntries, palette->palPalEntry);
		if (result != 0) {
			*g_pErrorOutput << "Direct Draw Set Palette Entries failed: "
							<< FormatUnknownDirectDrawError(result & DIRECT_DRAW_ERROR_CODE_MASK) << "\n";
			return false;
		}
	}
	else {
		result = m_directDraw->CreatePalette(0xc, palette->palPalEntry, &m_paletteInterface, NULL);
		if (result != 0) {
			*g_pErrorOutput << "Direct Draw Create Palette failed: "
							<< FormatUnknownDirectDrawError(result & DIRECT_DRAW_ERROR_CODE_MASK) << "\n";
			return false;
		}
		primary = (IDirectDrawSurface*) m_primarySurface;
		result = primary->SetPalette(m_paletteInterface);
		if (result != 0) {
			*g_pErrorOutput << "Direct Draw Set Palette failed: "
							<< FormatUnknownDirectDrawError(result & DIRECT_DRAW_ERROR_CODE_MASK) << "\n";
			return false;
		}
	}
	return true;
}

// FUNCTION: LEMBALL 0x00458350
bool CDirectDrawDriver::HasPalette()
{
	return m_paletteInterface != NULL;
}
