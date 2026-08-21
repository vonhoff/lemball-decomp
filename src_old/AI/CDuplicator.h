#ifndef LEMBALL_CDUPLICATOR_H
#define LEMBALL_CDUPLICATOR_H

#include "AI/AICoord.h"

class CGameObject;

// Mac blueprint: CDuplicator occupies a two-cell footprint on the level tile
// grid and passes animation coords through helper 0x4023e2.

class CDuplicator {
public:
	int m_vtable00;                 // vtable pointer
	unsigned char m_abReserved04[0x3c];
	int m_anInitialPosition40[3];   // 0x40
	unsigned char m_abReserved4C[0x10];
	CGameObject* m_pChild5C;        // 0x5c
	unsigned char m_abReserved60[0x34];
	int m_nFrameTime94;             // 0x94
	unsigned char m_abReserved98[4];
	int m_xPosWorld9C;              // 0x9c
	int m_yPosWorldA0;              // 0xa0
	int m_zPosWorldA4;              // 0xa4
	unsigned char m_abReservedA8[0x10];
	int m_nStateB8;                 // 0xb8
	short m_nActionPhaseBC;         // 0xbc
	unsigned char m_abReservedBE[0xe];
	int m_nNextUpdateTickCC; // 0xcc
	unsigned char m_abReservedD0[0x68];
	int m_nPlaced138;                  // 0x138
	int m_nActive13C;                  // 0x13c
	CGameObject* m_pActivatedChild140; // 0x140

	void Restart(void);
	void Set(const AICOORD& position);
	void Delete(void);
	void DoActivate(void);
};

typedef char CDuplicatorSizeCheck[sizeof(CDuplicator) == 0x144 ? 1 : -1];

#endif // LEMBALL_CDUPLICATOR_H
