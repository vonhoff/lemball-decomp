#include "CDirectDrawDriver.h"

#include "../../Platform/Windows/Entry.h"
#include "../Foundation/CVsOStream.h"
#include "../Graphics/CWnd.h"
#include "CDirectDrawContext.h"
#include "CDirectDrawSurface.h"
#include "DirectDrawError.h"
#include "Platform/DirectX/IDirectDraw.h"

#define WIN32_LEAN_AND_MEAN
#include "Platform/DirectX/DDBLTFX.h"
#include "Platform/DirectX/DDSURFACEDESC.h"
#include "Platform/DirectX/IDirectDrawPalette.h"
#include "Platform/DirectX/IDirectDrawSurface.h"
#include "Visos/Foundation/CVsPoint.h"
#include "Visos/Foundation/CVsRect.h"
#include "Visos/Foundation/CVsSize.h"
#include "Visos/Target/CDibContext.h"
#include "Visos/Target/CDrawingContext.h"

#include <windows.h>

// FUNCTION: LEMBALL 0x00457410
CDirectDrawDriver::CDirectDrawDriver(CVsSize* p_size, int p_fullScreen)
{
	WNDCLASSA windowClass;
	DDSURFACEDESC description;
	unsigned long cooperativeFlags;
	long result;
	m_screenSize.m_width = p_size->m_width;
	m_screenSize.m_height = p_size->m_height;
	IDirectDraw** directDraw = &m_directDraw;
	*directDraw = 0;
	m_primarySurface = 0;
	m_surface24 = 0;
	m_surface28 = 0;
	m_surface2c = 0;
	m_nextContextIndex = 1;
	m_paletteInterface = 0;
	m_driverModule = LoadLibraryA("DDRAW.DLL");
	if (m_driverModule == 0) {
		return;
	}
	typedef long(__stdcall * CreateFunction)(void*, IDirectDraw**, void*);
	m_contextSurfaces[0] = (void*) GetProcAddress((HMODULE) m_driverModule, "DirectDrawCreate");
	if (m_contextSurfaces[0] == 0) {
		return;
	}
	result = ((CreateFunction) m_contextSurfaces[0])(0, directDraw, 0);
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
		windowClass.hIcon = LoadIconA(0, IDI_APPLICATION);
		windowClass.hCursor = 0;
		windowClass.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH);
		windowClass.lpszMenuName = 0;
		windowClass.lpszClassName = "DirectDrawClass";
		// Original leaves hCursor null after loading the system cursor.
		LoadCursorA(0, IDC_ARROW);
		ATOM registered = RegisterClassA(&windowClass);
		ShowCursor(0);
		if (registered == 0) {
			*g_pErrorOutput << "Unable to register DD base window class\n";
			return;
		}
		m_window = CreateWindowExA(8,
								   "DirectDrawClass",
								   "DirectDraw",
								   WS_POPUP,
								   0,
								   0,
								   m_screenSize.m_width,
								   m_screenSize.m_height,
								   0,
								   0,
								   (HINSTANCE) g_pApplicationInstance,
								   0);
		if (m_window == 0) {
			return;
		}
		ShowWindow((HWND) m_window, SW_SHOW);
		UpdateWindow((HWND) m_window);
		SetForegroundWindow((HWND) m_window);
		cooperativeFlags = 0x11;
	}
	else {
		cooperativeFlags = 8;
	}
	result = (*directDraw)->SetCooperativeLevel(m_window, cooperativeFlags);
	if (result != 0) {
		*g_pErrorOutput << "Direct Draw Set Coorperative Level (DD object) failed : "
						<< FormatUnknownDirectDrawError(result & 0xfff) << "\n";
		return;
	}
	if ((cooperativeFlags & 1) != 0) {
		result = (*directDraw)->SetDisplayMode(m_screenSize.m_width, m_screenSize.m_height, 8);
		if (result != 0) {
			*g_pErrorOutput << "Direct Draw Set Display Mode failed : " << FormatUnknownDirectDrawError(result & 0xfff)
							<< "\n";
			return;
		}
	}
	description.dwSize = sizeof(DDSURFACEDESC);
	description.dwFlags = 0;
	description.ddsCaps = 0x200;
	result = (*directDraw)->CreateSurface(&description, &m_primarySurface, 0);
	if (result != 0) {
		*g_pErrorOutput << "Direct Draw Create Primary Surface failed : "
						<< FormatUnknownDirectDrawError(result & 0xfff) << "\n";
		return;
	}
	m_ready = 1;
}

