#ifndef LEMBALL_VISOS_FOUNDATION_VSMEM_H
#define LEMBALL_VISOS_FOUNDATION_VSMEM_H

#include <stddef.h>

void* InternalNew(unsigned long p_size);
void InternalDelete(void* p_ptr);
void* operator new(size_t allocationSize);
void operator delete(void* memory);
bool CheckValidPointer(void* pointer);
#endif
