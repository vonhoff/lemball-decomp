#include "TargetMciMusicDevice.h"

#include "../../Platform/Windows/Entry.h"
#include "../Foundation/VsIOs.h"
#include "../Foundation/VsOStream.h"
#include "../Resources/ResString.h"
#include "TargetPlatformServices.h"

#include <string.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <mmsystem.h>

// GLOBAL: LEMBALL 0x004aa228
static MciMusicDevice* g_pActiveMciMusicDevice;

// GLOBAL: LEMBALL 0x004aa22c
static unsigned int g_nPreparedMciMusicTrackHandle;

// GLOBAL: LEMBALL 0x004a3af8
static const char g_szMciSequencerDevice[] = "sequencer";

// GLOBAL: LEMBALL 0x004a3ae8
static const char g_szMciMusicWindow[] = "HLMusicWindow";

// FUNCTION: LEMBALL 0x0047e900
static LRESULT CALLBACK MciMusicWindowProc(HWND p_hwnd, UINT p_message, WPARAM p_wParam, LPARAM p_lParam)
{
	if (p_message == 0x3b9 && p_wParam == 1) {
		g_pActiveMciMusicDevice->Dummy1c(g_nPreparedMciMusicTrackHandle);
	}
	return DefWindowProcA(p_hwnd, p_message, p_wParam, p_lParam);
}

// FUNCTION: LEMBALL 0x0047e940
MciMusicDevice::MciMusicDevice()
{
	MCI_OPEN_PARMS openParms;
	WNDCLASSA windowClass;
	MCIERROR error;
	char errorText[0x80];

	m_reserved0c = 0;
	m_reserved08 = 0;
	m_preparedHandle = 0;
	g_nPreparedMciMusicTrackHandle = 0;
	m_playing = 0;
	m_paused = 0;
	m_pausePosition = 0;
	m_notifyWindow = 0;
	openParms.dwCallback = 0;
	openParms.wDeviceID = 0;
	openParms.lpstrDeviceType = g_szMciSequencerDevice;
	openParms.lpstrElementName = 0;
	openParms.lpstrAlias = 0;
	g_pActiveMciMusicDevice = this;
	error = mciSendCommandA(0, 0x803, 0x2000, (DWORD) &openParms);
	if (error != 0) {
		mciGetErrorStringA(error, errorText, sizeof(errorText));
		if (g_pErrorOutput != 0) {
			*g_pErrorOutput << "Error!     HL Midi Device Not Found.\n";
			*g_pErrorOutput << "MCI Error:\t" << errorText << "\n";
		}
		m_available = 0;
		return;
	}
	m_deviceId = openParms.wDeviceID;
	m_available = 1;
	mciSendCommandA(openParms.wDeviceID, 0x804, 0, 0);
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = (HINSTANCE) g_pApplicationInstance;
	windowClass.hIcon = 0;
	windowClass.hCursor = 0;
	windowClass.hbrBackground = 0;
	windowClass.lpszMenuName = g_szMciMusicWindow;
	windowClass.lpszClassName = g_szMciMusicWindow;
	windowClass.style = 3;
	windowClass.lpfnWndProc = MciMusicWindowProc;
	RegisterClassA(&windowClass);
	m_notifyWindow = CreateWindowExA(0,
									 g_szMciMusicWindow,
									 g_szMciMusicWindow,
									 0,
									 (int) 0x80000000,
									 (int) 0x80000000,
									 (int) 0x80000000,
									 (int) 0x80000000,
									 0,
									 0,
									 (HINSTANCE) g_pApplicationInstance,
									 0);
	if (m_notifyWindow == 0) {
		if (g_pErrorOutput != 0) {
			*g_pErrorOutput << "Error! Unable to Create Window for HL Music.\n";
		}
	}
}

MciMusicDevice::~MciMusicDevice()
{
}

