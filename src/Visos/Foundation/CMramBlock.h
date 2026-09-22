#ifndef LEMBALL_VISOS_FOUNDATION_CMRAMBLOCK_H
#define LEMBALL_VISOS_FOUNDATION_CMRAMBLOCK_H

#include "CMBlock.h" // complete type

class CArena;

// SIZE 0x28
// VTABLE: LEMBALL 0x00498948
class CMramBlock : public CMBlock {
public:
	CMramBlock(CArena* p_arena, CMBlock* p_previous, char* p_description, unsigned long p_totalSize);
	virtual ~CMramBlock() {}
};

// SYNTHETIC: LEMBALL 0x0045a900
// CMramBlock::`scalar deleting destructor'

#endif
