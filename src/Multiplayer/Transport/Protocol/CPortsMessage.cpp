#include "CPortsMessage.h"

#include "Multiplayer/Transport/NetworkConstants.h"

// FUNCTION: LEMBALL 0x00479810
CPortsMessage::CPortsMessage()
{
	int i;

	m_useCounts = (unsigned char*) operator new(NETWORK_PORT_COUNT);
	m_payloadCapacity += NETWORK_PORT_COUNT;
	i = 0;
	do {
		m_useCounts[i] = 0;
		i++;
	} while (i < NETWORK_PORT_COUNT);
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
	} while (i < NETWORK_PORT_COUNT);
	return false;
}

// FUNCTION: LEMBALL 0x0047b870
void CPortsMessage::AddData()
{
	Add(m_useCounts, NETWORK_PORT_COUNT);
}

// FUNCTION: LEMBALL 0x0047b880
void CPortsMessage::GetData()
{
	GetCopy(m_useCounts, NETWORK_PORT_COUNT);
}
