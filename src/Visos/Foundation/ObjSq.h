#ifndef LEMBALL_SCAFFOLD_VISOS_FOUNDATION_OBJSQ_H
#define LEMBALL_SCAFFOLD_VISOS_FOUNDATION_OBJSQ_H

#include "../../Common.h"

// SIZE 0x0a
class ObjSq {
public:
	ObjSq();

private:
	unsigned short m_objectCount; // 0x00
	undefined m_unknown02[8];     // 0x02
};

#endif
