#ifndef LEMBALL_SCAFFOLD_VISOS_FOUNDATION_RAMARENA_H
#define LEMBALL_SCAFFOLD_VISOS_FOUNDATION_RAMARENA_H

#include "../../Common.h"
#include "Arena.h" // complete type

// SIZE 0x50
class RamArena : public Arena {
public:
	Arena* CreateNew(unsigned char* p_arg0, unsigned long p_arg1, char* p_arg2, Arena* p_arg3, Arena* p_arg4);
	MBlock* CreateNewBlock(unsigned char* p_arg0, Arena* p_arg1, MBlock* p_arg2, char* p_arg3, unsigned long p_arg4);
	RamArena(unsigned long p_arg0, char* p_arg1, Arena* p_arg2, Arena* p_arg3);
	int GetSizeOf();
	int GetSizeOfBlock();
	~RamArena();
};

#endif
