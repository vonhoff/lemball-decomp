#include "base_window.h"
#include "vsgdi.h"
#include "vsinit.h"

static int g_fResourceGeometryHelperReady = 0;
static int g_fStartupFullscreen = 0;
static int g_fStartupGraphicsDriverGdk = 0;
static int g_fGraphicsFallbackWarningShown = 0;
static unsigned int g_cbResourceGeometryHelperState = 0x20;
static VSGDI_DisplayState *g_pDisplayState = 0;

static const char g_VSGDI_DdrawDll[] = "DDRAW.DLL";
static const char g_VSGDI_DirectDrawCreate[] = "DirectDrawCreate";
static const char g_VSGDI_DirectDrawWindowClass[] = "DirectDrawClass";
static const char g_VSGDI_DirectDrawWindowTitle[] = "DirectDraw";
static const char g_VSGDI_ErrorNoValidDriver[] = "No valid driver selected to initialise";
static const char g_VSGDI_ErrorNoValidDriverAvailable[] = "No valid driver available";
static const char g_VSGDI_ErrorRegisterClass[] = "Unable to register DD base window class";
static const char g_VSGDI_InitDriverPrefix[] = "Initialising graphics device driver: ";
static const char g_VSGDI_AutoSelectedPrefix[] = "[ Auto selected: ";
static const char g_VSGDI_AutoSelectedSuffix[] = "]\n";

static const char *g_apszGraphicsDriverNames[] = {
    0,
    "Defaulting to normal window mode (using CreateDIBSection)",
    "Direct Draw (Windowed 320*200)",
    "Direct Draw (Windowed 640*480)",
    "Direct Draw (Full Screen 320*200)",
    0,
    "Direct Draw (Full Screen 640*480)",
    0,
    "Auto",
};

static void VSGDI_AppendStatusLine(const char *pszLeft, const char *pszRight, const char *pszSuffix) {
    AppendStatusCString(pszLeft);
    AppendStatusCString(pszRight);
    AppendStatusCString(pszSuffix);
}

static VSGDI_DisplayState *CreateDisplayStateForDriver(int nDriver) {
    switch (nDriver) {
    case VSGDI_DRIVER_METRICS:
        return new VSGDI_DibDisplayState(0, 0);
    case VSGDI_DRIVER_DIB_320_200:
        return new VSGDI_DibDisplayState(320, 200);
    case VSGDI_DRIVER_DIB_640_480:
        return new VSGDI_DibDisplayState(640, 480);
    case VSGDI_DRIVER_DDRAW_320_200:
        return new VSGDI_DirectDrawDisplayState(320, 200);
    case VSGDI_DRIVER_DDRAW_640_480:
        return new VSGDI_DirectDrawDisplayState(640, 480);
    default:
        break;
    }

    return 0;
}

VSGDI_DisplayState::VSGDI_DisplayState(void) {
    m_cxDisplay = 0;
    m_cyDisplay = 0;
    m_fReady = 0;
}

int VSGDI_DisplayState::IsReady(void) const {
    return m_fReady;
}

short VSGDI_DisplayState::Width(void) const {
    return m_cxDisplay;
}

short VSGDI_DisplayState::Height(void) const {
    return m_cyDisplay;
}

VSGDI_HelperSurface::VSGDI_HelperSurface(void) {
    m_WorkingRect.m_x = 0;
    m_WorkingRect.m_y = 0;
    m_WorkingRect.m_cx = 0;
    m_WorkingRect.m_cy = 0;
    m_cxBacking = 0;
    m_cyBacking = 0;
    m_fReady = 0;
}

// FUNCTION: LEMBALL 0x0046D090
void VSGDI_HelperSurface::UpdateWorkingRectAndBacking(const VSGDI_Rect &Rect) {
    m_WorkingRect = Rect;

    if (Rect.m_cx <= 0 || Rect.m_cy <= 0) {
        m_cxBacking = 0;
        m_cyBacking = 0;
        m_fReady = 0;
        return;
    }

    m_cxBacking = (short)((Rect.m_cx + 3) & ~3);
    m_cyBacking = Rect.m_cy;
    m_fReady = 1;
}

int VSGDI_HelperSurface::IsReady(void) const {
    return m_fReady;
}

short VSGDI_HelperSurface::BackingWidth(void) const {
    return m_cxBacking;
}

short VSGDI_HelperSurface::BackingHeight(void) const {
    return m_cyBacking;
}

VSGDI_DibDisplayState::VSGDI_DibDisplayState(short cxDisplay, short cyDisplay) {
    m_cxDisplay = cxDisplay;
    m_cyDisplay = cyDisplay;
    m_fReady = 0;
}

int VSGDI_DibDisplayState::Create(HWND hWnd) {
    (void)hWnd;
    m_fReady = 1;
    return 1;
}

