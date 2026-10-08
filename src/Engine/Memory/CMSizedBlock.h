#ifndef LEMBALL_VISOS_FOUNDATION_CMSIZEDBLOCK_H
#define LEMBALL_VISOS_FOUNDATION_CMSIZEDBLOCK_H

#include "CMBlock.h"

class CArena;

// MINIMUM SIZE 0x28
// VTABLE: LEMBALL 0x00498950
class CMSizedBlock : public CMBlock {
public:
	CMSizedBlock(CArena* p_arena, CMBlock* p_previous, char* p_description, unsigned long p_size);
	virtual ~CMSizedBlock() {}
};

// SYNTHETIC: LEMBALL 0x0045a920
// CMSizedBlock::`scalar deleting destructor'

#endif
