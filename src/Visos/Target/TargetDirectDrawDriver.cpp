#include "TargetDirectDrawDriver.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

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

// FUNCTION: LEMBALL 0x004578a0
int TargetDirectDrawDriver::DestroyDIBContext(TargetDibContext* p_dibContext)
{
	if (p_dibContext != 0) {
		delete p_dibContext;
		return 1;
	}
	return 1;
}

// FUNCTION: LEMBALL 0x00457c70
TargetDibContext* TargetDirectDrawDriver::RestoreDIBContext(TargetDrawingContext* p_drawingContext,
															TargetDibContext* p_dibContext)
{
	return p_dibContext;
}
