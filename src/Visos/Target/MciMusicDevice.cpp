#include "MciMusicDevice.h"

#include "../../Platform/Windows/Entry.h"
#include "../Foundation/String.h"
#include "../Foundation/VsIOs.h"
#include "../Foundation/VsOStream.h"
#include "../Resources/ResString.h"
#include "TargetPlatformServices.h"

#include <string.h>

// GLOBAL: LEMBALL 0x004aa228
static MciMusicDevice* g_pActiveMciMusicDevice;

// GLOBAL: LEMBALL 0x004aa22c
static unsigned int g_nPreparedMciMusicTrackHandle;

// GLOBAL: LEMBALL 0x004aa230
static char g_szMciDeviceInfo[256];

// GLOBAL: LEMBALL 0x004a3af8
static const char g_szMciSequencerDevice[] = "sequencer";

// GLOBAL: LEMBALL 0x004a3ae8
static const char g_szMciMusicWindow[] = "HLMusicWindow";

// FUNCTION: LEMBALL 0x0047e900
static LRESULT CALLBACK MciMusicWindowProc(HWND p_hwnd, UINT p_message, WPARAM p_wParam, LPARAM p_lParam)
{
	if (p_message == 0x3b9 && p_wParam == 1) {
		g_pActiveMciMusicDevice->Resume(g_nPreparedMciMusicTrackHandle);
	}
	return DefWindowProcA(p_hwnd, p_message, p_wParam, p_lParam);
}

// FUNCTION: LEMBALL 0x0047e940
MciMusicDevice::MciMusicDevice()
{
	WNDCLASSA windowClass;
	MCI_OPEN_PARMS openParms;
	MCIERROR error;
	char errorText[0x80];

	m_preparedHandle = 0;
	g_nPreparedMciMusicTrackHandle = 0;
	m_playing = 0;
	m_paused = 0;
	m_pausePosition = 0;
	g_pActiveMciMusicDevice = this;
	memset(&openParms, 0, sizeof(openParms));
	openParms.lpstrDeviceType = g_szMciSequencerDevice;
	openParms.lpstrElementName = 0;
	error = mciSendCommandA(0, 0x803, 0x2000, (DWORD) &openParms);
	if (error != 0) {
		mciGetErrorStringA(error, errorText, sizeof(errorText));
		*g_pErrorOutput << "Error!     HL Midi Device Not Found.\n";
		*g_pErrorOutput << "MCI Error:\t" << errorText << "\n";
		m_available = 0;
		return;
	}
	m_deviceId = openParms.wDeviceID;
	m_available = 1;
	mciSendCommandA(m_deviceId, 0x804, 0, 0);
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
		*g_pErrorOutput << "Error! Unable to Create Window for HL Music.\n";
	}
}

MciMusicDevice::~MciMusicDevice()
{
}

// FUNCTION: LEMBALL 0x0047ead0
void MciMusicDevice::Prepare(unsigned long p_handle, unsigned long p_resourceId)
{
	MCI_OPEN_PARMS openParms;
	MCI_SET_PARMS setParms;
	MCIERROR error;
	ResString* name;
	char* cdDir;

	if (p_handle == 0) {
		*g_pErrorOutput << "Error Call to Prepare Music (HL) with Invalid Handle!\n";
	}
	if (m_preparedHandle != 0) {
		*g_pErrorOutput << "Error! Call to Prepare Music when already prepared!\n";
	}
	if (m_playing == 1) {
		*g_pErrorOutput << "Error! Cannot Prepare Music while playing.\n";
	}
	m_preparedHandle = p_handle;
	g_nPreparedMciMusicTrackHandle = p_handle;
	name = ResString::Load(p_resourceId);
	if (name->m_loaded != 0) {
		name->m_age = 0;
	}
	else {
		name->LoadData();
	}
	name->m_directUseCount++;
	openParms.lpstrDeviceType = (LPCSTR) 0x20b;
	String musicName;
	if (m_usePathPrefix != 0) {
		musicName = m_path;
		if (musicName.GetText()[musicName.Getlength() - 1] != '\\') {
			musicName += "\\";
		}
	}
	musicName += (const char*) name->m_data;
	musicName += ".mid";
	String fullPath;
	if (m_useCdDirectory == 0) {
		fullPath = g_szCurrentDirectory;
		if (fullPath.GetText()[fullPath.Getlength() - 1] != '\\') {
			fullPath += "\\";
		}
	}
	else {
		cdDir = g_pTargetPlatformServices->GetCdDir(musicName.GetText());
		if (cdDir == 0) {
			cdDir = g_szCurrentDirectory;
		}
		fullPath = cdDir;
		if (fullPath.GetText()[fullPath.Getlength() - 1] != '\\') {
			fullPath += "\\";
		}
	}
	fullPath += musicName;
	openParms.lpstrElementName = (char*) fullPath.GetText();
	error = mciSendCommandA(0, 0x803, 0x3200, (DWORD) &openParms);
	name->m_directUseCount--;
	name->UnLoad();
	if (error != 0) {
		char errorText[0x80];
		mciGetErrorStringA(error, errorText, sizeof(errorText));
		*g_pErrorOutput << "Error!     Unable to Prepare Music (Open) " << fullPath << "!\n";
		*g_pErrorOutput << "MCI Error:\t" << errorText << "\n";
		m_preparedHandle = 0;
		g_nPreparedMciMusicTrackHandle = 0;
		return;
	}
	m_deviceId = openParms.wDeviceID;
	MCI_SEEK_PARMS seekParms;
	error = mciSendCommandA(m_deviceId, 0x807, 0x100, (DWORD) &seekParms);
	if (error != 0) {
		char errorText[0x80];
		mciGetErrorStringA(error, errorText, sizeof(errorText));
		*g_pErrorOutput << "Error!     Unable to Prepare Music (Seek)!\n";
		*g_pErrorOutput << "MCI Error:\t" << errorText << "\n";
		m_preparedHandle = 0;
		g_nPreparedMciMusicTrackHandle = 0;
		return;
	}
	setParms.dwTimeFormat = 0;
	error = mciSendCommandA(m_deviceId, 0x80d, 0x400, (DWORD) &setParms);
	if (error != 0) {
		char errorText[0x80];
		mciGetErrorStringA(error, errorText, sizeof(errorText));
		*g_pErrorOutput << "Error!     Unable to Prepare Music! (Time)\n";
		*g_pErrorOutput << "MCI Error:\t" << errorText << "\n";
		m_preparedHandle = 0;
		g_nPreparedMciMusicTrackHandle = 0;
		return;
	}
	m_playing = 0;
	m_paused = 0;
}