// FUNCTION: LEMBALL 0x0047ead0
void MciMusicDevice::Dummy08(unsigned long p_allocated, unsigned long p_resourceId)
{
	MCI_OPEN_PARMS openParms;
	MCI_SEEK_PARMS seekParms;
	MCI_SET_PARMS setParms;
	MCIERROR error;
	char errorText[0x80];
	char musicName[256];
	char fullPath[512];
	ResString* name;
	const char* midi;
	char* cdDir;
	int length;

	if (p_allocated == 0) {
		if (g_pErrorOutput != 0) {
			*g_pErrorOutput << "Error Call to Prepare Music (HL) with Invalid Handle!\n";
		}
	}
	if (m_preparedHandle != 0) {
		if (g_pErrorOutput != 0) {
			*g_pErrorOutput << "Error! Call to Prepare Music when already prepared!\n";
		}
	}
	if (m_playing == 1) {
		if (g_pErrorOutput != 0) {
			*g_pErrorOutput << "Error! Cannot Prepare Music while playing.\n";
		}
	}
	m_preparedHandle = p_allocated;
	g_nPreparedMciMusicTrackHandle = p_allocated;
	name = ResString::Load(p_resourceId);
	if (name == 0) {
		m_preparedHandle = 0;
		g_nPreparedMciMusicTrackHandle = 0;
		return;
	}
	if (name->GetData() == 0) {
		name->LoadData();
	}
	midi = (const char*) name->GetData();
	musicName[0] = 0;
	if (m_reserved08 != 0) {
		strcpy(musicName, m_path.GetText());
		length = (int) strlen(musicName);
		if (length != 0 && musicName[length - 1] != '\\') {
			strcat(musicName, "\\");
		}
	}
	if (midi != 0) {
		strcat(musicName, midi);
	}
	strcat(musicName, ".mid");
	if (m_reserved0c == 0) {
		strcpy(fullPath, g_szCurrentDirectory);
	}
	else {
		cdDir = 0;
		if (g_pTargetPlatformServices != 0) {
			cdDir = g_pTargetPlatformServices->GetCDDir(musicName);
		}
		if (cdDir == 0) {
			strcpy(fullPath, g_szCurrentDirectory);
		}
		else {
			strcpy(fullPath, cdDir);
		}
	}
	length = (int) strlen(fullPath);
	if (length != 0 && fullPath[length - 1] != '\\') {
		strcat(fullPath, "\\");
	}
	strcat(fullPath, musicName);
	openParms.dwCallback = 0;
	openParms.wDeviceID = 0;
	openParms.lpstrDeviceType = (LPCSTR) 0x20b;
	openParms.lpstrElementName = fullPath;
	openParms.lpstrAlias = 0;
	error = mciSendCommandA(0, 0x803, 0x3200, (DWORD) &openParms);
	name->UnLoad();
	if (error != 0) {
		mciGetErrorStringA(error, errorText, sizeof(errorText));
		if (g_pErrorOutput != 0) {
			*g_pErrorOutput << "Error!     Unable to Prepare Music (Open) " << fullPath << "!\n";
			*g_pErrorOutput << "MCI Error:\t" << errorText << "\n";
		}
		m_preparedHandle = 0;
		g_nPreparedMciMusicTrackHandle = 0;
		return;
	}
	m_deviceId = openParms.wDeviceID;
	seekParms.dwCallback = 0;
	seekParms.dwTo = 0;
	error = mciSendCommandA(m_deviceId, 0x807, 0x100, (DWORD) &seekParms);
	if (error != 0) {
		mciGetErrorStringA(error, errorText, sizeof(errorText));
		if (g_pErrorOutput != 0) {
			*g_pErrorOutput << "Error!     Unable to Prepare Music (Seek)!\n";
			*g_pErrorOutput << "MCI Error:\t" << errorText << "\n";
		}
		m_preparedHandle = 0;
		g_nPreparedMciMusicTrackHandle = 0;
		return;
	}
	setParms.dwCallback = 0;
	setParms.dwTimeFormat = 0;
	error = mciSendCommandA(m_deviceId, 0x80d, 0x400, (DWORD) &setParms);
	if (error != 0) {
		mciGetErrorStringA(error, errorText, sizeof(errorText));
		if (g_pErrorOutput != 0) {
			*g_pErrorOutput << "Error!     Unable to Prepare Music! (Time)\n";
			*g_pErrorOutput << "MCI Error:\t" << errorText << "\n";
		}
		m_preparedHandle = 0;
		g_nPreparedMciMusicTrackHandle = 0;
		return;
	}
	m_playing = 0;
	m_paused = 0;
}

