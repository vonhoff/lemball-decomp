#ifndef LEMBALL_CHANDMANAGER_H
#define LEMBALL_CHANDMANAGER_H

class CAI;
class CHand;

class CHandManager {
public:
	CHandManager(CAI* pAI, int nCapacity);
	void Restart(void);
	~CHandManager(void);

private:
	unsigned char m_abReserved00[0x30];
	int m_nCapacity30;
	int m_nObjectCount34;
	CHand* m_pObjects38;
	CAI* m_pAI3C;
};

struct CHandManagerVtable {
	void* m_apBaseSlots00[6];
	void (CHandManager::*m_pRestart18)(void);
	void* m_apRemainingSlots1C[3];
};

extern CHandManagerVtable g_LINKSCF_CaptureChunkManagerVtable;

typedef char CHandManagerSizeMustMatchLayout[sizeof(CHandManager) == 0x40 ? 1 : -1];
typedef char CHandManagerMemberPointerMustFitVtableSlot[sizeof(void (CHandManager::*)(void)) == sizeof(void*) ? 1 : -1];

#endif
