#ifndef LEMBALL_VISOS_FOUNDATION_VSMEM_H
#define LEMBALL_VISOS_FOUNDATION_VSMEM_H

#include <stddef.h>

void* InternalNew(unsigned long allocationSize);
void InternalDelete(void* memory);
void* operator new(size_t allocationSize);
void operator delete(void* memory);
bool CheckValidPointer(void* pointer);
#endif
