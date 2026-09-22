#include "CPortsMessage.h"

// FUNCTION: LEMBALL 0x00479810
CPortsMessage::CPortsMessage()
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

// FUNCTION: LEMBALL 0x00479860
bool CPortsMessage::AnyUsed()
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

// FUNCTION: LEMBALL 0x0047b870
void CPortsMessage::AddData()
{
	Add(m_useCounts, 0x200);
}

// FUNCTION: LEMBALL 0x0047b880
void CPortsMessage::GetData()
{
	GetCopy(m_useCounts, 0x200);
}
