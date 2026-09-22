#ifndef LEMBALL_VISOS_RESOURCES_CMOGLOADARENA_H
#define LEMBALL_VISOS_RESOURCES_CMOGLOADARENA_H

class CMogloadArena {
public:
	void operator delete(void* p_memory);
	void* operator new(unsigned int p_size);
};

extern class CArena* g_pMogloadArena;

#endif
