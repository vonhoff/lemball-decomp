#include "CMciMusicDevice.h"

#include "Engine/Resources/Types/CResSTRING.h"
#include "Engine/Streams/CVSOStream.h"
#include "Engine/Strings/CString.h"
#include "Platform/Windows/CPlatformServices.h"
#include "Platform/Windows/Entry.h"

#include <mmsystem.h>
#include <string.h>
#include <windows.h>

#define MCI_ERROR_TEXT_CAPACITY 128

// GLOBAL: LEMBALL 0x004aa228
static CMciMusicDevice* g_pActiveMciMusicDevice;

// GLOBAL: LEMBALL 0x004aa22c
static unsigned long g_nPreparedMciMusicTrackHandle;

// GLOBAL: LEMBALL 0x004aa230
static char g_szMciDeviceInfo[256];

// GLOBAL: LEMBALL 0x004a3af8
static const char g_szMciSequencerDevice[] = "sequencer";

// GLOBAL: LEMBALL 0x004a3ae8
static const char g_szMciMusicWindow[] = "HLMusicWindow";

// FUNCTION: LEMBALL 0x0047e900
static LRESULT CALLBACK MciMusicWindowProc(HWND p_hwnd, UINT p_message, WPARAM p_wParam, LPARAM p_lParam)
{
	if (p_message == MM_MCINOTIFY && p_wParam == MCI_NOTIFY_SUCCESSFUL) {
		g_pActiveMciMusicDevice->Resume(g_nPreparedMciMusicTrackHandle);
	}
	return DefWindowProcA(p_hwnd, p_message, p_wParam, p_lParam);
}

// FUNCTION: LEMBALL 0x0047e940
CMciMusicDevice::CMciMusicDevice()
{
	WNDCLASSA windowClass;
	MCI_OPEN_PARMS openParms;
	MCIERROR error;
	char errorText[MCI_ERROR_TEXT_CAPACITY];

	m_preparedHandle = 0;
	g_nPreparedMciMusicTrackHandle = 0;
	m_playing = false;
	m_paused = false;
	m_pausePosition = 0;
	g_pActiveMciMusicDevice = this;
	memset(&openParms, 0, sizeof(openParms));
	openParms.lpstrDeviceType = g_szMciSequencerDevice;
	openParms.lpstrElementName = NULL;
	error = mciSendCommandA(0, MCI_OPEN, MCI_OPEN_TYPE, (DWORD) &openParms);
	if (error != 0) {
		mciGetErrorStringA(error, errorText, sizeof(errorText));
		*g_pErrorOutput << "Error!     HL Midi Device Not Found.\n";
		*g_pErrorOutput << "MCI Error:\t" << errorText << "\n";
		m_available = false;
		return;
	}
	m_deviceId = openParms.wDeviceID;
	m_available = true;
	mciSendCommandA(m_deviceId, MCI_CLOSE, 0, 0);
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = (HINSTANCE) g_pApplicationInstance;
	windowClass.hIcon = NULL;
	windowClass.hCursor = NULL;
	windowClass.hbrBackground = NULL;
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = MciMusicWindowProc;
	windowClass.lpszMenuName = g_szMciMusicWindow;
	windowClass.lpszClassName = g_szMciMusicWindow;
	RegisterClassA(&windowClass);
	m_notifyWindow = CreateWindowExA(0,
									 g_szMciMusicWindow,
									 g_szMciMusicWindow,
									 0,
									 CW_USEDEFAULT,
									 CW_USEDEFAULT,
									 CW_USEDEFAULT,
									 CW_USEDEFAULT,
									 NULL,
									 NULL,
									 (HINSTANCE) g_pApplicationInstance,
									 NULL);
	if (m_notifyWindow == NULL) {
		*g_pErrorOutput << "Error! Unable to Create Window for HL Music.\n";
	}
}

// FUNCTION: LEMBALL 0x0047eac0
CMciMusicDevice::~CMciMusicDevice()
{
}

