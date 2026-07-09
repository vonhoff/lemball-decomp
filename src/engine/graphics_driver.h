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
    virtual ~VSGDI_DisplayState(void) {
    }

    virtual int Create(HWND hWnd) = 0;

    int IsReady(void) const;
    short Width(void) const;
    short Height(void) const;

protected:
    HMODULE m_hGraphicsModule;
    void *m_pReserved08;
    int m_fReady;
    HWND m_hDisplayWindow;
    short m_cxDisplay;
    short m_cyDisplay;
    int m_dwReserved18;
};

class VSGDI_MetricsDisplayState : public VSGDI_DisplayState {
public:
    VSGDI_MetricsDisplayState(void);

    virtual int Create(HWND hWnd);
};

class VSGDI_HelperSurface {
public:
    VSGDI_HelperSurface(void);

    void UpdateWorkingRectAndBacking(const VSGDI_Rect *pRect);
    void ClearBackingBorderRows(void);
    void ClearBackingBorderRowsThunk(void);
    int IsReady(void) const;
    short BackingWidth(void) const;
    short BackingHeight(void) const;

private:
    unsigned char m_abState[0x560];
};

class VSGDI_DibDisplayState : public VSGDI_DisplayState {
public:
    VSGDI_DibDisplayState(short cxDisplay, short cyDisplay);

    virtual int Create(HWND hWnd);

private:
    void *m_pDisplayAddress;
    int m_dwReserved20;
    FARPROC m_pfnDisplayDibStart32;
    FARPROC m_pfnDisplayDibEnd32;
    FARPROC m_pfnDisplayDibGetAddress32;
};

class VSGDI_DirectDrawDisplayState : public VSGDI_DisplayState {
public:
    VSGDI_DirectDrawDisplayState(short cxDisplay, short cyDisplay);

    virtual int Create(HWND hWnd);

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
int InitializeSelectedGraphicsDriver(int nRequestedDriver);
int GetSelectedGraphicsDriverId(void);
VSGDI_DisplayState *GetDisplayState(void);
void InitializeHelperUploadStatePending(int nUploadState);
void PromoteHelperUploadStateToActive(int nUploadState);
void SampleRootHelperGeometryAndDispatchRenderGroups(void *pPrimaryContext, int nToken);

extern int g_fStartupGraphicsDriverWing;
extern int g_fStartupGraphicsDriverCds;
extern int g_fStartupGraphicsDriverGdk;
extern int g_fStartupFullscreen;
extern int *g_pArrowCursorStatusIndicatorRenderClient;

#endif
