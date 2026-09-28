#ifndef LEMBALL_VISOS_FOUNDATION_CRAMARENA_H
#define LEMBALL_VISOS_FOUNDATION_CRAMARENA_H

#include "CArena.h"

// SIZE 0x50
// VTABLE: LEMBALL 0x00498918 CArenaBase
// VTABLE: LEMBALL 0x00498910 CCritical
class CRAMArena : public CArena {
public:
	CRAMArena(unsigned long arenaSize, char* description, CArena* parentArena, CArena* arenaLink);
	virtual ~CRAMArena();
	virtual int GetSizeOf();
	virtual int GetSizeOfBlock();
	virtual CArena* CreateNew(unsigned char* memory,
							  unsigned long arenaSize,
							  char* description,
							  CArena* parentArena,
							  CArena* arenaLink);
	virtual CMBlock* CreateNewBlock(unsigned char* memory,
									CArena* arena,
									CMBlock* previousBlock,
									char* description,
									unsigned long totalSize);
	void operator delete(void*) {}
};

// SYNTHETIC: LEMBALL 0x0045a8e0
// CRAMArena::`scalar deleting destructor'

#endif