// FUNCTION: LEMBALL 0x0047ead0
void CMciMusicDevice::Prepare(unsigned long p_handle, unsigned long p_resourceId)
{
	MCI_OPEN_PARMS openParms;
	MCI_SET_PARMS setParms;
	MCIERROR error;
	CResSTRING* name;
	char* cdDir;

	if (p_handle == 0) {
		*g_pErrorOutput << "Error Call to Prepare Music (HL) with Invalid Handle!\n";
	}
	if (m_preparedHandle != 0) {
		*g_pErrorOutput << "Error! Call to Prepare Music when already prepared!\n";
	}
	if (m_playing == true) {
		*g_pErrorOutput << "Error! Cannot Prepare Music while playing.\n";
	}
	m_preparedHandle = p_handle;
	g_nPreparedMciMusicTrackHandle = p_handle;
	name = CResSTRING::Load(p_resourceId);
	if (name->m_loaded != 0) {
		name->m_age = 0;
	}
	else {
		name->LoadData();
	}
	name->m_directUseCount++;
	openParms.lpstrDeviceType = (LPCSTR) MCI_DEVTYPE_SEQUENCER;
	CString musicName;
	if (m_usePathPrefix != 0) {
		musicName = m_path;
		if (musicName[musicName.getlength() - 1] != '\\') {
			musicName += "\\";
		}
	}
	musicName += (const char*) name->m_data;
	musicName += ".mid";
	CString fullPath;
	if (m_useCdDirectory == 0) {
		fullPath = g_szCurrentDirectory;
		if (fullPath[fullPath.getlength() - 1] != '\\') {
			fullPath += "\\";
		}
	}
	else {
		cdDir = g_pTargetPlatformServices->GetCDDir(musicName);
		if (cdDir == NULL) {
			cdDir = g_szCurrentDirectory;
		}
		fullPath = cdDir;
		if (fullPath[fullPath.getlength() - 1] != '\\') {
			fullPath += "\\";
		}
	}
	fullPath += musicName;
	openParms.lpstrElementName = fullPath;
	error = mciSendCommandA(0, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_TYPE_ID | MCI_OPEN_ELEMENT, (DWORD) &openParms);
	name->m_directUseCount--;
	name->UnLoad();
	if (error != 0) {
		char errorText[MCI_ERROR_TEXT_CAPACITY];
		mciGetErrorStringA(error, errorText, sizeof(errorText));
		*g_pErrorOutput << "Error!     Unable to Prepare Music (Open) " << fullPath << "!\n";
		*g_pErrorOutput << "MCI Error:\t" << errorText << "\n";
		m_preparedHandle = 0;
		g_nPreparedMciMusicTrackHandle = 0;
		return;
	}
	m_deviceId = openParms.wDeviceID;
	MCI_SEEK_PARMS seekParms;
	error = mciSendCommandA(m_deviceId, MCI_SEEK, MCI_SEEK_TO_START, (DWORD) &seekParms);
	if (error != 0) {
		char errorText[MCI_ERROR_TEXT_CAPACITY];
		mciGetErrorStringA(error, errorText, sizeof(errorText));
		*g_pErrorOutput << "Error!     Unable to Prepare Music (Seek)!\n";
		*g_pErrorOutput << "MCI Error:\t" << errorText << "\n";
		m_preparedHandle = 0;
		g_nPreparedMciMusicTrackHandle = 0;
		return;
	}
	setParms.dwTimeFormat = MCI_FORMAT_MILLISECONDS;
	error = mciSendCommandA(m_deviceId, MCI_SET, MCI_SET_TIME_FORMAT, (DWORD) &setParms);
	if (error != 0) {
		char errorText[MCI_ERROR_TEXT_CAPACITY];
		mciGetErrorStringA(error, errorText, sizeof(errorText));
		*g_pErrorOutput << "Error!     Unable to Prepare Music! (Time)\n";
		*g_pErrorOutput << "MCI Error:\t" << errorText << "\n";
		m_preparedHandle = 0;
		g_nPreparedMciMusicTrackHandle = 0;
		return;
	}
	m_playing = false;
	m_paused = false;
}

// FUNCTION: LEMBALL 0x0047ee70
void CMciMusicDevice::Free(unsigned long p_handle)
{
	if (p_handle == 0) {
		*g_pErrorOutput << "Error Call to Free Music (HL) with Invalid Handle!\n";
	}
	if (m_preparedHandle != p_handle) {
		*g_pErrorOutput << "Error Call to Free Music (HL) with unknown Handle!\n";
	}
	m_preparedHandle = 0;
	g_nPreparedMciMusicTrackHandle = 0;
	if (m_playing == true) {
		*g_pErrorOutput << "Error! Must stop music before closing...\n";
	}
	mciSendCommandA(m_deviceId, MCI_CLOSE, 0, 0);
}

// FUNCTION: LEMBALL 0x0047eee0
void CMciMusicDevice::Play(unsigned long p_handle)
{
	MCI_SEEK_PARMS seekParms;
	MCI_PLAY_PARMS playParms;
	MCIERROR error;
	char errorText[MCI_ERROR_TEXT_CAPACITY];

	if (p_handle == 0) {
		*g_pErrorOutput << "Error Call to Play Music (HL) with Invalid Handle!\n";
	}
	if (m_preparedHandle != p_handle) {
		*g_pErrorOutput << "Error Call to Play (HL) with unknown Handle!\n";
	}
	if (m_playing == true) {
		*g_pErrorOutput << "Error! Play Command (HL) While already playing!\n";
	}
	seekParms.dwTo = 0;
	error = mciSendCommandA(m_deviceId, MCI_SEEK, MCI_SEEK_TO_START, (DWORD) &seekParms);
	if (error != 0) {
		mciGetErrorStringA(error, errorText, sizeof(errorText));
		*g_pErrorOutput << "Error!     Unable to Play Music (Seek)! (HL)\n";
		*g_pErrorOutput << "MCI Error:\t" << errorText << "\n";
		return;
	}
	playParms.dwCallback = (DWORD) m_notifyWindow;
	error = mciSendCommandA(m_deviceId, MCI_PLAY, MCI_NOTIFY, (DWORD) &playParms);
	if (error != 0) {
		mciGetErrorStringA(error, errorText, sizeof(errorText));
		*g_pErrorOutput << "Error!     Unable to Play Music (Play)! (HL)\n";
		*g_pErrorOutput << "MCI Error:\t" << errorText << "\n";
		return;
	}
	m_paused = false;
	m_pausePosition = 0;
	m_playing = true;
}

