#ifndef LEMBALL_GRAPHICS_DRIVER_H
#define LEMBALL_GRAPHICS_DRIVER_H

// FUNCTION: LEMBALL 0x00458380
// DeleteDirectDrawBinding
// FUNCTION: LEMBALL 0x00458210
// VsGdiDeleteDisplayState

#include "ENGINE/CORE/WIN32.H"

enum {
	VsGdiDRIVER_METRICS = 1,
	VsGdiDRIVER_DIB_320_200 = 2,
	VsGdiDRIVER_DIB_640_480 = 3,
	VsGdiDRIVER_DDRAW_320_200 = 4,
	VsGdiDRIVER_DDRAW_640_480 = 6,
	VsGdiDRIVER_AUTO = 8,
};

// SIZE 0x08
struct VsGdiRect {
	short m_x;  // 0x00
	short m_y;  // 0x02
	short m_cx; // 0x04
	short m_cy; // 0x06
};

class VsGdiHelperSurface;

// SIZE 0x10
struct VsGdiHelperDispatchQueue {
	void** m_ppEntries;
	int m_cEntries;
	int m_nReserved08;
	VsGdiHelperSurface* m_pHelperTarget;

	void AppendPointerQueueEntry(void* pEntry);
};

// SIZE 0x1c
class VsGdiDisplayState {
public:
	VsGdiDisplayState(void);
	~VsGdiDisplayState(void);
	void* CreateDisplayBinding(void);
	int ReleaseDisplayBinding(void* pBinding);
	int InitializeDisplayBitmapInfo(void* pBitmapInfo);
	void* CreateDisplayBitmap(int nDriver, void* pBitmapInfo);
	int ReleaseDisplayBitmap(void* pBitmap);
	void* BindDisplayBitmap(int nDriver, void* pBitmap);
	void* DeleteDisplayBitmap(int nDriver, void* pBitmap);
	unsigned int DispatchSplitRectAtBackingRowBoundary(void* pDestination,
													   short* pDestinationRect,
													   void* pSource,
													   short* pSourceRect,
													   void* pHelperTarget);

protected:
	void* m_pVtable;           // 0x00
	HMODULE m_hGraphicsModule; // 0x04
	void* m_pReserved08;       // 0x08
	int m_fReady;              // 0x0c
	HWND m_hDisplayWindow;     // 0x10
	short m_cxDisplay;         // 0x14
	short m_cyDisplay;         // 0x16
	int m_dwReserved18;        // 0x18
};

// SIZE 0x04
struct VsGdiDisplayStateDispatch {
	virtual void Destroy(void) = 0;                                        // vtable+0x00
	virtual void* CreateDisplayBinding(void) = 0;                          // vtable+0x04
	virtual int ReleaseDisplayBinding(void* pBinding) = 0;                 // vtable+0x08
	virtual int InitializeDisplayBitmapInfo(void* pBitmapInfo) = 0;        // vtable+0x0c
	virtual void* CreateDisplayBitmap(int nDriver, void* pBitmapInfo) = 0; // vtable+0x10
	virtual int ReleaseDisplayBitmap(void* pBitmap) = 0;                   // vtable+0x14
	virtual void Reserved6(void) = 0;                                      // vtable+0x18
	virtual void Reserved7(void) = 0;                                      // vtable+0x1c
	virtual void Reserved8(void) = 0;                                      // vtable+0x20
	virtual void* BindDisplayBitmap(int nDriver, void* pBitmap) = 0;       // vtable+0x24
	virtual void* DeleteDisplayBitmap(int nDriver, void* pBitmap) = 0;     // vtable+0x28
	virtual void Reserved11(void) = 0;                                     // vtable+0x2c
	virtual void Reserved12(void) = 0;                                     // vtable+0x30
	virtual int IsReady(void) = 0;                                         // vtable+0x34
};

// SIZE 0x0c
struct VsGdiSelectedGraphicsDriverRuntime {
	int m_nSelectedDriver;       // 0x00
	void* m_pWindowOwner;        // 0x04
	int m_fFallbackWarningShown; // 0x08

	int InitializeSelectedGraphicsDriver(int nRequestedDriver);
	void ResizeActiveDisplayState(unsigned short* pSize);
};

void LEMBALL_FASTCALL NotifyWindowOwnersDisplayChanged(const int* pnSelectedDriver);

// SIZE 0x1c
class VsGdiMetricsDisplayState : public VsGdiDisplayState {
public:
	VsGdiMetricsDisplayState(void);
	void* Delete(int fDelete);
};

