#ifndef LEMBALL_VISOS_RESOURCES_RAWREAD_H
#define LEMBALL_VISOS_RESOURCES_RAWREAD_H

#include "../../Common.h"

class RawRead {
public:
	bool NameCmp(char* p_path, char* p_name);
	bool Open(char* p_path, char* p_mode);
	unsigned char InputByte();
	unsigned int InputDword();
};

extern _Filet* g_pMogFile;

// LIBRARY: LEMBALL 0x0047fac0
// _toupper

// LIBRARY: LEMBALL 0x0047fca0 SYMBOL
// __purecall

#endif