// FUNCTION: LEMBALL 0x0047f040
void CMciMusicDevice::Stop(unsigned long p_handle)
{
	MCIERROR error;
	char errorText[MCI_ERROR_TEXT_CAPACITY];

	if (p_handle == 0) {
		*g_pErrorOutput << "Error Call to Stop Music (HL) with Invalid Handle!\n";
	}
	if (m_preparedHandle != p_handle) {
		*g_pErrorOutput << "Error Call to Stop (HL) with unknown Handle!\n";
	}
	if (m_playing == false) {
		*g_pErrorOutput << "Error! Stop Command (HL) when not playing!\n";
		return;
	}
	error = mciSendCommandA(m_deviceId, MCI_STOP, 0, 0);
	if (error != 0) {
		mciGetErrorStringA(error, errorText, sizeof(errorText));
		*g_pErrorOutput << "Error!     Unable to Stop Music! (HL)\n";
		*g_pErrorOutput << "MCI Error:\t" << errorText << "\n";
		return;
	}
	m_playing = false;
}

// FUNCTION: LEMBALL 0x0047f120
void CMciMusicDevice::Pause(unsigned long p_handle)
{
	MCI_STATUS_PARMS statusParms;
	MCIERROR error;
	char errorText[MCI_ERROR_TEXT_CAPACITY];

	if (p_handle == 0) {
		*g_pErrorOutput << "Error Call to Pause Music (HL) with Invalid Handle!\n";
	}
	if (m_preparedHandle != p_handle) {
		*g_pErrorOutput << "Error Call to Pause (HL) with unknown Handle!\n";
	}
	statusParms.dwItem = MCI_STATUS_POSITION;
	error = mciSendCommandA(m_deviceId, MCI_STATUS, MCI_STATUS_ITEM, (DWORD) &statusParms);
	if (error != 0) {
		mciGetErrorStringA(error, errorText, sizeof(errorText));
		*g_pErrorOutput << "Error!     Unable to Get Position for Pause! (HL)\n";
		*g_pErrorOutput << "MCI Error:\t" << errorText << "\n";
	}
	m_pausePosition = statusParms.dwReturn;
	error = mciSendCommandA(m_deviceId, MCI_STOP, 0, 0);
	if (error != 0) {
		mciGetErrorStringA(error, errorText, sizeof(errorText));
		*g_pErrorOutput << "Error!     Unable to Stop Music! (HL)\n";
		*g_pErrorOutput << "MCI Error:\t" << errorText << "\n";
	}
	else {
		m_playing = false;
		m_paused = true;
	}
}

// FUNCTION: LEMBALL 0x0047f250
void CMciMusicDevice::Resume(unsigned long p_handle)
{
	MCI_SEEK_PARMS seekParms;
	MCI_PLAY_PARMS playParms;
	MCIERROR error;
	char errorText[MCI_ERROR_TEXT_CAPACITY];

	if (p_handle == 0) {
		*g_pErrorOutput << "Error Call to Resume Music (HL) with Invalid Handle!\n";
	}
	if (m_preparedHandle != p_handle) {
		*g_pErrorOutput << "Error Call to Resume (HL) with unknown Handle!\n";
	}
	seekParms.dwTo = 0;
	error = mciSendCommandA(m_deviceId, MCI_SEEK, MCI_SEEK_TO_START, (DWORD) &seekParms);
	if (error != 0) {
		mciGetErrorStringA(error, errorText, sizeof(errorText));
		*g_pErrorOutput << "Error!     Unable to Restart Music (Seek)! (HL)\n";
		*g_pErrorOutput << "MCI Error:\t" << errorText << "\n";
		return;
	}
	playParms.dwCallback = (DWORD) m_notifyWindow;
	error = mciSendCommandA(m_deviceId, MCI_PLAY, MCI_NOTIFY, (DWORD) &playParms);
	if (error != 0) {
		mciGetErrorStringA(error, errorText, sizeof(errorText));
		*g_pErrorOutput << "Error!     Unable to Restart Music (Play)! (HL)\n";
		*g_pErrorOutput << "MCI Error:\t" << errorText << "\n";
		return;
	}
	m_playing = true;
	m_paused = false;
}

// FUNCTION: LEMBALL 0x0047f390
int CMciMusicDevice::IsAvailable()
{
	return m_available;
}

// FUNCTION: LEMBALL 0x0047f3a0
char* CMciMusicDevice::GetInfo()
{
	MIDIOUTCAPSA capabilities;
	char deviceType[256];

	if (m_available != false) {
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