// FUNCTION: LEMBALL 0x0047ee70
void MciMusicDevice::Free(unsigned long p_handle)
{
	if (p_handle == 0) {
		*g_pErrorOutput << "Error Call to Free Music (HL) with Invalid Handle!\n";
	}
	if (m_preparedHandle != p_handle) {
		*g_pErrorOutput << "Error Call to Free Music (HL) with unknown Handle!\n";
	}
	m_preparedHandle = 0;
	g_nPreparedMciMusicTrackHandle = 0;
	if (m_playing == 1) {
		*g_pErrorOutput << "Error! Must stop music before closing...\n";
	}
	mciSendCommandA(m_deviceId, 0x804, 0, 0);
}

// FUNCTION: LEMBALL 0x0047eee0
void MciMusicDevice::Play(unsigned long p_handle)
{
	MCI_SEEK_PARMS seekParms;
	MCI_PLAY_PARMS playParms;
	MCIERROR error;
	char errorText[0x80];

	if (p_handle == 0) {
		*g_pErrorOutput << "Error Call to Play Music (HL) with Invalid Handle!\n";
	}
	if (m_preparedHandle != p_handle) {
		*g_pErrorOutput << "Error Call to Play (HL) with unknown Handle!\n";
	}
	if (m_playing == 1) {
		*g_pErrorOutput << "Error! Play Command (HL) While already playing!\n";
	}
	seekParms.dwTo = 0;
	error = mciSendCommandA(m_deviceId, 0x807, 0x100, (DWORD) &seekParms);
	if (error != 0) {
		mciGetErrorStringA(error, errorText, sizeof(errorText));
		*g_pErrorOutput << "Error!     Unable to Play Music (Seek)! (HL)\n";
		*g_pErrorOutput << "MCI Error:\t" << errorText << "\n";
		return;
	}
	playParms.dwCallback = (DWORD) m_notifyWindow;
	error = mciSendCommandA(m_deviceId, 0x806, 1, (DWORD) &playParms);
	if (error != 0) {
		mciGetErrorStringA(error, errorText, sizeof(errorText));
		*g_pErrorOutput << "Error!     Unable to Play Music (Play)! (HL)\n";
		*g_pErrorOutput << "MCI Error:\t" << errorText << "\n";
		return;
	}
	m_paused = 0;
	m_pausePosition = 0;
	m_playing = 1;
}

// FUNCTION: LEMBALL 0x0047f040
void MciMusicDevice::Stop(unsigned long p_handle)
{
	MCIERROR error;
	char errorText[0x80];

	if (p_handle == 0) {
		*g_pErrorOutput << "Error Call to Stop Music (HL) with Invalid Handle!\n";
	}
	if (m_preparedHandle != p_handle) {
		*g_pErrorOutput << "Error Call to Stop (HL) with unknown Handle!\n";
	}
	if (m_playing == 0) {
		*g_pErrorOutput << "Error! Stop Command (HL) when not playing!\n";
		return;
	}
	error = mciSendCommandA(m_deviceId, 0x808, 0, 0);
	if (error != 0) {
		mciGetErrorStringA(error, errorText, sizeof(errorText));
		*g_pErrorOutput << "Error!     Unable to Stop Music! (HL)\n";
		*g_pErrorOutput << "MCI Error:\t" << errorText << "\n";
		return;
	}
	m_playing = 0;
}

