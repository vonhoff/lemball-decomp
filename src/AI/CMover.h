#ifndef LEMBALL_CMOVER_H
#define LEMBALL_CMOVER_H

#include "AI/AICoord.h"

struct ManagedEntitySlotOwnerView {
	char m_abReserved00[0x6c];
	unsigned short m_nSlotId6C;

	void SetManagedEntitySlotId(unsigned short nSlotId);
};

struct CMover : public ManagedEntitySlotOwnerView {
public:
	void Restart(void);
	void Initialise(void);
	void Switch(void);
	int IsOn(AICOORD& point);
	void StopObjectsMoving(void);
	int IsAt(int x, int y, int* pHeight);
	void Set(unsigned short nSlotId, short nHeading, int nMode, int iNode, int nValue);

private:
	char m_abReserved6E[0x2e];
	int m_nWorldX9C;
	int m_nWorldYA0;
	int m_nWorldZA4;
	char m_abReservedA8[0x14];
	short m_nHeadingBC;
	char m_abReservedBE[0x7a];
	int m_fActive138;
	char m_abReserved13C[8];
	int m_nMode144;
	char m_abReserved148[0x20];
	int m_iNode168;
	int m_nValue16C;
	int m_nState170;
	int m_cAttachedEntities174;
	void* m_apAttachedEntities178[10];
};

typedef char CMoverSizeCheck[sizeof(CMover) == 0x1a0 ? 1 : -1];

#endif
