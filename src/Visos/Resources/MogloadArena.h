#ifndef LEMBALL_VISOS_RESOURCES_MOGLOADARENA_H
#define LEMBALL_VISOS_RESOURCES_MOGLOADARENA_H

#include "../../Common.h"

class MogloadArena {
public:
	void operator delete(void* p_memory);
	void* operator new(unsigned int p_size);
};

extern class Arena* g_pMogloadArena;

#endif