VSGDI_DirectDrawDisplayState::VSGDI_DirectDrawDisplayState(short cxDisplay, short cyDisplay) {
    m_cxDisplay = cxDisplay;
    m_cyDisplay = cyDisplay;
    m_fReady = 0;
    m_hDdraw = 0;
    m_pfnDirectDrawCreate = 0;
    m_hWindow = 0;
}

int VSGDI_DirectDrawDisplayState::Create(HWND hWnd) {
    m_hWindow = hWnd;
    (void)g_VSGDI_DdrawDll;
    (void)g_VSGDI_DirectDrawCreate;
    (void)g_VSGDI_DirectDrawWindowClass;
    (void)g_VSGDI_DirectDrawWindowTitle;
    (void)g_VSGDI_ErrorRegisterClass;
    m_hDdraw = (HMODULE)1;
    m_pfnDirectDrawCreate = (FARPROC)1;
    m_fReady = 1;
    return 1;
}

// FUNCTION: LEMBALL 0x0046BA80
int InitializeResourceGeometryHelperRuntime(void) {
    if (VS_Base_Window::ClassName() == 0) {
        g_fResourceGeometryHelperReady = 0;
        return 0;
    }

    g_fResourceGeometryHelperReady = 1;
    return 1;
}

// FUNCTION: LEMBALL 0x0046BB70
void ShutdownResourceGeometryHelperRuntime(void) {
    if (g_pDisplayState != 0) {
        delete g_pDisplayState;
        g_pDisplayState = 0;
    }
    g_fResourceGeometryHelperReady = 0;
}

// FUNCTION: LEMBALL 0x00457E10
int InitializeSelectedGraphicsDriver(int nRequestedDriver) {
    int nSelectedDriver;
    VSGDI_DisplayState *pDisplayState;

    if (nRequestedDriver < 9 && g_apszGraphicsDriverNames[nRequestedDriver] != 0) {
        VSGDI_AppendStatusLine(g_VSGDI_InitDriverPrefix, g_apszGraphicsDriverNames[nRequestedDriver], "\n");
    }

    nSelectedDriver = nRequestedDriver;
    if (nRequestedDriver == VSGDI_DRIVER_AUTO) {
        if (g_fStartupGraphicsDriverGdk == 0) {
            nSelectedDriver = g_fStartupFullscreen ? VSGDI_DRIVER_DIB_640_480 : VSGDI_DRIVER_DIB_320_200;
        } else {
            nSelectedDriver = g_fStartupFullscreen ? VSGDI_DRIVER_DDRAW_640_480 : VSGDI_DRIVER_DDRAW_320_200;
        }
    }

    pDisplayState = 0;
    switch (nSelectedDriver) {
    case VSGDI_DRIVER_METRICS:
        pDisplayState = CreateDisplayStateForDriver(nSelectedDriver);
        break;
    case VSGDI_DRIVER_DIB_320_200:
        pDisplayState = CreateDisplayStateForDriver(nSelectedDriver);
        break;
    case VSGDI_DRIVER_DIB_640_480:
        pDisplayState = CreateDisplayStateForDriver(nSelectedDriver);
        break;
    case VSGDI_DRIVER_DDRAW_320_200:
        pDisplayState = CreateDisplayStateForDriver(nSelectedDriver);
        break;
    case VSGDI_DRIVER_DDRAW_640_480:
        pDisplayState = CreateDisplayStateForDriver(nSelectedDriver);
        break;
    default:
        VSGDI_AppendStatusLine(g_VSGDI_ErrorNoValidDriver, "", "\n");
        return 0;
    }

    g_pDisplayState = pDisplayState;
    if (g_pDisplayState == 0 || !g_pDisplayState->Create((HWND)0)) {
        if (g_pDisplayState != 0) {
            delete g_pDisplayState;
        }
        g_pDisplayState = CreateDisplayStateForDriver(VSGDI_DRIVER_METRICS);
        if (g_pDisplayState == 0 || !g_pDisplayState->Create((HWND)0)) {
            VSGDI_AppendStatusLine(g_VSGDI_ErrorNoValidDriverAvailable, "", "\n");
            return 0;
        }
        g_fGraphicsFallbackWarningShown = 1;
        nSelectedDriver = VSGDI_DRIVER_METRICS;
    }

    if (nRequestedDriver != nSelectedDriver && g_apszGraphicsDriverNames[nSelectedDriver] != 0) {
        VSGDI_AppendStatusLine(g_VSGDI_AutoSelectedPrefix, g_apszGraphicsDriverNames[nSelectedDriver], g_VSGDI_AutoSelectedSuffix);
    }

    g_cbResourceGeometryHelperState = 0x20;
    return 1;
}

VSGDI_DisplayState *GetDisplayState(void) {
    return g_pDisplayState;
}
