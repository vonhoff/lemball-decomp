#include "CMaze.h"

#include <stddef.h>

// FUNCTION: LEMBALL 0x004230c0
CMaze::~CMaze()
{
	int row = 0;
	if (m_distances != NULL) {
		if (m_height > 0) {
			do {
				delete[] m_distances[row];
				row++;
			} while (row < m_height);
		}
		delete[] m_distances;
	}
}
