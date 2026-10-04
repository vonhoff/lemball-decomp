#ifndef LEMBALL_VISOS_FOUNDATION_CMRAMBLOCK_H
#define LEMBALL_VISOS_FOUNDATION_CMRAMBLOCK_H

#include "CMBlock.h"

class CArena;

// SIZE 0x28
// VTABLE: LEMBALL 0x00498948
class CMRAMBlock : public CMBlock {
public:
	CMRAMBlock(CArena* p_arena, CMBlock* p_previous, char* p_description, unsigned long p_totalSize);
	virtual ~CMRAMBlock() {}
};

// SYNTHETIC: LEMBALL 0x0045a900
// CMRAMBlock::`scalar deleting destructor'

#endif
