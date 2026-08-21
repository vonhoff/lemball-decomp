#include "Platform/Windows/Mixed/Engine/GDI/VSGDI.H"
#include "Visos/VsGdiSelectedGraphicsDriverSwitchView.h"
#include "Platform/Windows/Mixed/Engine/GDI/VSWINDOW.H"

;

// FUNCTION: LEMBALL 0x00458130
int VsGdiSelectedGraphicsDriverSwitchView::SwitchSelectedGraphicsDriver(int nRequestedDriver)
{
	struct DisplayStateDeleteInterface {
		virtual void Delete(int fDelete) = 0;
	};
	VsGdiSelectedGraphicsDriverRuntime* pRuntime;
	DisplayStateDeleteInterface* pDisplayState;

	pRuntime = (VsGdiSelectedGraphicsDriverRuntime*) this;
	if (pRuntime->m_nSelectedDriver != nRequestedDriver) {
		if (g_pDisplayState != 0) {
			pDisplayState = (DisplayStateDeleteInterface*) g_pDisplayState;
			pDisplayState->Delete(1);
		}
		pRuntime->InitializeSelectedGraphicsDriver(nRequestedDriver);
		NotifyWindowOwnersDisplayChanged(&pRuntime->m_nSelectedDriver);
		return 1;
	}
	return 0;
}
