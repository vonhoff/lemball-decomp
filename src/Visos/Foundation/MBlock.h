#ifndef LEMBALL_VISOS_FOUNDATION_MBLOCK_H
#define LEMBALL_VISOS_FOUNDATION_MBLOCK_H

#include "../../Common.h"

// SIZE 0x28
// VTABLE: LEMBALL 0x00498940
class MBlock {
public:
	MBlock(Arena* p_arg0, struct MBlock* p_arg1, char* p_arg2, unsigned long p_arg3);
	virtual void StreamOut(VsOStream& p_stream); // vtable+0x00
	virtual ~MBlock();                           // vtable+0x04
	void SetDesc(char* p_arg0);
	void* operator new(size_t p_arg0, void* p_arg1);
	MBlock();

private:
	unsigned int m_signature;      // 0x04
	unsigned char* m_data;         // 0x08
	unsigned int m_size;           // 0x0c
	char* m_description;           // 0x10
	class MBlock* m_nextBlock;     // 0x14
	class MBlock* m_previousBlock; // 0x18
	class MBlock* m_previousFree;  // 0x1c
	class MBlock* m_nextFree;      // 0x20
	unsigned int m_flags;          // 0x24
};

#endif
