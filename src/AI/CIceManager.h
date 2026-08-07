#ifndef LEMBALL_CICEMANAGER_H
#define LEMBALL_CICEMANAGER_H

class CAI;
class CIce;
class CViewData;
struct tCoord3d;

class CIceManager {
public:
	CIceManager(CAI* pAI, int nCapacity);
	void Restart(void);
	void Initialise(int nCount);
	~CIceManager(void);
	void Process(void);
	int GetViewData(CViewData* pViewData);
	void Switch(int nAction, unsigned int nSlot);
	int StepOn(void* pCoord, void* pGameObject);
#ifdef LEMBALL_CICE_MANAGER_RECONSTRUCTION
	void Add(unsigned short nSlot,
			 const tCoord3d& start,
			 const tCoord3d& end,
			 int nMoveX,
			 int nMoveY,
			 unsigned char fActive);
	void LoadLevel(unsigned char* pData, int cbData, unsigned char nVersion);
#endif
	void ResetObjectCount(void);

private:
	unsigned char m_abReserved00[0x30];
	int m_nCapacity30;
	int m_nObjectCount34;
	CIce* m_pObjects38;
	CAI* m_pAI3C;
};

struct CIceManagerVtable {
	void* m_apBaseSlots00[6];
	void (CIceManager::*m_pRestart18)(void);
	void (CIceManager::*m_pProcess1C)(void);
	void* m_pCommand20;
	int (CIceManager::*m_pGetViewData24)(CViewData* pViewData);
};

extern CIceManagerVtable g_LINKSCF_IceChunkManagerVtable;

typedef char CIceManagerSizeMustMatchLayout[sizeof(CIceManager) == 0x40 ? 1 : -1];
typedef char CIceManagerMemberPointerMustFitVtableSlot[sizeof(void (CIceManager::*)(void)) == sizeof(void*) ? 1 : -1];

#endif
