#ifndef LEMBALL_VISOS_RESOURCES_RAWREAD_H
#define LEMBALL_VISOS_RESOURCES_RAWREAD_H

#include "../../Common.h"

class RawRead {
public:
	static bool NameCmp(char* p_path, char* p_name);
	static bool Open(char* p_path, char* p_mode);
	static unsigned char InputByte();
	static unsigned int InputDword();
};

#endif
