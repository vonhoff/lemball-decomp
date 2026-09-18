#include "AiDestinationEntry.h"

// FUNCTION: LEMBALL 0x00414cb0
AiCoord AiDestinationEntry::GetCoordinate()
{
	return m_coordinate;
}

// FUNCTION: LEMBALL 0x00417ab0
AiDestinationEntry::AiDestinationEntry() : m_type(DESTINATION_NONE)
{
}
