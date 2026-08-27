#include "FileNetwork.h"

#include "../Foundation/VsOStream.h"
#include "FileBroadcast.h"
#include "FileConnect.h"
#include "FileNetworkAddress.h"

#include <new.h>

extern "C" __declspec(dllimport) int __stdcall KillTimer(void* p_window, unsigned int p_id);
extern "C" __declspec(dllimport) unsigned int __stdcall SetTimer(void* p_window,
																unsigned int p_id,
																unsigned int p_elapse,
																void* p_callback);

// 68K 0x10106fca __ct__12CFileNetworkFv
// STUB: LEMBALL 0x0046f6b0
FileNetwork::FileNetwork()
{
}

// 68K 0x1010700c Initialise__12CFileNetworkFv
// FUNCTION: LEMBALL 0x0046f6f0
void FileNetwork::Initialise()
{
	*g_pDebugOutput << "Network Initialised:\n";
	*g_pDebugOutput << "Windows file-based networking\n";
	m_timerId = SetTimer(m_windowHandle, 0x12345679, 0x14, 0);
}

// 68K 0x1010707a UnInitialise__12CFileNetworkFv
// FUNCTION: LEMBALL 0x0046f730
void FileNetwork::UnInitialise()
{
	KillTimer(m_windowHandle, m_timerId);
}

// 68K 0x101070ac ResetTimer__12CFileNetworkFUl
// FUNCTION: LEMBALL 0x0046f740
void FileNetwork::ResetTimer(unsigned int p_interval)
{
	*g_pDebugOutput << "Setting next timer event to " << (unsigned long) p_interval << "ms from now\n";

	KillTimer(m_windowHandle, m_timerId);
	m_timerId = SetTimer(m_windowHandle, 0x12345679, p_interval, 0);
	m_alternateTimer = m_alternateTimer == 0;
}

// 68K 0x10107122 Setup__12CFileNetworkFPCcPCc
// STUB: LEMBALL 0x0046f7a0
void FileNetwork::Setup(const char* p_peerName, const char* p_path)
{
}

// 68K 0x10107156 BeforeDestroyConnections__12CFileNetworkFv
// STUB: LEMBALL 0x0046f7c0
void FileNetwork::BeforeDestroyConnections()
{
}

// 68K 0x101071ac AfterDestroyConnections__12CFileNetworkFv
// STUB: LEMBALL 0x0046f7d0
void FileNetwork::AfterDestroyConnections()
{
}

// 68K 0x10107202 Process__12CFileNetworkFv
// STUB: LEMBALL 0x0046f7e0
int FileNetwork::Process()
{
	return 0;
}

// 68K 0x10107296 ForceProcess__12CFileNetworkFv
// STUB: LEMBALL 0x0046f840
void FileNetwork::ForceProcess()
{
}

// 68K 0x10107920 GetNewNetworkAddress__12CFileNetworkFv
// FUNCTION: LEMBALL 0x0046f860
FileNetworkAddress* FileNetwork::GetNewNetworkAddress()
{
	void* storage;
	FileNetworkAddress* address;

	storage = operator new(sizeof(FileNetworkAddress));
	if (storage != 0) {
		address = new (storage) FileNetworkAddress();
		address->m_text[0] = '\0';
		return address;
	}
	return 0;
}

// 68K 0x1010797a GetNewConnect__12CFileNetworkFv
// FUNCTION: LEMBALL 0x0046f930
FileConnect* FileNetwork::GetNewConnect()
{
	void* storage;

	storage = operator new(sizeof(FileConnect));
	if (storage != 0) {
		return new (storage) FileConnect();
	}
	return 0;
}

// 68K 0x101079d8 GetNewBroadcast__12CFileNetworkFv
// FUNCTION: LEMBALL 0x0046f950
FileBroadcast* FileNetwork::GetNewBroadcast()
{
	void* storage;

	storage = operator new(sizeof(FileBroadcast));
	if (storage != 0) {
		return new (storage) FileBroadcast();
	}
	return 0;
}

// 68K 0x10107a3a __dt__12CFileNetworkFv
FileNetwork::~FileNetwork()
{
}

// GLOBAL: LEMBALL 0x004a2260
unsigned int g_dwFileNetworkThreadId = 0x12345678;

// GLOBAL: LEMBALL 0x004a2264
void* g_hFileNetworkThread = 0;

// GLOBAL: LEMBALL 0x004a2de4
void* g_pFileBroadcastData = 0;

