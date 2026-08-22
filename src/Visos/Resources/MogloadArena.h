#ifndef LEMBALL_SCAFFOLD_VISOS_RESOURCES_MOGLOADARENA_H
#define LEMBALL_SCAFFOLD_VISOS_RESOURCES_MOGLOADARENA_H

#include "../../Common.h"

class MogloadArena {
public:
	void operator delete(void* p_memory);
	void* operator new(unsigned int p_size);
};

#endif
