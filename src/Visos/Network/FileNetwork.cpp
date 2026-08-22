#include "FileNetwork.h"

// 68K 0x10106fca __ct__12CFileNetworkFv
// STUB: LEMBALL 0x0046f6b0
FileNetwork::FileNetwork()
{
	// STRING: LEMBALL 0x004a2334 "File-based Network"
}

// 68K 0x1010700c Initialise__12CFileNetworkFv
// STUB: LEMBALL 0x0046f6f0
void FileNetwork::Initialise()
{
	// STRING: LEMBALL 0x004a2348 "Network Initialised:\n"
	// STRING: LEMBALL 0x004a2360 "Windows file-based networking\n"
}

// 68K 0x1010707a UnInitialise__12CFileNetworkFv
// STUB: LEMBALL 0x0046f730
void FileNetwork::UnInitialise()
{
}

// 68K 0x101070ac ResetTimer__12CFileNetworkFUl
// STUB: LEMBALL 0x0046f740
void FileNetwork::ResetTimer(unsigned int p_interval)
{
	// STRING: LEMBALL 0x004a23a0 "ms from now\n"
	// STRING: LEMBALL 0x004a2380 "Setting next timer event to "
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
// STUB: LEMBALL 0x0046f860
FileNetworkAddress* FileNetwork::GetNewNetworkAddress()
{
	return 0;
}

// 68K 0x1010797a GetNewConnect__12CFileNetworkFv
// STUB: LEMBALL 0x0046f930
FileConnect* FileNetwork::GetNewConnect()
{
	return 0;
}

// 68K 0x101079d8 GetNewBroadcast__12CFileNetworkFv
// STUB: LEMBALL 0x0046f950
FileBroadcast* FileNetwork::GetNewBroadcast()
{
	return 0;
}

// 68K 0x10107a3a __dt__12CFileNetworkFv
// SYNTHETIC: LEMBALL 0x0046f970
// FileNetwork::`scalar deleting destructor'
FileNetwork::~FileNetwork()
{
}

// Confirmed class-scoped globals.
// GLOBAL: LEMBALL 0x004a2260
unsigned int g_dwFileNetworkThreadId;

// GLOBAL: LEMBALL 0x004a2264
void* g_hFileNetworkThread;

// GLOBAL: LEMBALL 0x004a2de4
void* g_pFileBroadcastData;
