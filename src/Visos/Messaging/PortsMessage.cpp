#include "PortsMessage.h"

// 68K 0x10206fb0 __ct__13CPortsMessageFv
// STUB: LEMBALL 0x00479810
PortsMessage::PortsMessage()
{
}

// 68K 0x10207028 AnyUsed__13CPortsMessageFv
// STUB: LEMBALL 0x00479860
bool PortsMessage::AnyUsed()
{
	return 0;
}

// 68K 0x10107624 AddData__13CPortsMessageFv
// FUNCTION: LEMBALL 0x0047b870
void PortsMessage::AddData()
{
	Add(m_useCounts, 0x200);
}

// 68K 0x10107660 GetData__13CPortsMessageFv
// FUNCTION: LEMBALL 0x0047b880
void PortsMessage::GetData()
{
	GetCopy(m_useCounts, 0x200);
}

// 68K 0x1010769c __dt__13CPortsMessageFv
// SYNTHETIC: LEMBALL 0x0047b890
// PortsMessage::`scalar deleting destructor'
PortsMessage::~PortsMessage()
{
}