// FUNCTION: LEMBALL 0x0047f120
void MciMusicDevice::Pause(unsigned long p_handle)
{
	MCI_STATUS_PARMS statusParms;
	MCIERROR error;
	char errorText[0x80];

	if (p_handle == 0) {
		*g_pErrorOutput << "Error Call to Pause Music (HL) with Invalid Handle!\n";
	}
	if (m_preparedHandle != p_handle) {
		*g_pErrorOutput << "Error Call to Pause (HL) with unknown Handle!\n";
	}
	statusParms.dwItem = 2;
	error = mciSendCommandA(m_deviceId, 0x814, 0x100, (DWORD) &statusParms);
	if (error != 0) {
		mciGetErrorStringA(error, errorText, sizeof(errorText));
		*g_pErrorOutput << "Error!     Unable to Get Position for Pause! (HL)\n";
		*g_pErrorOutput << "MCI Error:\t" << errorText << "\n";
	}
	m_pausePosition = statusParms.dwReturn;
	error = mciSendCommandA(m_deviceId, 0x808, 0, 0);
	if (error != 0) {
		mciGetErrorStringA(error, errorText, sizeof(errorText));
		*g_pErrorOutput << "Error!     Unable to Stop Music! (HL)\n";
		*g_pErrorOutput << "MCI Error:\t" << errorText << "\n";
	}
	else {
		m_playing = 0;
		m_paused = 1;
	}
}

// FUNCTION: LEMBALL 0x0047f250
void MciMusicDevice::Resume(unsigned long p_handle)
{
	MCI_SEEK_PARMS seekParms;
	MCI_PLAY_PARMS playParms;
	MCIERROR error;
	char errorText[0x80];

	if (p_handle == 0) {
		*g_pErrorOutput << "Error Call to Resume Music (HL) with Invalid Handle!\n";
	}
	if (m_preparedHandle != p_handle) {
		*g_pErrorOutput << "Error Call to Resume (HL) with unknown Handle!\n";
	}
	seekParms.dwTo = 0;
	error = mciSendCommandA(m_deviceId, 0x807, 0x100, (DWORD) &seekParms);
	if (error != 0) {
		mciGetErrorStringA(error, errorText, sizeof(errorText));
		*g_pErrorOutput << "Error!     Unable to Restart Music (Seek)! (HL)\n";
		*g_pErrorOutput << "MCI Error:\t" << errorText << "\n";
		return;
	}
	playParms.dwCallback = (DWORD) m_notifyWindow;
	error = mciSendCommandA(m_deviceId, 0x806, 1, (DWORD) &playParms);
	if (error != 0) {
		mciGetErrorStringA(error, errorText, sizeof(errorText));
		*g_pErrorOutput << "Error!     Unable to Restart Music (Play)! (HL)\n";
		*g_pErrorOutput << "MCI Error:\t" << errorText << "\n";
		return;
	}
	m_playing = 1;
	m_paused = 0;
}

// FUNCTION: LEMBALL 0x0047f390
int MciMusicDevice::IsAvailable()
{
	return (int) m_available;
}

// FUNCTION: LEMBALL 0x0047f3a0
char* MciMusicDevice::GetInfo()
{
	MIDIOUTCAPSA capabilities;
	char deviceType[256];

	if (m_available != 0) {
		midiOutGetDevCapsA(m_deviceId, &capabilities, sizeof(capabilities));
		deviceType[0] = '\0';
		if (capabilities.wTechnology & MOD_MIDIPORT) {
			memcpy(deviceType + strlen(deviceType), "MIDI Hardware Port", sizeof("MIDI Hardware Port"));
		}
		else if (capabilities.wTechnology & MOD_SYNTH) {
			memcpy(deviceType + strlen(deviceType),
				   "Generic Internal Synthesiser",
				   sizeof("Generic Internal Synthesiser"));
		}
		else if ((capabilities.wTechnology & MOD_SQSYNTH) == MOD_SQSYNTH) {
			memcpy(deviceType + strlen(deviceType), "Square Wave Synthesiser", sizeof("Square Wave Synthesiser"));
		}
		else if (capabilities.wTechnology & MOD_FMSYNTH) {
			memcpy(deviceType + strlen(deviceType), "FM Synthesiser", sizeof("FM Synthesiser"));
		}
		else if ((capabilities.wTechnology & MOD_MAPPER) == MOD_MAPPER) {
			memcpy(deviceType + strlen(deviceType), "Microsoft Midi Mapper", sizeof("Microsoft Midi Mapper"));
		}
		wsprintfA(g_szMciDeviceInfo,
				  "MIDI driver version : %d.%d\nMIDI product name : %s\nMIDI Device Type : %s\n",
				  HIBYTE(capabilities.vDriverVersion),
				  LOBYTE(capabilities.vDriverVersion),
				  capabilities.szPname,
				  deviceType);
	}
	else {
		wsprintfA(g_szMciDeviceInfo, "No MIDI device activated\n");
	}
	return g_szMciDeviceInfo;
}
