#ifndef LEMBALL_SCAFFOLD_VISOS_FOUNDATION_BUCKET_H
#define LEMBALL_SCAFFOLD_VISOS_FOUNDATION_BUCKET_H

#include "../../Common.h"
#include "Critical.h" // complete type

// SIZE 0x54
// VTABLE: LEMBALL 0x0049a478
class Bucket : public Critical {
public:
	Boffset FindFreeOffset(Boffset p_offset);
	Bucket(int p_arg0, int p_arg1, unsigned char* p_arg2, unsigned long* p_arg3);
	bool Allocate(unsigned char** p_result);
	bool CheckValidPointer(unsigned char* p_memory);
	bool Free(unsigned char* p_memory);
	void NewChild();
	void RemoveChild();
	void RemoveThis();
	~Bucket();

private:
	class Bucket* m_child;            // 0x1c
	class Bucket* m_parent;           // 0x20
	unsigned int m_blockSize;         // 0x24
	unsigned int m_blockCount;        // 0x28
	unsigned int m_totalBytes;        // 0x2c
	unsigned int m_freeBytes;         // 0x30
	unsigned int m_totalAllocations;  // 0x34
	unsigned int m_activeAllocations; // 0x38
	unsigned int m_peakAllocations;   // 0x3c
	unsigned short m_flags;           // 0x40
	unsigned int m_mapWordCount;      // 0x44
	unsigned int* m_map;              // 0x48
	unsigned char* m_memory;          // 0x4c
	Boffset m_freeOffset;             // 0x50
};

#endif
