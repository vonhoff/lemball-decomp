#include "Arena.h"

Arena::Arena()
{
}

// 68K 0x102147c2 __ct__6CArenaFUlPcP6CArenaP6CArena
// STUB: LEMBALL 0x004599f0
Arena::Arena(unsigned long p_arg0, char* p_arg1, class Arena* p_arg2, class Arena* p_arg3)
{
}

// 68K 0x1021484c DeleteLists__6CArenaFv
// STUB: LEMBALL 0x00459a40
void Arena::DeleteLists()
{
}

// 68K 0x102148f6 __dt__6CArenaFv
// STUB: LEMBALL 0x00459aa0
Arena::~Arena()
{
}

// 68K 0x1021492e CheckAndAmalgamate__6CArenaFP7CMBlockP7CMBlock
// STUB: LEMBALL 0x00459b10
bool Arena::CheckAndAmalgamate(MBlock* p_arg0, MBlock* p_arg1)
{
	return 0;
}

// 68K 0x102149fc AddToFreeList__6CArenaFP7CMBlock
// STUB: LEMBALL 0x00459bd0
bool Arena::AddToFreeList(MBlock* p_arg0)
{
	return 0;
}

// 68K 0x10214a5a AddToBlockList__6CArenaFP7CMBlockP7CMBlock
// STUB: LEMBALL 0x00459c00
bool Arena::AddToBlockList(MBlock* p_arg0, MBlock* p_arg1)
{
	return 0;
}

// 68K 0x10214ad4 AddToArenaList__6CArenaFP6CArena
// STUB: LEMBALL 0x00459c40
bool Arena::AddToArenaList(class Arena* p_arg0)
{
	return 0;
}

// 68K 0x10214b40 RemoveFromFreeList__6CArenaFP7CMBlock
// STUB: LEMBALL 0x00459c80
bool Arena::RemoveFromFreeList(MBlock* p_arg0)
{
	return 0;
}

// 68K 0x10214bac RemoveFromBlockList__6CArenaFP7CMBlock
// STUB: LEMBALL 0x00459cb0
bool Arena::RemoveFromBlockList(MBlock* p_arg0)
{
	return 0;
}

// 68K 0x10214c18 RemoveFromArenaList__6CArenaFP6CArena
// STUB: LEMBALL 0x00459ce0
bool Arena::RemoveFromArenaList(class Arena* p_arg0)
{
	return 0;
}

// 68K 0x10214c90 FindSmallestBlock__6CArenaFUlPc
// STUB: LEMBALL 0x00459d20
MBlock* Arena::FindSmallestBlock(unsigned long p_arg0, char* p_arg1)
{
	return 0;
}

// 68K 0x10214d12 Allocate__6CArenaFPPUcUlPc
// STUB: LEMBALL 0x00459d70
bool Arena::Allocate(unsigned char** p_arg0, unsigned long p_arg1, char* p_arg2)
{
	// STRING: LEMBALL 0x004a11bc " : Free Size is "
	// STRING: LEMBALL 0x004a11b4 " : 0x"
	// STRING: LEMBALL 0x004a1184 "CArena::Allocate : Not enough free memory : "
	// STRING: LEMBALL 0x004a1220 " : Free Size is "
	// STRING: LEMBALL 0x004a1218 " : 0x"
	// STRING: LEMBALL 0x004a11d4 "CArena::Allocate : memory blocks are too fragmented to satisfy : "
	return 0;
}

// 68K 0x102150e4 Free__6CArenaFPUc
// STUB: LEMBALL 0x00459f70
bool Arena::Free(unsigned char* p_memory)
{
	return 0;
}

// 68K 0x102151ce AllocateArena__6CArenaFPP6CArenaUlPc
// STUB: LEMBALL 0x0045a010
bool Arena::AllocateArena(class Arena** p_arg0, unsigned long p_arg1, char* p_arg2)
{
	// STRING: LEMBALL 0x004a1240 "Arena container"
	return 0;
}

// 68K 0x102152a2 FreeArena__6CArenaFP6CArena
// STUB: LEMBALL 0x0045a0a0
bool Arena::FreeArena(class Arena* p_arg0)
{
	return 0;
}

// 68K 0x1021531e CheckValidPointer__6CArenaFPv
// STUB: LEMBALL 0x0045a0e0
bool Arena::CheckValidPointer(void* p_arg0)
{
	return 0;
}

// 68K 0x1021539c CheckMemoryBlock__6CArenaFP7CMBlock
// STUB: LEMBALL 0x0045a1e0
bool Arena::CheckMemoryBlock(MBlock* p_arg0)
{
	return 0;
}

// 68K 0x102153dc CheckFreeMemoryBlock__6CArenaFP7CMBlock
// STUB: LEMBALL 0x0045a230
bool Arena::CheckFreeMemoryBlock(MBlock* p_arg0)
{
	return 0;
}

// 68K 0x10215438 StreamOut__6CArenaFR10CVSOStream
// STUB: LEMBALL 0x0045a260
void Arena::StreamOut(VsOStream& p_stream)
{
	// STRING: LEMBALL 0x004a1250 "************************************************\n"
	// STRING: LEMBALL 0x004a1284 "Memory Block list free size = "
	// STRING: LEMBALL 0x004a12a8 "--------------------\n"
	// STRING: LEMBALL 0x004a12c0 "Addr\t\tSize\t\tDesc\n"
	// STRING: LEMBALL 0x004a12d4 "------------------------------------------------\n"
	// STRING: LEMBALL 0x004a1308 "Child Arena list\n"
	// STRING: LEMBALL 0x004a131c "--------------------\n"
	// STRING: LEMBALL 0x004a1334 "************************************************\n"
}

// 68K 0x1021557e CheckIntegrity__6CArenaFv
// STUB: LEMBALL 0x0045a330
bool Arena::CheckIntegrity()
{
	return 0;
}

// 68K 0x102155a4 GetFreeSize__6CArenaFv
// STUB: LEMBALL 0x0045a340
unsigned long Arena::GetFreeSize()
{
	return 0;
}

// 68K 0x102155ce GetAllocSize__6CArenaFv
// STUB: LEMBALL 0x0045a350
unsigned long Arena::GetAllocSize()
{
	return 0;
}

// 68K 0x1021562c __nw__6CArenaFUlPv
// STUB: LEMBALL 0x0045a390
void* Arena::operator new(size_t p_arg0, void* p_arg1)
{
	return 0;
}

// 68K 0x1021564e MemSet__6CArenaFPUcUcUl
// STUB: LEMBALL 0x0045a3a0
void Arena::MemSet(unsigned char* p_destination, unsigned char p_value, unsigned long p_size)
{
}

// 68K 0x10215686 MemCopy__6CArenaFPUcPUcUl
// STUB: LEMBALL 0x0045a3d0
void Arena::MemCopy(unsigned char* p_destination, unsigned char* p_source, unsigned long p_size)
{
}

// GLOBAL: LEMBALL 0x004aa100
Arena* g_pMasterArena;
