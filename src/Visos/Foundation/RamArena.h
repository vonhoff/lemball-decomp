#ifndef LEMBALL_VISOS_FOUNDATION_RAMARENA_H
#define LEMBALL_VISOS_FOUNDATION_RAMARENA_H

#include "../../Common.h"
#include "Arena.h" // complete type

// SIZE 0x50
// VTABLE: LEMBALL 0x00498918
class RamArena : public Arena {
public:
	RamArena(unsigned long p_arg0, char* p_arg1, Arena* p_arg2, Arena* p_arg3);
	virtual ~RamArena();
	virtual int GetSizeOf();
	virtual int GetSizeOfBlock();
	virtual Arena* CreateNew(unsigned char* p_arg0, unsigned long p_arg1, char* p_arg2, Arena* p_arg3, Arena* p_arg4);
	virtual MBlock* CreateNewBlock(unsigned char* p_arg0, Arena* p_arg1, MBlock* p_arg2, char* p_arg3, unsigned long p_arg4);
};

#endif
