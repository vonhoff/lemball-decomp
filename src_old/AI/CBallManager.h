#ifndef LEMBALL_AI_CBALLMANAGER_H
#define LEMBALL_AI_CBALLMANAGER_H

class CBall;

// MANAGER for CBall entries (Windows physical layout: vtable + array + count + capacity).
class CBallManager {
public:
	void** m_pVtable00;
	void** m_apBalls04;   // +0x04 array of CBall* (active prefix, then free slot bloat)
	int m_nActiveCount08; // +0x08 count of active entries
	int m_nCapacity0C;    // +0x0c allocated capacity

	void Initialise(int nCapacity);
	void Delete(CBall* pBall);
	void LoadLevel(unsigned short* pStream);
};

#endif
