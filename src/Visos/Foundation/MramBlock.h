#ifndef LEMBALL_VISOS_FOUNDATION_MRAMBLOCK_H
#define LEMBALL_VISOS_FOUNDATION_MRAMBLOCK_H

#include "../../Common.h"
#include "MBlock.h" // complete type

// SIZE 0x28
class MramBlock : public MBlock {
public:
	MramBlock(Arena* p_arg0, MBlock* p_arg1, char* p_arg2, unsigned long p_arg3);
	~MramBlock();
};

#endif
