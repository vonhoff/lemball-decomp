#ifndef LEMBALL_VISOS_ANIMATION_STATMANAGER_H
#define LEMBALL_VISOS_ANIMATION_STATMANAGER_H

#include "../../Common.h"

// SIZE 0x14
// VTABLE: LEMBALL 0x00498958
class StatManager {
public:
	StatManager(int p_arg0);
	virtual VsOStream& StreamOut(VsOStream& p_stream); // vtable+0x00
	void Register(BaseStat* p_stat);
	~StatManager();

private:
	undefined4 m_unk0x04; // 0x04
	BaseStat** m_stats;   // 0x08
	int m_capacity;       // 0x0c
	int m_statCount;      // 0x10
};

extern StatManager* g_pStatManager;
#endif
