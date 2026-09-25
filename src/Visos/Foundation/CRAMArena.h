#ifndef LEMBALL_VISOS_FOUNDATION_CRAMARENA_H
#define LEMBALL_VISOS_FOUNDATION_CRAMARENA_H

#include "CArena.h"

// SIZE 0x50
// VTABLE: LEMBALL 0x00498918 CArenaBase
// VTABLE: LEMBALL 0x00498910 CCritical
class CRAMArena : public CArena {
public:
	CRAMArena(unsigned long p_arg0, char* p_arg1, CArena* p_arg2, CArena* p_arg3);
	virtual ~CRAMArena();
	virtual int GetSizeOf();
	virtual int GetSizeOfBlock();
	virtual CArena* CreateNew(unsigned char* p_arg0,
							  unsigned long p_arg1,
							  char* p_arg2,
							  CArena* p_arg3,
							  CArena* p_arg4);
	virtual CMBlock* CreateNewBlock(unsigned char* p_arg0,
									CArena* p_arg1,
									CMBlock* p_arg2,
									char* p_arg3,
									unsigned long p_arg4);
	void operator delete(void*) {}
};

// SYNTHETIC: LEMBALL 0x0045a8e0
// CRAMArena::`scalar deleting destructor'

#endif