// FUNCTION: LEMBALL 0x0047ee70
void MciMusicDevice::Dummy0c(unsigned long p_handle)
{
	if (p_handle == 0) {
		if (g_pErrorOutput != 0) {
			*g_pErrorOutput << "Error Call to Free Music (HL) with Invalid Handle!\n";
		}
	}
	if (m_preparedHandle != p_handle) {
		if (g_pErrorOutput != 0) {
			*g_pErrorOutput << "Error Call to Free Music (HL) with unknown Handle!\n";
		}
	}
	m_preparedHandle = 0;
	g_nPreparedMciMusicTrackHandle = 0;
	if (m_playing == 1) {
		if (g_pErrorOutput != 0) {
			*g_pErrorOutput << "Error! Must stop music before closing...\n";
		}
	}
	mciSendCommandA(m_deviceId, 0x804, 0, 0);
}

// FUNCTION: LEMBALL 0x0047eee0
void MciMusicDevice::Dummy10(unsigned long p_handle)
{
	MCI_SEEK_PARMS seekParms;
	MCI_PLAY_PARMS playParms;
	MCIERROR error;
	char errorText[0x80];

	if (p_handle == 0) {
		if (g_pErrorOutput != 0) {
			*g_pErrorOutput << "Error Call to Play Music (HL) with Invalid Handle!\n";
		}
	}
	if (m_preparedHandle != p_handle) {
		if (g_pErrorOutput != 0) {
			*g_pErrorOutput << "Error Call to Play (HL) with unknown Handle!\n";
		}
	}
	if (m_playing == 1) {
		if (g_pErrorOutput != 0) {
			*g_pErrorOutput << "Error! Play Command (HL) While already playing!\n";
		}
	}
	seekParms.dwCallback = 0;
	seekParms.dwTo = 0;
	error = mciSendCommandA(m_deviceId, 0x807, 0x100, (DWORD) &seekParms);
	if (error != 0) {
		mciGetErrorStringA(error, errorText, sizeof(errorText));
		if (g_pErrorOutput != 0) {
			*g_pErrorOutput << "Error!     Unable to Play Music (Seek)! (HL)\n";
			*g_pErrorOutput << "MCI Error:\t" << errorText << "\n";
		}
		return;
	}
	playParms.dwCallback = (DWORD) m_notifyWindow;
	playParms.dwFrom = 0;
	playParms.dwTo = 0;
	error = mciSendCommandA(m_deviceId, 0x806, 1, (DWORD) &playParms);
	if (error != 0) {
		mciGetErrorStringA(error, errorText, sizeof(errorText));
		if (g_pErrorOutput != 0) {
			*g_pErrorOutput << "Error!     Unable to Play Music (Play)! (HL)\n";
			*g_pErrorOutput << "MCI Error:\t" << errorText << "\n";
		}
		return;
	}
	m_paused = 0;
	m_pausePosition = 0;
	m_playing = 1;
}

// FUNCTION: LEMBALL 0x0047f040
void MciMusicDevice::Dummy14(unsigned long p_handle)
{
	MCIERROR error;
	char errorText[0x80];

	if (p_handle == 0) {
		if (g_pErrorOutput != 0) {
			*g_pErrorOutput << "Error Call to Stop Music (HL) with Invalid Handle!\n";
		}
	}
	if (m_preparedHandle != p_handle) {
		if (g_pErrorOutput != 0) {
			*g_pErrorOutput << "Error Call to Stop (HL) with unknown Handle!\n";
		}
	}
	if (m_playing == 0) {
		if (g_pErrorOutput != 0) {
			*g_pErrorOutput << "Error! Stop Command (HL) when not playing!\n";
		}
		return;
	}
	error = mciSendCommandA(m_deviceId, 0x808, 0, 0);
	if (error != 0) {
		mciGetErrorStringA(error, errorText, sizeof(errorText));
		if (g_pErrorOutput != 0) {
			*g_pErrorOutput << "Error!     Unable to Stop Music! (HL)\n";
			*g_pErrorOutput << "MCI Error:\t" << errorText << "\n";
		}
		return;
	}
	m_playing = 0;
}

