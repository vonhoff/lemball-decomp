#ifndef LEMBALL_CSLINKYMANAGER_H
#define LEMBALL_CSLINKYMANAGER_H

#include "AI/AICoord.h"

// Mac blueprint: CSlinkyManager owns an array of CSlinky (stride 0x150).
// Its vtable (offset 0) points to level data that exposes the version at +0x54.

class CSlinkyManager {
public:
	int m_vtable00;                 // vtable pointer / level-data base
	int m_pObjects04;               // 0x04
	int m_nCapacity08;              // 0x08
	int m_nObjectCount0C;           // 0x0c

	void Process(void);
	void Add(int arg1, int arg2, int arg3, int arg4, int arg5);
	void Remove(void* pSlinky);
	void LoadLevel(unsigned short* pLevelData, int nLen, unsigned char nFormat);
};

typedef char CSlinkyManagerSizeCheck[sizeof(CSlinkyManager) == 0x10 ? 1 : -1];

#endif // LEMBALL_CSLINKYMANAGER_H
