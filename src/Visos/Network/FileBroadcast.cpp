#include "FileBroadcast.h"

// 68K 0x10106e3e Setup__14CFileBroadcastFPCcPCc
// STUB: LEMBALL 0x0046f4f0
void FileBroadcast::Setup(const char* p_peerName, const char* p_path)
{
	// STRING: LEMBALL 0x004a2308 "Found Local host OK: "
}

// 68K 0x10208278 __ct__14CFileBroadcastFv
// STUB: LEMBALL 0x0047a570
FileBroadcast::FileBroadcast()
{
}

// 68K 0x10208d44 InitialiseFile__14CFileBroadcastFv
// STUB: LEMBALL 0x0047aa10
void FileBroadcast::InitialiseFile()
{
}

// 68K 0x10208e36 GetSpecificAddr__14CFileBroadcastFPCc
// STUB: LEMBALL 0x0047ab10
void FileBroadcast::GetSpecificAddr(const char* p_name)
{
}

// 68K 0x10208e68 Start__14CFileBroadcastFPCc
// STUB: LEMBALL 0x0047ab20
bool FileBroadcast::Start(const char* p_name)
{
	return 0;
}

// 68K 0x10208fb0 ReadPortInfo__14CFileBroadcastFv
// STUB: LEMBALL 0x0047ac50
bool FileBroadcast::ReadPortInfo()
{
	return 0;
}

// 68K 0x1020909c WritePortInfo__14CFileBroadcastFv
// STUB: LEMBALL 0x0047ace0
bool FileBroadcast::WritePortInfo()
{
	return 0;
}

// 68K 0x10209196 FindPort__14CFileBroadcastFPCUc
// STUB: LEMBALL 0x0047ad70
short FileBroadcast::FindPort(const unsigned char* p_data)
{
	return 0;
}

// 68K 0x10209276 ResetPort__14CFileBroadcastFs
// STUB: LEMBALL 0x0047ae00
void FileBroadcast::ResetPort(short p_port)
{
}

// 68K 0x1020935e StartListen__14CFileBroadcastFv
// STUB: LEMBALL 0x0047aed0
void FileBroadcast::StartListen()
{
}

// 68K 0x10209392 StopListen__14CFileBroadcastFv
// STUB: LEMBALL 0x0047aee0
void FileBroadcast::StopListen()
{
}

// 68K 0x102093c4 Process__14CFileBroadcastFv
// STUB: LEMBALL 0x0047aef0
void FileBroadcast::Process()
{
}

// 68K 0x102088cc __dt__14CFileBroadcastFv
// SYNTHETIC: LEMBALL 0x0047bb60
// FileBroadcast::`scalar deleting destructor'
FileBroadcast::~FileBroadcast()
{
}

// 68K 0x10116874 Closed__14CFileBroadcastFUc
// STUB: LEMBALL 0x0047bbf0
void FileBroadcast::Closed(unsigned char p_notifyPeer)
{
}

// Confirmed class-scoped globals.
// GLOBAL: LEMBALL 0x004a2de0
FileBroadcast* g_pFileBroadcast;
