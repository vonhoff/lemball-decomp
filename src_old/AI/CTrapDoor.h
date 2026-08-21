#ifndef LEMBALL_CTRAPDOOR_H
#define LEMBALL_CTRAPDOOR_H

#include "AI/CGameObject.h"

struct CViewData;

class CTrapDoor : public CGameObject {
public:
	int m_fPrepare124;
	int m_nPreviousNetworkState128;
	char m_abReserved12C[0x0c];
	AICOORD m_Position138;
	int m_nVariant144;
	int m_fTransitionActive148;
	int m_nTransitionDuration14C;

	void GetViewData(CViewData& ViewData);
	int Process(void);
};

typedef char CTrapDoorSizeCheck[(sizeof(CTrapDoor) == 0x150) ? 1 : -1];

#endif
