#include "GdiDevice.h"

// 68K 0x10107d0e __ct__10CGDIDeviceFi
// STUB: LEMBALL 0x0046bc00
GdiDevice::GdiDevice(int p_arg0)
{
}

// 68K 0x10107dc4 __dt__10CGDIDeviceFv
// STUB: LEMBALL 0x0046bc90
GdiDevice::~GdiDevice()
{
	// STRING: LEMBALL 0x004a2060 "Trying to delete device when a surface has not been free'd\n"
}

// 68K 0x10107e48 FindFreeSurface__10CGDIDeviceFv
// STUB: LEMBALL 0x0046bce0
int GdiDevice::FindFreeSurface()
{
	return 0;
}

// 68K 0x10107e98 AllocateSurface__10CGDIDeviceFRC7CVSRectP8CSurface
// STUB: LEMBALL 0x0046bd10
Surface* GdiDevice::AllocateSurface(const VsRect& p_rect, Surface* p_parentSurface)
{
	// STRING: LEMBALL 0x004a209c "Surface"
	return 0;
}

// 68K 0x101080fa FreeSurface__10CGDIDeviceFP8CSurface
// STUB: LEMBALL 0x0046bed0
void GdiDevice::FreeSurface(Surface* p_surface)
{
}

// 68K 0x101081bc Sync__10CGDIDeviceFv
// STUB: LEMBALL 0x0046bf60
void GdiDevice::Sync()
{
}

// 68K 0x101081dc FindSurface__10CGDIDeviceFP8CSurface
// STUB: LEMBALL 0x0046bf70
int GdiDevice::FindSurface(Surface* p_surface)
{
	return 0;
}

// 68K 0x1010823a Flush__10CGDIDeviceFP8CSurface
// STUB: LEMBALL 0x0046bfd0
void GdiDevice::Flush(Surface* p_surface)
{
}
