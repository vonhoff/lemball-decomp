#ifndef LEMBALL_VISOS_FOUNDATION_CMBLOCK_H
#define LEMBALL_VISOS_FOUNDATION_CMBLOCK_H

#include <stddef.h>

class CVSOStream;

// SIZE 0x28
// VTABLE: LEMBALL 0x00498940
class CMBlock {
public:
	CMBlock(class CArena* p_arena, class CMBlock* p_previous, char* p_description, unsigned long p_size);
	virtual CVSOStream& StreamOut(CVSOStream& p_stream); // vtable+0x00
	virtual ~CMBlock();                                  // vtable+0x04
	void SetDesc(char* p_arg0);
	bool ContainsAddress(void* p_address);
	void* operator new(size_t p_arg0, void* p_arg1);
	void operator delete(void*) {}
	CMBlock();

	friend class CArena;
	friend class CRAMArena;

protected:
	unsigned int m_signature;       // 0x04
	unsigned char* m_data;          // 0x08
	unsigned int m_size;            // 0x0c
	class CArena* m_ownerArena;     // 0x10
	class CMBlock* m_nextBlock;     // 0x14
	class CMBlock* m_previousBlock; // 0x18
	class CMBlock* m_previousFree;  // 0x1c
	class CMBlock* m_nextFree;      // 0x20
	unsigned int m_flags;           // 0x24
};

// SYNTHETIC: LEMBALL 0x0045a8f0
// CMBlock::`scalar deleting destructor'

#endif
