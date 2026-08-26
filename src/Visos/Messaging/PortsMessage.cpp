#include "PortsMessage.h"

#include <new.h>

// 68K 0x10206fb0 __ct__13CPortsMessageFv
// FUNCTION: LEMBALL 0x00479810
PortsMessage::PortsMessage()
{
	int i;

	m_useCounts = (unsigned char*) operator new(0x200);
	m_payloadCapacity += 0x200;
	i = 0;
	do {
		m_useCounts[i] = 0;
		i++;
	} while (i < 0x200);
}

// 68K 0x10207028 AnyUsed__13CPortsMessageFv
// FUNCTION: LEMBALL 0x00479860
bool PortsMessage::AnyUsed()
{
	int i;

	i = 0;
	do {
		if (m_useCounts[i] != 0) {
			return true;
		}
		i++;
	} while (i < 0x200);
	return false;
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
	delete[] m_useCounts;
}