// SIZE 0x560
class VsGdiHelperSurface {
public:
	int ClipRenderRectToSurfaceBounds(VsGdiRect* pRect, VsGdiRect* pClippedRect);
	void FillRect(VsGdiRect Rect, int nColor);
	void UpdateWorkingRectAndBacking(const VsGdiRect* pRect);
	void ComputeBackingDimensions(short* paOut, short* paRect, int nWidth);
	void ConfigureBackingStrideAndOrigin(int nStride, int nOrigin);
	void ClearBackingBorderRows(void);
	void ClearBackingBorderRowsThunk(void);
	void CopyClippedBitmapIntoHelperBufferAndNotify(const void* pBitmapInfo, const void* pBitmapBits);
	void SplitRectAtBackingRowBoundary(short* pRect, short** ppFirstRect, short** ppSecondRect);

private:
	unsigned char m_abState[0x560]; // 0x00
};

// SIZE 0x30
class VsGdiDibDisplayState : public VsGdiMetricsDisplayState {
public:
	VsGdiDibDisplayState(const unsigned short* pDisplaySize);
	~VsGdiDibDisplayState(void);
	void* Delete(int fDelete);

	int StretchDisplayRect(void* pDestinationBinding,
						   short* pDestinationRect,
						   void* pSourceBinding,
						   short* pSourceRect);
	int CopyDisplayRect(void* pDestinationBinding, short* pDestinationRect, void* pSourceBinding, short* pSourcePoint);

private:
	void* m_pDisplayAddress;             // 0x1c
	int m_dwReserved20;                  // 0x20
	FARPROC m_pfnDisplayDibStart32;      // 0x24
	FARPROC m_pfnDisplayDibEnd32;        // 0x28
	FARPROC m_pfnDisplayDibGetAddress32; // 0x2c
};

// SIZE 0x4a8
class VsGdiDirectDrawDisplayState : public VsGdiDisplayState {
public:
	VsGdiDirectDrawDisplayState(const unsigned short* pDisplaySize, int fCreateWindow);
	~VsGdiDirectDrawDisplayState(void);
	void* Delete(int fDelete);

	void* CreateDirectDrawDisplayBinding(void);
	int ReleaseDirectDrawDisplayBinding(void* pBinding);
	int InitializeDirectDrawBitmapInfo(void* pBitmapInfo);
	void* CreateDirectDrawDisplayBitmap(int nDriver, void* pBitmapInfo);
	int ReleaseDirectDrawDisplayBitmap(void* pBitmap);
	int StretchDirectDrawSurfaceRect(void* pDestinationBinding,
									 short* pDestinationRect,
									 void* pSourceBinding,
									 short* pSourceRect);
	int CopyDirectDrawSurfaceRect(void* pDestinationBinding,
								  short* pDestinationRect,
								  void* pSourceBinding,
								  short* pSourcePoint);
	int ApplyDirectDrawPalette(void* pPalette);
	int BindDirectDrawSurface(int nDriver, void* pSurface);
	int ReleaseDirectDrawSurface(int nDriver, void* pSurface);
	int IsDirectDrawPaletteReady(void) const;

private:
	void* m_pDirectDrawObject;           // 0x1c
	void* m_pPrimarySurface;             // 0x20
	void* m_pOffscreenSurface;           // 0x24
	void* m_pPalette;                    // 0x28
	int m_fDirectDrawCursorVisible;      // 0x2c
	void* m_pDirectDrawPalette;          // 0x30
	unsigned char m_abReserved34[0x6c];  // 0x34
	FARPROC m_pfnDirectDrawCreate;       // 0xa0
	unsigned char m_abReservedA4[0x400]; // 0xa4
	int m_fWindowOwnershipInitialized;   // 0x4a4
};

int InitializeResourceGeometryHelperRuntime(void);
int ShutdownResourceGeometryHelperRuntime(void);
void LEMBALL_FASTCALL BuildGeometryHelperFromRenderRect(void* pOwner);
void LEMBALL_FASTCALL SelectIntroSequenceMediaPath(void* pOwner);
void LEMBALL_FASTCALL DestroyGeometryHelperPointerArray(void* pHelper);
void LEMBALL_FASTCALL BindHelperTargetDisplayDc(void* pTarget, int nUnused, void* hDc);
void LEMBALL_FASTCALL PropagateGlobalPaletteToResourceGeometries(void* pTarget);
void LEMBALL_FASTCALL PropagateGlobalPaletteToResourceGeometriesBody(void* pTarget);
void LEMBALL_FASTCALL CopyRectTupleIfExtentEmpty(void* pTarget, int nUnused, short* paRect);
extern void* g_pSelectedGraphicsDriverRuntime;
extern VsGdiDisplayState* g_pDisplayState;
extern void* g_pResourceGeometryHelperTarget;
extern unsigned int g_adwGlobalResourceGeometryPaletteTable[256];
void LEMBALL_FASTCALL InitializeHelperUploadStatePending(int nUploadState);
void LEMBALL_FASTCALL PromoteHelperUploadStateToActive(int nUploadState);

extern int g_fStartupGraphicsDriverWing;
extern int g_fStartupGraphicsDriverCds;
extern int g_fStartupGraphicsDriverGdk;
extern int g_fStartupFullscreen;
extern int* g_pArrowCursorStatusIndicatorRenderClient;

#endif
