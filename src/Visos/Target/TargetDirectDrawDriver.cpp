#include "TargetDirectDrawDriver.h"

#include "../Foundation/VsOStream.h"
#include "IDirectDraw.h"
#include "TargetDirectDrawContext.h"
#include "TargetDirectDrawError.h"
#include "TargetDirectDrawSurfaceContext.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// FUNCTION: LEMBALL 0x00457760
TargetDrawingContext* TargetDirectDrawDriver::CreateDrawingContext()
{
	int index = m_nextContextIndex;
	m_contextSurfaces[index] = 0;
	m_nextContextIndex++;
	return new TargetDirectDrawContext(index);
}

// FUNCTION: LEMBALL 0x004577a0
int TargetDirectDrawDriver::DestroyDrawingContext(TargetDrawingContext* p_drawingContext)
{
	delete p_drawingContext;
	return 1;
}

// FUNCTION: LEMBALL 0x004577c0
bool TargetDirectDrawDriver::InitializeBitmapInfo(void* p_bitmapInfo)
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
TargetDibContext* TargetDirectDrawDriver::CreateDIBContext(TargetDrawingContext* p_drawingContext, void* p_bitmapInfo)
{
	DDSURFACEDESC description;
	IDirectDrawSurface* surface;
	TargetDirectDrawSurfaceContext* context;
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
	context = new TargetDirectDrawSurfaceContext(surface);
	context->RefreshDescription();
	return context;
}

// FUNCTION: LEMBALL 0x004578a0
int TargetDirectDrawDriver::DestroyDIBContext(TargetDibContext* p_dibContext)
{
	if (p_dibContext != 0) {
		delete p_dibContext;
		return 1;
	}
	return 1;
}

// FUNCTION: LEMBALL 0x004578c0
unsigned int TargetDirectDrawDriver::UpdateDIBColourTable(TargetDrawingContext* p_drawingContext,
														  unsigned int p_startIndex,
														  unsigned int p_entryCount,
														  void* p_colours)
{
	return 256;
}

// FUNCTION: LEMBALL 0x00457c50
TargetDibContext* TargetDirectDrawDriver::SelectDIBContext(TargetDrawingContext* p_drawingContext,
														   TargetDibContext* p_dibContext)
{
	TargetDirectDrawContext* context = (TargetDirectDrawContext*) p_drawingContext;
	m_contextSurfaces[context->m_surfaceIndex] = p_dibContext;
	return p_dibContext;
}

// FUNCTION: LEMBALL 0x00457c70
TargetDibContext* TargetDirectDrawDriver::RestoreDIBContext(TargetDrawingContext* p_drawingContext,
															TargetDibContext* p_dibContext)
{
	return p_dibContext;
}

// FUNCTION: LEMBALL 0x00457c80
bool TargetDirectDrawDriver::CreatePalette(void* p_paletteDescription)
{
	DDBLTFX effects;
	long result;
	LOGPALETTE* palette = (LOGPALETTE*) p_paletteDescription;
	effects.dwSize = sizeof(DDBLTFX);
	effects.dwFillColor = 0;
	IDirectDrawSurface* primary = (IDirectDrawSurface*) m_primarySurface;
	result = primary->lpVtbl->Blt(primary, 0, 0, 0, 0x400, &effects);
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
		result = ((IDirectDraw*) m_directDraw)->CreatePalette(0xc, palette->palPalEntry, &m_paletteInterface, 0);
		if (result != 0) {
			*g_pErrorOutput << "Direct Draw Create Palette failed: " << FormatUnknownDirectDrawError(result & 0xfff)
							<< "\n";
			return 0;
		}
		primary = (IDirectDrawSurface*) m_primarySurface;
		result = primary->lpVtbl->SetPalette(primary, m_paletteInterface);
		if (result != 0) {
			*g_pErrorOutput << "Direct Draw Set Palette failed: " << FormatUnknownDirectDrawError(result & 0xfff)
							<< "\n";
			return 0;
		}
	}
	return 1;
}