// FUNCTION: LEMBALL 0x0047f120
void MciMusicDevice::Dummy18(unsigned long p_handle)
{
	MCI_STATUS_PARMS statusParms;
	MCIERROR error;
	char errorText[0x80];

	if (p_handle == 0) {
		if (g_pErrorOutput != 0) {
			*g_pErrorOutput << "Error Call to Pause Music (HL) with Invalid Handle!\n";
		}
	}
	if (m_preparedHandle != p_handle) {
		if (g_pErrorOutput != 0) {
			*g_pErrorOutput << "Error Call to Pause (HL) with unknown Handle!\n";
		}
	}
	statusParms.dwCallback = 0;
	statusParms.dwItem = 2;
	statusParms.dwReturn = 0;
	error = mciSendCommandA(m_deviceId, 0x814, 0x100, (DWORD) &statusParms);
	if (error != 0) {
		mciGetErrorStringA(error, errorText, sizeof(errorText));
		if (g_pErrorOutput != 0) {
			*g_pErrorOutput << "Error!     Unable to Get Position for Pause! (HL)\n";
			*g_pErrorOutput << "MCI Error:\t" << errorText << "\n";
		}
	}
	m_pausePosition = statusParms.dwReturn;
	error = mciSendCommandA(m_deviceId, 0x808, 0, 0);
	if (error != 0) {
		mciGetErrorStringA(error, errorText, sizeof(errorText));
		if (g_pErrorOutput != 0) {
			*g_pErrorOutput << "Error!     Unable to Stop Music! (HL)\n";
			*g_pErrorOutput << "MCI Error:\t" << errorText << "\n";
		}
	}
	else {
		m_playing = 0;
		m_paused = 1;
	}
}

// FUNCTION: LEMBALL 0x0047f250
void MciMusicDevice::Dummy1c(unsigned long p_handle)
{
	MCI_SEEK_PARMS seekParms;
	MCI_PLAY_PARMS playParms;
	MCIERROR error;
	char errorText[0x80];

	if (p_handle == 0) {
		if (g_pErrorOutput != 0) {
			*g_pErrorOutput << "Error Call to Resume Music (HL) with Invalid Handle!\n";
		}
	}
	if (m_preparedHandle != p_handle) {
		if (g_pErrorOutput != 0) {
			*g_pErrorOutput << "Error Call to Resume (HL) with unknown Handle!\n";
		}
	}
	seekParms.dwCallback = 0;
	seekParms.dwTo = 0;
	error = mciSendCommandA(m_deviceId, 0x807, 0x100, (DWORD) &seekParms);
	if (error != 0) {
		mciGetErrorStringA(error, errorText, sizeof(errorText));
		if (g_pErrorOutput != 0) {
			*g_pErrorOutput << "Error!     Unable to Restart Music (Seek)! (HL)\n";
			*g_pErrorOutput << "MCI Error:\t" << errorText << "\n";
		}
		return;
	}
	playParms.dwCallback = (DWORD) m_notifyWindow;
	playParms.dwFrom = 0;
	playParms.dwTo = 0;
	error = mciSendCommandA(m_deviceId, 0x806, 1, (DWORD) &playParms);
	if (error != 0) {
		mciGetErrorStringA(error, errorText, sizeof(errorText));
		if (g_pErrorOutput != 0) {
			*g_pErrorOutput << "Error!     Unable to Restart Music (Play)! (HL)\n";
			*g_pErrorOutput << "MCI Error:\t" << errorText << "\n";
		}
		return;
	}
	m_playing = 1;
	m_paused = 0;
}

// FUNCTION: LEMBALL 0x0047f390
int MciMusicDevice::Dummy2c()
{
	return (int) m_available;
}

// FUNCTION: LEMBALL 0x0047f3a0
char* MciMusicDevice::GetInfo()
{
	return PvMusicDevice::GetInfo();
}
