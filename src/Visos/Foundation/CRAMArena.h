#ifndef LEMBALL_VISOS_FOUNDATION_CRAMARENA_H
#define LEMBALL_VISOS_FOUNDATION_CRAMARENA_H

#include "CArena.h"

// SIZE 0x50
// VTABLE: LEMBALL 0x00498918 CArenaBase
// VTABLE: LEMBALL 0x00498910 CCritical
class CRAMArena : public CArena {
public:
	CRAMArena(unsigned long p_arenaSize, char* p_description, CArena* p_parentArena, CArena* p_arenaLink);
	virtual ~CRAMArena();
	virtual int GetSizeOf();
	virtual int GetSizeOfBlock();
	virtual CArena* CreateNew(unsigned char* p_memory,
							  unsigned long p_arenaSize,
							  char* p_description,
							  CArena* p_parentArena,
							  CArena* p_arenaLink);
	virtual CMBlock* CreateNewBlock(unsigned char* p_memory,
									CArena* p_arena,
									CMBlock* p_previousBlock,
									char* p_description,
									unsigned long p_totalSize);
	void operator delete(void*) {}
};

// SYNTHETIC: LEMBALL 0x0045a8e0
// CRAMArena::`scalar deleting destructor'

#endif
