#include "Platform/Windows/Mixed/Engine/CORE/VSINIT.H"

extern void LEMBALL_FASTCALL DispatchLiftChunkObjectActionBySlot(void* pManager,
																 void* pUnusedEdx,
																 int nAction,
																 unsigned int nSlotId,
																 int nUnused3,
																 int nUnused4);
struct CDoorManager {
	void Switch(int nAction, unsigned int nSlotId);
};
extern void LEMBALL_FASTCALL DispatchMoveChunkObjectActionBySlot(void* pManager,
																 void* pUnusedEdx,
																 int nAction,
																 unsigned int nSlotId);
struct CIceManager {
	void Switch(int nAction, unsigned int nSlotId);
};

struct LevelSlotActionDispatcherView {
	char m_abReserved000[0x18c];
	void* m_pLiftManager18c;
	CDoorManager* m_pDoorManager190;
	char m_abReserved194[0x18];
	CIceManager* m_pIceManager1ac;
	char m_abReserved1b0[0x10];
	void* m_pMoveManager1c0;

	void DispatchLevelSlotActionByType(int nAction, unsigned int nSlotId, int nRangeEnd, int nUnused4);
};

struct GmobChunkActionRecord {
	int m_nAction;
	unsigned short m_nSlotId;
	unsigned short m_nReserved;
};

struct GmobChunkType14ActionView {
	virtual void ReservedSlot0(int nValue);
	virtual void ReservedSlot1(int nValue);
	virtual void ReservedSlot2(int nValue);
	virtual void ReservedSlot3(int nValue);
	virtual void ReservedSlot4(int nValue);
	virtual void ReservedSlot5(int nValue);
	virtual void ReservedSlot6(int nValue);
	virtual void ReservedSlot7(int nValue);
	virtual void ReservedSlot8(int nValue);
	virtual void ReservedSlot9(int nValue);
	virtual void ReservedSlot10(int nValue);
	virtual void ReservedSlot11(int nValue);
	virtual void ReservedSlot12(int nValue);
	virtual void SetActionState(int nStateId);
	void DispatchGmobChunkType14Actions(void);
};

extern void* g_pActiveManagedEntityOwner;

// Split from the original LEVELVT source group to preserve MSVC 4.20 code generation.

// MACINTOSH: CAI::SwitchMessage(swMessage, int, int, int)
// FUNCTION: LEMBALL 0x00412c80
void LevelSlotActionDispatcherView::DispatchLevelSlotActionByType(int nAction,
																  unsigned int nSlotId,
																  int nRangeEnd,
																  int nUnused4)
{
	switch (nAction) {
	case 1:
		DispatchLiftChunkObjectActionBySlot(m_pLiftManager18c, 0, nAction, nSlotId, nRangeEnd, nUnused4);
		return;
	case 2:
		while ((int) nSlotId < nRangeEnd) {
			DispatchLiftChunkObjectActionBySlot(m_pLiftManager18c, 0, 1, nSlotId, 0, 0);
			++nSlotId;
		}
		return;
	case 3:
		m_pDoorManager190->Switch(3, nSlotId);
		return;
	case 4:
		DispatchMoveChunkObjectActionBySlot(m_pMoveManager1c0, 0, 4, nSlotId);
		return;
	case 5:
		m_pIceManager1ac->Switch(5, nSlotId);
		return;
	}
}

// FUNCTION: LEMBALL 0x0041d130
void GmobChunkType14ActionView::DispatchGmobChunkType14Actions(void)
{
	char* pObjectBytes = (char*) this;
	int iAction = 0;

	if (*(unsigned short*) (pObjectBytes + 0x15c) > iAction) {
		GmobChunkActionRecord* pAction = (GmobChunkActionRecord*) (pObjectBytes + 0x160);
		do {
			((LevelSlotActionDispatcherView*) g_pActiveManagedEntityOwner)
				->DispatchLevelSlotActionByType(pAction->m_nAction, pAction->m_nSlotId, 0, 0);
			++pAction;
			++iAction;
		} while (*(unsigned short*) (pObjectBytes + 0x15c) > iAction);
	}
	SetActionState(0x15);
}
