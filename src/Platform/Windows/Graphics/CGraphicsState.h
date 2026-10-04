#ifndef LEMBALL_VISOS_TARGET_GRAPHICS_CGRAPHICSSTATE_H
#define LEMBALL_VISOS_TARGET_GRAPHICS_CGRAPHICSSTATE_H

enum eGraphicsDriverMode {
	GFX_MODE_NONE = 0,
	GFX_MODE_GDI = 1,
	GFX_MODE_VGA_320X200 = 2,
	GFX_MODE_VGA_320X240 = 3,
	GFX_MODE_DD_FS_640X480 = 4,
	GFX_MODE_DD_FS_320X200 = 5,
	GFX_MODE_DD_WIN_640X480 = 6,
	GFX_MODE_DD_WIN_320X200 = 7,
	GFX_MODE_AUTO = 8
};

// SIZE 0x0c
struct CGraphicsState {
	bool SelectDriver(int p_driverMode);
	void NotifyDriverChange();
	bool ChangeDriver(int p_driverMode);
	bool IsFullscreenDriver();
	bool IsDirectDrawDriver();
	bool IsDisplayDibDriver();
	void UpdateDriverSize(const struct CVSSize& p_size);

	int m_driverMode;                    // 0x00
	unsigned int m_targetWindow;         // 0x04
	unsigned int m_fallbackWarningShown; // 0x08
};

#endif
