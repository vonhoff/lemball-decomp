#ifndef LEMBALL_SCAFFOLD_VISOS_VSMEM_H
#define LEMBALL_SCAFFOLD_VISOS_VSMEM_H

#include "../Common.h"

void* InternalNew(unsigned long p_arg0);
void InternalDelete(void* p_arg0);
void* operator new(size_t p_arg0);
void operator delete(void* p_arg0);
bool CheckValidPointer(void* p_arg0);
#endif
