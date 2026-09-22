#ifndef LEMBALL_VISOS_ANIMATION_CSTATMANAGER_H
#define LEMBALL_VISOS_ANIMATION_CSTATMANAGER_H

class CVSOStream;
class CBaseStat;

// SIZE 0x14
// VTABLE: LEMBALL 0x00498958
class CStatManager {
public:
	CStatManager(int p_arg0);
	virtual CVSOStream& StreamOut(CVSOStream& p_stream); // vtable+0x00
	void Register(CBaseStat* p_stat);
	~CStatManager();

private:
	unsigned int m_unk0x04; // 0x04
	CBaseStat** m_stats;    // 0x08
	int m_capacity;         // 0x0c
	int m_statCount;        // 0x10
};

extern CStatManager* g_pStatManager;
#endif
