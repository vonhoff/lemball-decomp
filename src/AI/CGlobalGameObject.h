#ifndef LEMBALL_CGLOBALGAMEOBJECT_H
#define LEMBALL_CGLOBALGAMEOBJECT_H

#include "AI/CGameObject.h"

class CGlobalGameObject : public CGameObject {
public:
	char m_abReserved120[0x14];
	int m_nUsableState134;

	int UsableState(void);
};

#endif
