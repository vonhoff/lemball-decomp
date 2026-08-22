#ifndef LEMBALL_SCAFFOLD_VISOS_ANIMATION_TIMESTAT_H
#define LEMBALL_SCAFFOLD_VISOS_ANIMATION_TIMESTAT_H

#include "../../Common.h"
#include "BaseStat.h" // complete type

// SIZE 0x20
class TimeStat : public BaseStat {
public:
	virtual void Update(unsigned int p_value); // vtable+0x04
};

#endif
