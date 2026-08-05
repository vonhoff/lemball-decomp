#ifndef LEMBALL_LEVELMANAGEDENTITYSTATEIDVIEW_H
#define LEMBALL_LEVELMANAGEDENTITYSTATEIDVIEW_H

// Level managed-entity state-id view. Consolidated from identical per-TU
// duplicate definitions (CPlayerLemmingGroup.cpp, LEVELVT.CPP).

struct LevelManagedEntityStateIdView {
	char m_abReserved00[0xb8];
	int m_nStateIdB8;
};

#endif
