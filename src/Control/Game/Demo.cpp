#include "Demo.h"

// 68K 0x10700a70 __ct__5CDemoFi
// STUB: LEMBALL 0x004091b0
Demo::Demo(int p_arg0)
{
}

// 68K 0x10700b20 __dt__5CDemoFv
// STUB: LEMBALL 0x00409220
Demo::~Demo()
{
}

// 68K 0x10700b8a SendNextPacket__5CDemoFi
// STUB: LEMBALL 0x00409250
bool Demo::SendNextPacket(int p_packetIndex)
{
	return 0;
}

// 68K 0x10700e38 LoadBuffer__5CDemoFv
// STUB: LEMBALL 0x00409460
bool Demo::LoadBuffer()
{
	return 0;
}

// 68K 0x10700f82 GetUserPacket__5CDemoFPUcRUl
// STUB: LEMBALL 0x00409560
void Demo::GetUserPacket(unsigned char* p_data, unsigned long& p_size)
{
}

// 68K 0x10701032 Reset__5CDemoFv
// STUB: LEMBALL 0x004095e0
void Demo::Reset()
{
}

// 68K 0x10701062 SetDemoMode__5CDemoFUc
// STUB: LEMBALL 0x00409600
void Demo::SetDemoMode(unsigned char p_enabled)
{
}

// 68K 0x107010ac Process__5CDemoFv
// STUB: LEMBALL 0x00409620
void Demo::Process()
{
}

// 68K 0x10701116 CleanUp__5CDemoFv
// STUB: LEMBALL 0x00409660
void Demo::CleanUp()
{
}

// 68K 0x10701178 GameIsOver__5CDemoFv
// STUB: LEMBALL 0x004096a0
void Demo::GameIsOver()
{
}

// 68K 0x107011aa ProcessMsg__5CDemoFP10tagMESSAGE
// STUB: LEMBALL 0x004096b0
int Demo::ProcessMsg(Message* p_message)
{
	return 0;
}

// Confirmed class-scoped globals.
// GLOBAL: LEMBALL 0x004a62a4
int g_nDemoMode;
