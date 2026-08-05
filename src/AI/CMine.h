#ifndef LEMBALL_CMINE_H
#define LEMBALL_CMINE_H

#include "AI/AICoord.h"

class CGameObject;

typedef int eAction;
struct AICOORD;

class CMine {
public:
	CMine(void);
	void Restart(void);
	void Initialise(void);
	void Trigger(int nDelay);
	void DoActivate(void);
	void SetTerrain(void);
	void Set(AICOORD position);
	void StepOn(CGameObject* pObject);
	int IsUsable(eAction action);
	int Process(void);
	void OnGround(void);

private:
	unsigned char m_abReserved00[0x94];
	int m_nFrameTimeMs94;                  // 0x94
	unsigned char m_abReserved98[4];
	AICOORD m_WorldPosition9C;            // 0x9c
	unsigned char m_abReservedA8[0x10];
	int m_nStateB8;                        // 0xb8
	unsigned char m_abReservedBC[0xc];
	int m_nNextTickC8;                     // 0xc8
	int m_nEndTickCC;                      // 0xcc
	unsigned char m_abReservedD0[0x68];
	int m_anRuntimeState138[4];            // 0x138..0x147
	int m_nDelay148;                       // 0x148
	unsigned char m_abReserved14C[4];      // 0x14c..0x14f
};

typedef char CMineSizeCheck[sizeof(CMine) == 0x150 ? 1 : -1];

#endif
