#include "ENGINE/CORE/VSINIT.H"
#include "ENGINE/DEBUG/CTEXTWIN.H"
#include "ENGINE/DEBUG/VSDBG.H"

extern int g_fSubsystemsReady;
extern int g_fDebugFileEnabled;
extern CTextWindow* g_pDebugTextWindow;

__inline static int RawOutputString(const char* pszText, const char* pszPreservedName, unsigned int dwColour)
{
	CTextWindow* pTextWindow;

	if (g_fSubsystemsReady == 0) {
		MessageBoxA(0, pszText, pszPreservedName, 0);
		return 1;
	}

	pTextWindow = g_pDebugTextWindow;
	if (pTextWindow != 0) {
		pTextWindow->PostAllocatedTextControlString(pszText, dwColour);
	}
	else if (g_fDebugFileEnabled != 0) {
		AppendStringToDebugOutFile(pszText);
	}
	pTextWindow = g_pDebugTextWindow;
	return pTextWindow != 0;
}

// FUNCTION: LEMBALL 0x00472910
int _RAWOUT_DebugString(const char* pszText)
{
	return RawOutputString(pszText, "_RAWOUT_DebugString", 0x00008000);
}

// FUNCTION: LEMBALL 0x00472980
int _RAWOUT_ErrorString(const char* pszText)
{
	return RawOutputString(pszText, "_RAWOUT_ErrorString", 0x000000ff);
}

// FUNCTION: LEMBALL 0x004729f0
int _RAWOUT_SysString(const char* pszText)
{
	return RawOutputString(pszText, "_RAWOUT_SysString", 0x00ff0000);
}
