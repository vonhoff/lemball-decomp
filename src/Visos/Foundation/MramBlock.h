#ifndef LEMBALL_VISOS_FOUNDATION_MRAMBLOCK_H
#define LEMBALL_VISOS_FOUNDATION_MRAMBLOCK_H

#include "../../Common.h"
#include "MBlock.h" // complete type

// SIZE 0x28
// VTABLE: LEMBALL 0x00498948
class MramBlock : public MBlock {
public:
	MramBlock(Arena* p_arena, MBlock* p_previous, char* p_description, unsigned long p_totalSize);
	// 68K 0x10100de0 __dt__10CMRAMBlockFv
	virtual ~MramBlock() {}
};

// SYNTHETIC: LEMBALL 0x0045a900
// MramBlock::`scalar deleting destructor'

#endif
