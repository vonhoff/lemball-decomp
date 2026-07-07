#ifndef LEMBALL_VSGDI_H
#define LEMBALL_VSGDI_H

#include "win32.h"

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
    short m_cxDisplay;
    short m_cyDisplay;
    int m_fReady;
};

class VSGDI_HelperSurface {
public:
    VSGDI_HelperSurface(void);

    void UpdateWorkingRectAndBacking(const VSGDI_Rect &Rect);
    int IsReady(void) const;
    short BackingWidth(void) const;
    short BackingHeight(void) const;

private:
    VSGDI_Rect m_WorkingRect;
    short m_cxBacking;
    short m_cyBacking;
    int m_fReady;
};

class VSGDI_DibDisplayState : public VSGDI_DisplayState {
public:
    VSGDI_DibDisplayState(short cxDisplay, short cyDisplay);

    virtual int Create(HWND hWnd);
};

class VSGDI_DirectDrawDisplayState : public VSGDI_DisplayState {
public:
    VSGDI_DirectDrawDisplayState(short cxDisplay, short cyDisplay);

    virtual int Create(HWND hWnd);

private:
    HMODULE m_hDdraw;
    FARPROC m_pfnDirectDrawCreate;
    HWND m_hWindow;
};

int InitializeResourceGeometryHelperRuntime(void);
void ShutdownResourceGeometryHelperRuntime(void);
int InitializeSelectedGraphicsDriver(int nRequestedDriver);
int GetSelectedGraphicsDriverId(void);
VSGDI_DisplayState *GetDisplayState(void);

#endif
