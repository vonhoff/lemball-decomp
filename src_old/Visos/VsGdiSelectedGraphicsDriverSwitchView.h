#ifndef LEMBALL_VSGDISELECTEDGRAPHICSDRIVERSWITCHVIEW_H
#define LEMBALL_VSGDISELECTEDGRAPHICSDRIVERSWITCHVIEW_H

// Selected-graphics-driver switch. Consolidated from identical per-TU
// duplicate definitions (ShellUI.cpp, CGDISwitch.cpp).

struct VsGdiSelectedGraphicsDriverSwitchView {
	int SwitchSelectedGraphicsDriver(int nRequestedDriver);
};

#endif
