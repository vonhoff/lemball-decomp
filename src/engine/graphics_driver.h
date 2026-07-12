#ifndef LEMBALL_GRAPHICS_DRIVER_H
#define LEMBALL_GRAPHICS_DRIVER_H

#include "platform/win32.h"

enum {
    VSGDI_DRIVER_METRICS = 1,
    VSGDI_DRIVER_DIB_320_200 = 2,
    VSGDI_DRIVER_DIB_640_480 = 3,
    VSGDI_DRIVER_DDRAW_320_200 = 4,
    VSGDI_DRIVER_DDRAW_640_480 = 6,
    VSGDI_DRIVER_AUTO = 8,
};

struct VSGDI_Rect {
    short m_x;
    short m_y;
    short m_cx;
    short m_cy;
};

class VSGDI_DisplayState {
public:
    VSGDI_DisplayState(void);
    int Create(HWND hWnd);

    void *CreateDisplayBinding(void);
    void ReleaseDisplayBinding(void *pBinding);
    int InitializeDisplayBitmapInfo(void *pBitmapInfo);
    void *CreateDisplayBitmap(int nDriver, void *pBitmapInfo);
    int ReleaseDisplayBitmap(void *pBitmap);
    int BindDisplayBitmap(int nDriver, void *pBitmap);
    int DeleteDisplayBitmap(int nDriver, void *pBitmap);

    int IsReady(void) const;
    short Width(void) const;
    short Height(void) const;

protected:
    void *m_pVtable;
    HMODULE m_hGraphicsModule;
    void *m_pReserved08;
    int m_fReady;
    HWND m_hDisplayWindow;
    short m_cxDisplay;
    short m_cyDisplay;
    int m_dwReserved18;
};

struct VSGDI_DisplayStateDispatch {
    virtual void Destroy(void) = 0;
    virtual void *CreateDisplayBinding(void) = 0;
    virtual void ReleaseDisplayBinding(void *pBinding) = 0;
    virtual int InitializeDisplayBitmapInfo(void *pBitmapInfo) = 0;
    virtual void *CreateDisplayBitmap(int nDriver, void *pBitmapInfo) = 0;
    virtual int ReleaseDisplayBitmap(void *pBitmap) = 0;
    virtual void Reserved6(void) = 0;
    virtual void Reserved7(void) = 0;
    virtual void Reserved8(void) = 0;
    virtual int BindDisplayBitmap(int nDriver, void *pBitmap) = 0;
    virtual int DeleteDisplayBitmap(int nDriver, void *pBitmap) = 0;
    virtual void Reserved11(void) = 0;
    virtual void Reserved12(void) = 0;
    virtual int IsReady(void) = 0;
};

struct VSGDI_SelectedGraphicsDriverRuntime {
    int m_nSelectedDriver;
    void *m_pWindowOwner;
    int m_fFallbackWarningShown;

    int InitializeSelectedGraphicsDriver(int nRequestedDriver);
    void ResizeActiveDisplayState(unsigned short *pSize);
};

void LEMBALL_FASTCALL NotifyWindowOwnersDisplayChanged(const int *pnSelectedDriver);

class VSGDI_MetricsDisplayState : public VSGDI_DisplayState {
public:
    VSGDI_MetricsDisplayState(void);

    int Create(HWND hWnd);
};

class VSGDI_HelperSurface {
public:
    VSGDI_HelperSurface(void);

    void UpdateWorkingRectAndBacking(const VSGDI_Rect *pRect);
    void ComputeBackingDimensions(short *paOut, short *paRect, int nWidth);
    void ConfigureBackingStrideAndOrigin(int nStride, int nOrigin);
    void ClearBackingBorderRows(void);
    void ClearBackingBorderRowsThunk(void);
    int IsReady(void) const;
    short BackingWidth(void) const;
    short BackingHeight(void) const;

private:
    unsigned char m_abState[0x560];
};

class VSGDI_DibDisplayState : public VSGDI_MetricsDisplayState {
public:
    VSGDI_DibDisplayState(const unsigned short *pDisplaySize);

    int Create(HWND hWnd);

private:
    void *m_pDisplayAddress;
    int m_dwReserved20;
    FARPROC m_pfnDisplayDibStart32;
    FARPROC m_pfnDisplayDibEnd32;
    FARPROC m_pfnDisplayDibGetAddress32;
};

class VSGDI_DirectDrawDisplayState : public VSGDI_DisplayState {
public:
    VSGDI_DirectDrawDisplayState(const unsigned short *pDisplaySize, int fCreateWindow);

    int Create(HWND hWnd);

private:
    void *m_pDirectDrawObject;
    void *m_pPrimarySurface;
    void *m_pOffscreenSurface;
    void *m_pPalette;
    int m_fDirectDrawCursorVisible;
    unsigned char m_abReserved34[0x70];
    FARPROC m_pfnDirectDrawCreate;
    unsigned char m_abReservedA4[0x400];
    int m_fWindowOwnershipInitialized;
};

int InitializeResourceGeometryHelperRuntime(void);
int ShutdownResourceGeometryHelperRuntime(void);
void LEMBALL_FASTCALL BuildGeometryHelperFromRenderRect(void *pOwner);
int InitializeSelectedGraphicsDriver(int nRequestedDriver);
int GetSelectedGraphicsDriverId(void);
extern void *g_pSelectedGraphicsDriverRuntime;
VSGDI_DisplayState *GetDisplayState(void);
void LEMBALL_FASTCALL InitializeHelperUploadStatePending(int nUploadState);
void LEMBALL_FASTCALL PromoteHelperUploadStateToActive(int nUploadState);

extern int g_fStartupGraphicsDriverWing;
extern int g_fStartupGraphicsDriverCds;
extern int g_fStartupGraphicsDriverGdk;
extern int g_fStartupFullscreen;
extern int *g_pArrowCursorStatusIndicatorRenderClient;

#endif