// FUNCTION: LEMBALL 0x004576e0
CDirectDrawDriver::~CDirectDrawDriver()
{
	if (m_directDraw != 0) {
		if (m_primarySurface != 0) {
			m_primarySurface->Release();
			m_primarySurface = 0;
		}
		if (m_surface28 != 0) {
			m_surface28 = 0;
		}
		if (m_surface2c != 0) {
			m_surface2c = 0;
		}
		if (m_paletteInterface != 0) {
			m_paletteInterface->Release();
			m_paletteInterface = 0;
		}
		m_directDraw->Release();
		m_directDraw = 0;
		ShowCursor(1);
	}
	if (m_window != 0) {
		m_window = 0;
	}
}

// FUNCTION: LEMBALL 0x00457760
CDrawingContext* CDirectDrawDriver::CreateDrawingContext()
{
	int index = m_nextContextIndex;
	m_contextSurfaces[index] = 0;
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
	header->biHeight = -1;
	header->biBitCount = 8;
	header->biClrImportant = 0;
	return 1;
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
	if (directDraw->CreateSurface(&description, &surface, 0) != 0) {
		return 0;
	}
	context = new CDirectDrawSurface(surface);
	context->RefreshDescription();
	return context;
}

// FUNCTION: LEMBALL 0x004578a0
int CDirectDrawDriver::DestroyDibContext(CDibContext* p_dibContext)
{
	if (p_dibContext != 0) {
		delete p_dibContext;
		return 1;
	}
	return 1;
}

// FUNCTION: LEMBALL 0x004578c0
unsigned int CDirectDrawDriver::UpdateDibColourTable(CDrawingContext* p_drawingContext,
													 unsigned int p_startIndex,
													 unsigned int p_entryCount,
													 void* p_colours)
{
	return 256;
}

// FUNCTION: LEMBALL 0x004578d0
int CDirectDrawDriver::BitBltContexts(CDrawingContext* p_destination,
									  CVsRect* p_destinationRect,
									  CDrawingContext* p_source,
									  CVsPoint* p_sourcePosition)
{
	RECT source;
	CVsRect clipped;
	clipped.m_height = p_destinationRect->m_height;
	clipped.m_width = p_destinationRect->m_width;
	CVsPoint* point = p_destinationRect;
	clipped.m_x = point->m_x;
	clipped.m_y = point->m_y;
	CVsSize limits;
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
	int height = clipped.m_height;
	int width = clipped.m_width;
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
						<< source.bottom << " - " << FormatUnknownDirectDrawError(result & 0xfff) << "\n";
	}
	return 1;
}

// FUNCTION: LEMBALL 0x00457a60
int CDirectDrawDriver::StretchBltContexts(CDrawingContext* p_destination,
										  CVsRect* p_destinationRect,
										  CDrawingContext* p_source,
										  CVsRect* p_sourceRect)
{
	CVsRect clipped;
	clipped.m_width = p_destinationRect->m_width;
	clipped.m_height = p_destinationRect->m_height;
	CVsPoint* point = p_destinationRect;
	clipped.m_x = point->m_x;
	clipped.m_y = point->m_y;
	CVsSize limits;
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
		0);
	if (result != 0) {
		*g_pErrorOutput << "Blit failed: " << destination.left << ", " << destination.top << ", " << destination.right
						<< ", " << destination.bottom << " - " << FormatUnknownDirectDrawError(result & 0xfff) << "\n";
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
	DDBLTFX effects;
	long result;
	LOGPALETTE* palette = (LOGPALETTE*) p_paletteDescription;
	effects.dwSize = sizeof(DDBLTFX);
	effects.dwFillColor = 0;
	IDirectDrawSurface* primary = (IDirectDrawSurface*) m_primarySurface;
	result = primary->Blt(0, 0, 0, 0x400, &effects);
	if (result != 0) {
		*g_pErrorOutput << "Direct Draw Initial rectangle blit failed : "
						<< FormatUnknownDirectDrawError(result & 0xfff) << "\n";
		return 0;
	}
	if (m_paletteInterface != 0) {
		result = m_paletteInterface->SetEntries(0, 0, palette->palNumEntries, palette->palPalEntry);
		if (result != 0) {
			*g_pErrorOutput << "Direct Draw Set Palette Entries failed: "
							<< FormatUnknownDirectDrawError(result & 0xfff) << "\n";
			return 0;
		}
	}
	else {
		result = m_directDraw->CreatePalette(0xc, palette->palPalEntry, &m_paletteInterface, 0);
		if (result != 0) {
			*g_pErrorOutput << "Direct Draw Create Palette failed: " << FormatUnknownDirectDrawError(result & 0xfff)
							<< "\n";
			return 0;
		}
		primary = (IDirectDrawSurface*) m_primarySurface;
		result = primary->SetPalette(m_paletteInterface);
		if (result != 0) {
			*g_pErrorOutput << "Direct Draw Set Palette failed: " << FormatUnknownDirectDrawError(result & 0xfff)
							<< "\n";
			return 0;
		}
	}
	return 1;
}

// FUNCTION: LEMBALL 0x00458350
bool CDirectDrawDriver::HasPalette()
{
	return m_paletteInterface != 0;
}
