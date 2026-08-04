#ifndef LEMBALL_CHAND_H
#define LEMBALL_CHAND_H

#include "AI/CGameObject.h"

class CHand : public CGameObject {
	friend class CHandManager;

public:
	CHand(void);
	void Initialise(void);
	void Restart(void);
	~CHand(void);
	void Set(unsigned short nSlotId, const AICOORD& position);
	int Process(void);
	int StepOn(const AICOORD& position, CGameObject* pObject);
	void DoActivate(void);

private:
	int m_fProcessFrame124;
	int m_nLastProcessedState128;
	unsigned char m_abReserved12C[0xc];
	int m_fActivated138;
	int m_fConfigured13C;
	CGameObject* m_pActivatedObject140;
};

struct CHandVtable {
	void* m_apSlots00[5];
	int (CHand::*m_pProcess14)(void);
	void* m_apSlots18[10];
};

extern CHandVtable g_LINKSCF_CaptureChunkObjectVtable;

typedef char CHandSizeMustMatchArrayStride[sizeof(CHand) == 0x144 ? 1 : -1];
typedef char CHandMemberPointerMustFitVtableSlot[sizeof(int (CHand::*)(void)) == sizeof(void*) ? 1 : -1];

#endif
