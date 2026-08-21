#ifndef LEMBALL_LEVELTILEGRIDCELL_H
#define LEMBALL_LEVELTILEGRIDCELL_H

// Tile-grid cell. Consolidated from identical per-TU duplicate definitions
// (CMover.cpp, LEVELSTAT.CPP). NOTE: LINKSCFBYTES.CPP has a DIFFERENT LevelTileGridCell
// (int m_anValues[3]) and keeps its local definition.

struct LevelTileGridCell {
	int m_nType00;
	short m_nVariant04;
	unsigned short m_nFlags06;
	short m_nReserved08;
	short m_nReserved0A;
};

#endif
